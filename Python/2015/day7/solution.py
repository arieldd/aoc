from collections import defaultdict
from typing import Optional

with open("input.txt") as f:
    s = f.read().strip()


class Gate:
    def __init__(self) -> None:
        self.value: int = -1
        self.op, self.lhs, self.rhs = "", "", ""


og = defaultdict(Gate)
instr = s.split("\n")

for line in instr:
    parts = line.split(" -> ")
    wire = parts[-1]
    entry = Gate()
    inputs = parts[0].split()
    if len(inputs) == 1:
        try:
            entry.value = int(inputs[0])
        except:
            entry.rhs = inputs[0]
    elif len(inputs) == 2:
        entry.op, entry.rhs = inputs
    else:
        entry.lhs, entry.op, entry.rhs = inputs
    og[wire] = entry


def run_gate(wire, gates) -> int:
    if str.isdecimal(wire):
        return int(wire)

    assert wire in gates
    gate = gates[wire]
    if gate.value != -1:
        return gate.value

    rhs = run_gate(gate.rhs, gates)
    if gate.op == "":
        gates[wire].value = rhs
    elif gate.op == "NOT":
        gates[wire].value = ~rhs
    else:
        lhs = run_gate(gate.lhs, gates)
        if gate.op == "AND":
            gates[wire].value = lhs & rhs
        if gate.op == "OR":
            gates[wire].value = lhs | rhs
        if gate.op == "RSHIFT":
            gates[wire].value = lhs >> rhs
        if gate.op == "LSHIFT":
            gates[wire].value = lhs << rhs

    return gates[wire].value


def solve(gates) -> int:
    for wire in gates:
        gates[wire].value = run_gate(wire, gates)
    return gates["a"].value


import copy

part1 = solve(copy.deepcopy(og))
print(part1)

og["b"].value = part1
part2 = solve(og)
print(part2)
