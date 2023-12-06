import numpy as np


class Range:
    def __init__(self, dst, src, length) -> None:
        self.dest = dst
        self.src = src
        self.length = length
        self.end = length - 1


class Almanac:
    def __init__(self) -> None:
        self.seeds = []
        self.ranges = []


def parse_lines(lines):
    result = Almanac()

    result.seeds = [int(x)
                    for x in lines[0].split(':')[1].strip().split(' ')]

    map = []
    for l in lines:
        if not l.strip():
            result.ranges.append(map)
            map = []
            continue

        if 'map' in l or "seeds" in l:
            continue

        map.append([int(x) for x in l.split(' ')])

    result.ranges.append(map)
    result.ranges.pop(0)

    return result


def part1(data):
    seeds = [s for s in data.seeds]

    for map in data.ranges:
        for i in range(0, len(seeds)):
            s = seeds[i]
            for r in map:
                if s >= r[1] and s < r[1] + r[2]:
                    seeds[i] += r[0] - r[1]

    return min(seeds)


def part2(data):
    seeds = []
    for i in range(0, len(data.seeds) - 1, 2):
        s = data.seeds[i]
        l = data.seeds[i+1]
        seeds += [[s, l]]

    for map in data.ranges:
        nr = []
        while (len(seeds)):
            [s, l] = seeds.pop()

            match = False
            for r in map:
                x1 = s
                x2 = r[1]
                y1 = s + l - 1
                y2 = r[1] + r[2] - 1

                if (x1 < x2 and y1 < x2) or (x2 < x1 and y2 < x1):
                    continue

                if x1 >= x2 and y1 <= y2:
                    nr.append([s + r[0] - r[1], l])
                    match = True

                elif x1 >= x2 and y1 > y2:
                    nr.append([s + r[0] - r[1], y2 - x1 + 1])
                    seeds.append([y2 + 1, y1 - y2])
                    match = True

                elif x1 < x2 and y1 <= y2:
                    nr.append([r[0], y1 - x2 + 1])
                    seeds.append([x1, x2 - x1])
                    match = True

                elif x1 < x2 and y1 > y2:
                    nr.append([r[0], r[2]])
                    seeds.append([x1, x2 - x1])
                    seeds.append([y2 + 1, y1 - y2])
                    match = True

                else:
                    print("missed a case")

            if not match:
                nr.append([s, l])

        seeds += nr
        
    return min(seeds, key=lambda x: x[0])[0]


if __name__ == "__main__":
    with open("input.txt", "r") as file:
        lines = [line.strip() for line in file.readlines()]
        data = parse_lines(lines)
        print(part1(data))
        print(part2(data))
