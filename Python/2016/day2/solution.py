with open("input.txt") as f:
    s = f.read().strip()

lines = s.split("\n")
n = 3


num_keypad = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
hex_keypad = [
    [0, 0, 1, 0, 0],
    [0, 2, 3, 4, 0],
    [5, 6, 7, 8, 9],
    [0, "A", "B", "C", 0],
    [0, 0, "D", 0, 0],
]


def get_code(keypad):
    n = len(keypad)
    code = []
    cx, cy = 1, 1
    for line in lines:
        for c in line:
            nx, ny = cx, cy
            if c == "R":
                nx = cx + 1
            elif c == "D":
                ny = cy + 1
            elif c == "L":
                nx = cx - 1
            elif c == "U":
                ny = cy - 1
            if nx in range(n) and ny in range(n) and keypad[ny][nx] != 0:
                cx, cy = nx, ny
        code.append(keypad[cy][cx])
    return code


print("".join([str(x) for x in get_code(num_keypad)]))
print("".join([str(x) for x in get_code(hex_keypad)]))
