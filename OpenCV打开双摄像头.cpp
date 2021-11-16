#include<iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#pragma comment(lib, "ws2_32.lib")
DWORD WINAPI CreateClientThread1(LPVOID lpParameter);
DWORD WINAPI CreateClientThread2(LPVOID lpParameter);

bool a1 = true;
bool a2 = true;
Mat as1;
Mat as2;
int main() {

	HANDLE  h_thread1 = ::CreateThread(nullptr, 0, CreateClientThread1, (LPVOID)"192.168.1.123", 0, nullptr);
	HANDLE  h_thread2 = ::CreateThread(nullptr, 0, CreateClientThread2, (LPVOID)"192.168.1.124", 0, nullptr);
	while (true)
	{

		if (!a1) {
			imshow("kkk", as1); a1 = true;
			waitKey(10);
		}
		if (!a2) {
			imshow("qqq", as2); a2= true;
			waitKey(10);
		}
	}
	//--------------------------------------------------------------------------------------------------
	
	//InternetCloseHandle(internet);

	return 0;
}
DWORD WINAPI CreateClientThread1(LPVOID lpParameter)
{
	const char SOI_MARKER[] = { (byte)0xFF, (byte)0xD8 };
	//加载套接字库
	WSADATA wsaData;
	int iRet = 0;
	const 	char* a = "GET /stream HTTP/1.1\r\nConnection:Close\r\n\r\n";
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	//创建套接字
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		return -1;
	}
	

	//初始化服务器端地址族变量
	SOCKADDR_IN srvAddr;
	inet_pton(AF_INET, (char*)lpParameter, &srvAddr.sin_addr);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(81);
	//连接服务器
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (0 != iRet)
	{
		cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}

	int x = 0;

	while (true)
	{
		x++;
		//发送消息

		send(clientSocket, a, strlen(a), 0);
		while (true)
		{
			//接收消息
			char q,recvBuf1[10000]; int jj = 0, sig, j = 0; bool t = true;
			for (int i = 0; t; i++) {
				sig = recv(clientSocket, &q, 1, 0);
				recvBuf1[i] = q;
				if (q == SOI_MARKER[j])
				{
					j++;
					if (j == 2) {
						j = 0;		recvBuf1[i] = '\0';
						char* b = strstr(recvBuf1, "Content-Length");
						b = strstr(b, ":");
						jj = atoi(b + 1);
						t = false;
					};
				}
				else if (q != SOI_MARKER[0] && j == 1)j = 0;//else j = 0;
			}
			jj -= 2; int js = 2; sig = 0;
			uchar* cs1 = new uchar[jj]; vector<uchar> jpg_buff;
			cs1[0] = SOI_MARKER[0]; cs1[1] = SOI_MARKER[1];
			while (jj > 0)
			{
				sig = recv(clientSocket, (char*)cs1 + js, jj, 0);
				if (sig <= 0)
				{
					printf("Error: Lost connection!\n");
					break;
				}
				jj -= sig; js += sig;
			}
			_InputArray pic_arr(cs1, js);
			if (a1) {
				as1 = imdecode(pic_arr, IMREAD_UNCHANGED);//图像解码
				a1 = false;
				
			}

		}
		if (x == 5)
		{
			break;
		}
	}
	
	//清理
	closesocket(clientSocket);
	WSACleanup();
	
	return 0;
}
DWORD WINAPI CreateClientThread2(LPVOID lpParameter)
{
	const char SOI_MARKE[] = { (byte)0xFF, (byte)0xD8 };
	//加载套接字库
	WSADATA wsaData;
	int iRet = 0;
	const 	char* a = "GET /stream HTTP/1.1\r\nConnection:Close\r\n\r\n";
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		return -1;
	}


	//初始化服务器端地址族变量
	SOCKADDR_IN srvAddr;
	inet_pton(AF_INET, (char*)lpParameter, &srvAddr.sin_addr);
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(81);
	//连接服务器
	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (0 != iRet)
	{
		cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << endl;
		return -1;
	}

	int x = 0;

	while (true)
	{
		x++;
		//发送消息

		send(clientSocket, a, strlen(a), 0);
		while (true)
		{
			//接收消息
			char q, recvBuf1[10000]; int jj = 0, sig, j = 0; bool t = true;
			for (int i = 0; t; i++) {
				sig = recv(clientSocket, &q, 1, 0);
				recvBuf1[i] = q;
				if (q == SOI_MARKE[j])
				{
					j++;
					if (j == 2) {
						j = 0;		recvBuf1[i] = '\0';
						char* b = strstr(recvBuf1, "Content-Length");
						b = strstr(b, ":");
						jj = atoi(b + 1);
						t = false;
					};
				}
				else if (q != SOI_MARKE[0] && j == 1)j = 0;//else j = 0;
			}
			jj -= 2; int js = 2; sig = 0;
			uchar* cs1 = new uchar[jj]; vector<uchar> jpg_buff;
			cs1[0] = SOI_MARKE[0]; cs1[1] = SOI_MARKE[1];
			while (jj > 0)
			{
				sig = recv(clientSocket, (char*)cs1 + js, jj, 0);
				if (sig <= 0)
				{
					printf("Error: Lost connection!\n");
					break;
				}
				jj -= sig; js += sig;
			}
			_InputArray pic_arr(cs1, js);
			if (a2) {
				as2 = imdecode(pic_arr, IMREAD_UNCHANGED);//图像解码
				a2 = false;

			}

		}
		if (x == 5)
		{
			break;
		}
	}

	//清理
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}
