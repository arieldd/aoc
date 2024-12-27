with open("input.txt") as f:
    s = f.read().strip()

part1 = 0
part2 = 0
for line in s.split("\n"):
    encoded, code, mem = 2, 0, 0
    i = 0
    while i < len(line):
        c = line[i]
        code += 1
        encoded += 1
        if c != '"':
            if c == "\\":
                if line[i + 1] in {'"', "\\"}:
                    code += 1
                    encoded += 3
                    i += 1
                elif line[i + 1] == "x":
                    code += 3
                    encoded += 4
                    i += 3
            mem += 1
        else:
            encoded += 1
        i += 1

    part1 += code - mem
    part2 += encoded - code


print(part1)
print(part2)
