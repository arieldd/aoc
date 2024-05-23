use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let init_seq = parse_sequence(&lines);

    println!("Part 1: {}", part1(&init_seq));
    println!("Part 2: {}", part2(&init_seq));
}

fn part1(seq: &Sequence) -> usize {
    seq.iter().fold(0, |acc, s| acc + hash(&s))
}

fn part2(seq: &Sequence) -> u64 {
    let mut boxes: Vec<Box> = vec![];
    for _ in 0..256 {
        boxes.push(vec![]);
    }

    seq.iter().for_each(|s| {
        let lens = Lens::from_seq(&s);
        let index = hash(&lens.label);

        match lens.op {
            Operation::Add => match boxes[index].iter().position(|l| *l == lens) {
                Some(slot) => boxes[index][slot] = lens,
                None => boxes[index].push(lens),
            },
            Operation::Remove => {
                if let Some(slot) = boxes[index].iter().position(|l| *l == lens) {
                    boxes[index].remove(slot);
                }
            }
        }
    });

    boxes.iter().enumerate().fold(0, |total, (i, b)| {
        let box_total = b
            .iter()
            .enumerate()
            .fold(0, |acc, (slot, l)| acc + l.focusing_power(i, slot));
        total + box_total
    })
}

fn parse_sequence(lines: &[&str]) -> Sequence {
    lines.iter().fold(vec![], |mut acc, l| {
        acc.extend(l.split(',').map(|s| s.to_string()));
        acc
    })
}

fn hash(s: &str) -> usize {
    s.chars().fold(0, |mut current, c| {
        current += c as usize;
        current *= 17;
        current = current % 256;
        current
    })
}

type Sequence = Vec<String>;
type Box = Vec<Lens>;

#[derive(Debug)]
struct Lens {
    label: String,
    op: Operation,
    length: u32,
}

impl PartialEq for Lens {
    fn eq(&self, other: &Self) -> bool {
        self.label == other.label
    }
}

#[derive(Debug)]
enum Operation {
    Add,
    Remove,
}

impl Lens {
    fn from_seq(s: &str) -> Self {
        let parts: Vec<&str> = s.split(['-', '=']).collect();
        let label = parts[0].to_string();
        let op = if parts[1] == "" {
            Operation::Remove
        } else {
            Operation::Add
        };
        let length = match parts[1].parse::<u32>() {
            Ok(value) => value,
            Err(..) => 0,
        };

        Lens { label, op, length }
    }

    fn focusing_power(&self, container: usize, slot: usize) -> u64 {
        ((container + 1) * (slot + 1) * self.length as usize) as u64
    }
}
