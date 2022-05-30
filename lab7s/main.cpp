#include <time.h>
#include "Header.h"
using namespace std;

CRITICAL_SECTION criticalSection;
HANDLE em;
HANDLE full;

DWORD WINAPI producer(LPVOID v) 
{
	toThread *parametrs = (toThread*)v;


	for (int i = 0; i < parametrs->number; i++) 
	{
		if (WaitForSingleObject(em, 10000) == WAIT_TIMEOUT)
		{
			cout << "The queue has not been released" << endl;
			break;
		}
		EnterCriticalSection(&criticalSection);

		unsigned int ui = rand() % 50;
		parametrs->queue->AddHead(ui);
		cout << "Producer: " << parametrs->threadNumder << ". ";
		cout << " Element: " << ui << " produced" << endl;
		parametrs->queue->getQueue();
		LeaveCriticalSection(&criticalSection);

		ReleaseSemaphore(full, 1, NULL);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID v) 
{
	toThread *parametrs = (toThread*)v;
	
	for (int i = 0; i < parametrs->number; i++)
	{
		if (WaitForSingleObject(full, 10000) == WAIT_TIMEOUT)
		{
			cout << "No elements were added to an empty queue" << endl;
			break;
		}
		EnterCriticalSection(&criticalSection);

		unsigned short c = parametrs->queue->RemoveTail();
		cout << "Consumer : " << parametrs->threadNumder << ". ";
		cout << " Element : " << c << " removed" << endl;
		parametrs->queue->getQueue();

		LeaveCriticalSection(&criticalSection);

		ReleaseSemaphore(em, 1, NULL);
	}

	return 0;
}

int main() 
{
	setlocale(LC_ALL, "rus");

	cout << "Enter the number of elements: " << endl;
	int size;
	cin >> size;
	MonitorQueue *queue = new MonitorQueue(size);
	em = CreateSemaphore(NULL, size, size, "Empty");
	full = CreateSemaphore(NULL, 0, size, "Full");
	InitializeCriticalSection(&criticalSection);


	int consumerThreadCount;
	int producerThreadCount;
	cout << "Enter the number of producer threads and the number of consumer threads : ";	
	cin >> producerThreadCount;
	cin >> consumerThreadCount;

	int product;
	int *producerProductCount = new int[producerThreadCount];
	for (int i = 0; i < producerThreadCount; i++) 
	{
		cout << "Enter quantity of products for " << i + 1 << " producer:" << endl;
		cin >> product;

		producerProductCount[i] = product;
	}

	int *consumerProductCount = new int[consumerThreadCount];
	for (int i = 0; i < consumerThreadCount; i++)
	{
		cout << "Enter quantity of products for " << i + 1 << " consumer: " << endl;
		cin >> product;

		consumerProductCount[i] = product;
	}
	
	int k = 0;

	DWORD *threadID = new DWORD[producerThreadCount + consumerThreadCount];
	HANDLE *threadHandle = new HANDLE[producerThreadCount + consumerThreadCount];

	for (int i = 0; i < producerThreadCount; i++) {
		srand(time(0));
		toThread *threadInfo = new toThread(queue, producerProductCount[i], i + 1);
		threadHandle[i] = CreateThread(NULL, 0, producer, (void*&)(threadInfo), 0, &threadID[i]);
		k++;
	}

	for (int i = 0; i < consumerThreadCount; i++) {
		toThread *threadInfo = new toThread(queue, consumerProductCount[i], i + 1);
		threadHandle[k] = CreateThread(NULL, 0, consumer, (void*)(threadInfo), 0, &threadID[k]);
		k++;
	}

	WaitForMultipleObjects(producerThreadCount + consumerThreadCount, threadHandle, TRUE, INFINITE);

	for (int i = 0; i < k; i++)
		CloseHandle(threadHandle[i]);

	system("pause");
	return 0;
}