using Fleck;
using Microsoft.Win32;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using WeChat.Events;
using WeChat.Exceptions;
using WeChat.Helper;

namespace WeChat
{
    public class WeChatSdk
    {
        private static string dllPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "WeChatExt.dll");
        private Dictionary<int, UserConnection> userConnections = new Dictionary<int, UserConnection>();
        private string url = "ws://127.0.0.1:8181";
        private WebSocketServer socketServer;
        private object _lock = new object();
        public delegate void ResponsHandlerCallBackDelegate(SocketRespons respons);
        private static Dictionary<string, List<ResponsHandlerCallBackDelegate>> incomingResponsHandlers = new Dictionary<string, List<ResponsHandlerCallBackDelegate>>();
        private static object _syncRoot = new object();

        public WeChatSdk()
        {
            StartServer();
        }

        /// <summary 
        /// 开启服务
        /// </summary>
        private void StartServer()
        {
            if (socketServer != null && socketServer.ListenerSocket.Connected) return;
            socketServer = new WebSocketServer(url);
            socketServer.Start((connection) =>
            {
                connection.OnMessage = (msg) =>
                {
                    Task.Factory.StartNew(() =>
                    {
                        Receive(connection, msg);
                    });
                };
                connection.OnClose = () =>
                {
                    var pid = userConnections.Values.FirstOrDefault(o => o.Connection.ConnectionInfo.Id == connection.ConnectionInfo.Id)?.ProcessId;
                    if (pid == null || pid == 0) return;
                    lock (_lock)
                        userConnections.Remove(pid.Value);
                    ConnetionCloseEvent?.Invoke(connection, null);
                };
                connection.OnError = (ex) =>
                {
                    if (!connection.IsAvailable)
                    {
                        var pid = userConnections.Values.FirstOrDefault(o => o.Connection.ConnectionInfo.Id == connection.ConnectionInfo.Id)?.ProcessId;
                        if (pid == null || pid == 0) return;
                        lock (_lock)
                            userConnections.Remove(pid.Value);
                        ConnetionCloseEvent?.Invoke(connection, null);
                    }
                };

            });
        }

        private void Receive(IWebSocketConnection connection, string message)
        {
            try
            {
                var temp = System.Text.Encoding.UTF8.GetString(Convert.FromBase64String(message));
                if (string.IsNullOrEmpty(temp)) return;
                var respons = QNSerialize.DeserializeObject<SocketRespons>(temp);

                switch ((Command)respons.Code)
                {
                    case Command.Cmd_Init:
                        UserConnection user = new UserConnection();
                        user.Connection = connection;
                        user.ProcessId = respons.Pid;
                        lock (_lock)
                        {
                            if (userConnections.ContainsKey(respons.Pid))
                                userConnections[respons.Pid] = user;
                            else userConnections.Add(respons.Pid, user);
                        }
                        WeChatInitEvent?.Invoke(connection, new WeChatInitArgs() { Error = respons.Error, IsSuccess = respons.Error == 0, Message = respons.Message, PorcessId = respons.Pid });
                        break;
                    case Command.Cmd_ReceiveMessage:
                        HandlerMessage(connection, respons);
                        break;
                    case Command.Cmd_GetUser:
                        ReceiveLoginUser(connection, respons);
                        break;
                    case Command.Cmd_ContactList:
                        ReceiveContactEvent?.Invoke(connection, respons.Data?.ToObject<Contact>());
                        break;
                    default:
                        List<ResponsHandlerCallBackDelegate> delegates;
                        if (incomingResponsHandlers.TryGetValue(respons.Callback, out delegates))
                        {
                            if (delegates != null && delegates.Count > 0)
                            {
                                delegates[0](respons);
                            }
                        }
                        break;
                }
            }
            catch (Exception ex)
            {
                QNLog.WriteError("Receive", ex, "解析消息通信数据失败");
            }
        }
        private void ReceiveLoginUser(IWebSocketConnection connection, SocketRespons respons)
        {
            if (respons.Error == 0 && respons.Data != null)
            {
                UserConnection user = respons.Data?.ToObject<UserConnection>();

                user.Connection = connection;
                user.ProcessId = respons.Pid;

                lock (_lock)
                {
                    if (userConnections.ContainsKey(respons.Pid))
                        userConnections[respons.Pid] = user;
                    else userConnections.Add(respons.Pid, user);
                }
            }
            List<ResponsHandlerCallBackDelegate> delegates;
            if (incomingResponsHandlers.TryGetValue(respons.Callback, out delegates))
            {
                if (delegates != null && delegates.Count > 0)
                {
                    delegates[0](respons);
                }
            }
        }

        private void HandlerMessage(IWebSocketConnection connection, SocketRespons respons)
        {
            if (respons.Error != 0) return;
            if (respons.Data == null) return;

            switch (respons.Data["Type"].ToObject<int>())
            {
                //todo:根据类型处理不同的消息
                case 1:
                case 2:
                default:
                    ReceiveOtherIMEvent?.Invoke(connection, respons.Data.ToObject<ReceiveOtherIMArgs>());
                    break;
            }
        }

