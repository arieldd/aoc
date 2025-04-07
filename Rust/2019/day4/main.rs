use std::collections::HashMap;

fn main() {
    let mut args = std::env::args().skip(1);
    assert!(args.len() > 0, "Please provide you puzzle input");

    let input = args.next().unwrap();
    let mut range = input.split('-');

    let min = range
        .next()
        .unwrap()
        .parse::<usize>()
        .expect("Need a valid number mate");
    let max = range
        .next()
        .unwrap()
        .parse::<usize>()
        .expect("Need a valid number mate");

    let (part1, part2) = solve(min, max);
    println!("Part 1: {}\nPart 2: {}", part1, part2);
}

fn solve(min: usize, max: usize) -> (usize, usize) {
    let mut part1 = 0;
    let mut part2 = 0;

    for pwd in min..max {
        let (valid1, valid2) = is_valid_password(pwd);
        if valid1 {
            part1 += 1;
        }
        if valid2 {
            part2 += 1;
        }
    }
    (part1, part2)
}

fn is_valid_password(pwd: usize) -> (bool, bool) {
    let mut prev = 10;
    let mut freq = HashMap::new();
    for digit in get_digits(pwd) {
        *freq.entry(digit).or_insert(0) += 1;
        if digit > prev {
            return (false, false);
        };
        prev = digit;
    }

    (
        freq.values().any(|&v| v >= 2),
        freq.values().any(|&v| v == 2),
    )
}

fn get_digits(mut number: usize) -> Vec<u8> {
    let mut digits = vec![];
    while number > 0 {
        digits.push((number % 10) as u8);
        number /= 10;
    }
    digits
}
