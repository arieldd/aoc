with open("test.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    ore = 0
    clay = 0
    obsidian = 0
    geodes = 0

    blueprints = {}

    for line in data:
        split = line.split(':')
        bp_id = int(split[0].split()[1])