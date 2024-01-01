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
    let mut count = 0;

    for l in lines {
        let digits = l
            .chars()
            .filter(|c| c.is_ascii_digit())
            .map(|c| (c as u8) - b'0')
            .collect::<Vec<_>>();

        let number: u32 = (digits[0] * 10 + digits[digits.len() - 1]).into();
        count += number;
    }

    count
}

fn part2(lines: &[&str]) -> u32 {
    let digit_names: HashMap<&str, u32> = HashMap::from([
        ("one", 1),
        ("two", 2),
        ("three", 3),
        ("four", 4),
        ("five", 5),
        ("six", 6),
        ("seven", 7),
        ("eight", 8),
        ("nine", 9),
    ]);

    let mut count = 0;

    for l in lines {
        let mut digits: Vec<u32> = vec![];
        for i in 0..l.len() {
            let c = l.chars().nth(i).unwrap();
            if c.is_ascii_digit() {
                digits.push(c.to_digit(10).unwrap());
            } else {
                for k in 2..=4 {
                    if i + k < l.len() {
                        let slice = &l[i..=i + k];
                        if digit_names.contains_key(slice) {
                            digits.push(digit_names[slice]);
                        }
                    }
                }
            }
        }
        let number: u32 = (digits[0] * 10 + digits[digits.len() - 1]).into();
        count += number;
    }

    count
}
