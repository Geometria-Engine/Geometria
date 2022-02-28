#include "Multithreading.h"
#include "Graphics/Cores/MainAPI/Graphics.h"

std::vector<PhysicalCore*> Multithreading::allCores;
std::vector<std::future<void>> Multithreading::allFutures;
int Multithreading::coreIndex = 0;

void Multithreading::Start()
{
    unsigned int threadCount = std::thread::hardware_concurrency() - 1;
    for (int i = 0; i < threadCount; i++)
    {
        PhysicalCore* newCore = new PhysicalCore();
        newCore->id = i;
        newCore->StartPhysicalCore();
        allCores.push_back(newCore);
    }
}

Thread* Multithreading::RunThread(std::function<void()> v)
{
    Thread* newThread = new Thread();
    newThread->function = v;
    newThread->milliseconds = 0;
    newThread->clock = 0;

    FindAvailableCoreToThread(newThread);
    return newThread;
}

Thread* Multithreading::RunUpdateThread(std::function<void()> v, int milliseconds)
{
    Thread* newThread = new Thread();
    newThread->function = v;

    newThread->update = true;
    newThread->milliseconds = milliseconds;

    FindAvailableCoreToThread(newThread);
    return newThread;
}

void Multithreading::FindAvailableCoreToThread(Thread* t)
{
    allCores[coreIndex]->allThreads.push_back(t);
    t->owner = allCores[coreIndex];

    if (t->update)
    {
        if (t->milliseconds < allCores[coreIndex]->milliseconds)
            allCores[coreIndex]->milliseconds = t->milliseconds;
    }

    if (coreIndex < allCores.size() - 1)
        coreIndex++;
    else
        coreIndex = 0;
}

void PhysicalCore::StartPhysicalCore()
{
    Multithreading::allFutures.push_back(std::async(std::launch::async, [this]() { PhysicalCoreLoop(); }));
}

void PhysicalCore::PhysicalCoreLoop()
{
    while (!Graphics::CanClose())
    {
        auto wait_duration = std::chrono::milliseconds(milliseconds);
        //std::cout << "Running Physical Core Number " << id << std::endl;
        std::lock_guard<std::mutex> mutex(NUT);
        int t = 0;
        for (auto i : allThreads)
        {
            i->clock += milliseconds;

            if (i->clock >= i->milliseconds)
            {
                i->function();
                i->clock = 0;

                if (!i->update)
                {
                    deleteThreadList.push_back(t);
                    threadsToDelete = true;
                }
            }
            t++;
        }

        if (threadsToDelete)
        {
            for (auto i : deleteThreadList)
            {
                delete allThreads[i];
                allThreads[i] = nullptr;
                allThreads.erase(allThreads.begin() + i);
            }
            deleteThreadList.clear();
        }

        NUT.unlock();
        std::this_thread::sleep_for(wait_duration);
    }
}

void Thread::ChangeMilliseconds(int change)
{
    milliseconds = change;
    clock = change;

    PhysicalCore* c = (PhysicalCore*)owner;

    if (milliseconds < c->milliseconds)
        c->milliseconds = milliseconds;
}