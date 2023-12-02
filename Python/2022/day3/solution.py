def char_value(c):
    value = ord(c)

    if value < 97:
        value -= 38
    else:
        value -= 96

    return value 

def sum_priority(data):
    sum = 0
    for sack in data:
        size = len(sack)
        half = size//2

        comp1 = {}
        for i in range(0, half):
            c = sack[i]
            comp1[c] = char_value(c)

        for i in range(half, size):
            c = sack[i]
            if c in comp1:
                sum += comp1[c]
                break
    
    return sum

def elf_groups(data):
    sum = 0
    group_count = 0

    items = {}
    for sack in data:
        size = len(sack)
        group_count += 1

        for i in range(0, size):
            c = sack[i]
            if c not in items: 
                items[c] = [group_count]
                continue
            if group_count not in items[c]:
                items[c].append(group_count)
                continue

        if group_count >= 3:
            badge = list(items.keys())[list(items.values()).index([1,2,3])]
            sum += char_value(badge)
            items = {}
            group_count = 0

    return sum

with open("input.txt", "r") as file:
    data = [line.strip() for line in file.readlines()]

    print(elf_groups(data))