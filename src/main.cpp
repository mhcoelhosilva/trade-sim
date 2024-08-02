#include "simulation.hpp"

#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int main()
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<thread> threads;
    auto runSim = [](Simulation&& sim) {
        sim.RunSimulation();
        sim.SaveOutputToJson();
    };

    for (auto& type : simulationTypes) {
        for (int people = 100; people < 10000; people *= 10) {
            for (int duration = 100; duration < 10000; duration *= 10) {
                SimulationArgs args = {people, 10, duration, type};
                threads.emplace_back(thread(runSim, Simulation(args)));
            }
        }
    }

    for (auto& t : threads) {
        t.join();
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Program took " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " ms" << endl;
}