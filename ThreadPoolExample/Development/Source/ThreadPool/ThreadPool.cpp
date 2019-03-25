#include "stdafx.h"
#include "ThreadPool.h"

/* Thread Function */

DWORD WINAPI ThreadFunc(LPVOID lParam)
{
	ThreadParam* pParam = (ThreadParam*)lParam;

	vector<IThreadTask*>::iterator it = pParam->pTasks->begin();

	for (; it != pParam->pTasks->end();)
	{
		(*it)->implement(pParam->pCritical);
		it = pParam->pTasks->erase(it);
	}

	return 0;
}


/* Thread Class Functions */

void Thread::addTask(IThreadTask* pTask)
{
	tasks.push_back(pTask);
}

void Thread::create(CRITICAL_SECTION* pCritical)
{
	DWORD dwThreadID;

	param.pTasks = &tasks;
	param.pCritical = pCritical;

	handle = CreateThread(NULL, 0, ThreadFunc, &param, CREATE_SUSPENDED, &dwThreadID);
}

void Thread::start()
{
	ResumeThread(handle);
}

int Thread::getTaskSize()
{
	return tasks.size();
}

HANDLE Thread::getHandle()
{
	return handle;
}


/* ThreadPool Class Functions*/

ThreadPool::ThreadPool(int threadNumber) : threadNumber(threadNumber)
{
	pThreads = new Thread[threadNumber];
}

ThreadPool::~ThreadPool()
{
	delete[] pThreads;
}

int ThreadPool::addTask(IThreadTask* pTask)
{
	int minSize = 10000;
	int index = -1;
	for (int i = 0; i < threadNumber; i++)
	{
		int taskSize = pThreads[i].getTaskSize();
		if (taskSize < minSize)
		{
			minSize = taskSize;
			index = i;
		}
	}

	pThreads[index].addTask(pTask);
	return index;
}

void ThreadPool::addTask(IThreadTask* pTask, int threadIndex)
{
	if (threadIndex >= threadNumber || threadIndex < 0)
		return;

	pThreads[threadIndex].addTask(pTask);
}

void ThreadPool::create()
{
	for (int i = 0; i < threadNumber; i++)
		pThreads[i].create(&critical);

	InitializeCriticalSection(&critical);
}

void ThreadPool::start()
{
	for (int i = 0; i < threadNumber; i++)
		pThreads[i].start();
}

void ThreadPool::wait()
{
	HANDLE* pHandles = new HANDLE[threadNumber];
	for (int i = 0; i < threadNumber; i++)
		pHandles[i] = pThreads[i].getHandle();

	WaitForMultipleObjects(threadNumber, pHandles, true, INFINITE);

	for (int i = 0; i < threadNumber; i++)
		CloseHandle(pHandles[i]);

	delete[] pHandles;

	DeleteCriticalSection(&critical);
}