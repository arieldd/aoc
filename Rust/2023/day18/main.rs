use aoc_utils::grid::Point;
use aoc_utils::grid::DX4;
use aoc_utils::grid::DY4;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let instr = read_instructions(&lines);
    let points = follow_instr(&instr);

    let real_instr = instr
        .iter()
        .map(|i| i.from_hex().unwrap())
        .collect::<Vec<_>>();
    let real_points = follow_instr(&real_instr);

    println!("Part 1: {}", enclosed_area(&points));
    println!("Part 2: {}", enclosed_area(&real_points));
}

fn read_instructions(lines: &[&str]) -> Vec<Instr> {
    lines
        .iter()
        .map(|l| {
            let [turn, number, hex] = l.split(' ').collect::<Vec<_>>()[..] else {
                panic!("Invalid input format on line {}", l);
            };
            let dir = match turn {
                "R" => 0,
                "D" => 1,
                "L" => 2,
                "U" => 3,
                _ => panic!("Where are you going? {}", turn),
            };

            let amount = number
                .parse::<u64>()
                .expect("Value should be a positive int");

            let color = hex[1..hex.len() - 1].to_string();
            Instr { dir, amount, color }
        })
        .collect()
}

fn follow_instr(instr: &[Instr]) -> Vec<Point> {
    instr
        .iter()
        .fold(vec![Point::new(0, 0)], |mut result, dig| {
            let current = result.last().expect("Result should not be empty");
            let next = Point(
                current.0 + DX4[dig.dir] * dig.amount as isize,
                current.1 + DY4[dig.dir] * dig.amount as isize,
            );
            result.push(next);
            result
        })
}

fn enclosed_area(points: &[Point]) -> u64 {
    let mut area = 0;
    let mut perimeter = 0;
    let n = points.len();

    for i in 0..n - 1 {
        let p_i = &points[i];
        let p_j = &points[i + 1];

        area += p_i.0 * p_j.1 - p_i.1 * p_j.0;
        perimeter += (p_j.0 - p_i.0 + p_j.1 - p_i.1).abs();
    }
    (area / 2).abs() as u64 + perimeter as u64 / 2 + 1
}

#[derive(Debug)]
struct Instr {
    dir: usize,
    amount: u64,
    color: String,
}

impl Instr {
    fn from_hex(&self) -> Option<Instr> {
        if self.color.is_empty() {
            None
        } else {
            let mut hex_color = self.color.clone();
            let dir = hex_color
                .pop()
                .unwrap()
                .to_digit(10)
                .expect("Should be a valid dir") as usize;
            let amount =
                u64::from_str_radix(&hex_color[1..], 16).expect("Should be a valid hex value");
            Some(Instr {
                dir,
                amount,
                color: String::new(),
            })
        }
    }
}
