use std::io::stdin;

#[derive(Debug, Clone)]
pub struct Computer {
    pub memory: Vec<i32>,
    ip: usize,
}

impl Computer {
    pub fn new(program: &str) -> Self {
        let instr = program
            .split(',')
            .map(|value| {
                value
                    .trim()
                    .parse::<i32>()
                    .expect("Should be a valid integer")
            })
            .collect();

        Self {
            memory: instr,
            ip: 0,
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

    fn fetch(self: &Computer, address: i32) -> i32 {
        let pos: usize = address.try_into().expect("Bad address");
        self.memory[pos]
    }

    fn put(self: &mut Computer, value: i32, address: i32) {
        let pos: usize = address.try_into().expect("Bad address");

        if pos >= self.memory.len() {
            self.memory.resize(pos + 1, 0);
        }
        self.memory[pos] = value;
    }

    fn decode(self: &Computer, address: usize) -> Instruction {
        let mut value = self.fetch(address as i32);
        let op = Opcode::from(value % 100);
        value /= 100;

        let mut modes = vec![];
        while value > 0 {
            modes.push((value % 10) as u8);
            value /= 10;
        }

        let mut params = vec![];
        match op {
            Opcode::Add | Opcode::Mult | Opcode::LessThan | Opcode::Equals => {
                for i in 0..2 {
                    let mode = if i < modes.len() { modes[i] } else { 0 };
                    let value = self.fetch((address + i + 1).try_into().unwrap());
                    params.push(match mode {
                        0 => self.fetch(value),
                        1 => value,
                        _ => panic!("Can't park there mate"),
                    });
                }
                params.push(self.fetch((address + 3).try_into().unwrap()));
            }
            Opcode::Input | Opcode::Output => {
                params.push(self.fetch((address + 1).try_into().unwrap()));
            }
            Opcode::JmpTrue | Opcode::JmpFalse => {
                for i in 0..2 {
                    let mode = if i < modes.len() { modes[i] } else { 0 };
                    let value = self.fetch((address + i + 1).try_into().unwrap());
                    params.push(match mode {
                        0 => self.fetch(value),
                        1 => value,
                        _ => panic!("Can't park there mate"),
                    });
                }
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
                        let value = buff.trim().parse::<i32>().expect("Need a number brotha!");
                        self.put(value, instr.params[0]);
                    }
                    Err(_) => panic!("Couldn't read from input"),
                }
            }
            Opcode::Output => {
                println!("{}", self.fetch(instr.params[0]));
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
    params: Vec<i32>,
}

#[derive(Debug, PartialEq)]
enum Opcode {
    Add = 1,
    Mult = 2,
    Input = 3,
    Output = 4,
    JmpTrue = 5,
    JmpFalse = 6,
    LessThan = 7,
    Equals = 8,
    Exit = 99,
    Unknown,
}

impl Opcode {
    fn from(value: i32) -> Self {
        match value {
            1 => Self::Add,
            2 => Self::Mult,
            3 => Self::Input,
            4 => Self::Output,
            5 => Self::JmpTrue,
            6 => Self::JmpFalse,
            7 => Self::LessThan,
            8 => Self::Equals,
            99 => Self::Exit,
            _ => Self::Unknown,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
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
}
