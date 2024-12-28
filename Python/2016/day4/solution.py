from functools import cmp_to_key
from typing import Counter

with open("input.txt") as f:
    s = f.read().strip()

lines = s.split("\n")


def cmp(x1, x2):
    if x1[1] < x2[1]:
        return 1
    elif x1[1] > x2[1]:
        return -1
    elif x1[0] < x2[0]:
        return -1
    elif x1[0] > x2[0]:
        return 1

    return 0


ans = 0
valid = []
for line in lines:
    if len(line) == 0:
        continue
    dashes = line.split("-")
    freq = Counter()
    for word in dashes[:-1]:
        freq.update(word)
    real = sorted(freq.most_common(), key=cmp_to_key(cmp))
    sector = dashes[-1]
    bracket = sector.index("[")
    checksum = sector[bracket + 1 : -1]

    real = "".join([l for l, _ in real[:5]])
    if real == checksum:
        id = int(sector[:bracket])
        ans += id
        valid.append((dashes[:-1], id))

print(ans)

for encrypted, id in valid:
    decrypted = []
    for word in encrypted:
        dword = ""
        for c in word:
            dword += chr((ord(c) - ord("a") + id) % 26 + ord("a"))
        decrypted.append(dword)

    if "northpole object storage" == " ".join(decrypted):
        print(id)
        break
