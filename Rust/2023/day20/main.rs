use std::collections::HashMap;
use std::collections::VecDeque;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let modules = parse_input(&lines);

    // print_modules(&modules);
    println!("Part 1: {}", part1(modules.clone()));
    println!("Part 2: {}", part2(&modules));
}

fn part1(mut modules: HashMap<String, Module>) -> usize {
    let (mut lows, mut highs) = (0, 0);

    let presses = 1000;
    let bc_name = "broadcaster";

    let mut processing = VecDeque::new();

    for i in 0..presses {
        lows += 1;
        processing.push_back((bc_name.to_string(), false, "button".to_string()));

        while let Some((label, pulse, sender)) = processing.pop_front() {
            modules.entry(label.to_string()).and_modify(|m| {
                m.recv(pulse, &sender);

                if let Some(value) = m.process() {
                    if value {
                        highs += m.relays.len();
                    } else {
                        lows += m.relays.len();
                    }

                    m.relays.iter().for_each(|name| {
                        processing.push_back((name.clone(), value, label.clone()));
                    });
                }
            });
        }
    }

    lows * highs
}

fn part2(_modules: &HashMap<String, Module>) -> u32 {
    0
}

fn print_modules(modules: &HashMap<String, Module>) {
    modules.iter().for_each(|(label, module)| {
        print!("{} -> ", label);
        module.relays.iter().for_each(|name| {
            print!("{}, ", name);
        });

        if module.kind == MType::Conjunction {
            module.memory.iter().for_each(|(k, v)| {
                print!("{}:{}, ", k, v);
            })
        }
        println!();
    });

    println!("Total modules: {}", modules.len());
}

fn parse_input(lines: &[&str]) -> HashMap<String, Module> {
    lines.iter().fold(HashMap::new(), |mut modules, l| {
        let parts = l.split("->").map(|token| token.trim()).collect::<Vec<_>>();
        let mut label = parts[0].to_string();
        let mut kind = MType::Broadcast;

        match label.chars().nth(0) {
            Some('%') => {
                kind = MType::FlipFlop;
                label.remove(0);
            }
            Some('&') => {
                kind = MType::Conjunction;
                label.remove(0);
            }
            _ => (),
        }

        let relays = parts[1]
            .split(",")
            .map(|token| token.trim().to_string())
            .collect::<Vec<_>>();

        let mut module = Module {
            label,
            pulses: VecDeque::new(),
            kind,
            relays,
            status: false,
            memory: HashMap::new(),
        };

        if module.kind == MType::Conjunction {
            modules.iter().for_each(|(key, value)| {
                if value.relays.contains(&module.label) {
                    module.memory.insert(key.clone(), false);
                }
            })
        }

        module
            .relays
            .iter()
            .for_each(|name| match modules.get_mut(name) {
                Some(value) => {
                    value.memory.insert(module.label.clone(), false);
                }
                None => (),
            });

        modules.insert(module.label.clone(), module);
        modules
    })
}

#[derive(Debug, Clone)]
struct Module {
    label: String,
    pulses: VecDeque<(bool, String)>,
    kind: MType,
    relays: Vec<String>,
    status: bool,
    memory: HashMap<String, bool>,
}

impl Module {
    fn recv(&mut self, pulse: bool, sender: &str) {
        self.pulses.push_back((pulse, sender.to_string()));
    }

    fn process(&mut self) -> Option<bool> {
        assert!(!self.pulses.is_empty()); // don't call with an empty queue
        let (pulse, sender) = self
            .pulses
            .pop_front()
            .expect("At least one pulse should have been received");

        match self.kind {
            MType::FlipFlop => {
                if pulse {
                    return None;
                }
                self.status = !self.status;
                Some(self.status)
            }
            MType::Conjunction => {
                assert!(self.memory.contains_key(&sender));

                self.memory
                    .entry(sender.to_string())
                    .and_modify(|v| *v = pulse);

                let all_high = self.memory.iter().all(|(_, v)| *v);
                Some(!all_high)
            }
            MType::Broadcast => Some(false),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone)]
enum MType {
    Broadcast,
    FlipFlop,
    Conjunction,
}
