use intcode::Computer;
use std::{self, env, fs};

fn main() {
    let mut args = env::args().skip(1);

    let program = fs::read_to_string(args.next().expect("Please provide your input file"))
        .expect("Input file had invalid characters");
    let mut pc = Computer::new(&program);
    pc.run_program();
}
