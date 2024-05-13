use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let values = parse_lines(&lines);
    let races = parse_races(&values);
    let big_race = parse_big_race(&values);

    println!("Part 1: {}", solve(&races));
    println!("Part 2: {}", count_wins(&big_race));
}

fn solve(races: &Vec<Race>) -> i64 {
    let wins = races.iter().map(|r| count_wins(&r));
    wins.fold(1, |acc, v| acc * v)
}

fn parse_lines(lines: &[&str]) -> Vec<Vec<i64>> {
    lines
        .iter()
        .map(|l| {
            l.split(':')
                .last()
                .expect("Line should contain a :")
                .trim()
                .split(' ')
                .filter(|v| *v != "")
                .map(|v| v.parse::<i64>().expect("Value should be an integer"))
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>()
}

fn parse_races(values: &Vec<Vec<i64>>) -> Vec<Race> {
    values
        .chunks(2)
        .map(|c| {
            c[0].iter()
                .zip(c[1].iter())
                .map(|pair| Race {
                    time: *pair.0,
                    record: *pair.1,
                })
                .collect::<Vec<_>>()
        })
        .last()
        .expect("Should contain some races")
}

fn parse_big_race(values: &Vec<Vec<i64>>) -> Race {
    let mut numbers = values
        .iter()
        .map(|v| v.iter().fold(0, |acc, n| acc * 10_i64.pow(digits(*n)) + n));

    Race {
        time: numbers.next().expect("Should have built a time"),
        record: numbers.last().expect("Should have built a record"),
    }
}

fn digits(mut n: i64) -> u32 {
    let mut digits = 0;
    loop {
        n /= 10;
        digits += 1;
        if n == 0 {
            break;
        }
    }
    digits
}

fn count_wins(race: &Race) -> i64 {
    // Solving for 0 > x^2 - t*x + r
    let d = race.time * race.time - 4 * race.record;
    let x1 = (race.time as f64 - f64::sqrt(d as f64)) / 2.0;
    let x2 = (race.time as f64 + f64::sqrt(d as f64)) / 2.0;
    let mut result = (x2 as i64 - x1 as i64 - 1).abs();
    //check endpoints
    if race.can_win(x1 as i64) {
        result += 1;
    }
    if race.can_win(x2 as i64) {
        result += 1;
    }
    result
}

#[derive(Debug)]
struct Race {
    time: i64,
    record: i64,
}

impl Race {
    fn can_win(&self, x: i64) -> bool {
        (self.time - x) * x > self.record
    }
}
