import matplotlib.pyplot as plt
import json
from pathlib import Path

pathlist = Path("../data").rglob('*.json')
plot_data = dict()
for path in pathlist:
    with open(str(path), 'r') as file:
        j = json.load(file)
        if j["m_duration"] not in plot_data:
            plot_data[j["m_duration"]] = dict()
        plot_data[j["m_duration"]][j["m_type"]] = j["final_data"]

for duration, val in plot_data.items():
    fig, axs = plt.subplots(1, len(val))
    i = 0
    for m_type, final_data in val.items():
        title = str(duration) + " time steps "
        if m_type == "cant_trade":
            title += "($0 Trap)"
        else:
            title += "(No $0 Trap)"
        y = final_data
        y.sort()
        x = range(0, len(y))
        axs[i].plot(x, y)
        axs[i].set_title(title)
        axs[i].set(ylabel="Dollars", xlabel="Population")

        i += 1
    plt.show()
