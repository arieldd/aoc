with open("input.txt") as f:
    s = f.read().strip()

lines = s.split("\n")


def get_next(term):
    i = 0
    next = []
    while i < len(term):
        count = 1
        val = term[i]
        i += 1
        while i < len(term) and term[i] == val:
            count += 1
            i += 1

        next.append(str(count) + val)
    return "".join(next)


def look_and_say(times, seed):
    seq = seed
    for _ in range(times):
        seq = get_next(seq)
    return seq


part1 = look_and_say(40, lines[0])
part2 = look_and_say(10, part1)
print(len(part1))
print(len(part2))
