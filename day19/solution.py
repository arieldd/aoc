class Resources:
    def __init__(self) -> None:
        self.robots = [1, 0, 0, 0]
        self.mats [0,0,0,0]

    def simulate_cycle(self):
        for i in range(4):
            self.mats[i] += self.robots[i]

    def can_build(self, cost, index):
        if index > 0:
            if cost[0] < self.mats[0] or cost[1] < self.mats[index]:
                return False

        else:
            if cost < self.mats[0]:
                return False
                
        return True

    def start_robot(self, cost, index):
        if index > 1:
            self.mats[0] -= cost[0]
            self.mats[index] -= cost[1]

        else:         
            self.mats[0] -= cost
                 
        return True

    def finish_robot(self, index):
        self.robots[index] += 1
    
    def destroy_robot(self, cost, index):
        if index > 1:
            self.mats[0] += cost[0]
            self.mats[index - 1] += cost[1]

        else:      
            self.mats[0] += cost
            
        self.robots[index] -= 1

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

    def optimize_production(self, endTime):
        res = Resources()
        self.__optimize(0, res, endTime)
    
    def __optimize(self, current, res, endTime):
        if current == endTime:
            return res.resources[3]

        for i in reversed(range(4)):
            if res.can_build(self.geode, i-1):
                res.start_robot(self.geode, i-1)
                res.simulate_cycle()
                res.finish_robot(i)
                
                best = self.__optimize(current + 1, res, endTime)
                if best > self.max:
                    self.max = best

        self.__optimize(minute + 1, past)


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
        blueprints[bp_id].optimize_production(endTime)

    blueprints[1].max = 9
    blueprints[2].max = 12

    q = find_quality_levels(blueprints)
    print(q, sum(q))