use std::collections::HashSet;
use std::fmt::Debug;
use std::fs;
use std::{env, isize};

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();
    let grid = read_grid(&lines);

    let solution = solve(&grid);

    println!("Part 1: {}", solution.0);
    println!("Part 2: {}", solution.1);
}

fn solve(grid: &[Vec<Tile>]) -> (usize, usize) {
    let mut energized: Vec<usize> = vec![];
    let n = grid.len();
    let m = grid[0].len();

    let mut visited: HashSet<Tile> = HashSet::new();
    for i in 0..n {
        visited.clear();
        follow_beam(&grid[i][0], &grid, &mut visited);

        energized.push(count_unique(&visited));

        visited.clear();
        let back = &grid[i][m - 1];
        follow_beam(
            &Tile {
                symbol: back.symbol,
                pos: back.pos.clone(),
                dir: 2,
            },
            &grid,
            &mut visited,
        );
        energized.push(count_unique(&visited));
    }

    for j in 0..m {
        visited.clear();
        let mut start = &grid[0][j];
        follow_beam(
            &Tile {
                symbol: start.symbol,
                pos: start.pos.clone(),
                dir: 1,
            },
            &grid,
            &mut visited,
        );

        energized.push(count_unique(&visited));

        visited.clear();
        start = &grid[n - 1][j];
        follow_beam(
            &Tile {
                symbol: start.symbol,
                pos: start.pos.clone(),
                dir: 3,
            },
            &grid,
            &mut visited,
        );
        energized.push(count_unique(&visited));
    }
    (
        energized[0],
        *energized
            .iter()
            .max()
            .expect("There should be a max energized route"),
    )
}

fn count_unique(visited: &HashSet<Tile>) -> usize {
    visited
        .iter()
        .fold(HashSet::new(), |mut set, tile| {
            set.insert(tile.pos.clone());
            set
        })
        .iter()
        .count()
}

fn follow_beam(start: &Tile, grid: &[Vec<Tile>], visited: &mut HashSet<Tile>) {
    if visited.contains(start) {
        return;
    }

    visited.insert(start.clone());

    let n = grid.len() as isize;
    let m = grid[0].len() as isize;
    let hor = start.dir % 2 == 0;
    let mut new_dirs = vec![start.dir];

    if start.symbol == '/' {
        // Reflect "up"
        new_dirs[0] = match start.dir {
            0 => 3,
            1 => 2,
            2 => 1,
            3 => 0,
            x => panic!("Where you going? {}", x),
        }
    } else if start.symbol == '\\' {
        // Reflect "down"
        new_dirs[0] = match start.dir {
            0 => 1,
            1 => 0,
            2 => 3,
            3 => 2,
            x => panic!("Where you going? {}", x),
        }
    } else if (start.symbol == '-' && !hor) || (start.symbol == '|' && hor) {
        // Split the beam
        match hor {
            true => {
                new_dirs[0] = 1;
                new_dirs.push(3);
            }
            false => {
                new_dirs[0] = 0;
                new_dirs.push(2);
            }
        }
    }

    for d in new_dirs {
        let ni = start.pos.0 + DY[d];
        let nj = start.pos.1 + DX[d];
        if is_valid(ni, nj, n, m) {
            let next = &grid[ni as usize][nj as usize];
            follow_beam(
                &Tile {
                    symbol: next.symbol,
                    pos: next.pos.clone(),
                    dir: d,
                },
                grid,
                visited,
            )
        }
    }
}

fn read_grid(lines: &[&str]) -> Vec<Vec<Tile>> {
    lines.iter().enumerate().fold(vec![], |mut grid, (i, l)| {
        grid.push(l.char_indices().fold(vec![], |mut row, (j, c)| {
            row.push(Tile {
                symbol: c,
                pos: Point(i as isize, j as isize),
                dir: 0,
            });
            row
        }));
        grid
    })
}

fn is_valid(i: isize, j: isize, n: isize, m: isize) -> bool {
    i >= 0 && i < n && j >= 0 && j < m
}

fn _print_grid(grid: &[Vec<Tile>]) {
    grid.iter().for_each(|row| {
        row.iter().for_each(|e| print!("{:?}", e));
        println!()
    });
}

#[derive(Eq, Hash, PartialEq, Clone)]
struct Tile {
    symbol: char,
    pos: Point,
    dir: usize,
}

impl Debug for Tile {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_fmt(format_args!("{}", self.symbol))
    }
}

#[derive(Eq, Hash, PartialEq, Clone)]
struct Point(isize, isize);

const DX: [isize; 4] = [1, 0, -1, 0];
const DY: [isize; 4] = [0, 1, 0, -1];
