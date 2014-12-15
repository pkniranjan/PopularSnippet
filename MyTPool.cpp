// simple2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <process.h>
#include "stdafx.h"
#include <vector>
#include <Windows.h>
#include <tchar.h>

typedef std::vector<job*> vJob;
typedef vJob::iterator it;

struct job
{
	int nJobID;
	void(*JobRoutine)(void*);
	void* args;
};

UINT __stdcall ThreadDefaultRoutine(LPVOID in);

class MyPool
{
	vJob JobQ;
	HANDLE* arrHandle;
	UINT(*PtToRoutine)(LPVOID in);
	int nSize;
public:
	HANDLE hJobNotifierEvent;
	HANDLE hMutex;
	MyPool()
	{
		arrHandle = NULL;
		hJobNotifierEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		hMutex = CreateMutex(NULL,FALSE,NULL);
		nSize = 10;
	}
	~MyPool()
	{
		while(!JobQ.empty())
		{
			job* pjob = JobQ.back();
			delete pjob;
			JobQ.pop_back();
		}
		for(int i=0;i<nSize;i++)
		{
			HANDLE h= arrHandle[i];
			_endthreadex((UINT)h);
			CloseHandle(h);
		}
		CloseHandle(hJobNotifierEvent);
		CloseHandle(hMutex);
	}
	void Initialize(int n=10);
	void AddJob(job* pJob);
	job* GetJob();
};
void  MyPool::Initialize(int n)
{
	__try
	{
		nSize = n;
		arrHandle = new HANDLE[nSize];
		ZeroMemory(arrHandle, sizeof(HANDLE)*nSize);
		for(int i=0;i<nSize; i++)
		{
			arrHandle[i] = (HANDLE)_beginthreadex(0,0,ThreadDefaultRoutine, (void*)this,0,NULL);
		}
	}
	__finally{}
}
void  MyPool::AddJob(job* pJob)
{
	__try
	{
		WaitForSingleObject(hMutex,INFINITE);
		JobQ.push_back(pJob);
		if(JobQ.size() == 1) //First Job, before this, it was unsignaled
			SetEvent(hJobNotifierEvent);
	}
	__finally
	{
		ReleaseMutex(hMutex);
	}
}

job*  MyPool::GetJob()
{
	__try
	{
		WaitForSingleObject(hMutex,INFINITE);
			job* tjob = JobQ.back();
			JobQ.pop_back();
			if(JobQ.empty())
				ResetEvent(hJobNotifierEvent);
		return tjob;	
	}
	__finally
	{
		ReleaseMutex(hMutex);
	}
}

UINT __stdcall ThreadDefaultRoutine(LPVOID in)
{
	MyPool * tPool = (MyPool*)in;
	DWORD dwWait;
	while(1)
	{
		dwWait = WaitForSingleObject(tPool->hJobNotifierEvent, INFINITE);
	
		job* tJob = NULL;
		tJob = tPool->GetJob();
		Sleep(2000);
		if(tJob != NULL && tJob->JobRoutine!= NULL)
		{
			tJob->JobRoutine((void*)tJob);
		}
	}
	return 0;
}

static int n=0;
void PrintChar(void* in);
int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	MyPool tPool;
	cout<<"starting pool";
	tPool.Initialize(10);

	for(int i=0;i<40;i++)
	{
		job* tjob = new job();
		tjob->nJobID = i;
		tjob->JobRoutine = PrintChar;
		tjob->args = NULL;
		tPool.AddJob(tjob);
	}

	Sleep(100000);
	return 0;
}

void PrintChar(void* in)//Worker Routine
{
	using namespace std;
	job* tJob = (job*)in;
	cout<<tJob->nJobID<<"\t";
	Sleep(3000);
}