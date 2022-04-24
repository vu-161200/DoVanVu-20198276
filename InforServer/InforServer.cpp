//2. Chương trình serverinfo đợi kết nối từ clientinfo và thu nhận thông tin từ client, hiện kết quả ra màn hình.Cổng mà server đợi kết nối nhận vào từ tham số dòng lệnh.

//VD : serverinfo.exe 1234
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

int main()
{
    printf("SERVER\n\n");

    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));

    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);

    // Cho ket noi moi
    SOCKET client = accept(listener, NULL, NULL);

    char buf[256];

    while (1)
    {
        int ret = recv(client, buf, sizeof(buf), 0);

        if (ret <= 0)
        {
            if (ret == -1)
                ret = WSAGetLastError();
            break;
        }

        if (ret < sizeof(buf))
            buf[ret] = 0;
        printf("%s", buf);
       
    }
    

    // Dong ket noi va giai phong thu vien
    closesocket(client);
    closesocket(listener);
    WSACleanup();
}
