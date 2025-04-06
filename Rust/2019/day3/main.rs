use std::{collections::HashSet, hash::Hash, ops::Add};

fn main() {
    let file = include_bytes!("input.txt");
    let input = String::from_utf8_lossy(file);
    let wires: [&str; 2] = input
        .split_whitespace()
        .collect::<Vec<&str>>()
        .try_into()
        .expect("Should be 2 lines with each wire");

    let (part1, part2) = solve(wires);
    println!("Part 1: {}\nPart 2: {}", part1, part2);
}

fn solve(wires: [&str; 2]) -> (i32, u32) {
    let path1 = build_path(wires[0]);
    let path2 = build_path(wires[1]);

    let intersects = path1.iter().filter_map(|p1| match path2.get(p1) {
        Some(p2) => Some((p1, p2)),
        _ => None,
    });

    (
        intersects
            .clone()
            .map(|pair| pair.0.x.abs() + pair.0.y.abs())
            .min()
            .unwrap(),
        intersects
            .map(|pair| pair.0.steps + pair.1.steps)
            .min()
            .unwrap(),
    )
}

fn build_path(wire: &str) -> HashSet<Point> {
    let mut path = HashSet::new();
    let mut pos = Point {
        x: 0,
        y: 0,
        steps: 0,
    };
    wire.split(',').for_each(|step| {
        let dir = match step.chars().nth(0).unwrap() {
            'L' => (-1, 0),
            'R' => (1, 0),
            'U' => (0, 1),
            'D' => (0, -1),
            _ => panic!("Can't park there mate!"),
        };

        let amount = step
            .get(1..)
            .unwrap()
            .parse::<u32>()
            .expect("Should be a valid amount");

        for _ in 0..amount {
            pos = pos + dir;
            pos.steps += 1;
            path.insert(pos);
        }
    });
    path
}

#[derive(Debug, Eq, Clone, Copy)]
struct Point {
    x: i32,
    y: i32,
    steps: u32,
}

impl PartialEq for Point {
    fn eq(&self, other: &Self) -> bool {
        self.x == other.x && self.y == self.y
    }
}

impl Hash for Point {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        state.write_i32(self.x);
        state.write_i32(self.y);
    }
}

impl Add<(i32, i32)> for Point {
    type Output = Self;

    fn add(self, rhs: (i32, i32)) -> Self {
        Self {
            x: self.x + rhs.0,
            y: self.y + rhs.1,
            steps: self.steps,
        }
    }
}

#[test]
fn test_cases() {
    let dist_og = solve(["R8,U5,L5,D3", "U7,R6,D4,L4"]);
    assert_eq!(dist_og, (6, 30));

    let dist = solve([
        "R75,D30,R83,U83,L12,D49,R71,U7,L72",
        "U62,R66,U55,R34,D71,R55,D58,R83",
    ]);
    assert_eq!(dist, (159, 610));

    let dist2 = solve([
        "R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51",
        "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7",
    ]);
    assert_eq!(dist2, (135, 410));
}
