// ThreadTutorial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "ThreadPool.h"

using namespace std;

class TestTask : IThreadTask
{
public:

	int code = 0;

	TestTask(int code) :code(code){}

	void implement(CRITICAL_SECTION* const pCritical)
	{
		for (int i = 0; i < 5; i++)
		{
			EnterCriticalSection(pCritical);
			cout << "task " << code << " iteration " << i << endl;
			LeaveCriticalSection(pCritical);
			Sleep(500);
		}
	}
};

int main()
{
	ThreadPool pThreadPool(2);

	IThreadTask* tasks[5];

	for (int i = 0; i < 5; i++)
	{
		tasks[i] = (IThreadTask*)new TestTask(i);
		pThreadPool.addTask(tasks[i]);
	}

	pThreadPool.create();
	pThreadPool.start();
	pThreadPool.wait();

	for (int i = 0; i < 5; i++)
		delete tasks[i];
	
	getchar();
	return 0;
}

