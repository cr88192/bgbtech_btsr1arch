volatile BTIC1H_Context *btic1h_workqueue;
void *btic1h_workqueue_lock;
volatile int btic1h_workqueue_workers;
volatile int btic1h_workqueue_taskcnt[1024];
volatile byte btic1h_workqueue_kill=0;

int btic1h_workqueue_defaultworkers=8;

void btic1h_thLockQueue()
{
//	if(!btic1h_workqueue_lock)
//		btic1h_workqueue_lock=thFastMutex();
//	thLockFastMutex(btic1h_workqueue_lock);

	if(!btic1h_workqueue_lock)
		btic1h_workqueue_lock=thMutex();
	thLockMutex(btic1h_workqueue_lock);
}

void btic1h_thUnlockQueue()
{
//	thUnlockFastMutex(btic1h_workqueue_lock);
	thUnlockMutex(btic1h_workqueue_lock);
}

int btic1h_QueueWorker(void *data)
{
	BTIC1H_Context *job, *ljob;
	int idle1, idle2;
	int i;
	
//	btic1h_workqueue_workers++;
	ljob=NULL;
//	idle1=1024;
//	idle2=32;

//	idle1=65536;
	idle1=4096;
	idle2=64;
	while(idle1>0)
	{
		if(btic1h_workqueue_kill)
			break;
	
		btic1h_thLockQueue();
		if(ljob)
			{ btic1h_workqueue_taskcnt[ljob->sltid]--; ljob=NULL; }
		job=(BTIC1H_Context *)btic1h_workqueue;
		if(job)
			{ btic1h_workqueue=job->wqnext; }
		btic1h_thUnlockQueue();
		if(!job)
		{
			if(idle2>0)
			{
				idle2--;
				thSleep(0);
				continue;
			}
			idle1--;
			thSleep(1);
			continue;
		}
		
//		idle1=1024;
//		idle2=32;
//		idle1=65536;
		idle1=4096;
		idle2=64;

		job->DoWork(job);
		ljob=job;

//		btic1h_thLockQueue();
//		btic1h_workqueue_taskcnt[job->sltid]--;
//		btic1h_thUnlockQueue();
	}
	btic1h_workqueue_workers--;
	return(0);
}

int BTIC1H_Work_CheckSpawnWorkers(int n)
{
	int i;
	
	btic1h_workqueue_kill=0;
	i=n-btic1h_workqueue_workers;
	while(i>0)
	{
		btic1h_workqueue_workers++;
		thThread(btic1h_QueueWorker, NULL);
		i--;
	}
	return(0);
}

int BTIC1H_Work_KillWorkers(void)
{
	int n;

	btic1h_workqueue_kill=1; n=256;
	while((btic1h_workqueue_workers>0) && ((n--)>0))
	{
		thSleep(1);
		if(!btic1h_workqueue_kill)
			break;
	}
	return(0);
}

int BTIC1H_Work_QueueJob(BTIC1H_Context *ctx, int tid)
{
	btic1h_thLockQueue();
	ctx->sltid=tid;
	btic1h_workqueue_taskcnt[tid]++;
	ctx->wqnext=(BTIC1H_Context *)btic1h_workqueue;
	btic1h_workqueue=ctx;
	btic1h_thUnlockQueue();
	return(0);
}

int BTIC1H_Work_QueueJobNb(BTIC1H_Context *ctx, int tid)
{
	ctx->sltid=tid;
	btic1h_workqueue_taskcnt[tid]++;
	ctx->wqnext=(BTIC1H_Context *)btic1h_workqueue;
	btic1h_workqueue=ctx;
	return(0);
}

int BTIC1H_Work_AllocTidNb(void)
{
	int i, j, k;
	
	for(i=0; i<1024; i++)
	{
		if(!btic1h_workqueue_taskcnt[i])
		{
			btic1h_workqueue_taskcnt[i]=1;
			return(i);
		}
	}
	return(-1);
}

void BTIC1H_Work_FreeTidNb(int tid)
{
	if(btic1h_workqueue_taskcnt[tid]!=1)
	{
		*(int *)-1=-1;
	}
	btic1h_workqueue_taskcnt[tid]=0;
}

int BTIC1H_Work_GetTidCountNb(int tid)
{
	return(btic1h_workqueue_taskcnt[tid]-1);
}

void BTIC1H_Work_WaitTaskComplete(int tid)
{
	static int iysf=128;
	int i, iys, iyc;

	i=BTIC1H_Work_GetTidCountNb(tid);
	iys=i;
//	iyc=(iys*5)>>4;
	iyc=(iys*iysf)>>8;
	while(i>0)
	{
		if(i>iyc)
		{
			thSleep(1);
			i=BTIC1H_Work_GetTidCountNb(tid);
//			if(!i && (iysf<240))
//				iysf+=16;
//			if(!i && (iysf<252))
//				iysf+=4;
			if(!i)
			{
				iysf+=32;
				if(iysf>255)iysf=255;
			}
			continue;
		}
		thSleep(0);
		i=BTIC1H_Work_GetTidCountNb(tid);
//		if(i && (iysf>0))
		if(i)
		{
			iysf--;
			if(iysf<1)iysf=1;
			iyc=(iys*iysf)>>8;
		}
	}
}
