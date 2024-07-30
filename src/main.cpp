#include "simulation.hpp"

#include <iostream>
#include <chrono>

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int type = 0; type < SimulationType::COUNT; ++type) {
        for (int people = 100; people < 10000; people *= 10) {
            for (int duration = 100; duration < 10000; duration *= 10) {
                Simulation sim(people, 10, duration, (SimulationType)type, false);
                sim.RunSimulation();
                sim.SaveOutputToJson();
            }
        }
        cout << "-----------" << endl;
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Program took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
}