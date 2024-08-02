#include "simulation.hpp"

using json = nlohmann::json;
using engine = std::mt19937;

Simulation::Simulation(SimulationArgs& args) 
    : m_args(args) {
    m_money = vector<int>(args.m_numPeople, args.m_initialAmount);
}

void Simulation::RunSimulation() {
    Log("-- Starting simulation where ");
    switch(m_args.m_type) {
        case SimulationType::CantTradeAtZero:
        {
            LogLine("traders can't trade anymore once they hit zero --");
        }
        break;
        case SimulationType::CanOnlyReceiveAtZero:
        {
            LogLine("traders can only receive money once they hit zero --");
        }
        break;
        default:
        {
            LogLine("m_type not valid --");
            return;
        }
        break;
    }

    std::random_device os_seed;
    const uint32_t seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution<uint32_t> distribute(0, 1);
    auto coinToss = std::bind(distribute, generator);
    
    for (int t = 0; t < m_args.m_duration; ++t) {
        for (size_t i = 0; i < m_money.size() - 1; ++i) {
            for (size_t j = i + 1; j < m_money.size(); ++j) {
                if (m_args.m_type == SimulationType::CantTradeAtZero && (m_money[j] == 0 || m_money[i] == 0))
                    continue;

                auto res = coinToss();
                if (res == 0) {
                    if (m_args.m_type == SimulationType::CanOnlyReceiveAtZero && m_money[i] == 0)
                        continue;
                    
                    --m_money[i];
                    ++m_money[j];
                } else if (res == 1) {
                    if (m_args.m_type == SimulationType::CanOnlyReceiveAtZero && m_money[j] == 0)
                        continue;
                    
                    --m_money[j];
                    ++m_money[i];
                }
            }
        }
    }

    int numAboveZero = 0;
    for (auto m : m_money) {
        if (m > 0) {
            ++numAboveZero;
        }
    }
    float pct = (((float)numAboveZero) / ((float)m_money.size())) * 100.0f;
    LogLine("Finished simulation");
    LogLine("For " + to_string(m_money.size()) + " people and " + to_string(m_args.m_duration) + " duration, only " + to_string(pct) + "\% are left with non-zero money at the end.");
}

void Simulation::SaveOutputToJson() {
    json j;
    j["num_people"] = m_money.size();
    j["initial_amount"] = m_args.m_initialAmount;
    j["m_duration"] = m_args.m_duration;
    j["m_type"] = m_args.m_type == SimulationType::CantTradeAtZero ? "cant_trade" : "can_receive";
    j["final_data"] = m_money;

    string filename = "../data/output_";
    filename += to_string(m_money.size()) + "_";
    filename += to_string(m_args.m_initialAmount) + "_";
    filename += to_string(m_args.m_duration) + "_";
    filename += to_string(m_args.m_type) + ".json";

    std::ofstream o;
    o.open(filename);
    o << j.dump(4) << std::endl;
    o.close();
}