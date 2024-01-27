#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

using namespace std;

class Prom {
private:
    int capacity;
    int waitTime;
    vector<int> cars;
    mutex mutex;
    condition_variable cond;

public:
    Ferry(int capacity, int waitTime) : capacity(capacity), waitTime(waitTime) {}

    void boardFerry(int carId) {
        unique_lock<std::mutex> lock(mutex);

        while (cars.size() >= capacity) {
            cout << "Car " << carId << " is waiting for the ferry." << endl;
            cond.wait(lock);
        }

        cars.push_back(carId);
        cout << "Car " << carId << " boarded the ferry." << endl;

        if (cars.size() == capacity) {
            sail();
        }
    }

    void sail() {
        cout << "Ferry is sailing with " << cars.size() << " cars onboard." << endl;
        this_thread::sleep_for(chrono::seconds(waitTime));
        cars.clear();
        cout << "Ferry has reached the other shore and is returning empty." << endl;
        cond.notify_all();
    }
};

void carThread(Ferry& ferry, int carId) {
    ferry.boardFerry(carId);
}

int main() {
    const int numCars = 10;
    const int ferryCapacity = 5;
    const int ferryWaitTime = 30;  // in seconds

    Ferry ferry(ferryCapacity, ferryWaitTime);
    vector<thread> carThreads;

    for (int i = 0; i < numCars; ++i) {
        carThreads.push_back(std::thread(carThread, ref(ferry), i));
        this_thread::sleep_for(chrono::seconds(1));  // Simulate cars arriving at different times
    }

    for (auto& t : carThreads) {
        t.join();
    }

    return 0;
}