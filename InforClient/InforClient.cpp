//Viết chương trình client / server thực hiện các chức năng theo yêu cầu :

//1. Chương trình clientinfo thực hiện kết nối đến máy chủ và gửi thông tin về tên máy, danh sách các ổ đĩa có trong máy, kích thước các ổ đĩa.Địa chỉ(tên miền) và cổng kết nối đến server nhận vào từ tham số dòng lệnh.

//VD : clientinfo.exe localhost 1234

//Gợi ý : sử dụng các hàm API của Windows


//+ GetComputerNameA() - Trả về tên của máy tính

//+ GetLogicalDriveStringsA() - Trả về danh sách ký tự ổ đĩa của máy tính

//+ GetDiskFreeSpaceA() - Trả về kích thước của ổ đĩa


#include <stdio.h>
#include <WinSock2.h>

#include <windows.h>
#include <iostream>
#include <lmcons.h>
#include <codecvt>
#include <sstream>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

using namespace std;

int main()
{
    printf("CLIENT\n\n");

    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Khoi tao socket
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8000);

    system("pause");

    // Ket noi toi server
    int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        ret = WSAGetLastError();
        printf("Ket noi khong thanh cong - %d\n", ret);
        return 1;
    }

    // Gửi thông tin  về tên máy tới server
    send(client, "Ten may: ", strlen("Ten may: "), 0);

    // Lấy tên của máy tính
    TCHAR computerName[UNCLEN + 1];
    DWORD computer_len = UNCLEN + 1;
    GetComputerName((TCHAR*)computerName, &computer_len);

    // Convert TCHAR sang char
    char msg[500];
    wcstombs(msg, computerName, wcslen(computerName) + 1);

    send(client, msg, strlen(msg), 0);

    system("pause");

    //Gửi danh sách ổ đĩa và dung lượng của nó
    send(client, "\n\nDanh sach o dia va kich thuoc cua tung o dia có trong may tính:\n", strlen("\n\nDanh sach o dia va kich thuoc cua tung o dia:\n"), 0);

    char* drives = new char[MAX_PATH]();
    string nameDisk = "C:\\\\";
    ULARGE_INTEGER FreeBytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    GetLogicalDriveStringsA(MAX_PATH, drives);

    for (int i = 0; i < 100; i += 4)
        if (drives[i] != (char)0)
        {
            nameDisk[0] = drives[i];    
            send(client, nameDisk.substr(0, 2).c_str(), strlen(nameDisk.substr(0, 2).c_str()), 0);

            wstring wst_nameDisk = converter.from_bytes(nameDisk);
            LPCWSTR nameDisk_convert = wst_nameDisk.c_str();

            GetDiskFreeSpaceEx(nameDisk_convert, &FreeBytesAvailable, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);

            string totalSize = "\nTong dung luong tren o dia: " + to_string(TotalNumberOfBytes.QuadPart / 1024.0 / 1024 / 1024) + " GB";
            send(client, totalSize.c_str(), strlen(totalSize.c_str()), 0);

            string freeAvailable = "\nTong dung luong trong co san: " + to_string(FreeBytesAvailable.QuadPart / 1024.0 / 1024 / 1024) + " GB";
            send(client, freeAvailable.c_str(), strlen(freeAvailable.c_str()), 0);

            string totalFree = "\nDung luong trong tren o dia: " + to_string(TotalNumberOfFreeBytes.QuadPart / 1024.0 / 1024 / 1024) + " GB\n\n";
            send(client, totalFree.c_str(), strlen(totalFree.c_str()), 0);

        }
    // Đóng kết nối và giải phóng thư viện
    closesocket(client);
    WSACleanup();
}