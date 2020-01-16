#pragma once
//----------个人信息相关 Start----------
#define MY_CONFIG				0x1618740	//微信ID
#define MY_WXID					0x1618774	//微信ID
#define MY_ACCOUNT				0x1618950	//微信号
#define MY_ACCOUNT2				0x1618BD8	//微信号
#define MY_NICKNAME				0x16187EC	//昵称
#define MY_EMAIL				0x1618808	//邮箱
#define MY_MOBILE				0x1618820	//手机号
#define MY_PROVINCE				0x16188F0	//省份
#define MY_CITY					0x16188D8	//城市
#define MY_NATION				0x16189C8	//国籍
#define MY_DEVICE				0x1618C28	//设备
#define MY_BIGHEAD				0x1618AB4	//大头像
#define MY_SMALLHEAD			0x1618ACC	//小头像
#define MY_SEX					0x16188D4	//性别
#define MY_SIGNNAME				0x1618908	//个性签名
#define MY_RANDOM_STR			0x1618998	//随机字符串
#define MY_PUBLIC_KEY			0x1618C44	//公钥
#define MY_PRIVATE_KEY			0x1618C5C	//私钥
//----------个人信息相关 Over----------

#define ISLOGIN					0x16188C4	//是否登录标识，0-未登录|1-已登录

#define LOGOUT					0x43DB70	//退出登录

#define LOGINQRCODESTR			0x1631A88	//登录二维码字符串：http://weixin.qq.com/x/ 加上该值是生成的二维码是登录二维码

#define GOTOQRCODE1				0x20F490	//跳转到二维码界面
#define GOTOQRCODE2				0x31F490	//跳转到二维码界面

#define RECIEVEHOOKADDR			0x34F4A7	//接收消息
#define RECIEVEHOOKCALL			0x1D0980	//接收消息

#define SENDTEXTADDR			0x327A20	//发送文本消息ecx参数偏移

#define SENDIMGADDR1			0x4E4F0		//发送图片文件call地址偏移
#define SENDIMGADDR2			0x327410	//发送图片文件call地址偏移
#define SENDIMGADDR3			0x6DDF0		//因微信发送图片会占用图片，该偏移是为了释放图片占用

#define SENDATTACHADDR1			0x4DC8F0	//发送附件
#define SENDATTACHADDR2			0x4DC930	//发送附件
#define SENDATTACHADDR3			0x543F0		//发送附件
#define SENDATTACHADDR4			0x25D7B0	//发送附件
#define SENDATTACHPARAM			0x13658A8	//发送附件参数

#define SENDXMLCARD				0x327A20	//发送xml名片
#define SENDXMLARTICLE			0x25D4B0	//发送xml文章

#define DATABASEHOOKADDR		0x47F903	//数据库hook
#define DATABASERUNCALL			0x8C4430	//数据库执行call

#define COLLECTMONEYCALL1		0x7A7A90	//收款CALL1
#define COLLECTMONEYCALL2		0x7A7B10	//收款CALL2

#define ADDCARDUSERPARAM		0x1366C34	//添加好友
#define ADDCARDUSERCALL1		0x52260		//添加好友
#define ADDCARDUSERCALL2		0x6E620		//添加好友
#define ADDCARDUSERCALL3		0x4DC8F0	//添加好友
#define ADDCARDUSERCALL4		0x4DC930	//添加好友
#define ADDCARDUSERCALL5		0x2A9F80	//添加好友

#define AGREEUSERREQUESTPARAM	0x1618E48	//同意好友请求
#define AGREEUSERREQUESTCALL1	0x1A4230	//同意好友请求
#define AGREEUSERREQUESTCALL2	0x482D0		//同意好友请求
#define AGREEUSERREQUESTCALL3	0xE4CC0		//同意好友请求
#define AGREEUSERREQUESTCALL4	0x188BD0	//同意好友请求

#define ADDUSERFROMWXIDPARAM	0x1366C34	//通过wxid添加好友
#define ADDUSERFROMWXIDCALL1	0x52260		//通过wxid添加好友
#define ADDUSERFROMWXIDCALL2	0x6E620		//通过wxid添加好友
#define ADDUSERFROMWXIDCALL3	0x4DC8F0	//通过wxid添加好友
#define ADDUSERFROMWXIDCALL4	0x4DC930	//通过wxid添加好友
#define ADDUSERFROMWXIDCALL5	0x2A9F80	//通过wxid添加好友

#define SETCHATROOMANNOUNCEMENT 0x2A1450	//设置群公告
#define SETCHATROOMNAME			0x29DC00	//设置群名称

#define QUITCHATROOM			0x29E730	//退出群聊

#define DELETECHATROOMMEMBER1	0x4DC930	//删除群成员
#define DELETECHATROOMMEMBER2	0x66050		//删除群成员
#define DELETECHATROOMMEMBER3	0x29950		//删除群成员

#define ADDCHATROOMMEMBER1		0x91F60004	//添加群成员
#define ADDCHATROOMMEMBER2		0x4DCA40	//添加群成员
#define ADDCHATROOMMEMBER3		0x91F60004	//添加群成员
#define ADDCHATROOMMEMBER4		0x2992E0	//添加群成员
#define ADDCHATROOMPARAM1		0x1618774	//添加群成员
#define ADDCHATROOMPARAM2		0x91F60000	//添加群成员

#define GETCHATROOMUSERCALL1	0x465D10	//获取群成员wxid
#define GETCHATROOMUSERCALL2	0x2F7530	//获取群成员wxid
#define GETCHATROOMUSERCALL3	0x46C650	//获取群成员wxid
#define GETCHATROOMUSERCALL4	0x466800	//获取群成员wxid

#define DELETEFRIEND			0x2ACF70	//删除好友

#define FRIENDHOOKADDR			0x477A78	//好友列表call
#define FRIENDHOOKCALL			0x4DCF50	//好友列表call

#define WxUpdateMemberDetails1 0x4DC930  //网络好友数据更新 482330
#define WxUpdateMemberDetails2 0x4E430  //网络好友数据更新	4FFF0
#define WxUpdateMemberDetails3 0x2ABA90 //网络好友数据更新	273820
#define WxUpdateMemberDetails4 0xF9B20  //网络好友数据更新	E27E0
#define WxUpdateMemberDetails5 0x224C80  //网络好友数据更新	2042A0
#define WxUpdateMemberDetails6 0x13C91C0 //网络好友数据更新