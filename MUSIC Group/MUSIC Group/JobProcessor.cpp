#pragma once
#include "stdafx.h"
#include "JobProcessor.h"


#include "FIFOQueue.cpp"


#include <thread>
#include <mutex>

#include <iostream>

using namespace std;


FIFOQueue<Job> jobs;

list<Job>  jobsToProcess;


void WriterThread() {

	list<Job>::iterator it;
	it = jobsToProcess.begin();

	for (it = jobsToProcess.begin(); it != jobsToProcess.end(); ++it)
	{
		jobs.push(*it);
	}
}


void ReaderThread(int jobSize) {

	Job whichJob;
	bool readSuccess = false;
	int iter = 0;
	do {
		

		readSuccess = jobs.pop(whichJob);
		iter++;

		if (readSuccess)
		{
			cout << "Mixing desk event name:" << whichJob.mixingDeskEventName << "\n" << "Mixing desk MIDI event value " << whichJob.midiValue << "\n";

			// This is for emulating that the reader is processing the job and taking a second to do so.....
			// In a more realistic scenario it would process this job to do something....
			cout << "Processing job.......\n";
			this_thread::sleep_for(0.5s);
		}

		if (iter == jobSize)
		{
			cout << "\nJob Completed ....Number of jobs in cycle: " << jobSize << "\n";
			jobs.stop();
			readSuccess = false;
		}

	} while (readSuccess);


	// Check the reader queue is empty, if not there is a problem as the job should have been finished. 
	jobs.pop_try(whichJob);

	if (&whichJob == NULL)
	{
		cout << "Not all jobs were processed by the reader";
	}

}



JobProcessor::JobProcessor()
{
	
}


JobProcessor::~JobProcessor()
{

}

void JobProcessor::setData(list<Job>   jobsToWrite)
{
	jobsToProcess = jobsToWrite;
	jobSize = jobsToWrite.size();
}
void JobProcessor::execute()
{
	thread Writer(WriterThread);
	thread Reader(ReaderThread, jobSize);

	Writer.join();
	Reader.join();

	return;
}

