#pragma once

#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "logger.hpp"

using namespace std;

enum SimulationType {
    CantTradeAtZero = 0,
    CanOnlyReceiveAtZero = 1,
    COUNT = 2
};

struct SimulationArgs {
    int m_numPeople = 10;
    int m_initialAmount = 10;
    int m_duration = 10;
    SimulationType m_type;
};

class Simulation {
public:
    Simulation(SimulationArgs& args);

    void RunSimulation();
    void SaveOutputToJson();

private:
    vector<int> m_money;
    SimulationArgs m_args;
};