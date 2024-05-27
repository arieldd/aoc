use std::collections::HashSet;
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

fn solve(grid: &[Vec<char>]) -> (usize, usize) {
    let mut energized: Vec<usize> = vec![];
    let n = grid.len();
    let m = grid[0].len();

    let mut visited: HashSet<Tile> = HashSet::new();
    for i in 0..n {
        visited.clear();
        follow_beam(
            &Tile {
                pos: Point(i as isize, 0),
                dir: 0,
            },
            &grid,
            &mut visited,
        );

        energized.push(count_unique(&visited));

        visited.clear();
        follow_beam(
            &Tile {
                pos: Point(i as isize, m as isize - 1),
                dir: 2,
            },
            &grid,
            &mut visited,
        );
        energized.push(count_unique(&visited));
    }

    for j in 0..m {
        visited.clear();
        follow_beam(
            &Tile {
                pos: Point(0, j as isize),
                dir: 1,
            },
            &grid,
            &mut visited,
        );

        energized.push(count_unique(&visited));

        visited.clear();
        follow_beam(
            &Tile {
                pos: Point(n as isize - 1, j as isize),
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
        .len()
}

fn follow_beam(start: &Tile, grid: &[Vec<char>], visited: &mut HashSet<Tile>) {
    let n = grid.len() as isize;
    let m = grid[0].len() as isize;

    let mut i = start.pos.0;
    let mut j = start.pos.1;
    let mut dir = start.dir;

    while is_valid(i, j, n, m) {
        let current = &Tile {
            pos: Point(i as isize, j as isize),
            dir,
        };

        if visited.contains(current) {
            return;
        }
        visited.insert(current.clone());

        let hor = dir % 2 == 0;
        let symbol = grid[i as usize][j as usize];

        if symbol == '/' {
            // Reflect "up"
            dir = match dir {
                0 => 3,
                1 => 2,
                2 => 1,
                3 => 0,
                x => panic!("Where you going? {}", x),
            }
        } else if symbol == '\\' {
            // Reflect "down"
            dir = match dir {
                0 => 1,
                1 => 0,
                2 => 3,
                3 => 2,
                x => panic!("Where you going? {}", x),
            }
        } else if (symbol == '-' && !hor) || (symbol == '|' && hor) {
            // Split the beam
            let dir2: usize;
            match hor {
                true => {
                    dir = 1;
                    dir2 = 3;
                }
                false => {
                    dir = 0;
                    dir2 = 2;
                }
            }
            let ni = i + DY[dir2];
            let nj = j + DX[dir2];
            follow_beam(
                &Tile {
                    pos: Point(ni, nj),
                    dir: dir2,
                },
                grid,
                visited,
            );
        }

        i += DY[dir];
        j += DX[dir]
    }
}

fn read_grid(lines: &[&str]) -> Vec<Vec<char>> {
    lines.iter().fold(vec![], |mut grid, l| {
        grid.push(l.chars().fold(vec![], |mut row, c| {
            row.push(c);
            row
        }));
        grid
    })
}

fn is_valid(i: isize, j: isize, n: isize, m: isize) -> bool {
    i >= 0 && i < n && j >= 0 && j < m
}

#[derive(Eq, Hash, PartialEq, Clone)]
struct Tile {
    pos: Point,
    dir: usize,
}

#[derive(Eq, Hash, PartialEq, Clone)]
struct Point(isize, isize);

const DX: [isize; 4] = [1, 0, -1, 0];
const DY: [isize; 4] = [0, 1, 0, -1];
