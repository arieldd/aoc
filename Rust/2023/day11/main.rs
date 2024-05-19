use std::cmp::max;
use std::cmp::min;
use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let universe = read_universe(&lines);

    println!("Part 1: {}", solve(&universe, 1));
    println!("Part 2: {}", solve(&universe, 999999));
}

fn solve(universe: &Universe, expansion: usize) -> usize {
    let mut result = 0;

    let n = universe.galaxies.len();

    for i in 0..(n - 1) {
        let g1 = universe.galaxies[i];
        for j in (i + 1)..n {
            let g2 = universe.galaxies[j];
            let dist = distance(g1, g2, expansion, &universe);
            result += dist;
        }
    }

    result
}

fn distance(
    g1: (usize, usize),
    g2: (usize, usize),
    expansion: usize,
    universe: &Universe,
) -> usize {
    let min_row = min(g1.0, g2.0);
    let min_column = min(g1.1, g2.1);
    let max_row = max(g1.0, g2.0);
    let max_column = max(g1.1, g2.1);

    let mut dr: usize = 0;
    let mut dc: usize = 0;

    for &row in universe.empty_rows.iter() {
        if min_row < row && row < max_row {
            dr += expansion;
        }
    }

    for &column in universe.empty_columns.iter() {
        if min_column < column && column < max_column {
            dc += expansion;
        }
    }

    max_row + dr - min_row + max_column + dc - min_column
}

fn read_universe(lines: &[&str]) -> Universe {
    let mut universe = Universe {
        image: vec![],
        galaxies: vec![],
        empty_rows: vec![],
        empty_columns: vec![],
    };

    universe.image = lines
        .iter()
        .enumerate()
        .map(|(i, l)| {
            let mut empty = true;
            let row = l
                .char_indices()
                .map(|(j, c)| {
                    let value = if c == '#' { 1 } else { 0 };
                    if value == 1 {
                        empty = false;
                        universe.galaxies.push((i, j));
                    }

                    value
                })
                .collect();
            if empty {
                universe.empty_rows.push(i);
            }
            row
        })
        .collect();

    for j in 0..universe.image[0].len() {
        let mut empty = true;
        for row in universe.image.iter() {
            if row[j] == 1 {
                empty = false;
            }
        }
        if empty {
            universe.empty_columns.push(j);
        }
    }

    universe
}

struct Universe {
    image: Vec<Vec<u8>>,
    galaxies: Vec<(usize, usize)>,
    empty_rows: Vec<usize>,
    empty_columns: Vec<usize>,
}
