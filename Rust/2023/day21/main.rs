use std::collections::HashSet;
use std::env;
use std::fs;

use aoc_utils::grid::FromCharElem;
use aoc_utils::grid::Grid;
use aoc_utils::grid::DX4;
use aoc_utils::grid::DY4;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let map = Grid::<Tile>::from_lines(&lines);
    let start = map
        .iter()
        .find(|t| t.start)
        .expect("A start tile should exist");

    println!("Part 1: {}", part1(&map, start));
    println!("Part 2: {}", part2(&map, start, 26501365));
}

fn part1(map: &Grid<Tile>, start: &Tile) -> usize {
    count_tiles(map, start, 64)
}

fn part2(map: &Grid<Tile>, start: &Tile, steps: usize) -> usize {
    count_tiles(map, start, steps)
}

fn count_tiles(map: &Grid<Tile>, start: &Tile, steps: usize) -> usize {
    let mut set = HashSet::<&Tile>::new();

    set.insert(start);

    for _ in 0..steps {
        let mut next_tiles = HashSet::<&Tile>::new();
        set.iter().for_each(|t| {
            for k in 0..4 {
                let nr = t.row as isize + DY4[k];
                let nc = t.column as isize + DX4[k];

                if let Some(adjacent) = map.get(nr, nc) {
                    if !adjacent.rock {
                        next_tiles.insert(adjacent);
                    }
                }
            }
        });

        set = next_tiles;
    }

    set.len()
}

fn _print_map(map: &Grid<Tile>) {
    for i in 0..map.height {
        for j in 0..map.width {
            let tile = map.at(i, j).expect("A tile should be here");

            let mut display = if tile.rock { '#' } else { '.' };
            if tile.start {
                display = 'S';
            }
            print!("{}", display);
        }
        println!();
    }
}

#[derive(Debug, PartialEq, Eq, Hash)]
struct Tile {
    row: usize,
    column: usize,
    rock: bool,
    start: bool,
}

impl FromCharElem for Tile {
    fn new(c: char, row: usize, column: usize) -> Self {
        Tile {
            row,
            column,
            rock: c == '#',
            start: c == 'S',
        }
    }
}
