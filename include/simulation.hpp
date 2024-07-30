#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
using engine = std::mt19937;

enum SimulationType {
    CantTradeAtZero = 0,
    CanOnlyReceiveAtZero = 1,
    COUNT = 2
};

class Simulation {
public:
    Simulation(int numPeople, int initialAmount, int duration, SimulationType type, bool verbose) 
        : m_initialAmount(initialAmount), m_duration(duration), m_type(type), m_verbose(verbose)
    {
        m_money = vector<int>(numPeople, initialAmount);
    }

    void RunSimulation() {

        cout << "-- Starting simulation where ";
        switch(m_type) {
            case SimulationType::CantTradeAtZero:
            {
                cout << "traders can't trade anymore once they hit zero --" << endl;
            }
            break;
            case SimulationType::CanOnlyReceiveAtZero:
            {
                cout << "traders can only receive money once they hit zero --" << endl;
            }
            break;
            default:
            {
                cout << "m_type not valid --" << endl;
                return;
            }
            break;
        }

        std::random_device os_seed;
        const uint32_t seed = os_seed();
        engine generator(seed);
        std::uniform_int_distribution<uint32_t> distribute(0, 1);
        auto coinToss = std::bind(distribute, generator);

        for (int t = 0; t < m_duration; ++t) {
            for (int i = 0; i < m_money.size() - 1; ++i) {
                for (int j = i + 1; j < m_money.size(); ++j) {
                    if (m_type == SimulationType::CantTradeAtZero && (m_money[j] == 0 || m_money[i] == 0))
                        continue;

                    auto res = coinToss();
                    if (res == 0) {
                        if (m_type == SimulationType::CanOnlyReceiveAtZero && m_money[i] == 0)
                            continue;
                        
                        m_money[i]--;
                        m_money[j]++;
                    } else if (res == 1) {
                        if (m_type == SimulationType::CanOnlyReceiveAtZero && m_money[j] == 0)
                            continue;
                        
                        m_money[j]--;
                        m_money[i]++;
                    }
                }
            }

            if (m_verbose) {
                cout << "For time " << t << endl;
                for (auto m : m_money) {
                    cout << m << ", ";
                }
                cout << endl;
            }
        }

        cout << "Finished simulation" << endl;
        int numAboveZero = 0;
        for (auto m : m_money) {
            if (m > 0) {
                ++numAboveZero;
            }
        }

        float pct = (((float)numAboveZero) / ((float)m_money.size())) * 100.0f;
        cout << "For " << m_money.size() << " people and " << m_duration << " duration, only " << pct << "\% are left with non-zero money at the end." << endl;
    }

    void SaveOutputToJson() {
        json j;
        j["num_people"] = m_money.size();
        j["initial_amount"] = m_initialAmount;
        j["m_duration"] = m_duration;
        j["m_type"] = m_type == SimulationType::CantTradeAtZero ? "cant_trade" : "can_receive";
        j["final_data"] = m_money;

        string filename = "../data/output_";
        filename += to_string(m_money.size()) + "_";
        filename += to_string(m_initialAmount) + "_";
        filename += to_string(m_duration) + "_";
        filename += to_string(m_type) + ".json";
        std::ofstream o;
        o.open(filename);
        o << j.dump(4) << std::endl;
        o.close();
    }

private:
    vector<int> m_money;
    int m_initialAmount = 10;
    int m_duration = 10;
    SimulationType m_type;
    bool m_verbose = false;
};