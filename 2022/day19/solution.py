from copy import deepcopy

class Resource:
    def __init__(self) -> None:
        self.mats = [0,0,0,0]
        self.robots = [1,0,0,0]
        self.last_not_built = -1
        self.build_order = []

    def  __str__(self) -> str:
        return f'{self.robots} : {self.mats} - {self.build_order}'

    def __eq__(self, other) -> bool:
        return self.mats == other.mats and self.robots == other.robots and self.build_order == other.build_order

    def collect_minerals(self):
        self.mats = [m + r for (m, r) in zip(self.mats, self.robots)]

    def spend_minerals(self, cost):
        self.mats = [ m - c for (m, c) in zip(self.mats, cost)]

    def build_robot(self, robot, time):
        self.robots[robot] += 1
        self.last_not_nuilt = -1
        self.build_order.append((robot, time))

    def can_build(self, costs):
        for ix, c in enumerate(costs):
            if c and self.mats[ix] < c:
                    return False 
        return True 
        
class Blueprint:
    def __init__(self, id, costs) -> None:
        self.id = id
        self.costs = costs
        self.max = 0
        self.max_ore_robots = max([self.costs[0], self.costs[1], self.costs[2][0], self.costs[3][0]])
        self.max_clay_robots = self.costs[2][1]
        self.max_obsidian_robots = self.costs[3][1]

    def __str__(self) -> str:
        return f'{self.id}: {self.ore} {self.clay} {self.obsidian} {self.geode}. Max {self.max} '

    def get_cost(self, robot):
        cost = [0] * 4
        cost[0] = self.costs[robot] if robot < 2 else self.costs[robot][0]
        if robot > 1:
            cost[robot - 1] = self.costs[robot][1]
        return cost

    def find_max_geodes(self, endTime):
        for robot in range(4):
            self.explore_build(robot, 0, endTime, Resource())

    def explore_build(self, robot, time, endTime, resource):
        if robot == 0 and resource.robots[robot] >= self.max_ore_robots:
            return 
        if robot == 1 and resource.robots[robot] >= self.max_clay_robots:
            return 
        if robot == 2 and resource.robots[robot] >= self.max_obsidian_robots:
            return

        time_left = endTime- time;
        if resource.mats[3] + interval_sum(resource.robots[3], resource.robots[3]+time_left) <= self.max:
            return

        while time < endTime:

            cost = self.get_cost(robot)
            if resource.can_build(cost):                
                for next_robot in range(4):
                    new_r = deepcopy(resource)

                    new_r.spend_minerals(cost)
                    new_r.collect_minerals()
                    new_r.build_robot(robot, time) 

                    self.explore_build(next_robot, time + 1, endTime, new_r)
                return 

            resource.collect_minerals()
            time += 1
        
        self.max = max(self.max, resource.mats[3])

def interval_sum(a, b):
    return (b - a) * (a + b) // 2

def ceiling_division(n, d):
    return -(n // -d)        

def find_quality_levels(blueprints):

    qualities = []
    for bp in blueprints.values():
        qualities.append(bp.id * bp.max)

    return qualities

import sys
with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    endTime = 32

    blueprints = {}
    
    check_ids = []
    if len(sys.argv) > 1:
        for id in sys.argv[1:]:
            if id.startswith('-'):
                break
            check_ids.append(int(id))

    for line in data:
        split = line.split(':')
        bp_id = int(split[0].split()[1])
        robots = split[1].split()
        
        costs = [int(robots[4]), int(robots[10]), [int(robots[16]), int(robots[19])], [int(robots[25]), int(robots[28])]]
        blueprints[bp_id] = Blueprint(bp_id, costs)
        if not check_ids or bp_id in check_ids:
            print(bp_id, costs)
            blueprints[bp_id].find_max_geodes(endTime)
            print(blueprints[bp_id].max)
        
    #Part 1
    # q = find_quality_levels(blueprints)

    # print(q, sum(q))

    #Part 2
    mult = 1
    for id in check_ids:
        mult *= blueprints[id].max

    print(mult)
    