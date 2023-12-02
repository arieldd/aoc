import math

snafu_digits = {'2': 2, '1' : 1, '0': 0, '-': -1, '=' : -2}

def parse_snafu(snafu):
    power = len(snafu) - 1
    result = []
    for pos in snafu:
        digit = snafu_digits[pos]
        value = int(math.pow(5, power)) * digit
        result.append(value)
        power-=1

    return sum(result)

def convert_to_snafu(number):
    snafu = []
    while number > 0:
        snafu.append(number % 5)
        number //= 5
    
    snafu.append(0)

    result = []
    carry_over = 0
    for i, d in enumerate(snafu):
        d += carry_over

        if d > 2:
            carry_over = 1
            d %= 5
        else:
            carry_over = 0

        result.append(d)

    if result[-1] == 0:
        result.pop()

    print(result)

    text = ''
    for d in reversed(result):
        s_d = str(d)
        if d == 3:
            s_d ='='
        elif d == 4:
            s_d = '-'
        text += s_d
    return text

with open("input.txt", "r") as file:

    data = [line.strip() for line in file.readlines()]

    numbers = []
    for line in data:
        numbers.append(parse_snafu(line))

    fuel = sum(numbers)

    result = convert_to_snafu(fuel)
    print(result)

    #print(convert_to_snafu(314159265))