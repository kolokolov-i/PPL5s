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
	Channel* chToServer = new Channel(L"ToServer");
	Channel* chToManager = new Channel(L"ToManager");
	Channel* chToMachine = new Channel(L"ToMachine");
	bool flag = true;
	while (flag) {
		Message* msg = chToServer->get();
		switch (msg->sender) {
		case Code::Manager:
			switch (msg->code) {
			case Code::STATE_DEVELOPED: {
				cout << "сервер принимает задание: " << msg->data << endl;
				chToMachine->putT(new Message(Code::Server, Code::STATE_DEVELOPED, msg->data), 2000);
			}break;
			case Code::MACHINE_OFF: {
				cout << "сервер отключает станок" << endl;
				chToMachine->putT(new Message(Code::Server, Code::MACHINE_OFF, ""), 2000);
				flag = false;
			}break;
			}
			break;
		case Code::Machine:
			cout << "сервер оповещает менеджера о готовности: " << msg->data << endl;
			chToManager->putT(new Message(Code::Server, Code::STATE_SUCCESS, msg->data), 2000);
			break;
		}
		Sleep(1);
	}
	cout << "сервер остановлен" << endl;
	delete chToServer;
	delete chToManager;
	delete chToMachine;
	int t = getchar();
	return 0;
}