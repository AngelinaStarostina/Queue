#include <math.h>
#include <time.h>
#include "Header.h"
#include <stack>

using namespace std;

CRITICAL_SECTION criticalSection;
HANDLE em;
HANDLE full;

DWORD WINAPI producer(LPVOID v) 
{
	toThread *parametrs = (toThread*)v;


	for (int i = 0; i < parametrs->number; i++) 
	{
		WaitForSingleObject(em, INFINITE);
		EnterCriticalSection(&criticalSection);

		unsigned int c = rand() % 50;
		parametrs->queue->AddHead(c);
		cout << "Producer: " << parametrs->threadNumder << ". ";
		cout << " Element: " << c << " produced" << endl;
		parametrs->queue->out();
		LeaveCriticalSection(&criticalSection);

		while (!ReleaseSemaphore(full, 1, NULL));
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID v) 
{
	toThread *parametrs = (toThread*)v;
	

	for (int i = 0; i < parametrs->number; i++)
	{
		WaitForSingleObject(full, INFINITE);
		EnterCriticalSection(&criticalSection);

		unsigned short c = parametrs->queue->RemoveTail();
		cout << "Consumer : " << parametrs->threadNumder << ". ";
		cout << " Element : " << c << " removed" << endl;
		parametrs->queue->out();

		LeaveCriticalSection(&criticalSection);

		while (!ReleaseSemaphore(em, 1, NULL));
	}

	return 0;
}

int main() 
{
	setlocale(LC_ALL, "rus");
	srand(time(0));


	cout << "Enter the number of elements: " << endl;
	int size;
	cin >> size;
	MonitorQueue *stack = new MonitorQueue(size);
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
	
	HANDLE *arr = new HANDLE[producerThreadCount + consumerThreadCount];
	int k = 0;

	ThreadData **producerThreadData = new ThreadData*[producerThreadCount + consumerThreadCount];

	for (int i = 0; i < producerThreadCount; i++) {
		producerThreadData[i] = new ThreadData();
		toThread *treadInfo = new toThread(stack, producerProductCount[i], i + 1);
		producerThreadData[i]->handle = CreateThread(NULL, 0, producer, (void*&)(treadInfo), 0, &producerThreadData[i]->id);

		arr[k] = producerThreadData[i]->handle;
		k++;
	}

	ThreadData **consumerThreadData = new ThreadData*[consumerThreadCount];

	for (int i = 0; i < consumerThreadCount; i++) {
		consumerThreadData[i] = new ThreadData();
		toThread *tp = new toThread(stack, consumerProductCount[i], i + 1);
		consumerThreadData[i]->handle = CreateThread(NULL, 0, consumer, (void*)(tp), 0, &consumerThreadData[i]->id);

		arr[k] = consumerThreadData[i]->handle;
		k++;
	}

	WaitForMultipleObjects(producerThreadCount + consumerThreadCount, arr, TRUE, INFINITE);

	for (int i = 0; i < producerThreadCount; i++)
		CloseHandle(producerThreadData[i]->handle);
	for (int i = 0; i < consumerThreadCount; i++)
		CloseHandle(consumerThreadData[i]->handle);

	system("pause");
	return 0;
}