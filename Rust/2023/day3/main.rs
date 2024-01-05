use std::env;
use std::fs;

fn main() {
    let input = fs::read_to_string(env::args().skip(1).next().expect("Input file name"))
        .expect("Input read");

    let (gears, parts) = read_schematic(&input);

    println!("Part 1: {}", part1(&&gears, &parts));
    println!("Part 2: {}", part2(&&gears, &parts));
}

static DX: [i32; 8] = [1, 1, 0, -1, -1, -1, 0, 1];
static DY: [i32; 8] = [0, 1, 1, 1, 0, -1, -1, -1];

fn part1(gears: &[Gear], parts: &[Part]) -> u32 {
    gears
        .iter()
        .map(|g| {
            let mut sum = 0;
            let mut count = 0;

            for p in parts {
                if is_adj(&g, &p) {
                    sum += p.value;
                    count += 1;
                }
                if count >= 4 {
                    break;
                }
            }
            sum
        })
        .sum()
}

fn part2(gears: &[Gear], parts: &[Part]) -> u32 {
    let mut sum = 0;
    for g in gears.iter().filter(|g| g.symbol == '*') {
        let mut ratio = 1;
        let mut count = 0;
        for p in parts {
            if is_adj(&g, &p) {
                count += 1;
                ratio *= p.value;
            }
            if count > 2 {
                break;
            }
        }
        if count == 2 {
            sum += ratio
        }
    }
    sum
}

fn read_schematic(input: &str) -> (Vec<Gear>, Vec<Part>) {
    let mut parts: Vec<Part> = vec![];
    let mut gears: Vec<Gear> = vec![];

    for (i, l) in input.lines().enumerate() {
        let mut digit = 10;
        for (j, c) in l.char_indices() {
            let pos = (i as i32, j as i32);
            match c.to_digit(10) {
                Some(x) => {
                    if digit == 10 {
                        parts.push(Part {
                            value: x,
                            positions: vec![pos],
                        });
                    } else {
                        let n = parts.len();
                        parts[n - 1].value *= 10;
                        parts[n - 1].value += x;
                        parts[n - 1].positions.push(pos)
                    }
                    digit = x;
                }
                None => {
                    digit = 10;
                    if c != '.' {
                        gears.push(Gear {
                            symbol: c,
                            coord: pos,
                        });
                    }
                }
            }
        }
    }

    (gears, parts)
}

fn is_adj(gear: &Gear, part: &Part) -> bool {
    for d in 0..8 {
        let pos = (gear.coord.0 + DY[d], gear.coord.1 + DX[d]);
        if part.positions.contains(&pos) {
            return true;
        }
    }
    false
}

#[derive(Debug)]
struct Part {
    value: u32,
    positions: Vec<(i32, i32)>,
}

#[derive(Debug)]
struct Gear {
    symbol: char,
    coord: (i32, i32),
}
