// MUSIC Group.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "FIFOQueue.cpp"
#include "JobProcessor.h"
#include <condition_variable>

#include <thread>
#include <mutex>

#include <iostream>
#include <list>




//std::mutex m;
//std::condition_variable waitForData;
//bool readyToRead = true;
//bool readerThreadStop = false;


//FIFOQueue<Job> jobs;

list<Job>  jobsToWrite;
/*
void WriterThread() {

	list<Job>::iterator it;
	it = jobsToWrite.begin();

	for (it = jobsToWrite.begin(); it != jobsToWrite.end(); ++it)
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
	} while (true);

}*/

int main()
{
	Job job1;
	job1.mixingDeskEventName = "Pan Channel 1";
	job1.midiValue = 127;

	Job job2;
	job2.mixingDeskEventName = "Volume Channel 1";

	job2.midiValue = 64;

	Job job3;
	job3.mixingDeskEventName = "Pan Channel 2";
	job3.midiValue = 127;

	Job job4;
	job4.mixingDeskEventName = "Volume Channel 2";

	job4.midiValue = 64;

	Job job5;
	job5.mixingDeskEventName = "Pan Channel 3";
	job5.midiValue = 127;

	Job job6;
	job6.mixingDeskEventName = "Volume Channel 3";

	job6.midiValue = 64;

	jobsToWrite.push_back(job1);
	jobsToWrite.push_back(job2);
	jobsToWrite.push_back(job3);
	jobsToWrite.push_back(job4);

	jobsToWrite.push_back(job5);
	jobsToWrite.push_back(job6);

	JobProcessor* processor1 = new JobProcessor(jobsToWrite);
	processor1->execute();

	delete processor1;
	

	return 0;

}


