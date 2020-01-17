#pragma once

void SetChatoomName(wchar_t* roomwxid, wchar_t* roomname);
void SetChatroomAnnouncement(wchar_t* chatroomwxid, wchar_t* Announcement);
void AddChatroomMember(wchar_t* chatroomwxid, wchar_t* wxid);
void DeleteChatroomMember(wchar_t* roomid, wchar_t* memberwxid);
void SendChatroomAtMsg(wchar_t* chatroomid, wchar_t* memberwxid, wchar_t* membernickname, wchar_t* msg);
void GetChatroomUser(wchar_t* chatroomid, string callback);
void QuitChatRoom(wchar_t* chatroomwxid);