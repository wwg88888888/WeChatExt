using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WeChat.Helper
{
    public static class QNLog

    {
        private static object _lock = new object();
        /// <summary>
        /// 记录文件日志
        /// </summary>
        /// <param name="logtype"></param>
        /// <param name="source"></param>
        /// <param name="message"></param>
        /// <param name="detail"></param>
        public static void Write(string logtype, string source, string message, string detail)
        {
            try
            {
                string path = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Logs");
                Directory.CreateDirectory(path);
                string str2 = Path.Combine(path, DateTime.Today.ToString("yyyyMMdd") + ".log");
                string str3 = "********************************" + DateTime.Now.ToString() + "********************************";
                lock (_lock)
                {
                    using (FileStream stream = new FileStream(str2, FileMode.Append, FileAccess.Write, FileShare.Read))
                    {
                        using (StreamWriter writer = new StreamWriter(stream, Encoding.UTF8))
                        {
                            writer.WriteLine(str3);
                            writer.WriteLine("Type: " + logtype);
                            writer.WriteLine("Source: " + source);
                            writer.WriteLine("Message: " + message);
                            writer.WriteLine("Detail: " + detail);
                            string str4 = "";
                            writer.WriteLine(str4.PadLeft(str3.Length, '*'));
                            writer.WriteLine();
                            writer.Flush();
                        }
                    }
                }
            }
            catch
            {
            }
        }

        /// <summary>
        /// 记录错误文件日志
        /// </summary>
        /// <param name="source"></param>
        /// <param name="ex"></param>
        /// <param name="message"></param>
        public static void WriteError(string source, Exception ex, string message = "")
        {
            Write("异常", source, string.IsNullOrEmpty(message) ? ex.Message : message, ex.ToString());
        }
    }
}
