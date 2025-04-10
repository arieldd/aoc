use std::{env::args, fs::read};

use intcode::Computer;

fn main() {
    let input = args()
        .skip(1)
        .next()
        .expect("Please provide your input file");

    let program = read(input).expect("Could not read input file");
    Computer::new(&String::from_utf8(program).expect("Invalid chars")).run_program();
}
