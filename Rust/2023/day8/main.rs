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

    let map = read_map(&lines);
    println!("Part 1: {}", part1(&map, "AAA"));
    println!("Part 2: {}", part2(&map));
}

fn part1(map: &Map, start: &str) -> u64 {
    let mut current = map.network.get(start).expect("Starting node should exist");

    let n = map.instr.len();
    let mut steps = 0;
    loop {
        if current.label.ends_with('Z') {
            break;
        }
        let dir = map.instr[steps % n];
        steps += 1;
        current = map
            .network
            .get(&current.conn[dir as usize])
            .expect("Connection should exist");
    }
    steps as u64
}

fn part2(map: &Map) -> u64 {
    let mut all_steps: Vec<u64> = vec![];
    for k in map.network.keys() {
        if k.ends_with("A") {
            all_steps.push(part1(map, k));
        }
    }

    all_steps
        .iter()
        .fold(1, |acc, v| num::integer::lcm(acc, *v))
}

fn read_map(lines: &[&str]) -> Map {
    let dirs = lines[0];
    let mut result = Map {
        instr: vec![],
        network: HashMap::new(),
    };

    for dir in dirs.chars() {
        let dir_value = if dir == 'L' { 0 } else { 1 };
        result.instr.push(dir_value);
    }

    for l in lines[2..].iter() {
        let [label, mut node_str] = l.split('=').map(|str| str.trim()).collect::<Vec<_>>()[..]
        else {
            panic!("Wrong node format")
        };

        // Remove ()
        node_str = &node_str[1..node_str.len() - 1];
        let [left, right] = &node_str
            .split(',')
            .map(|str| str.trim().to_string())
            .collect::<Vec<_>>()[..]
        else {
            panic!("Wrong node format");
        };

        result.network.insert(
            label.to_string(),
            Node {
                label: label.to_string(),
                conn: [left.to_string(), right.to_string()],
            },
        );
    }

    result
}

#[derive(Debug)]
struct Map {
    instr: Vec<u8>,
    network: HashMap<String, Node>,
}

#[derive(Debug)]
struct Node {
    label: String,
    conn: [String; 2],
}
