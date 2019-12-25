#include "Code.h"
#include "Channel.h"
#include <Windows.h>
#include <string>
#include <iostream>

#define ORDER_COUNT 100

using namespace std;

string generateOrder();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Channel* chToClient = new Channel(L"ToClient");
	Channel* chToDeveloper = new Channel(L"ToDeveloper");
	Channel* chToManager = new Channel(L"ToManager");
	srand(0);
	bool flag = true;
	int orderN = 0;
	int orderF = 0;
	while (flag) {
		if (orderN < ORDER_COUNT) {
			string order = generateOrder();
			Message* newOrder = new Message(Code::Client, Code::STATE_NEW, order);
			chToDeveloper->putT(newOrder, 2000);
			orderN++;
			cout << "клиент сделал заказ: " << order << endl;
		}
		if (orderF == ORDER_COUNT) {
			cout << "все заказы выполнены" << endl;
			flag = false;
			continue;
		}
		Message* msg = chToClient->get(10000);
		if (msg == nullptr) {
			cout << "клиент не дождался" << endl;
			flag = false;
			continue;
		}
		switch (msg->sender) {
		case Code::Developer:
			switch (msg->code) {
			case Code::STATE_ACCEPT:
				cout << "пришло подтверждение заказа: " << msg->data << endl;
				break;
			case Code::STATE_REJECT:
				cout << "пришел отказ заказа: " << msg->data << endl;
				orderF++;
				break;
			}
			break;
		case Code::Manager:
			if (msg->code == Code::STATE_SUCCESS) {
				cout << "клиент получил готовый заказ: " << msg->data << endl;
				orderF++;
			}
			break;
		}
		if (orderF < ORDER_COUNT) {
			cout << "клиент спрашивает про готовые заказы " << endl;
			chToManager->putT(new Message(Code::Client, Code::REQ_GET_RESULT, ""), 2000);
		}
		Sleep(100);
	}
	cout << "клиент ушел" << endl;
	delete chToDeveloper;
	delete chToClient;
	delete chToManager;
	int t = getchar();
	return 0;
}

string generateOrder() {
	char c = 'a' + (rand() % 26);
	int len = rand() % 60 + 1;
	char* adata = new char[len + 1];
	for (int j = 0; j < len; j++) {
		adata[j] = c;
	}
	adata[len] = 0;
	return string(adata);
}