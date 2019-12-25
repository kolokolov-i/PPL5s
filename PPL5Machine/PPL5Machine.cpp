#include "Code.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "Channel.h"

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Channel* chToMachine = new Channel(L"ToMachine");
	Channel* chToServer = new Channel(L"ToServer");
	bool flag = true;
	while (flag) {
		Message* msg = chToMachine->get(10000);
		if (msg == nullptr) {
			flag = false;
			cout << "������ ���������������" << endl;
			continue;
		}
		if (msg->sender == Code::Server) {
			switch (msg->code) {
			case Code::STATE_DEVELOPED: {
				const char* src = msg->data.c_str();
				int len = msg->data.length();
				char* res = new char[len + 1];
				char dif = 'a' - 'A';
				for (int i = 0; src[i] != '\0'; i++) {
					res[i] = src[i] - dif;
				}
				res[len] = '\0';
				Sleep(10);
				chToServer->putT(new Message(Code::Machine, Code::STATE_SUCCESS, std::string(res)), 2000);
				cout << "������ �����������: " << res << endl;
			}break;
			case Code::MACHINE_OFF: {
				cout << "������ ������ � ����������" << endl;
				flag = false;
			}break;
			}
		}
	}
	cout << "������ ����������" << endl;
	delete chToMachine;
	delete chToServer;
	int t = getchar();
	return 0;
}