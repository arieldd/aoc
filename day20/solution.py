
def mix(code):
    print_mix(code)
    size = len(code)
    for val, ix in code.items():
        if val == 0:
            continue
        shift = (ix + val)
        if shift < 0:
            shift += size -1

        new_pos = shift % size

        if new_pos == 0:
            print(val, ix)
            new_pos = size -1
        if new_pos == size-1:
            new_pos = 0

        for other, index in code.items():
            if other == val:
                continue
            
            s2 = (index - 1)
            if s2 < 0:
                s2 += size -1
            
            if val > 0:
                if new_pos > ix and ix < index <= new_pos:
                    code[other] = s2 % size
                elif new_pos < ix and new_pos <= index <ix:
                    code[other] = (index + 1) % size 
            elif val < 0:
                if new_pos < ix and ix > index >= new_pos:
                    code[other] = (index + 1) % size
                elif new_pos > ix and ix < index <= new_pos:
                    code[other] = s2 % size
        
        code[val] = new_pos
        print_mix(code)

    return code
        
def mix_native(code):
    result = list(code)
    size = len(result)
    for val in code:
        if val == 0:
            continue
        ix = index = result.index(val)
        
        if val < 0:
            ix = size - 1 - ix
 
        shift = abs(val) % (size - 1)
        new_pos = (ix + shift) % (size - 1)

        if val < 0:
            new_pos = size - 1 - new_pos

        result.pop(index)
        result.insert(new_pos, val)

        #print(result)
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

def print_mix(code):
    numbers = [0] * len(code)

    for v, ix in code.items():
        numbers[ix] = v

    print(numbers)


with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    code = []
    for val in data:
        code.append(int(val))

    mix = mix_native(code)

    print(len(mix))
    print(decrypt(mix))