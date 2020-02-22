#include "ThreadWorkerBase.h"

uint32 FThreadWorkerBase::Run()
{
	RunProcess();
	bFinished = true;
	return uint32();
}

void FThreadWorkerBase::Start() { 
	bFinished = false;
	Thread = FRunnableThread::Create(this, TEXT("Thread Worker"), 0, TPri_Normal); 
}

void FThreadWorkerBase::End()
{
	Thread->Kill();
	bFinished = true;
}

void FThreadWorkerBase::Stop() {
	bFinished = true; 
	FRunnable::Stop(); 
}
