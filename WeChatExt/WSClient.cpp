#include "stdafx.h"
#include "WSClient.h"
#include "Loger.h" 
#include "Command.h" 
#include "websocketpp/base64/base64.hpp"
#include "Init.h"
#include "Util.h" 
#include "EVString.h"

using namespace std;

client wsClient;
client::connection_ptr con;
const string wsurl = "ws://127.0.0.1:8181";

void InitWebSocket() {

Start:
	try
	{

		wsClient.set_access_channels(websocketpp::log::alevel::all);
		wsClient.clear_access_channels(websocketpp::log::alevel::frame_payload);

		wsClient.init_asio();
		wsClient.set_reuse_addr(true);

		wsClient.set_message_handler(bind(&RecvMessage, &wsClient, ::_1, ::_2));
		wsClient.set_close_handler(bind(&OnClose, &wsClient, ::_1));
		wsClient.set_open_handler(bind(&OnOpen, &wsClient, ::_1));

	Next:
		websocketpp::lib::error_code ec;
		con = wsClient.get_connection(wsurl, ec);
		if (ec) {
			Sleep(1000);//发生异常，休眠一秒后再次连接
			goto Next;
		}
		con->set_termination_handler(bind(OnTermination, &wsClient, ::_1));
		con = wsClient.connect(con);
		while (true)
		{
		Run:
			try
			{
				switch (con->get_state())
				{
				case 0:
					Sleep(1000);
					break;
				case 2:
				case 3:
					Sleep(1000);
					goto Next;
				default:
					break;
				}
				wsClient.run_one();
				Sleep(10);
			}
			catch (const std::exception & e)
			{
				string log = e.what();
				Sleep(1000);
				goto Run;
			}

		}
	}
	catch (websocketpp::exception const& e) {
		string log = e.what();
		Sleep(1000);
		goto Start;
	}
}

void StopWebSocket() {
	try
	{
		WriteInfo("WebSocket Stop.");

		if (con != NULL && con->get_state() == websocketpp::session::state::value::open)
		{
			websocketpp::close::status::value cvValue = 0;
			std::string strReason = "";
			con->close(cvValue, strReason);
		}
	}
	catch (...)
	{

	}
}


void OnOpen(client* c, websocketpp::connection_hdl hdl) {
	try {
		InitWeChat();
	}
	catch (...) {
		neb::CJsonObject json;
		Send(Cmd_Init, json, "", 1, L"初始化失败。");
	}
}

void OnClose(client* c, websocketpp::connection_hdl hdl) {

	StopWebSocket();
}

void OnTermination(client* c, websocketpp::connection_hdl hdl) {

	StopWebSocket();
}

void RecvMessage(client* wsclient, websocketpp::connection_hdl hdl, message_ptr msg) {

	string message = msg->get_payload();
	try
	{
		const string  decode = websocketpp::base64_decode(message);

		neb::CJsonObject json(decode);
		int code = 0;
		string callback = "";
		json.Get("Callback", callback);
		if (!json.Get("Code", code)) return;
		neb::CJsonObject  respons;

		switch (code)
		{
		case Cmd_Init:
			InitWeChat();
			break;
		case Cmd_CheckLoginStatus://检查登录状态:
			respons.Add("Status", CheckLogin());
			Send(Cmd_CheckLoginStatus, respons, callback);
			break;
		case Cmd_GetUser://获取用户信息
			if (!CheckLogin()) {
				Send(Cmd_GetUser, respons, callback, 1, L"微信还未登录，不能获取用户信息。");
				break;
			}

			Send(Cmd_GetUser, respons, callback);
			break;
		case Cmd_FriendLists:

			Send(Cmd_FriendLists, respons, callback);
			break;
		case Cmd_ChatRoomLists:
			Send(Cmd_ChatRoomLists, respons, callback);
			break;
		case Cmd_PatchWeChat:
			Send(Cmd_PatchWeChat, respons, callback);
			break;
		case Cmd_SendTextMessage:
			break;
		case Cmd_SendFileMessage:
			break;
		case Cmd_SendImageMessage:
			break;
		case Cmd_SendAtMsg:
			break;
		case Cmd_SetRoomName:
			break;
		}
		json.Clear();
	}
	catch (const std::exception & ex)
	{
		ostringstream ostr;
		ostr << "Source:" << "RecvMessage" << "		" << "Message:" << message.c_str() << "		" << "Exception:" << ex.what() << endl;
		WriteError(ostr.str());
	}
	catch (...) {
		ostringstream ostr;
		ostr << "Source:" << "RecvMessage" << "		" << "Message:" << message.c_str() << "		" << "Exception:" << "未知异常" << endl;
		WriteError(ostr.str());
	}
}


void Send(int code, neb::CJsonObject data, string callback, int err, wstring msg) {
	try
	{
		neb::CJsonObject json;
		json.Add("Code", code);
		json.Add("Error", err);
		json.Add("Message", EVString::w2a(msg.c_str()));
		json.Add("Pid", (long)GetCurrentProcessId());
		json.Add("Callback", callback);
		json.Add("Data", data);
		string data = json.ToString();
		string tmp = websocketpp::base64_encode(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());

		websocketpp::lib::error_code ec;

		wsClient.send(con, tmp, websocketpp::frame::opcode::text, ec);
		if (ec) {
			WriteError("发送数据失败：" + ec.message());
		}
		data.clear();
		json.Clear();
	}
	catch (const std::exception & ex)
	{
		ostringstream ostr;
		ostr << "Source:" << "Send" << "		" << "Data:" << data.ToString() << "		" << "Exception:" << ex.what() << endl;
		WriteError(ostr.str());
	}
	catch (...) {
		ostringstream ostr;
		ostr << "Source:" << "Send" << "		" << "Data:" << data.ToString() << "		" << "Exception:" << "未知异常" << endl;
		WriteError(ostr.str());
	}
}