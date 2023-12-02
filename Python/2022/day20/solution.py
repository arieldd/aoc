class Number:
    def __init__(self, val, pos) -> None:
        self.val = val
        self.pos = pos
        self.start = pos

def mix_native(code, times):
    result = list(code)

    size = len(result)
    for _ in range(times):
        for n in code:
            val = n.val
            if val == 0:
                continue

            ix = n.pos
            sign = 1 if val > 0 else -1

            shift = abs(val) % (size - 1)
            while shift:
                next = ix + sign
                if next < 0:
                    next = size - 1
                elif next >= size:
                    next = 0

                result[ix] = result[next]
                result[next] = n

                n.pos = next
                result[ix].pos = ix

                ix = next
                shift -= 1

    return format_code(result)

def format_code(mix):
    result = [0] * len(mix)
    for n in mix:
        result[n.pos] = n.val

    return result

def decrypt(mix):
    size =len(mix)
    zero = mix.index(0)

    indices = []
    for k in [1000, 2000, 3000]:
        indices.append((zero + k) % size)

    encryption = [mix[i] for i in indices]
    print(encryption)
    return sum(encryption)   


with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    code = []
    for ix, val in enumerate(data):
        code.append(Number(int(val), ix))

    #Part 1
    # mix = mix_native(code, 1)

    # print(len(mix))
    # print(decrypt(mix))

    #Part 2
    enc_key = 811589153

    for n in code:
        n.val *= enc_key 
    mix = mix_native(code, 10)

    print(len(mix))
    print(decrypt(mix))

