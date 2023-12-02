import ast
from functools import cmp_to_key

class Pair:
    def __init__(self, left, right, asIs = False) -> None:
        self.left = left if asIs else ast.literal_eval(left)
        self.right = right if asIs else ast.literal_eval(right)


    def order(self) -> int:
        size_l, size_r = len(self.left), len(self.right)
        size = max(size_l, size_r)

        for i in range(size):
            if i >= size_l:
                return -1
            if i >= size_r:
                return 1

            l = self.left[i]
            r = self.right[i]

            ll =  type(l) == list
            lr = type(r) == list

            if ll and lr:
                inner = Pair(l,r, True).order()
                if abs(inner) == 1:
                    return inner
                continue

            if ll and not lr:
                inner = Pair(l,[r], True).order()
                if abs(inner) == 1:
                    return inner
                continue
            
            if not ll and lr:
                inner = Pair([l],r, True).order()
                if abs(inner) == 1:
                    return inner
                continue

            l = int(l)
            r = int(r)
            if l < r: return -1
            if l > r : return 1

        return 0


    def __str__(self) -> str:
        result = ''
        result += str(self.left) + ' : '
        result += str(self.right)

        return result


def compare(p1, p2) -> int:
    return Pair(p1, p2).order()

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    pair_list=[None]
    packets = []
    for i in range(0, len(data), 3):
        pair_list.append(Pair(data[i], data[i+1]))
        packets.append(data[i])
        packets.append(data[i+1])

    #Part 1
    indices = 0
    for i in range(1, len(pair_list)):
        p = pair_list[i]
        #print(p)
        if p.order() < 0:
            indices += i

    print(indices)

    #Part 2
    divider_packets = ['[[2]]', '[[6]]']
    packets.extend(divider_packets)

    packets.sort(key=cmp_to_key(compare))
    part2 = 1
    for i, p in enumerate(packets):
        if p in divider_packets:
            part2 *= i + 1

    print(part2)