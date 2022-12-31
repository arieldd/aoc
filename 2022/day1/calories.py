def most_calories(data):
    max = 0
    current_total = 0

    for value in data:
        if value == '':
            if current_total > max:
                max = current_total
            current_total = 0
            continue

        current_total += int(value)

    return max

def top_three(data):
    elves = []
    current_total = 0
    for value in data:
        if value == '':
            elves.append(current_total)
            current_total = 0
            continue

        current_total += int(value)

    elves.sort(reverse=True)
    return sum(elves[0:3])


if __name__ == "__main__":
    with open("calories.txt", "r") as file:
        data = [line.strip() for line in file.readlines()]
        print("most: " + str(most_calories(data)))
        print("top 3: " + str(top_three(data)))

