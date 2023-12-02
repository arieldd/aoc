def full_overlaps(data):
    sum = 0
    for entry  in data:
        pair1Text, pair2Text = entry
        pair1 = [int(p) for p in pair1Text.split('-')]
        pair2 = [int(p) for p in pair2Text.split('-')]
        
        v1 =  pair1[0] - pair2[0]
        v2 =  pair1[1] - pair2[1]
        
        if v1 * v2 <= 0:
            sum += 1 

    return sum

def partial_overlaps(data):
    sum = 0
    for entry  in data:
        pair1Text, pair2Text = entry
        pair1 = [int(p) for p in pair1Text.split('-')]
        pair2 = [int(p) for p in pair2Text.split('-')]
        
        r1 = pair2[1] - pair1[0]
        r2 = pair1[1] - pair2[0]

        if r1 * r2 >=0:
            sum += 1

    return sum

with open("input.txt", "r") as file:
    data = [line.strip().split(',') for line in file.readlines()]

    print(partial_overlaps(data))