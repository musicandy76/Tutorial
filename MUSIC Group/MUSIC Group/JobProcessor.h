#pragma once
#include "Job.h"
#include <list>


using namespace std;
class JobProcessor
{
public:
	JobProcessor(list<Job>   jobsToWrite);
	~JobProcessor();
	void execute();
};