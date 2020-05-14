// win32api_server.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

////////////////////////////////////////////////////////////////
#include <iostream>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

SOCKET sockSer;
SOCKADDR_IN addrSer,addrCli;
SOCKET sockConn;
////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	int nErrCode;
    WORD wVersionReauested;
    WSADATA wsaData;
    wVersionReauested = MAKEWORD(2, 2);
    nErrCode = WSAStartup(wVersionReauested, &wsaData);
    if (nErrCode != 0)
    {
        return -1;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return -1;
    }
	sockSer = socket(AF_INET,SOCK_STREAM,0);
	// TODO: Place code here.
	//////////////////////////////////////////////////////////////
	DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, NULL, (DLGPROC)About);
	return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char port[5];
    char ip[16];

    int len = sizeof(SOCKADDR);

    char sendbuf[256];
    char recvbuf[256];

	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			switch ( LOWORD(wParam))// 拦截系统消息
            {
            case IDC_QUIT:
                EndDialog(hDlg, LOWORD(wParam));
                closesocket(sockSer);
                WSACleanup();
                return TRUE;
            case IDC_CREATE:
                GetDlgItemText(hDlg, IDC_IP, ip, 16);
                GetDlgItemText(hDlg, IDC_PORT, port, 5);

                addrSer.sin_family = AF_INET;
                addrSer.sin_port = htons(atoi(port));
                addrSer.sin_addr.S_un.S_addr = inet_addr(ip);
                bind(sockSer, (SOCKADDR*)&addrSer, len);

                listen(sockSer, 5);
                sockConn = accept(sockSer, (SOCKADDR*)&addrCli, &len);
                if (sockConn == INVALID_SOCKET)
                {
                    MessageBox(NULL, "创建服务器 失败", "创建服务器", 0);
                }
                else
                {
                    MessageBox(NULL, "创建服务器 成功", "创建服务器", 0);
                }
                break;
            case IDC_SEND:
                GetDlgItemText(hDlg, IDC_SENDBUF, sendbuf, 256);
                send(sockConn, sendbuf, strlen(sendbuf) + 1, 0);
                SetDlgItemText(hDlg, IDC_SENDBUF, "");
                break;
            case IDC_RECV:
                recv(sockConn, recvbuf, 256, 0);
                SetDlgItemText(hDlg, IDC_RECVBUF, recvbuf);
                break;
            }
            if (LOWORD(wParam) == IDOK || LOWORD(wParam)==IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                closesocket(sockSer);
                WSACleanup();
                return TRUE;
            }
            break;
	}
    return FALSE;
}
