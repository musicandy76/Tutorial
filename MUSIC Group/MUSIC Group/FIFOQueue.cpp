
#pragma once
#include "stdafx.h"
#include <queue>
#include <mutex>



using namespace std;


template <class t_Job> class FIFOQueue {

private:
	queue<t_Job> queue;

	std::mutex g_lock;


public:

	bool pop(t_Job & tJob);
	bool pop_try(t_Job  & tJob);
	bool push(t_Job const& elem);
};




template<class t_Job>
bool FIFOQueue<t_Job>::pop(t_Job & tJob)
{
	bool bRet = false;
	if (!queue.empty())
	{
		g_lock.lock();

		tJob = queue.front();
		queue.pop();
		g_lock.unlock();
		
		bRet = true;
	}

	return bRet;
}

template<class t_Job>
bool FIFOQueue<t_Job>::pop_try(t_Job & tJob)
{
	if (queue.empty())
	{
		return false;
	}
	else
	{
		pop(tJob);
		return true;
	}
}

template <class t_Job>
bool FIFOQueue<t_Job>::push(t_Job const& elem)
{
	bool bRet = true;
	try 
	{
		g_lock.lock();

		queue.push(elem);

		g_lock.unlock();
	}
	catch(const std::exception&)
	{
		bRet = false;
	}

	return bRet;
}
