use std::io::stdin;

#[derive(Debug, Clone)]
pub struct Computer {
    pub memory: Vec<isize>,
    ip: usize,
    relative: isize,
}

impl Computer {
    pub fn new(program: &str) -> Self {
        let instr = program
            .split(',')
            .map(|value| {
                value
                    .trim()
                    .parse::<isize>()
                    .expect("Should be a valid integer")
            })
            .collect();

        Self {
            memory: instr,
            ip: 0,
            relative: 0,
        }
    }

    pub fn run_program(&mut self) {
        loop {
            assert!(
                self.ip < self.memory.len(),
                "Instruction pointer was outside of bounds"
            );
            let instr = self.decode(self.ip);
            match self.execute(&instr) {
                Ok(true) => { /* All good, continue */ }
                Ok(false) =>
                /*Exit code, stop*/
                {
                    break
                }
                Err(msg) => panic!("{}", msg),
            }
        }
    }

    fn fetch(self: &Computer, address: isize) -> isize {
        let pos: usize = address
            .try_into()
            .expect(&format!("Bad address {}", address));
        if pos < self.memory.len() {
            self.memory[pos]
        } else {
            0
        }
    }

    fn put(self: &mut Computer, value: isize, address: isize) {
        let pos: usize = address.try_into().expect("Bad address");

        if pos >= self.memory.len() {
            self.memory.resize(pos + 1, 0);
        }
        self.memory[pos] = value;
    }

    fn decode(self: &Computer, address: usize) -> Instruction {
        let mut value = self.fetch(address as isize);
        let op = Opcode::from(value % 100);
        value /= 100;

        let mut modes = vec![];
        while value > 0 {
            modes.push((value % 10) as u8);
            value /= 10;
        }

        let mut params = vec![];
        let mut load_parameters = |is_mem: &[bool]| {
            let count = is_mem.len();
            for i in 0..count {
                let mode = if i < modes.len() { modes[i] } else { 0 };
                let value = self.fetch((address + i + 1).try_into().unwrap());
                params.push(match mode {
                    0 => {
                        if is_mem[i] {
                            value
                        } else {
                            self.fetch(value)
                        }
                    }
                    1 => value,
                    2 => {
                        if is_mem[i] {
                            self.relative + value
                        } else {
                            self.fetch(self.relative + value)
                        }
                    }
                    _ => panic!("Can't park there mate"),
                });
            }
        };

        match op {
            Opcode::Add | Opcode::Mult | Opcode::LessThan | Opcode::Equals => {
                load_parameters(&[false, false, true]);
            }
            Opcode::Input => {
                load_parameters(&[true]);
            }
            Opcode::Output => {
                load_parameters(&[false]);
            }
            Opcode::JmpTrue | Opcode::JmpFalse => {
                load_parameters(&[false, false]);
            }
            Opcode::RelativeOffset => {
                load_parameters(&[false]);
            }
            _ => {}
        };

        Instruction { op, params }
    }

    fn execute(self: &mut Computer, instr: &Instruction) -> Result<bool, &str> {
        let mut jmp = self.ip + instr.params.len() + 1;
        match instr.op {
            Opcode::Add | Opcode::Mult => {
                let op1 = instr.params[0];
                let op2 = instr.params[1];
                let dst = instr.params[2];

                let result = if instr.op == Opcode::Add {
                    op1 + op2
                } else {
                    op1 * op2
                };
                self.put(result, dst);
            }
            Opcode::Input => {
                println!("Enter a value: ");
                let mut buff = String::new();
                match stdin().read_line(&mut buff) {
                    Ok(_) => {
                        let value = buff.trim().parse::<isize>().expect("Need a number brotha!");
                        self.put(value, instr.params[0]);
                    }
                    Err(_) => panic!("Couldn't read from input"),
                }
            }
            Opcode::Output => {
                println!("{}", instr.params[0]);
            }
            Opcode::JmpTrue => {
                if instr.params[0] != 0 {
                    jmp = instr.params[1] as usize;
                }
            }
            Opcode::JmpFalse => {
                if instr.params[0] == 0 {
                    jmp = instr.params[1] as usize;
                }
            }
            Opcode::LessThan => {
                self.put(
                    if instr.params[0] < instr.params[1] {
                        1
                    } else {
                        0
                    },
                    instr.params[2],
                );
            }
            Opcode::Equals => {
                self.put(
                    if instr.params[0] == instr.params[1] {
                        1
                    } else {
                        0
                    },
                    instr.params[2],
                );
            }
            Opcode::RelativeOffset => {
                self.relative += instr.params[0];
            }
            Opcode::Exit => return Ok(false),
            Opcode::Unknown => return Err("hmmmm"),
        }

        self.ip = jmp;

        return Ok(true);
    }
}

#[derive(Debug)]
struct Instruction {
    op: Opcode,
    params: Vec<isize>,
}

#[derive(Debug, PartialEq)]
enum Opcode {
    Add,
    Mult,
    Input,
    Output,
    JmpTrue,
    JmpFalse,
    LessThan,
    Equals,
    RelativeOffset,
    Exit,
    Unknown,
}

impl Opcode {
    fn from(value: isize) -> Self {
        match value {
            1 => Self::Add,
            2 => Self::Mult,
            3 => Self::Input,
            4 => Self::Output,
            5 => Self::JmpTrue,
            6 => Self::JmpFalse,
            7 => Self::LessThan,
            8 => Self::Equals,
            9 => Self::RelativeOffset,
            99 => Self::Exit,
            _ => Self::Unknown,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn day2() {
        let mut p1 = Computer::new("1, 0,0,0 ,99 ");
        let mut p2 = Computer::new("2, 3, 0, 3, 99");
        let mut p3 = Computer::new("2, 4, 4, 5, 99");
        let mut p4 = Computer::new("1, 1, 1, 4, 99, 5, 6, 0, 99");

        [&mut p1, &mut p2, &mut p3, &mut p4]
            .iter_mut()
            .for_each(|p| p.run_program());

        assert_eq!(p1.memory[0], 2, "was {:?}", p1.memory);
        assert_eq!(p2.memory[3], 6);
        assert_eq!(p3.memory[5], 9801);
        assert_eq!(p4.memory[0], 30);
    }

    #[test]
    fn day9() {
        let mut p1 = Computer::new("109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99");
        p1.run_program();

        let mut p2 = Computer::new("1102,34915192,34915192,7,4,7,99,0");
        p2.run_program();
        assert!(true);
    }
}
