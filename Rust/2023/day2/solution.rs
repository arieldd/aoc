use std::cmp::max;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();

    let input = fs::read_to_string(&args[1]).unwrap();
    let games: Vec<Game> = input.lines().map(|l| l.into()).collect();

    println!("Part 1: {}", part1(&games, (12, 13, 14)));
    println!("Part 2: {}", part2(&games));
}

fn part1(games: &Vec<Game>, limits: (u32, u32, u32)) -> u32 {
    games
        .iter()
        .filter_map(|g| {
            match g
                .sets
                .iter()
                .any(|s| s.0 > limits.0 || s.1 > limits.1 || s.2 > limits.2)
            {
                true => None,
                false => Some(g.id),
            }
        })
        .sum()
}

fn part2(games: &Vec<Game>) -> u32 {
    games
        .iter()
        .map(|g| {
            let min_set = g.sets.iter().fold((0, 0, 0), |acc, s| {
                (max(acc.0, s.0), max(acc.1, s.1), max(acc.2, s.2))
            });
            min_set.0 * min_set.1 * min_set.2
        })
        .sum()
}

#[derive(Debug)]
struct Game {
    id: u32,
    sets: Vec<(u32, u32, u32)>,
}

impl From<&str> for Game {
    fn from(s: &str) -> Self {
        let (head, body) = s.split_once(':').unwrap();
        let id = head.split_once(' ').expect("Game,id").1.parse().unwrap();
        let sets = body
            .split(';')
            .map(|round| {
                let (mut red, mut green, mut blue) = (0, 0, 0);
                for play in round.split(',') {
                    let mut color = play.split_whitespace();
                    let n = color.next().expect("Color amount").parse().unwrap();
                    match color.last() {
                        Some("red") => red = n,
                        Some("green") => green = n,
                        Some("blue") => blue = n,
                        Some(&_) | None => panic!("{}", play),
                    }
                }
                (red, green, blue)
            })
            .collect();
        Game { id, sets }
    }
}
