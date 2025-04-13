use intcode::{Computer, IntCodeIO};
use std::{self, env, fs};

fn main() {
    let mut args = env::args().skip(1);

    let program = fs::read_to_string(args.next().expect("Please provide your input file"))
        .expect("Input file had invalid characters");
    let mut pc = Computer::new(&program);

    let args = vec![1, 5];
    let mut io = IntCodeIO::new(&args);

    pc.clone().run_program(&mut io);
    println!("Part 1: {:?}", io.output().last().unwrap());
    pc.run_program(&mut io);
    println!("Part 2: {:?}", io.output().last().unwrap());
}
