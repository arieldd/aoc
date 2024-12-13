from z3 import *

file = open("input.txt", "r")
lines = file.read().split("\n")


def min_tokens(claw):
    offset = 10000000000000

    b = [(claw.prize[0] + offset), (claw.prize[1] + offset)]

    x, y = Ints("x y")
    s = Solver()
    eq1 = claw.bA[0] * x + claw.bB[0] * y == b[0]
    eq2 = claw.bA[1] * x + claw.bB[1] * y == b[1]
    s.add(eq1, eq2, x > 0, y > 0)
    r = s.check()
    if r == unsat or r == unknown:
        return None

    return [s.model()[x].as_long(), s.model()[y].as_long()]


class Machine:
    def __init__(self, bA, bB, prize) -> None:
        self.bA = bA
        self.bB = bB
        self.prize = prize


machines = []
bA = []
bB = []
prize = []
for i, line in enumerate(lines):
    if i % 4 == 3:
        machines.append(Machine(bA, bB, prize))
    elif i % 4 == 2:
        # prize
        coords = line.split(": ")[1].split(", ")
        prize = [int(coords[0].split("=")[1]), int(coords[1].split("=")[1])]
    else:
        coords = line.split(": ")[1].split(", ")
        point = [int(coords[0].split("+")[1]), int(coords[1].split("+")[1])]
        if "A" in line:
            bA = point
        elif "B:" in line:
            bB = point
part2 = 0
for i, claw in enumerate(machines):
    res = min_tokens(claw)
    if res is not None:
        part2 += 3 * res[0] + res[1]
print(part2)
