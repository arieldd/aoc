use std::collections::HashMap;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let records = read_records(&lines);

    println!("Part 1: {}", part1(&records));
    println!("Part 2: {}", part2(&records));
}

fn part1(records: &[Record]) -> u64 {
    let mut cache: HashMap<Record, u64> = HashMap::new();
    records.iter().fold(0, |acc, r| {
        let count = count_valid_configs(r, 0, 0, 0, &mut cache);
        acc + count
    })
}

fn part2(records: &[Record]) -> u64 {
    let unfolded_records = records.iter().map(|r| unfold_record(r)).collect::<Vec<_>>();
    part1(&unfolded_records)
}

fn count_valid_configs(
    r: &Record,
    index: usize,
    mut gindex: usize,
    gsize: u32,
    cache: &mut HashMap<Record, u64>,
) -> u64 {
    if let Some(&value) = cache.get(&r) {
        return value;
    }

    let value;
    if r.springs.is_empty() || index >= r.springs.len() {
        value =
            if !r.groups.is_empty() && (gindex < r.groups.len() - 1 || r.groups[gindex] != gsize) {
                0
            } else {
                1
            };
    } else {
        let c = r.springs[index];
        match c {
            '#' => {
                value = if gindex >= r.groups.len() || gsize + 1 > r.groups[gindex] {
                    0
                } else {
                    count_valid_configs(r, index + 1, gindex, gsize + 1, cache)
                }
            }
            '.' => {
                if gsize != 0 {
                    if r.groups[gindex] != gsize {
                        cache.insert(r.clone(), 0);
                        return 0;
                    }
                    gindex += 1;
                }
                let next = Record {
                    springs: r.springs[index + 1..].to_vec(),
                    groups: r.groups[gindex..].to_vec(),
                };
                value = count_valid_configs(&next, 0, 0, 0, cache);
            }
            '?' => {
                let mut new_springs = r.springs.clone();
                new_springs[index] = '.';

                let dot = count_valid_configs(
                    &Record {
                        springs: new_springs.clone(),
                        groups: r.groups.clone(),
                    },
                    index,
                    gindex,
                    gsize,
                    cache,
                );

                new_springs[index] = '#';

                let hash = count_valid_configs(
                    &Record {
                        springs: new_springs,
                        groups: r.groups.clone(),
                    },
                    index,
                    gindex,
                    gsize,
                    cache,
                );
                value = dot + hash;
            }
            _ => panic!("Where did {} come from?", c),
        }
    }

    cache.insert(r.clone(), value);
    value
}

fn read_records(lines: &[&str]) -> Vec<Record> {
    lines
        .iter()
        .map(|l| {
            let [springs, group_str] = l.split(' ').collect::<Vec<_>>()[..] else {
                panic!("Invalid line format: {}", l);
            };
            let groups = group_str
                .split(',')
                .map(|v| {
                    v.parse::<u32>()
                        .expect("Group size should be a valid number")
                })
                .collect();

            Record {
                springs: springs.chars().collect(),
                groups,
            }
        })
        .collect()
}

fn unfold_record(r: &Record) -> Record {
    let mut result = Record {
        springs: r.springs.clone(),
        groups: r.groups.clone(),
    };

    for _ in 1..5 {
        result.springs.push('?');
        result.springs.extend(r.springs.iter());

        result.groups.extend(r.groups.iter());
    }

    result
}

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
struct Record {
    springs: Vec<char>,
    groups: Vec<u32>,
}
