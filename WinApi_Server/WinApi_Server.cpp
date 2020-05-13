// WinApi_Server.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WinApi_Server.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

///////////////////////////////////////////////////////////////////////////////
SOCKET sockSer;
SOCKADDR_IN addrSer,addrCli;
SOCKET sockConn;
///////////////////////////////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
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

    sockSer = socket(AF_INET, SOCK_STREAM, 0);
    /////////////////////////////////////////////////////////////////////////
    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, About);
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    char port[10];
    char ip[20];
    //char *port = new char[10];
    //char *ip = new char[20];
    int len = sizeof(SOCKADDR);

    //char *sendbuf = new char[256];
    //char *recvbuf = new char[256];
    char sendbuf[256];
    char recvbuf[256];
    ////////////////////////////////////////////////////////////////////////
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))// 拦截系统消息
            {
            case IDC_QUIT:
                EndDialog(hDlg, LOWORD(wParam));
                closesocket(sockSer);
                WSACleanup();
                return (INT_PTR)TRUE;
            case IDC_CREATE:
                GetDlgItemText(hDlg, IDC_IP, LPWSTR(ip), 20);
                GetDlgItemText(hDlg, IDC_PORT, LPWSTR(port), 10);

                addrSer.sin_family = AF_INET;
                addrSer.sin_port = htons(atoi(port));
                addrSer.sin_addr.S_un.S_addr = inet_addr(ip);
                bind(sockSer, (SOCKADDR*)&addrSer, len);

                listen(sockSer, 5);
                sockConn = accept(sockSer, (SOCKADDR*)&addrCli, &len);
                if (sockConn == INVALID_SOCKET)
                {
                    MessageBox(NULL, L"创建服务器 失败", L"创建服务器", 0);
                }
                else
                {
                    MessageBox(NULL, L"创建服务器 成功", L"创建服务器", 0);
                }
                break;
            case IDC_SEND:
                GetDlgItemText(hDlg, IDC_SENDBUF, LPWSTR(sendbuf), 256);
                send(sockConn, sendbuf, strlen(sendbuf) + 1, 0);
                SetDlgItemText(hDlg, IDC_SENDBUF, LPWSTR(""));
                break;
            case IDC_RECV:
                recv(sockConn, recvbuf, 256, 0);
                SetDlgItemText(hDlg, IDC_RECVBUF, LPWSTR(recvbuf));
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
    }
    return (INT_PTR)FALSE;
}
