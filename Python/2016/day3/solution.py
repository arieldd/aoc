with open("input.txt") as f:
    s = f.read().strip()

lines = s.split("\n")

part1 = 0
part2 = 0


def vertical_triangles(trio):
    result = 0
    for triangle2 in trio:
        if len(triangle2) == 0:
            continue
        t1, t2, t3 = triangle2
        if t1 >= t2 + t3 or t2 >= t1 + t3 or t3 >= t2 + t1:
            continue
        result += 1
    return result


trio = [[], [], []]
for i, line in enumerate(lines):
    triangle1 = [int(value) for value in line.split()]
    l1, l2, l3 = triangle1

    if i % 3 == 0:
        part2 += vertical_triangles(trio)
        trio = [[], [], []]

    for k in range(3):
        trio[k].append(triangle1[k])

    if l1 >= l2 + l3 or l2 >= l1 + l3 or l3 >= l2 + l1:
        continue
    part1 += 1

part2 += vertical_triangles(trio)

print(part1)
print(part2)
