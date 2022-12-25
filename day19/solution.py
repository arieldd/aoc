from copy import deepcopy
class Resources:
    def __init__(self) -> None:
        self.robots = [1, 0, 0, 0]
        self.mats = [0,0,0,0]

    def __str__(self) -> str:
        return f'Robots : {self.robots}, Mats : {self.mats}' 

    def simulate_cycle(self):
        for i in range(4):
            self.mats[i] += self.robots[i]

    def can_build(self, cost, index):
        if index > 1:
            if cost[0] > self.mats[0] or cost[1] > self.mats[index]:
                return False

        else:
            if cost > self.mats[0]:
                return False
                
        return True
        
    def build_robot(self, cost, index):
        if index > 1:
            self.mats[0] -= cost[0]
            self.mats[index] -= cost[1]

        else:         
            self.mats[0] -= cost
        
        self.robots[index] += 1

class Blueprint:
    def __init__(self, id, costs) -> None:
        self.id = id
        self.ore = costs[0]
        self.clay = costs[1]
        self.obsidian = costs[2]
        self.geode = costs[3]
        self.max = 0

    def __str__(self) -> str:
        return f'{self.id}: {self.ore} {self.clay} {self.obsidian} {self.geode}. Max {self.max} '

    def max_geodes(self, endTime):
        time = 1

        paths =[Resources()]
        while time <= endTime:
            
            current_builds = list(paths)

            for r in current_builds:
                r.simulate_cycle()
                for ix, cost in enumerate([self.ore, self.clay, self.obsidian, self.geode]):
                    if r.can_build(cost, ix):
                        new_r = deepcopy(r)
                        new_r.build_robot(cost, ix)
                        paths.append(new_r)
                        print(time, new_r)

            time += 1
        
        print(paths)

        best = 0
        for r in paths:
            geodes = r.mats[3]
            if geodes > best:
                best = geodes

        self.max = best

def find_quality_levels(blueprints):

    qualities = []
    for bp in blueprints.values():
        qualities.append(bp.id * bp.max)

    return qualities

with open("test.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    endTime = 24

    blueprints = {}

    for line in data:
        split = line.split(':')
        bp_id = int(split[0].split()[1])
        robots = split[1].split()
        
        costs = [int(robots[4]), int(robots[10]), [int(robots[16]), int(robots[19])], [int(robots[25]), int(robots[28])]]
        blueprints[bp_id] = Blueprint(bp_id, costs)
        blueprints[bp_id].max_geodes(endTime)
    

    q = find_quality_levels(blueprints)
    print(q, sum(q))