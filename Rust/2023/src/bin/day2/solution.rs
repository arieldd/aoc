use std::env;
use std::fs;
use std::collections::HashMap;

fn main() {
    let args: Vec<String> = env::args().collect();

    let input = fs::read_to_string(&args[1]).unwrap();
    let lines = input.lines().collect::<Vec<_>>();

    println!("Part 1: {}", part1(&lines));
    println!("Part 1: {}", part2(&lines));
}

fn part1(lines: &[&str]) -> u32 {
    0
}

fn part2(lines: &[&str]) -> u32 {
    0
}
