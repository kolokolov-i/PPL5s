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
	Channel* chToDeveloper = new Channel(L"ToDeveloper");
	Channel* chToClient = new Channel(L"ToClient");
	Channel* chToManager = new Channel(L"ToManager");
	bool flag = true;
	while (flag) {
		Message* msg = chToDeveloper->get();
		string order;
		order = msg->data;
		cout << "������� ������� �����: " << order << endl;
		if (order.size() > 40) {
			cout << "����� ��������, ������� ������� ������: " << order << endl;
			chToClient->putT(new Message(Code::Developer, Code::STATE_REJECT, order), 2000);
		}
		else {
			cout << "����� ������: " << order << endl;
			Sleep(10);
			chToClient->putT(new Message(Code::Developer, Code::STATE_ACCEPT, order), 2000);
			chToManager->putT(new Message(Code::Developer, Code::STATE_DEVELOPED, order), 2000);
			cout << "����� ������� ���������" << endl;
		}
	}
	cout << "������� ����" << endl;
	delete chToDeveloper;
	delete chToClient;
	delete chToManager;
	int t = getchar();
	return 0;
}