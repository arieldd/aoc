use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).unwrap();
    let lines = input.lines().collect::<Vec<_>>();
    println!("Part 1: {}", part1(&lines));
    println!("Part 2: {}", part2(&lines));
}

fn part1(_lines: &[&str]) -> u32 {
    0
}

fn part2(_lines: &[&str]) -> u32 {
    0
}
