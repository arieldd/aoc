import numpy as np
import sys
import matplotlib.pyplot as plt
from sympy import *


class Hailstone:
    def __init__(self, pos, velocity):
        self.pos = pos
        self.velocity = velocity

    def __str__(self) -> str:
        return str(self.pos) + " -> " + str(self.velocity)


def intersect_2d(h1, h2):

    p1 = h1.pos
    p2 = h2.pos
    v1 = h1.velocity
    v2 = h2.velocity

    A = np.array([v1[:-1], v2[:-1]]).T
    b = np.array([p2[0] - p1[0], p2[1] - p1[1]])

    try:
        (t, s) = np.linalg.solve(A, b)
    except:
        return (np.nan, np.nan, [])

    return (t, s, p1 + np.multiply(v1, t))


def part1(hails):
    n = len(hails)

    count = 0
    for i in range(n-1):
        h1 = hails[i]
        for j in range(i+1, n):
            h2 = hails[j]
            (t, s, p) = intersect_2d(h1, h2)
            if len(p) == 0:
                pass
                # print("No intersect between", hails[i], "and", hails[j], )
            else:
                [x, y, _] = p

                x1 = h1.pos[0] + h1.velocity[0]
                x2 = h2.pos[0] + h2.velocity[0]

                if abs(x - x1) > abs(x - h1.pos[0]) or abs(x - x2) > abs(x - h2.pos[0]):
                    continue  # in the past

                # print(i, j, "intercept at", t, s)

                if lower <= x <= upper and lower <= y <= upper:
                    count += 1

    return count


def point_at_time(h, time):
    return Hailstone([p + time * v for (p, v) in zip(h.pos, h.velocity)], h.velocity)


def part2(hails):

    px, py, pz, v1x, v2x, v3x, t1, t2, t3 = symbols(
        "px py pz v1x v2x v3x t1 t2 t3")
    eqs = [
        px + v1x * t1 - hails[0].pos[0] - hails[0].velocity[0] * t1,
        py + v2x * t1 - hails[0].pos[1] - hails[0].velocity[1] * t1,
        pz + v3x * t1 - hails[0].pos[2] - hails[0].velocity[2] * t1,
        px + v1x * t2 - hails[1].pos[0] - hails[1].velocity[0] * t2,
        py + v2x * t2 - hails[1].pos[1] - hails[1].velocity[1] * t2,
        pz + v3x * t2 - hails[1].pos[2] - hails[1].velocity[2] * t2,
        px + v1x * t3 - hails[2].pos[0] - hails[2].velocity[0] * t3,
        py + v2x * t3 - hails[2].pos[1] - hails[2].velocity[1] * t3,
        pz + v3x * t3 - hails[2].pos[2] - hails[2].velocity[2] * t3,
    ]

    solution, = nonlinsolve(eqs, (px, py, pz, v1x, v2x, v3x, t1, t2, t3))
    print(solution)
    return sum(solution[:3])


with open(sys.argv[1], "r") as file:

    lower = int(sys.argv[2])
    upper = int(sys.argv[3])
    data = [line.strip() for line in file.readlines()]

    hails = []
    for line in data:
        sides = line.split('@')
        pos = [int(v) for v in sides[0].split(',')]
        velocity = [int(v) for v in sides[1].split(',')]
        hail = Hailstone(pos, velocity)
        hails.append(hail)

    r1 = part1(hails)
    print(r1)

    r2 = part2(hails)
    print(r2)
