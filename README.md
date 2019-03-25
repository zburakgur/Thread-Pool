# Thread-Pool
Thread Pool Pattern Example - Visual C++

There is an example of thread pool design pattern in this repository which is developed in Visual C++.

A thread pool is a group of threads and each thread performs several tasks in sequence. 
Instead of creating a new thread for all task, a group of specific number threads are used 
to accomplish tasks, therefore, application will be high performance and scalable.

As everybody knows, multithreading provides us to run codes parallelly, in this way, a great number of tasks can 
be imlemented in a very short time. Let's say, we have hundreds of tasks to accomplish, so, first solution 
come to mind is create seperate thread for each task. But, what if there are thousands of tasks to do in parallel?
Is there a limit for creating threads?

In 32 bit Windows, there is 2GB addressable memory in virtual address space reserved for user usage and a default <br>
stack size for a thread is 1MB. So, 2GB / 1MB, operation system will be allow to create 2000 threads. 

In addition to limitaions of operation system, because of scheduling and shifting of threads increasing of thread number redundantly will cause performance issues.

This problem can be solved by creating specific number thread and assign each task number / thread number 
task to a thread, so, performance will be improved.

```c++
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
```