        /// <summary>
        /// 获取登录用户
        /// </summary>
        /// <param name="pid"></param>
        /// <returns></returns>
        public User GetLoginUser(int pid)
        {
            var data = new
            {
                Code = Command.Cmd_GetUser
            };
            var respons = SendData(pid, Command.Cmd_GetUser);
            if (respons.Error != 0) throw new QNException((uint)respons.Error, respons.Message);
            return respons.Data?.ToObject<User>();
        }

        /// <summary>
        /// 检查微信是否登录
        /// </summary>
        /// <param name="pid"></param>
        /// <returns></returns>
        public bool CheckLoginState(int pid)
        {
            var result = SendData(pid, Command.Cmd_CheckLoginStatus);
            var b = result.Data["Status"]?.ToObject<bool>();
            if (b.HasValue) return b.Value;
            return false;
        }

        /// <summary>
        /// 获取联系人
        /// </summary>
        /// <param name="pid"></param>
        public void GetContactList(int pid)
        {
            var data = new
            {
                Code = Command.Cmd_ContactList
            };
            SendData(pid, data);
        }

        /// <summary>
        /// 修改群组名称
        /// </summary>
        /// <param name="pid"></param>
        /// <param name="roomId"></param>
        /// <param name="roomName"></param>
        public void SetRoomName(int pid, string roomId, string roomName)
        {
            var data = new
            {
                Code = Command.Cmd_SetRoomName,
                ChatRoomId = roomId,
                ChatRoomName = roomName
            };
            SendData(pid, data);
        }


        /// <summary>
        /// 发送文本消息
        /// </summary>
        /// <param name="pid">微信进程Id</param>
        /// <param name="wxid">接收消息的Wxid</param>
        /// <param name="message">消息内容</param>
        public void SendTextMessage(int pid, string wxid, string message)
        {
            var data = new
            {
                Code = Command.Cmd_SendTextMessage,
                WxId = wxid,
                Message = message
            };
            SendData(pid, data);
        }


        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="pid"></param>
        /// <param name="command"></param>
        /// <param name="data"></param>
        /// <returns></returns>
        private SocketRespons SendData(int pid, Command command, object data = null)
        {
            if (!userConnections.ContainsKey(pid))
                throw new Exception("当前微信Id不在线。");
            SocketRespons respons = null;
            string key = Guid.NewGuid().ToString();

            JObject obj = null;
            if (data != null)
                obj = QNSerialize.DeserializeObject<JObject>(QNSerialize.SerializeObject(data));
            else obj = new JObject();

            obj.Add("Code", (int)command);
            obj.Add("Callback", key);
            var encode = Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(obj.ToString()));
            userConnections[pid].Connection.Send(encode);

