#pragma once
#include <windows.h>
#include <iostream>

using namespace std;

class MonitorQueue {
private:
	unsigned short *arr;
	int n;
	int num;

public:
	MonitorQueue(int size) {
		n = size;
		arr = new unsigned short[size];
		num = 0;
	}

	~MonitorQueue() {
		delete[]arr;
	}

	void AddHead(unsigned short element) {
		if (num < n) {
			arr[num++] = element;
		}
	}

	unsigned short RemoveTail() {
		if (num > 0) {
			return arr[--num];

		}
	}

	void out() {
		for (int i = 0; i < num; i++) {
			cout << arr[i] << " ";
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

struct ThreadData {
	HANDLE handle;
	DWORD id;
};

