def excute_commands(data) -> list:
    regx = 1

    signals = [0]

    for command in data:

        signals.append(regx)
        
        if len(command) < 2:
            continue

        value = int(command[1])
        
        signals.append(regx)
        
        regx += value

    return signals

def render_image(signals):

    for i in range(6):
        for j in range(1, 41):
            cycle = j + 40 * i
            pos =  signals[cycle]
            sprite = [pos -1, pos, pos +1]
            #print(sprite, cycle)
            if j-1 in sprite:
                print('#', end ='')
            else:
                print('.', end='')
        print('')


with open("input.txt", "r") as file:

    data = [line.strip().split() for line in file.readlines()]
    
    signals = excute_commands(data)

    #Part 1
    sum = 0 
    for cycle in range(20, 221, 40):
        sum += cycle * signals[cycle]
    print(sum)

    #Part 2
    render_image(signals)