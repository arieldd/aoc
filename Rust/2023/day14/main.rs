use std::collections::HashMap;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let rocks = read_rocks(&lines);

    println!("Part 1: {}", part1(&rocks));
    println!("Part 2: {}", part2(&rocks, 1000000000));
}

fn part1(rocks: &Platform) -> usize {
    let mut tilted = rocks.clone();
    tilt(&mut tilted, 0);
    north_load(&tilted)
}

fn part2(rocks: &Platform, cycles: u32) -> usize {
    let mut tilted = rocks.clone();

    let mut store = HashMap::new();
    let mut start = 0;
    let mut length = 0;

    for cycle in 1..cycles + 1 {
        for dir in 0..4 {
            tilt(&mut tilted, dir);
        }

        if let Some(value) = store.get(&tilted) {
            start = *value;
            length = cycle - start;
            break;
        } else {
            store.insert(tilted.clone(), cycle);
        }
    }

    let remaining = (cycles - start) % length;
    for _ in 0..remaining {
        for dir in 0..4 {
            tilt(&mut tilted, dir);
        }
    }

    north_load(&tilted)
}

fn read_rocks(lines: &[&str]) -> Platform {
    lines.iter().map(|l| l.chars().collect()).collect()
}

fn _print_rocks(rocks: &Platform) {
    rocks.iter().for_each(|row| println!("{:?}", row))
}

fn tilt(rocks: &mut Platform, dir: usize) {
    let n = rocks.len();
    let m = rocks[0].len();
    match dir {
        0 => {
            for j in 0..m {
                for i in 0..n {
                    if rocks[i][j] == 'O' {
                        let mut k = i;
                        while k as i32 - 1 >= 0 && rocks[k - 1][j] == '.' {
                            k -= 1;
                        }
                        if k != i {
                            rocks[k][j] = 'O';
                            rocks[i][j] = '.';
                        }
                    }
                }
            }
        }
        1 => {
            for i in 0..n {
                for j in 0..m {
                    if rocks[i][j] == 'O' {
                        let mut k = j;
                        while k as i32 - 1 >= 0 && rocks[i][k - 1] == '.' {
                            k -= 1;
                        }
                        if k != j {
                            rocks[i][k] = 'O';
                            rocks[i][j] = '.';
                        }
                    }
                }
            }
        }
        2 => {
            for j in 0..m {
                for i in (0..n).rev() {
                    if rocks[i][j] == 'O' {
                        let mut k = i;
                        while k + 1 < n && rocks[k + 1][j] == '.' {
                            k += 1;
                        }
                        if k != i {
                            rocks[k][j] = 'O';
                            rocks[i][j] = '.';
                        }
                    }
                }
            }
        }
        3 => {
            for i in 0..n {
                for j in (0..m).rev() {
                    if rocks[i][j] == 'O' {
                        let mut k = j;
                        while k + 1 < m && rocks[i][k + 1] == '.' {
                            k += 1;
                        }
                        if k != j {
                            rocks[i][k] = 'O';
                            rocks[i][j] = '.';
                        }
                    }
                }
            }
        }
        dir => panic!("{} this shouldn't be here you know", dir),
    }
}

fn north_load(rocks: &Platform) -> usize {
    let n = rocks.len();
    rocks.iter().enumerate().fold(0, |total, (i, r)| {
        total
            + r.iter()
                .fold(0, |acc, &c| if c == 'O' { acc + n - i } else { acc })
    })
}

type Platform = Vec<Vec<char>>;
