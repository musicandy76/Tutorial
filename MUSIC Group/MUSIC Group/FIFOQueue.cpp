
#pragma once

#include "stdafx.h"
#include "FIFOQueue.h"

using namespace std;

static bool valueHasBeenPushed;

template<class t_Job>
bool FIFOQueue<t_Job>::pop(t_Job & tJob)
{
	bool bRet = false;
	valueHasBeenPushed = false;
	g_lock.lock();

	if (!fifoQueue.empty())
	{
		
		tJob = fifoQueue.front();
	
		fifoQueue.pop();

		bRet = true;
	}
	else
	{
		unique_lock<mutex> lk(m);
		waitForData.wait(lk, [] {return valueHasBeenPushed  == true; });
	}

	g_lock.unlock();

	return bRet;
}

template<class t_Job>
void FIFOQueue<t_Job>::stop()
{
	unique_lock<mutex> lk(m);
	waitForData.notify_all();
}

template<class t_Job>
bool FIFOQueue<t_Job>::pop_try(t_Job & tJob)
{
	bool bRet = false;
	g_lock.lock();

	if (fifoQueue.empty())
	{
		bRet = false;
	}
	else
	{
		pop(tJob);
		bRet = true;
	}

	g_lock.unlock();

	return bRet;
}

template <class t_Job>
bool FIFOQueue<t_Job>::push(t_Job const& elem)
{
	bool bRet = true;
	try 
	{
		g_lock.lock();

		fifoQueue.push(elem);


		// TODO: Maybe rename thi
		unique_lock<mutex> lk(m);

		valueHasBeenPushed = true;
		waitForData.notify_all();


		g_lock.unlock();
	}
	catch(const exception&)
	{
		bRet = false;
	}

	return bRet;
}
