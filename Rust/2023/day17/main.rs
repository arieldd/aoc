use aoc_utils::grid::FromCharElem;
use aoc_utils::grid::Grid;
use aoc_utils::grid::DX4;
use aoc_utils::grid::DY4;
use std::collections::BinaryHeap;
use std::collections::HashSet;
use std::env;
use std::fs;
use std::hash::Hash;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }
    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let grid = Grid::<Block>::from_lines(&lines);

    println!("Part 1: {}", solve(&grid, 1, 3));
    println!("Part 2: {}", solve(&grid, 4, 10));
}

fn solve(grid: &Grid<Block>, min_steps: usize, max_steps: usize) -> u32 {
    let mut queue = BinaryHeap::new();

    queue.push(State {
        block: Block {
            heat_loss: 0,
            row: 0,
            column: 0,
        },
        dir: 0,
        steps: 0,
    });

    let end = (grid.height - 1, grid.width - 1);

    let mut visited = HashSet::new();

    while let Some(state) = queue.pop() {
        let current = (state.block.row, state.block.column);
        if current == end {
            return state.block.heat_loss;
        }

        if visited.contains(&state) {
            // We were here before with a lower value
            continue;
        }

        visited.insert(state);

        if state.steps < max_steps {
            let ni = state.block.row as isize + DY4[state.dir];
            let nj = state.block.column as isize + DX4[state.dir];

            if let Some(&block) = grid.get(ni, nj) {
                queue.push(State {
                    block: Block {
                        heat_loss: state.block.heat_loss + block.heat_loss,
                        row: block.row,
                        column: block.column,
                    },
                    dir: state.dir,
                    steps: state.steps + 1,
                })
            }
        }

        let left = (state.dir + 1).rem_euclid(4);
        let right = (state.dir as isize - 1).rem_euclid(4) as usize;
        for k in [left, right] {
            let ni = state.block.row as isize + DY4[k] * min_steps as isize;
            let nj = state.block.column as isize + DX4[k] * min_steps as isize;

            if let Some(&block) = grid.get(ni, nj) {
                let mut total_loss = block.heat_loss;
                for step in 1..min_steps {
                    let i = state.block.row as isize + DY4[k] * step as isize;
                    let j = state.block.column as isize + DX4[k] * step as isize;
                    total_loss += grid.get(i, j).expect("Pos should be valid").heat_loss;
                }
                queue.push(State {
                    block: Block {
                        heat_loss: state.block.heat_loss + total_loss,
                        row: block.row,
                        column: block.column,
                    },
                    dir: k,
                    steps: min_steps,
                })
            }
        }
    }

    0
}

#[derive(Debug, Copy, Clone, Eq, PartialEq)]
struct Block {
    heat_loss: u32,
    row: usize,
    column: usize,
}

impl FromCharElem for Block {
    fn new(c: char, row: usize, column: usize) -> Self {
        Block {
            heat_loss: c.to_digit(10).expect("Heat loss should be a digit"),
            row,
            column,
        }
    }
}

#[derive(Copy, Clone, Debug, Eq)]
struct State {
    block: Block,
    dir: usize,
    steps: usize,
}

impl Hash for State {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        state.write_usize(self.block.row);
        state.write_usize(self.block.column);
        state.write_usize(self.dir);
        state.write_usize(self.steps);
    }
}

impl PartialEq for State {
    fn eq(&self, other: &Self) -> bool {
        self.block.row == other.block.row
            && self.block.column == other.block.column
            && self.dir == other.dir
            && self.steps == other.steps
    }
}

impl Ord for State {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        other.block.heat_loss.cmp(&self.block.heat_loss)
    }
}

impl PartialOrd for State {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}