            AutoResetEvent returnWaitSignal = new AutoResetEvent(false);
            ResponsHandlerCallBackDelegate SendReceiveDelegate = (result) =>
            {
                respons = result;
                returnWaitSignal.Set();
            };
            AppendIncomingPacketHandler(key, SendReceiveDelegate);
            if (!returnWaitSignal.WaitOne(30 * 1000))//三十秒超时
            {
                RemoveIncomingPacketHandler(key, SendReceiveDelegate);
                throw new QNException(26, "请求超时。", null);
            }
            RemoveIncomingPacketHandler(key, SendReceiveDelegate);
            if (respons.Error != 0)
            {
                throw new QNException((uint)respons.Error, respons.Message);
            }
            return respons;
        }

        /// <summary>
        /// 发送数据
        /// </summary>
        /// <param name="pid"></param>
        /// <param name="data"></param>
        private void SendData(int pid, object data)
        {
            if (!userConnections.ContainsKey(pid))
                throw new Exception("当前微信Id不在线。");
            userConnections[pid].Connection.Send(Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(QNSerialize.SerializeObject(data))));

        }

        /// <summary>
        /// dll注入
        /// </summary>
        /// <param name="processId"></param>
        public void InjectDll(int processId)
        {
            if (!File.Exists(dllPath))
                throw new Exception("dll文件不存在。");


            //1) 遍历系统中的进程，找到微信进程（CreateToolhelp32Snapshot、Process32Next）
            Process[] processes = Process.GetProcesses();
            Process WxProcess = null;
            foreach (Process process in processes)
            {
                if (process.ProcessName.ToLower() == "WeChat".ToLower())
                {
                    WxProcess = process;
                    foreach (ProcessModule processModule in WxProcess.Modules)
                    {
                        if (processModule.ModuleName == "WeChatExt.dll") return;
                    }
                    break;
                }
            }

            if (WxProcess == null)
            {
                throw new Exception("请先启动微信！");
            }

            //2) 打开微信进程，获得HANDLE（OpenProcess）。

            //3) 在微信进程中为DLL文件路径字符串申请内存空间（VirtualAllocEx）。


            int DllPathSize = dllPath.Length * 2 + 1;
            int MEM_COMMIT = 0x00001000;
            int PAGE_READWRITE = 0x04;
            int DllAddress = PInvoke.VirtualAllocEx((int)WxProcess.Handle, 0, DllPathSize, MEM_COMMIT, PAGE_READWRITE);
            if (DllAddress == 0) throw new Exception("申请dll内存失败。");


            //4) 把DLL文件路径字符串写入到申请的内存中（WriteProcessMemory）
            if (PInvoke.WriteProcessMemory((int)WxProcess.Handle, DllAddress, dllPath, DllPathSize, 0) == false) throw new Exception("内存写入失败。");


            //5) 从Kernel32.dll中获取LoadLibraryA的函数地址（GetModuleHandle、GetProcAddress）
            int module = PInvoke.GetModuleHandleA("Kernel32.dll");
            int LoadLibraryAddress = PInvoke.GetProcAddress(module, "LoadLibraryA");
            if (LoadLibraryAddress == 0) throw new Exception("查找LoadLibraryA地址失败。");

            //6) 在微信中启动内存中指定了文件名路径的DLL（CreateRemoteThread）。
            if (PInvoke.CreateRemoteThread((int)WxProcess.Handle, 0, 0, LoadLibraryAddress, DllAddress, 0, 0) == 0) throw new Exception("执行远程线程失败。");
        }

        /// <summary>
        /// 是否被注入
        /// </summary>
        /// <param name="processId"></param>
        /// <returns></returns>
        public bool IsInjected(int processId)
        {
            var process = Process.GetProcessById(processId);
            if (process.Modules.OfType<ProcessModule>().Any(o => o.FileName.Contains("WeChatExt.dll")))
                return true;
            return false;
        }

        /// <summary>
        /// 打开微信
        /// </summary>
        /// <returns></returns>
        public int OpenWechat()
        {
            //在注册表中查找微信
            //计算机\HKEY_CURRENT_USER\Software\Tencent\WeChat
            //InstallPath
            string WxPath = "";
            try
            {
                RegistryKey registryKey = Registry.CurrentUser;
                RegistryKey software = registryKey.OpenSubKey("Software\\Tencent\\WeChat", true);
                object InstallPath = software.GetValue("InstallPath");
                WxPath = InstallPath.ToString() + "\\WeChat.exe";

                //NeedUpdateType 设置低于100，禁止自动升级
                software.SetValue("NeedUpdateType", 80);

                registryKey.Close();
            }
            catch (Exception ex)
            {
                WxPath = "";
            }
            if (!string.IsNullOrEmpty(WxPath))
            {
                var process = Process.Start(WxPath);
                Thread.Sleep(500);
                return process.Id;
            }
            return 0;
        }


        /// <summary>
        /// 服务是否运行中
        /// </summary>
        public bool IsRunning
        {
            get
            {
                if (socketServer == null || userConnections.Count == 0)
                    return false;
                return true;
            }
        }



        private static void AppendIncomingPacketHandler(string key, ResponsHandlerCallBackDelegate responsHandlerDelgatePointer)
        {

            lock (_syncRoot)
            {
                if (incomingResponsHandlers.ContainsKey(key))
                {

                    incomingResponsHandlers[key].Add(responsHandlerDelgatePointer);
                }
                else
                    incomingResponsHandlers.Add(key, new List<ResponsHandlerCallBackDelegate>() { responsHandlerDelgatePointer });
            }
        }
        private static void RemoveIncomingPacketHandler(string key, ResponsHandlerCallBackDelegate responsHandlerDelgatePointer)
        {
            lock (_syncRoot)
            {
                if (incomingResponsHandlers.ContainsKey(key))
                {
                    ResponsHandlerCallBackDelegate toRemove = null;

                    foreach (var handler in incomingResponsHandlers[key])
                    {
                        if (handler == responsHandlerDelgatePointer)
                        {
                            toRemove = handler;
                            break;
                        }
                    }
                    if (toRemove != null)
                        incomingResponsHandlers[key].Remove(toRemove);
                }
            }
        }


        /// <summary>
        /// 微信注入Hook初始化
        /// </summary>
        public event EventHandler<WeChatInitArgs> WeChatInitEvent;

        /// <summary>
        /// 接收消息事件
        /// </summary>
        public event EventHandler<ReceiveOtherIMArgs> ReceiveOtherIMEvent;

        /// <summary>
        /// 接收联系人事件
        /// </summary>
        public event EventHandler<Contact> ReceiveContactEvent;
        /// <summary>
        /// 连接关闭
        /// </summary>
        public event EventHandler<ConnetionCloseArgs> ConnetionCloseEvent;

        /// <summary>
        /// 日志记录
        /// </summary>
        public event EventHandler<string> LogEvent;
    }
}
