#pragma once

#include <vector>
#include <string>
#include <functional>
#include <future>
#include <thread>

#ifndef MULTITHREADING_H
#define MULTITHREADING_H
struct Thread
{
	std::function<void()> function;
	int milliseconds = 1000;
	int clock = 1000;
	bool update = false;
	void* owner = nullptr;

	void SetHybernationState()
	{
		milliseconds = 1000;
		clock = 1000;
	}

	void ChangeMilliseconds(int change);
};

struct PhysicalCore
{
	int milliseconds = 1000;
	int id = 0;
	std::mutex NUT;
	std::vector<Thread*> allThreads;
	std::vector<int> deleteThreadList;
	bool threadsToDelete = false;

	void StartPhysicalCore();
	void PhysicalCoreLoop();
};

struct Multithreading
{
	static std::vector<PhysicalCore*> allCores;
	static std::vector<std::future<void>> allFutures;
	static int coreIndex;

	static void Start();

	static Thread* RunThread(std::function<void()> v);
	static Thread* RunUpdateThread(std::function<void()> v, int milliseconds);

	static void FindAvailableCoreToThread(Thread* t);
};
#endif