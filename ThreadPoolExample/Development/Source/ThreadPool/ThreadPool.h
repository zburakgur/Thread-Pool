#pragma once

#define _DLLExport __declspec( dllexport )

#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

class IThreadTask
{
public:
	virtual ~IThreadTask(){};

	virtual void implement(CRITICAL_SECTION* const pCritical) = 0;

};

struct ThreadParam
{
	vector<IThreadTask*>* pTasks = NULL;

	CRITICAL_SECTION* pCritical = NULL;
};

class Thread
{
private:
	HANDLE handle;

	ThreadParam param;

	/*
	* Thread object removes task as soon as operate it.
	*/
	vector<IThreadTask*> tasks;

public:

	void addTask(IThreadTask* pTask);

	void create(CRITICAL_SECTION* const pCritical);

	void start();

	int getTaskSize();

	HANDLE getHandle();
};


class _DLLExport ThreadPool
{
private:
	int threadNumber = 0;

	Thread* pThreads = NULL;

	CRITICAL_SECTION critical;
	
public:
	
	ThreadPool(int threadNumber);

	~ThreadPool();

	/*
	* Assings task to thread which has minimum number of task
	*
	* Returns: added thread's index
	*/
	int addTask(IThreadTask* pTask);

	/*
	* Assigns task to the indexed thread.
	* threadIndex is began at 0.
	*/
	void addTask(IThreadTask* pTask, int threadIndex);

	/*
	* Creates windows's thread objects, but not start.
	*/
	void create();

	/*
	* Starts all task assigned threads.
	*/
	void start();

	/*
	* Waits for threads' ending.
	*/
	void wait();
};