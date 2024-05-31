use std::cmp::Ordering;
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

    let (workflows, parts) = parse_input(&lines);

    println!("Part 1: {}", part1(&workflows, &parts));
    println!("Part 2: {}", part2(&workflows));
}

fn part1(workflows: &HashMap<&str, Vec<Rule>>, parts: &Vec<Part>) -> u32 {
    parts
        .iter()
        .map(|p| {
            if is_part_accepted(workflows, p) {
                p.iter().sum()
            } else {
                0
            }
        })
        .sum()
}

fn part2(workflows: &HashMap<&str, Vec<Rule>>) -> u64 {
    count_accepted_parts(
        "in",
        workflows,
        Range {
            lower: [0; 4],
            upper: [4001; 4],
        },
    )
}

fn parse_input<'a>(lines: &'a [&str]) -> (HashMap<&'a str, Vec<Rule>>, Vec<Part>) {
    let space = lines
        .iter()
        .position(|l| l.is_empty())
        .expect("One blank line should be present");

    let workflows = lines[..space].iter().fold(HashMap::new(), |mut wf, l| {
        let [name, mut rules_str] = l.split('{').collect::<Vec<_>>()[..] else {
            panic!("Wrong worflow format {}", l);
        };
        rules_str = &rules_str[..rules_str.len() - 1]; // Drop the '}' at the end
        let rules = rules_str
            .split(',')
            .map(|s| Rule::new_from_str(s))
            .collect::<Vec<_>>();

        wf.insert(name, rules);
        wf
    });

    let parts = lines[space + 1..]
        .iter()
        .map(|l| {
            l[1..l.len() - 1]
                .split(",")
                .map(|v| {
                    v[2..]
                        .parse::<u32>()
                        .expect("Should be a valid u32 part value")
                })
                .collect::<Vec<_>>()
                .try_into()
                .unwrap_or_else(|_| panic!("Expected 4 element vector"))
        })
        .collect::<Vec<_>>();

    (workflows, parts)
}

fn is_part_accepted(workflows: &HashMap<&str, Vec<Rule>>, p: &Part) -> bool {
    let mut current = "in";
    while let Some(rules) = workflows.get(current) {
        for r in rules.iter() {
            if r.matches(p) {
                current = &r.dest;
                break;
            }
        }

        match current {
            "A" => return true,
            "R" => return false,
            _ => (),
        }
    }
    false
}

fn count_accepted_parts(
    current: &str,
    workflows: &HashMap<&str, Vec<Rule>>,
    mut bounds: Range,
) -> u64 {
    match current {
        "A" => distinct_parts_in_range(&bounds),
        "R" => 0,
        _ => {
            let mut total = 0;
            if let Some(rules) = workflows.get(current) {
                for r in rules.iter() {
                    if let (Some(matched), remaining) = r.split_range(&bounds) {
                        total += count_accepted_parts(&r.dest, workflows, matched);

                        match remaining {
                            Some(range) => bounds = range,
                            None => break,
                        };
                    }
                }
            }
            total
        }
    }
}

fn distinct_parts_in_range(r: &Range) -> u64 {
    r.upper
        .iter()
        .zip(r.lower)
        .fold(1, |acc, (u, l)| acc * (u - l - 1) as u64)
}

type Part = [u32; 4];

#[derive(Debug)]
struct Rule {
    pindex: usize,
    relation: Ordering,
    value: u32,
    dest: String,
}

impl Rule {
    fn new() -> Self {
        Rule {
            pindex: 4,
            relation: Ordering::Equal,
            value: 0,
            dest: String::new(),
        }
    }

    fn new_from_str(s: &str) -> Self {
        let mut result = Rule::new();

        let mut split = s.split(':').collect::<Vec<_>>();

        result.dest = split.pop().expect("Split should not be empty").to_string();
        if let Some(condition) = split.last() {
            result.pindex = match condition.chars().nth(0) {
                Some('x') => 0,
                Some('m') => 1,
                Some('a') => 2,
                Some('s') => 3,
                _ => 4,
            };

            result.relation = match condition.chars().nth(1) {
                Some('>') => Ordering::Greater,
                Some('<') => Ordering::Less,
                _ => Ordering::Equal,
            };

            result.value = condition[2..]
                .parse::<u32>()
                .expect("Should be a valid value");
        }
        result
    }

    fn matches(&self, p: &Part) -> bool {
        if self.pindex >= 4 {
            true
        } else {
            p[self.pindex].cmp(&self.value) == self.relation
        }
    }

    fn split_range(&self, r: &Range) -> (Option<Range>, Option<Range>) {
        let mut matched = Range {
            lower: r.lower,
            upper: r.upper,
        };
        let mut remaining = matched;
        match self.relation {
            Ordering::Less => {
                if self.value >= r.upper[self.pindex] {
                    return (None, None);
                } else {
                    matched.upper[self.pindex] = self.value;
                    remaining.lower[self.pindex] = self.value - 1;
                }
            }
            Ordering::Greater => {
                if self.value <= matched.lower[self.pindex] {
                    return (None, None);
                } else {
                    matched.lower[self.pindex] = self.value;
                    remaining.upper[self.pindex] = self.value + 1;
                }
            }
            Ordering::Equal => return (Some(matched), None),
        };

        (Some(matched), Some(remaining))
    }
}

#[derive(Debug, Copy, Clone)]
struct Range {
    lower: Part,
    upper: Part,
}
