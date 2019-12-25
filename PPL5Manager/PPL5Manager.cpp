#include "Code.h"
#include "Channel.h"
#include <Windows.h>
#include <string>
#include <queue>
#include <iostream>

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Channel* chToManager = new Channel(L"ToManager");
	Channel* chToClient = new Channel(L"ToClient");
	Channel* chToServer = new Channel(L"ToServer");
	std::queue<Message*> storehouse;
	bool flag = true;
	while (flag) {
		Message* msg = chToManager->get(5000);
		if (msg == nullptr) {
			flag = false;
			cout << "больше обращений нет" << endl;
			chToServer->putT(new Message(Code::Manager, Code::MACHINE_OFF, ""), 2000);
			cout << "менеджер отключает станки" << endl;
			continue;
		}
		switch (msg->sender) {
		case Code::Developer:
			cout << "менеджер отправляет задание на сервер: " << msg->data << endl;
			chToServer->putT(new Message(Code::Manager, Code::STATE_DEVELOPED, msg->data), 2000);
			break;
		case Code::Server:
			cout << "менеджер кладёт готовый заказ на склад: " << msg->data << endl;
			storehouse.push(new Message(Code::Manager, Code::STATE_SUCCESS, msg->data));
			break;
		case Code::Client:
			switch (msg->code) {
			case Code::REQ_GET_RESULT:
				if (!storehouse.empty()) {
					cout << "на складе лежат готовые заказы: " << storehouse.size() << endl;
					bool r = chToClient->putT(storehouse.front(), 10);
					if (r) {
						cout << "менеджер отдал готовый заказ: " << storehouse.front()->data << endl;
						storehouse.pop();
					}
				}
				break;
			case Code::REQ_CLAIM:
				cout << "пришла жалоба, переделывать: " << msg->data << endl;
				chToServer->putT(new Message(Code::Manager, Code::STATE_DEVELOPED, msg->data), 1000);
				break;
			}
			break;
		}
		Sleep(10);
	}
	cout << "менеджер ушел" << endl;
	delete chToManager;
	delete chToClient;
	delete chToServer;
	int t = getchar();
	return 0;
}