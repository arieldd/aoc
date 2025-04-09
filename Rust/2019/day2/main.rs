use intcode::Computer;

fn main() {
    let file = include_bytes!("input.txt");
    let input = String::from_utf8_lossy(file);
    let program = Computer::new(&input);

    println!("Part 1: {}", part1(&program));
    println!("Part 2: {}", part2(&program, 19690720));
}

fn part1(program: &Computer) -> i32 {
    let mut copy = program.clone();
    copy.memory[1] = 12;
    copy.memory[2] = 2;
    copy.run_program();

    copy.memory[0]
}

fn part2(program: &Computer, target: i32) -> i32 {
    for noun in 0..100 {
        for verb in 0..100 {
            let mut copy = program.clone();
            copy.memory[1] = noun;
            copy.memory[2] = verb;
            copy.run_program();

            if copy.memory[0] == target {
                return 100 * noun + verb;
            }
        }
    }
    panic!("You can't park here mate!");
}
