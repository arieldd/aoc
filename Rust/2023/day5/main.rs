use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).unwrap();
    let lines = input.lines().collect::<Vec<_>>();

    let almanac = read_almanac(&lines);

    println!("Part 1: {}", part1(&almanac));
    println!("Part 2: {}", part2(&almanac));
}

fn part1(almanac: &Almanac) -> i64 {
    match almanac
        .seeds
        .iter()
        .map(|s| {
            let mut value = *s;
            for map in almanac.maps.iter() {
                for pair in map {
                    if pair.0.contains(value) {
                        value = pair.0.convert(&pair.1, value);
                        break;
                    }
                }
            }
            value
        })
        .min()
    {
        Some(loc) => loc,
        None => -1,
    }
}

fn part2(almanac: &Almanac) -> i64 {
    let mut seed_ranges: Vec<Range> = almanac.seeds.chunks(2).map(|c| Range(c[0], c[1])).collect();

    0
}

fn read_almanac(lines: &[&str]) -> Almanac {
    let seeds = lines[0]
        .split(" ")
        .skip(1) //Skip 'seeds:'
        .map(|s| s.parse::<i64>().expect("Failed to parse seed value"))
        .collect::<Vec<_>>();

    let mut maps = Vec::new();

    maps = lines[1..].iter().fold(maps, |mut maps, l| {
        match l.chars().nth(0) {
            Some(c) => {
                if c > '9' {
                    return maps; // Ignore map header text
                }
            }
            None => {
                maps.push(Vec::new());
                return maps;
            }
        }

        let [dst, src, size] = l
            .split(" ")
            .map(|v| v.parse::<i64>().expect("Failed to parse value"))
            .collect::<Vec<_>>()[..]
        else {
            panic!("Failed to read mappings")
        };

        match maps.last_mut() {
            Some(m) => m.push((Range(src, src + size - 1), Range(dst, dst + size - 1))),
            None => (),
        }

        maps
    });

    Almanac { seeds, maps }
}

fn _print_almanac(almanac: &Almanac) {
    println!("{:?}", almanac.seeds);

    for map in almanac.maps.iter() {
        println!();
        println!("{:?}", map);
    }
}

#[derive(Debug)]
struct Almanac {
    seeds: Vec<i64>,
    maps: Vec<Vec<(Range, Range)>>,
}

#[derive(Debug)]
struct Range(i64, i64);

impl Range {
    fn intercepts(&self, other: &Range) -> bool {
        (self.0 - other.1) * (self.1 - other.0) <= 0
    }

    fn contains(&self, value: i64) -> bool {
        self.0 <= value && self.1 >= value
    }

    fn convert(&self, other: &Range, value: i64) -> i64 {
        other.0 + (value - self.0)
    }

    fn split(&self, by: &Range) -> Option<Vec<Range>> {
        if !self.intercepts(by) {
            return None;
        }

        Some(vec![])
    }
}

#[test]
fn range_intercepts() {
    let r1 = Range(2, 10);
    let r2 = Range(6, 15);
    let r3 = Range(11, 15);
    assert!(r1.intercepts(&r2));
    assert!(r2.intercepts(&r1));
    assert!(!r3.intercepts(&r1));
}
