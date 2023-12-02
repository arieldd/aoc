class Monkey:
    def __init__(self, name) -> None:
        self.op = ''
        self.value = None
        self.name = name
        self.left = None
        self.right = None
        self.humn = -1

    def __str__(self) -> str:
        return f'{self.name} : {self.left} {self.op} {self.right} -> {self.value}'

    def get_value(self, monkeys):
        if self.value != None:
            return self.value

        left = monkeys[self.left]
        right = monkeys[self.right]

        o1 = left.get_value(monkeys)
        o2 = right.get_value(monkeys)

        if self.op == '+': return o1 + o2
        elif self.op == '-': return o1 - o2
        elif self.op == '*': return o1 * o2

        return o1 // o2

    def get_diff(self, monkeys):
        if self.name == 'humn':
            return 0

        if self.value != None:
            return self.value

        left = monkeys[self.left]
        right = monkeys[self.right]

        o1 = left.get_diff(monkeys)
        o2 = right.get_diff(monkeys)

        if self.name != 'root':

            if self.op == '+': return o1 + o2
            elif self.op == '-': return o1 - o2
            elif self.op == '*': return o1 * o2
            
            return o1 // o2

        if self.humn == 1:
            next = right
            value = o1
        elif self.humn == 0:
            next = left
            value = o2

        return  (next, value)

    def find_human(self, monkeys):
        if self.name =='humn':
            self.humn = 2
            return

        if self.value != None:
            return

        left = monkeys[self.left]
        left.find_human(monkeys)

        if left.humn != -1:
            self.humn = 0
            return

        right = monkeys[self.right]
        right.find_human(monkeys)

        if right.humn != -1:
            self.humn = 1

    def pass_equality(self, result, monkeys):
        if self.value != None:
            return self.value

        
        left = monkeys[self.left]
        right = monkeys[self.right]
        if left.humn == -1:
            next = right
            val = left.get_value(monkeys)


            if self.op == '+': result -= val
            elif self.op == '-': result = val - result
            elif self.op == '*': result //= val
            else: result = val // result
        
        else:
            next = left
            val = right.get_value(monkeys)

            if self.op == '+': result -= val
            elif self.op == '-': result += val
            elif self.op == '*': result //= val
            else: result *= val

        if next.name == 'humn':
            return result

        return next.pass_equality(result, monkeys)

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    monkeys = {}
    for line in data:
        parts = line.split(':')
        name = parts[0]
        monkeys[name] = Monkey(name)
        op = parts[1].strip().split(' ')
        l = len(op)
        monkey = monkeys[name] 
        if l < 2:
            monkey.value = int(op[0])
        else:
            monkey.op = op[1]
            monkey.left = op[0]
            monkey.right = op[2]

    #Part 1
    # r = monkeys['root'].get_value(monkeys)
    # print(r)

    #Part 2
    root = monkeys['root']
    root.find_human(monkeys)
    (next, result) = root.get_diff(monkeys)
    r = next.pass_equality(result, monkeys)
    print(r)

