use std::env;
use std::fs;

fn main() {
    let args: Vec<_> = env::args().collect();
    if args.len() < 2 {
        panic!("Please provide an input file.")
    }

    let input = fs::read_to_string(&args[1]).expect("Should be able to read input file");
    let lines = input.lines().collect::<Vec<_>>();

    let maze = read_maze(&lines);
    let cycle = find_loop(&maze);

    println!("Part 1: {}", cycle.len() / 2);
    println!("Part 2: {}", find_enclosed_area(&cycle, &maze));
}

fn find_loop(maze: &Maze) -> Vec<(i32, i32)> {
    let mut cycle = vec![maze.start.pos];
    let mut prev = (-1, -1);
    loop {
        let current = cycle[cycle.len() - 1];
        for &k in get_connections(&maze.grid[current.0 as usize][current.1 as usize]) {
            let (ni, nj) = (current.0 + DY[k], current.1 + DX[k]);
            if (ni, nj) != prev {
                prev = current;
                cycle.push((ni, nj));
                break;
            }
        }
        if cycle[cycle.len() - 1] == maze.start.pos {
            cycle.pop();
            break;
        }
    }
    cycle
}

fn find_enclosed_area(cycle: &[(i32, i32)], maze: &Maze) -> i32 {
    let mut vertices = vec![];
    for pos in cycle.iter() {
        let p = maze.grid[pos.0 as usize][pos.1 as usize];
        if p.label != '|' && p.label != '-' {
            vertices.push(pos);
        }
    }

    vertices.push(vertices[0]);
    let mut result = 0;

    for i in 0..vertices.len() - 1 {
        let (x_i, y_i) = vertices[i];
        let (x_j, y_j) = vertices[i + 1];
        result += x_i * y_j - x_j * y_i;
    }

    // Shoelace formula for the area and remove the perimeter
    (result / 2).abs() - (cycle.len() as i32 / 2) + 1
}

fn read_maze(lines: &[&str]) -> Maze {
    let mut maze = Maze {
        start: Pipe::new(),
        grid: vec![],
    };

    for (i, l) in lines.iter().enumerate() {
        let mut row = vec![];
        for (j, c) in l.char_indices() {
            let p = Pipe {
                label: c,
                pos: (i as i32, j as i32),
            };
            row.push(p);

            if p.label == 'S' {
                maze.start = p;
            }
        }
        maze.grid.push(row);
    }

    set_starting_pipe(&mut maze);

    maze
}

fn set_starting_pipe(maze: &mut Maze) {
    let mut conn = 0;
    let n = maze.grid.len() as i32;
    let m = maze.grid[0].len() as i32;

    let mut start = maze.grid[maze.start.pos.0 as usize][maze.start.pos.1 as usize];

    for k in 0..4 {
        let (ni, nj) = (start.pos.0 + DY[k], start.pos.1 + DX[k]);
        if is_valid(ni, nj, n, m) {
            let next = maze.grid[ni as usize][nj as usize];
            if can_move(&next, k) {
                conn *= 2;
                conn += k;
            }
        }
    }

    match conn {
        1 => start.label = 'F',
        2 => start.label = '-',
        3 => start.label = 'L',
        4 => start.label = '7',
        5 => start.label = '|',
        7 => start.label = 'J',
        _ => panic!("Too many pipes leave S"),
    }

    maze.grid[start.pos.0 as usize][start.pos.1 as usize] = start;
}

fn can_move(p: &Pipe, dir: usize) -> bool {
    match dir {
        0 => p.label == '-' || p.label == 'J' || p.label == '7',
        1 => p.label == '|' || p.label == 'J' || p.label == 'L',
        2 => p.label == '-' || p.label == 'F' || p.label == 'L',
        3 => p.label == '|' || p.label == 'F' || p.label == '7',
        _ => panic!("Invalid move direction {}", dir),
    }
}

fn is_valid(i: i32, j: i32, n: i32, m: i32) -> bool {
    i >= 0 && i < n && j >= 0 && j < m
}

fn get_connections(p: &Pipe) -> &[usize] {
    match p.label {
        '|' => &[1, 3],
        '-' => &[0, 2],
        'F' => &[0, 1],
        'J' => &[2, 3],
        'L' => &[0, 3],
        '7' => &[1, 2],
        _ => panic!("Invalid pipe label {}", p.label),
    }
}

#[derive(Debug)]
struct Maze {
    start: Pipe,
    grid: Vec<Vec<Pipe>>,
}

#[derive(Debug, Clone, Copy, Eq, PartialEq)]
struct Pipe {
    label: char,
    pos: (i32, i32),
}

impl Pipe {
    fn new() -> Self {
        Pipe {
            label: '?',
            pos: (-1, -1),
        }
    }
}

const DX: [i32; 4] = [1, 0, -1, 0];
const DY: [i32; 4] = [0, 1, 0, -1];
