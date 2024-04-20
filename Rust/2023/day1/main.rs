use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();

    let input = fs::read_to_string(&args[1]).expect("Read input correctly");
    let lines = input.lines().collect::<Vec<_>>();

    println!("Part 1: {}", solve(&lines, false));
    println!("Part 1: {}", solve(&lines, true));
}

fn solve(lines: &[&str], part2: bool) -> u32 {
    let digit_names = [
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    ];

    lines
        .iter()
        .map(|l| {
            let mut digits = vec![];
            for (i, c) in l.char_indices() {
                match c.to_digit(10) {
                    Some(x) => digits.push(x),
                    None => {
                        if part2 {
                            match digit_names.iter().position(|s| l[i..].starts_with(s)) {
                                Some(pos) => digits.push((pos + 1) as u32),
                                None => (),
                            }
                        }
                    }
                }
            }
            digits[0] * 10 + digits[digits.len() - 1]
        })
        .sum()
}
