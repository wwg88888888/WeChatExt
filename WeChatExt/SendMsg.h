#pragma once

/**
 * 发送文本消息
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendTextMsg(wchar_t* wxid, wchar_t* msg);

/**
 * 发送图片文件
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendImageMsg(wchar_t* wxid, wchar_t* filepath);

/**
 * 发送附件文件
 * @param wchar_t* wxid, string content
 * @return void
 */
void SendAttachMsg(wchar_t* wxid, wchar_t* filepath);


/**
 * 发送好友名片
 * @param wchar_t* wxid, wchar_t* xmlData
 * @return void
 */
void SendXmlCard(wchar_t* RecverWxid, wchar_t* xml);

/**
 * 发送文章链接
 * @param wchar_t* RecverWxid, wchar_t* FromWxid, wchar_t* xmlData
 * @return void
 */
void SendXmlArticle(wchar_t* RecverWxid, wchar_t* FromWxid, wchar_t* xmlData);