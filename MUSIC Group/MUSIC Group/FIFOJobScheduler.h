#pragma once

#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <forward_list>
#include <iostream>


template <class t_Job> class FIFOJobScheduler {

private: 
	std::forward_list<long t_Job> list;

	void FIFOJobScheduler<t_Job>::pop();
	void FIFOJobScheduler<t_Job>::pop_try();
	void FIFOJobScheduler<t_Job>::push();
}; 


