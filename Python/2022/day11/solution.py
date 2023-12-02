from collections import deque

class Monkey:
    def __init__(self) -> None:
        self.number = 0
        self.items = []
        self.operation = []
        self.test = []
        self.inspected = 0


    def __str__(self) -> str:
        return 'Monkey:' + str(self.number) + ', ' + str(self.items) + ' op: ' + str(self.operation) + ', test: ' + str(self.test) + ', inspected:' + str(self.inspected) 

def create_monkey(description) -> Monkey:
    result = Monkey()

    #Monkey x:
    result.number = int(description[0].split()[1][0])
    result.items = [int(w) for w in description[1].split(':')[1].split(',')]
    
    result.operation = description[2].split('=')[1].split()[1:]
    
    result.test.append(int(description[3].split()[-1]))
    result.test.append(int(description[4].split()[-1]))
    result.test.append(int(description[5].split()[-1]))

    return result

def monkey_bussiness(data, rounds):
    monkey_list = []
    for i in range(6, len(data) + 1, 7):
        monkey_list.append(create_monkey(data[i-6:i]))

    prime = 1
    for monke in monkey_list:
        prime *= monke.test[0]

    for i in range(rounds):
        for monkey in monkey_list:
            op = monkey.operation
            for item in monkey.items:
                worry = item
                operand = worry if op[1] == 'old' else int(op[1]) 
                if op[0] == '+':
                    worry += operand
                else:
                    worry *= operand
                
                #worry //= 3

                receiver = monkey.test[1] if worry % monkey.test[0] == 0 else monkey.test[2]
                monkey_list[receiver].items.append(worry % prime)

                monkey.inspected += 1

            monkey.items.clear()

    return monkey_list


with open("input.txt", "r") as file:

    data = [line.strip() for line in file.readlines()]

    rounds = 10000
    list = monkey_bussiness(data, rounds)

    list.sort(key=lambda x : x.inspected, reverse=True)

    for monke in list:
        print(monke)

    print(list[0].inspected * list[1].inspected)