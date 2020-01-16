
# WeChatExt
WeChatExt是一个基于微信PC hook的项目，可实现微信PC端个人号接口、微信机器人等功能。

注入Dll参考[pcWeChat](https://github.com/codeByDog/pcWechat.git)项目，使用C++开发，控制端使用WPF开发。

其中C++和C#的通信采用WebSocket方式通信，当前控制端也可以使用其他语言开发，只需要实现一个WebScoket服务即可。

# 开发环境以及类库依赖
## 一、开发环境
WeChatPC：2.8.0.112

语言环境：C++、WPF

开发工具：VS2019

分析工具：Cheat Engine 7.0、Ollydbg

## 二、依赖库
1、C++标准库：[Boost 17.2.0](https://www.boost.org/)

2、WebSocket Client:[WebSocketpp](https://github.com/zaphoyd/websocketpp.git)

3、WebSocket Server:[Fleck](https://github.com/statianzo/Fleck.git)

4、Hook功能实现：[pcWeChat](https://github.com/codeByDog/pcWechat.git)
##### 说明
非常感谢[codeByDog](https://github.com/codeByDog)开源的此项目，让我能够接触到逆向相关的知识。

# 功能列表
1、获取登录二维码

2、获取登录信息

3、获取联系列表

4、获取数据库句柄

5、获取聊天信息

6、发送文本消息

7、发送图片消息

8、发送文件消息

9、发送群@消息

10、发送名片消息

11、发送链接消息

12、添加好友

13、删除好友

14、无痕清粉

15、获取群成员

16、设置群名称

17、设置群公告

18、添加群成员

19、删除群群成员

20、退出群

# 声明

本项目仅供技术研究，请勿用于任何商业用途，请勿用于非法用途，如有任何人凭此做何非法事情，均于作者无关，特此声明。
