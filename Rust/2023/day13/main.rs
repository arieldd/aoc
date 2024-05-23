use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let notes = read_notes(&lines);

    println!("Part 1: {}", solve(&notes, 0));
    println!("Part 2: {}", solve(&notes, 1));
}

fn solve(notes: &[Pattern], allowed_diff: u32) -> usize {
    let summary = notes.iter().fold((0, 0), |acc, p| {
        let reflection = find_reflection(p, allowed_diff);
        (acc.0 + reflection.0, acc.1 + reflection.1)
    });

    100 * summary.0 + summary.1
}

fn find_reflection(p: &Pattern, allowed_diff: u32) -> (usize, usize) {
    let n = p.grid.len();

    for i in 1..n {
        let mut diff = str_diff(&p.grid[i - 1], &p.grid[i]);
        if diff <= allowed_diff {
            let unexplored = std::cmp::min(i - 1, n - i - 1) + 1;
            for k in 1..unexplored {
                diff += str_diff(&p.grid[i - k - 1], &p.grid[i + k]);
                if diff > allowed_diff {
                    break;
                }
            }
            if diff == allowed_diff {
                return (i, 0);
            }
        }
    }

    let m = p.grid[0].len();
    for j in 1..m {
        let mut diff = str_diff(&p.get_column(j - 1), &p.get_column(j));
        if diff <= allowed_diff {
            let unexplored = std::cmp::min(j - 1, m - j - 1) + 1;
            for k in 1..unexplored {
                diff += str_diff(&p.get_column(j - k - 1), &p.get_column(j + k));
                if diff > allowed_diff {
                    break;
                }
            }
            if diff == allowed_diff {
                return (0, j);
            }
        }
    }

    (0, 0)
}

fn str_diff(s1: &str, s2: &str) -> u32 {
    s1.chars()
        .zip(s2.chars())
        .fold(0, |acc, pair| if pair.0 == pair.1 { acc } else { acc + 1 })
}

fn read_notes(lines: &[&str]) -> Vec<Pattern> {
    lines.iter().fold(vec![Pattern::new()], |mut result, l| {
        if l.is_empty() {
            result.push(Pattern::new())
        } else {
            let mut current = result.pop().expect("Result should not be empty");
            current.grid.push(l.to_string());
            result.push(current);
        }
        result
    })
}

#[derive(Debug)]
struct Pattern {
    grid: Vec<String>,
}

impl Pattern {
    fn new() -> Self {
        Pattern { grid: vec![] }
    }

    fn get_column(&self, j: usize) -> String {
        self.grid.iter().fold(String::new(), |mut acc, r| {
            acc.push(r.chars().nth(j).unwrap());
            acc
        })
    }
}
