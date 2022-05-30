#pragma once
#include <windows.h>
#include <iostream>

using namespace std;

class MonitorQueue {
private:
	unsigned short *queue;
	int n;
	int num;

public:
	MonitorQueue(int size) {
		n = size;
		queue = new unsigned short[size];
		num = 0;
	}

	~MonitorQueue() {
		delete[]queue;
	}

	void AddHead(unsigned short element) {
		if (num < n) {
			queue[num++] = element;
		}
	}

	unsigned short RemoveTail() {
		if (num > 0) {
			return queue[--num];

		}
	}

	void getQueue() {
		for (int i = 0; i < num; i++) {
			cout << queue[i] << " ";
		}
		cout << endl;
	}


};

struct toThread {
	MonitorQueue *queue;
	int number;
	int threadNumder;

	toThread(MonitorQueue *&q, int num, int threadNum) {
		queue = q;
		number = num;
		threadNumder = threadNum;
	}
};