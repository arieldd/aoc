use std::{env::args, fs::read};

use intcode::{Computer, IntCodeIO};

fn main() {
    let input = args()
        .skip(1)
        .next()
        .expect("Please provide your input file");

    let program = read(input).expect("Could not read input file");
    let mut pc = Computer::new(&String::from_utf8(program).expect("Invalid chars"));

    let args = vec![1, 2];
    let mut io = IntCodeIO::new(&args);

    pc.clone().run_program(&mut io);
    println!("Part 1: {}", io.output().last().unwrap());

    pc.run_program(&mut io);
    println!("Part 2: {}", io.output().last().unwrap());
}
