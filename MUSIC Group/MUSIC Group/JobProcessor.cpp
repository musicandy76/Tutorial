#pragma once
#include "stdafx.h"
#include "JobProcessor.h"


#include "FIFOQueue.cpp"

#include <condition_variable>

#include <thread>
#include <mutex>

#include <iostream>

using namespace std;


FIFOQueue<Job> jobs;

list<Job>  jobsToProcess;

std::mutex m;
std::condition_variable waitForData;
bool readyToRead = true;
bool readerThreadStop = false;

bool threadEnded = false;


void WriterThread() {

	list<Job>::iterator it;
	it = jobsToProcess.begin();

	for (it = jobsToProcess.begin(); it != jobsToProcess.end(); ++it)
	{
		std::unique_lock<std::mutex> lck(m);
		if (jobs.push(*it))
		{
			readyToRead = true;
			waitForData.notify_all();
		}
	}
}


void ReaderThread() {

	Job whichJob;

	do {

		readyToRead = jobs.pop(whichJob);

		if (readyToRead)
			cout << "Mixing desk event name:" << whichJob.mixingDeskEventName << "\n" << "Mixing desk MIDI event value" << whichJob.midiValue << "\n";

		std::unique_lock<std::mutex> lk(m);

		while (!readyToRead) {
			std::cout << "No jobs on queue";
			waitForData.wait(lk);
		}
	} while (jobs.pop_try(whichJob));

}



JobProcessor::JobProcessor(list<Job>   jobsToWrite)
{
	jobsToProcess = jobsToWrite;
	
}


JobProcessor::~JobProcessor()
{
	waitForData.notify_all();
	threadEnded = true;


}

void JobProcessor::execute()
{
	thread Writer(WriterThread);
	thread Reader(ReaderThread);


	Writer.join();
	Reader.join();
	

	return;
}

