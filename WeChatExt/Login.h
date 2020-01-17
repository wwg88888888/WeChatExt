#pragma once
#include <Windows.h>

/**
 * 检查微信登录状态
 * @param
 * @return int
 */
bool CheckLogin();

/**
 * 退出微信
 * @param
 * @return void
 */
void LogoutWeChat();


/**
 * 跳转到二维码
 * @param
 * @return void
 */
void GotoQrCode();