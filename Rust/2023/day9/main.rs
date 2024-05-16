use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();
    let histories = read_histories(&lines);

    let solution = solve(&histories);

    println!("Part 1: {}", solution.1);
    println!("Part 2: {}", solution.0);
}

fn solve(histories: &[Vec<i64>]) -> (i64, i64) {
    histories.iter().fold((0, 0), |acc, h| {
        let predictions = predict_history(h);
        (acc.0 + predictions.0, acc.1 + predictions.1)
    })
}

fn read_histories(lines: &[&str]) -> Vec<Vec<i64>> {
    let mut result = vec![];
    for &l in lines {
        let history = l
            .split(' ')
            .map(|s| s.trim().parse::<i64>().expect("Value should be an integer"))
            .collect::<Vec<_>>();
        result.push(history)
    }
    result
}

fn predict_history(history: &[i64]) -> (i64, i64) {
    let mut front = vec![];
    let mut back = vec![];

    let mut current = history.to_vec();

    front.push(current[0]);
    back.push(current[current.len() - 1]);

    loop {
        let n = current.len();
        let mut next = vec![];
        for i in 1..n {
            next.push(current[i] - current[i - 1]);
        }

        if next.len() == 0 || next.iter().all(|&v| v == 0) {
            break;
        }

        front.push(next[0]);
        back.push(next[next.len() - 1]);

        current = next;
    }
    (
        front.iter().rev().fold(0, |acc, v| v - acc),
        back.iter().rev().fold(0, |acc, v| acc + v),
    )
}
