use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();

    let input = fs::read_to_string(&args[1]).expect("Read input correctly");
    let lines = input.lines().collect::<Vec<_>>();

    println!("Part 1: {}", part1(&lines));
    println!("Part 1: {}", part2(&lines));
}

fn part1(lines: &[&str]) -> u32 {
    let mut count = 0;

    for l in lines {
        let digits = l.chars().filter_map(|c| c.to_digit(10)).collect::<Vec<_>>();

        let number: u32 = (digits[0] * 10 + digits[digits.len() - 1]).into();
        count += number;
    }

    count
}

fn part2(lines: &[&str]) -> u32 {
    let digit_names = [
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
    ];

    let mut count = 0;

    for l in lines {
        let mut digits: Vec<u32> = vec![];
        for (i, c) in l.char_indices() {
            match c.to_digit(10) {
                Some(x) => digits.push(x),
                None => match digit_names.iter().position(|s| l[i..].starts_with(s)) {
                    Some(pos) => digits.push((pos + 1) as u32),
                    None => (),
                },
            }
        }
        let number = (digits[0] * 10 + digits[digits.len() - 1]) as u32;
        count += number;
    }

    count
}
