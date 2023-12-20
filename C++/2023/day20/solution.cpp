#include "utils.h"

using namespace std;

enum ModuleType { Broadcaster, FlipFlop, Conjunction };

struct Module {

  deque<tuple<int, string>> pulses;
  vector<string> dest;
  bool status;
  map<string, int> memory;

  string label;
  ModuleType type;

  void receive_pulse(int pulse, string src) {
    pulses.push_back(make_tuple(pulse, src));
  }

  int process_pulse(map<string, Module> &network) {

    if (pulses.empty())
      return;
    // println("Processing queue with size ", pulses.size(), " on ", label);

    auto pulse = pulses.front();
    pulses.pop_front();

    auto sent = -1;
    switch (type) {
    case Broadcaster:
      sent = process_broadcaster(network, get<0>(pulse));
      break;
    case FlipFlop:
      sent = process_flip_flop(network, get<0>(pulse));
      break;
    case Conjunction:
      sent = process_conjunction(network, get<1>(pulse), get<0>(pulse));
      break;
    }

    if (sent != -1) {
      return (sent) ? dest.size() : -dest.size();
    }

    return 0;
  }

  int process_broadcaster(map<string, Module> &network, int pulse) {

    for (auto d : dest) {
      if (network.find(d) != network.end())
        network.at(d).receive_pulse(pulse, label);
    }

    return pulse;
  }

  int process_flip_flop(map<string, Module> &network, int pulse) {

    if (pulse) // Ignore high pulse
      return -1;

    status = !status;
    for (auto d : dest) {
      if (network.find(d) != network.end())
        network.at(d).receive_pulse(status, label);
    }

    return status;
  }

  int process_conjunction(map<string, Module> &network, string src, int pulse) {

    memory[src] = pulse;

    bool all_high = all_of(
        memory.begin(), memory.end(),
        [](const pair<string, int> &entry) { return entry.second == 1; });

    for (auto d : dest) {
      if (network.find(d) != network.end())
        network.at(d).receive_pulse(!all_high, label);
    }

    return !all_high;
  }

  bool intial_state() const {
    if (!pulses.empty())
      return false;

    switch (type) {
    case Broadcaster:
      return true;
    case FlipFlop:
      return !status;
    case Conjunction:
      return all_of(
          memory.begin(), memory.end(),
          [](const pair<string, int> &entry) { return entry.second == 0; });
    }
  }
};

map<string, Module> parse_network(const vector<string> &lines) {
  map<string, Module> result;

  for (auto l : lines) {
    auto first_split = split(l, ' ');
    auto name = first_split[0];

    Module current{.type = Broadcaster};

    if (name != "broadcaster") {
      auto type_symbol = name[0];
      name = name.substr(1);

      if (type_symbol == '%')
        current.type = FlipFlop;
      else
        current.type = Conjunction;
    }
    current.label = name;

    for (int i = 2; i < first_split.size(); i++) {
      auto dest = first_split[i];
      if (dest.ends_with(','))
        dest = dest.substr(0, dest.size() - 1);
      current.dest.push_back(dest);
    }

    result[name] = current;
  }

  for (auto &pair : result) {
    if (pair.second.type == Conjunction) {
      string conj = pair.first;
      for (auto &other : result) {
        Module watching = other.second;
        if (conj != watching.label &&
            find(watching.dest.begin(), watching.dest.end(), conj) !=
                watching.dest.end()) {
          result[conj].memory[watching.label] = 0;
        }
      }
    }
  }

  return result;
}

void print_network(const map<string, Module> &network) {
  for (auto &pair : network) {
    print(pair.second.label, " ", pair.second.type, " -> ");

    for (auto d : pair.second.dest) {
      print(d, " ");
    }

    println();
  }
}

bool is_original_state(const map<string, Module> &network) {
  return all_of(network.begin(), network.end(),
                [](const pair<string, Module> &entry) {
                  return entry.second.intial_state();
                });
}

int part1(map<string, Module> network, int cycles) {
  int low_pulses = 0, high_pulses = 0;

  const string broadcaster = "broadcaster";
  for (int i = 0; i < cycles; i++) {

    network.at(broadcaster).receive_pulse(0, "button");
    low_pulses += 1;

    deque<string> processing{broadcaster};

    while (!processing.empty()) {
      auto label = processing.front();
      processing.pop_front();

      auto emitted = network.at(label).process_pulse(network);
      if (emitted != 0) {
        if (emitted > 0)
          high_pulses += emitted;
        else
          low_pulses += abs(emitted);

        for (auto &d : network.at(label).dest) {
          if (network.find(d) != network.end())
            processing.push_back(d);
        }
      }
    }
  }

  return high_pulses * low_pulses;
}

int64_t part2(map<string, Module> network,
              const map<string, int> &terminators) {

  println("Starting from initial state ", is_original_state(network));
  const string broadcaster = "broadcaster";

  vector<int64_t> cycles(terminators.size());

  for (int i = 0;; i++) {

    network.at(broadcaster).receive_pulse(0, "button");
    deque<string> processing{broadcaster};

    while (!processing.empty()) {
      auto label = processing.front();
      processing.pop_front();

      auto emitted = network.at(label).process_pulse(network);
      if (emitted != 0) {

        if (emitted > 0 && terminators.find(label) != terminators.end()) {
          int index = terminators.at(label);
          if (!cycles[index])
            cycles[terminators.at(label)] = i + 1;
        }

        for (auto &d : network.at(label).dest) {
          if (network.find(d) != network.end())
            processing.push_back(d);
        }
      }
    }
    if (all_of(cycles.begin(), cycles.end(),
               [](int value) { return value > 0; })) {

      int64_t result = 1;
      for (int k = 0; k < cycles.size(); k++) {
        result = lcm(result, cycles[k]);
      }

      return result;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto network = parse_network(lines);

  auto r1 = part1(network, 1000);
  println("Part 1: ", r1);

  map<string, int> terminators{
      {"vq", 0},
      {"sr", 1},
      {"sn", 2},
      {"rf", 3},
  };

  map<string, int> terminators2{
      {"sg", 0},
      {"lm", 1},
      {"dh", 2},
      {"db", 3},
  };

  auto r2 = part2(network, terminators);
  println("Part 2: ", r2);

  return 0;
}