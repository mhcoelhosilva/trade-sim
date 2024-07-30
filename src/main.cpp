#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int main()
{
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    vector<thread> threads;
    auto runSim = [](Simulation&& sim) {
        sim.RunSimulation();
        sim.SaveOutputToJson();
    };

    for (int type = 0; type < SimulationType::COUNT; ++type) {
        for (int people = 100; people < 10000; people *= 10) {
            for (int duration = 100; duration < 10000; duration *= 10) {
                threads.emplace_back(thread(runSim, Simulation(people, 10, duration, (SimulationType)type, false)));
            }
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Program took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
}