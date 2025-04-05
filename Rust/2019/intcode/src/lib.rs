#[derive(Debug, Clone)]
pub struct Program {
    pub memory: Vec<i32>,
    ip: usize,
}

impl Program {
    pub fn new(code: &str) -> Self {
        let instr = code
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

    pub fn run(&mut self) {
        loop {
            assert!(
                self.ip < self.memory.len(),
                "Instruction pointer was outside of bounds"
            );
            let opcode = Opcode::from(self.memory[self.ip]);
            match opcode {
                Opcode::Add | Opcode::Mult => {
                    let op1 = self.fetch(self.memory[self.ip + 1]);
                    let op2 = self.fetch(self.memory[self.ip + 2]);
                    let dst = self.memory[self.ip + 3];

                    let result = if opcode == Opcode::Add {
                        op1 + op2
                    } else {
                        op1 * op2
                    };
                    self.put(result, dst);
                    self.ip += 4;
                }
                Opcode::Exit => break,
                Opcode::Unknown => {
                    panic!("Oops something went wrong!")
                }
            }
        }
    }

    fn fetch(self: &Program, address: i32) -> i32 {
        let pos: usize = address.try_into().expect("Bad address");
        self.memory[pos]
    }

    fn put(self: &mut Program, value: i32, address: i32) {
        let pos: usize = address.try_into().expect("Bad address");

        if pos >= self.memory.len() {
            self.memory.resize(pos + 1, 0);
        }
        self.memory[pos] = value;
    }
}

#[derive(Debug)]
struct Instruction {
    op: Opcode,
    parameters: Vec<i32>,
    jump: i32,
}

#[derive(Debug, PartialEq)]
enum Opcode {
    Add = 1,
    Mult = 2,
    Exit = 99,
    Unknown,
}

impl Opcode {
    fn from(value: i32) -> Self {
        match value {
            1 => Self::Add,
            2 => Self::Mult,
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
        let mut p1 = Program::new("1, 0,0,0 ,99 ");
        let mut p2 = Program::new("2, 3, 0, 3, 99");
        let mut p3 = Program::new("2, 4, 4, 5, 99");
        let mut p4 = Program::new("1, 1, 1, 4, 99, 5, 6, 0, 99");

        [&mut p1, &mut p2, &mut p3, &mut p4]
            .iter_mut()
            .for_each(|p| p.run());

        assert_eq!(p1.memory[0], 2, "was {:?}", p1.memory);
        assert_eq!(p2.memory[3], 6);
        assert_eq!(p3.memory[5], 9801);
        assert_eq!(p4.memory[0], 30);
    }
}
