def stack_top(stacks, order):
    for move, frm, to in order:
        frm -= 1
        to  -= 1

        chunk = stacks[frm][:move]
        #chunk.reverse() uncomment for Part 1
        
        stacks[to] = chunk + stacks[to]
        stacks[frm] = stacks[frm][move:]

    return [top[0] for top in stacks]

with open("input.txt", "r") as file:

    stacks = []

    data = [line for line in file.readlines()]

    sep = data.index('\n')

    raw_stacks = data[:sep-1]
    raw_order = [line.strip() for line in data[sep+1:]]

    stacks = []
    for line in raw_stacks:
        i = 0

        for j in range(0, len(line), 4):
            char = line[j+1]
            if i >= len(stacks):
                stacks.append([])

            if  char != ' ':
                stacks[i].append(char)
        
            i += 1

    order = []
    for line in raw_order:
        s1 = line.split('to')
        to = int(s1[1].strip())
        
        s2 = s1[0].strip().split('from')
        frm = int(s2[1].strip())

        s3 = s2[0].strip().split('move')
        move = int(s3[1].strip())

        order.append([move, frm, to])

   
    print(''.join(stack_top(stacks, order)))