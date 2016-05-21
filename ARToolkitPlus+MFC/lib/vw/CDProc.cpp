#include "StdAfx.h"
#include "CDProc.h"

CDProc::CDProc(void)
{
	int i;
	for(i=0; i<KEEP_MAX_NO; i++)
	{
		eventHist[i] = 0;

	}
}

CDProc::~CDProc(void)
{
}

void CDProc::Add(int count)
{
	Dbg("Cnt: %d", count);

	int i;
	for(i=0; i<KEEP_MAX_NO-1; i++)
	{
		eventHist[KEEP_MAX_NO -1-i] = eventHist[KEEP_MAX_NO -1-i-1];
	}
	
	eventHist[0] = count;
}





void CDProc::Add(CDInfo cd)
{
	int i;
	for(i=0; i<KEEP_MAX_NO-1; i++)
	{
		eventHist[KEEP_MAX_NO -1-i] = eventHist[KEEP_MAX_NO -1-i-1];
	}
	
	if(cd.hit_ID==-1)
	{
		eventHist[0] = 0;
	}
	else
	{
		eventHist[0] = 1;
	}

}

void CDProc::Run()
{
	

}