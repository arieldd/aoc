use std::cmp;
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
    almanac
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
        .expect("Locations should have a minimum")
}

fn part2(almanac: &Almanac) -> i64 {
    let seed_ranges: Vec<Range> = almanac
        .seeds
        .chunks(2)
        .map(|c| Range(c[0], c[0] + c[1] - 1))
        .collect();

    seed_ranges
        .iter()
        .map(|sr| {
            let mut src_ranges = vec![*sr];
            let mut dest_ranges: Vec<Range> = vec![];

            for map in almanac.maps.iter() {
                while let Some(range) = src_ranges.pop() {
                    let mut found = false;
                    for pair in map {
                        if let Some(split) = range.split(&pair.0) {
                            let mut intersect = split[0];
                            intersect.0 = pair.0.convert(&pair.1, intersect.0);
                            intersect.1 = pair.0.convert(&pair.1, intersect.1);
                            dest_ranges.push(intersect);

                            src_ranges.extend(split[1..].iter());

                            found = true;
                            break;
                        }
                    }

                    if !found {
                        dest_ranges.push(range);
                    }
                }
                src_ranges = dest_ranges.clone();
                dest_ranges.clear();
            }

            src_ranges
                .iter()
                .min_by(|r1, r2| r1.0.cmp(&r2.0))
                .expect("Ranges should have a minimum")
                .0
        })
        .min()
        .expect("Locations should have a minimum")
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

#[derive(Debug, Hash, Copy, Clone, Eq)]
struct Range(i64, i64);

impl Range {
    fn intercepts(&self, other: &Self) -> bool {
        (self.0 - other.1).signum() * (self.1 - other.0).signum() <= 0
    }

    fn contains(&self, value: i64) -> bool {
        self.0 <= value && self.1 >= value
    }

    fn convert(&self, to: &Self, value: i64) -> i64 {
        to.0 + (value - self.0)
    }

    /// Splits a range by intersecting it with another.
    /// Returns the intersection as the first element followed by any leftovers as independent ranges
    /// Returns None if the ranges do not intercept.
    fn split(&self, by: &Self) -> Option<Vec<Range>> {
        if !self.intercepts(by) {
            return None;
        }

        let mut result = vec![];
        let first = Range(cmp::max(self.0, by.0), cmp::min(self.1, by.1));

        result.push(first);

        if first == *self {
            return Some(result);
        }

        if first == *by {
            if self.0 < first.0 {
                result.push(Range(self.0, first.0 - 1));
            }
            if self.1 > first.1 {
                result.push(Range(first.1 + 1, self.1));
            }
        } else if self.0 < by.0 {
            result.push(Range(self.0, first.0 - 1));
        } else {
            result.push(Range(first.1 + 1, self.1));
        }

        Some(result)
    }
}

impl PartialEq for Range {
    fn eq(&self, other: &Self) -> bool {
        self.0 == other.0 && self.1 == other.1
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
