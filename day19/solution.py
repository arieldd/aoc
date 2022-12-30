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

    def can_wait(self, costs, time, endTime):
        wait_times = []
        for ix, c in enumerate(costs):
            if c:
                if not self.robots[ix]:
                    return -1
                if self.mats[ix] >= c:
                    wait_times.append(0)
                    continue

                div = ceiling_division(c - self.mats[ix], self.robots[ix])
                wait_times.append(div) 
        return max(wait_times)
        
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

    def find_max_geodes(self, time, endTime, resource):
        if time == endTime:
            resource.collect_minerals()
            if resource.mats[3] > self.max:
                self.max = resource.mats[3]
            return

        if self.theoretical_max(resource, time, endTime) <= self.max:
            return

        time_left = endTime - time
        for robot in reversed(range(4)):
            t = time
            cost = self.get_cost(robot)
            if self.dont_have_enough_of(robot, resource, t, endTime):
                new_r = deepcopy(resource)
                
                wait = new_r.can_wait(cost, t, endTime)
                if wait < 0 or wait > time_left:
                    continue

                for _ in range(wait): 
                    new_r.collect_minerals()
                    t += 1

                if t > endTime:
                    continue

                new_r.collect_minerals()
                other_r = deepcopy(new_r)

                new_r.spend_minerals(cost)
                new_r.build_robot(robot, t)
                
                if t == endTime:
                    if other_r.mats[3] > self.max:
                        self.max = other_r.mats[3]
                    continue

                self.find_max_geodes(t + 1, endTime, new_r)

                other_r.last_not_built = robot
                self.find_max_geodes(t + 1, endTime, other_r)

    def find_max_geodes2(self, endTime):
        for robot in range(4):
            self.dfs(robot, 0, endTime, Resource())

    def dfs(self, robot, time, endTime, resource):
        if robot == 0 and resource.robots[robot] >= self.max_ore_robots:
            return 
        if robot == 1 and resource.robots[robot] >= self.max_clay_robots:
            return 
        if robot == 2 and resource.robots[robot] >= self.max_obsidian_robots:
            return

        time_left = endTime- time;
        if resource.mats[3] + int_seq_sum(resource.robots[3], resource.robots[3]+time_left) <= self.max:
            return

        while time < endTime:
            
            if self.build_next_bot(resource, robot, time):
                return

            resource = update(resource, -1)
            time += 1
        
        self.max = max(self.max, resource.mats[3])

    def dont_have_enough_of(self, robot, resource, time, endTime):
        if resource.last_not_built == robot:
            return False

        if robot == 3:
            return True #Never enough goedes

        time_left = endTime - time
        
        mats = resource.mats[robot]

        ore_cost = [self.costs[0], self.costs[1], self.costs[2][0], self.costs[3][0]]

        fleet = resource.robots[robot]
        if robot == 0:
            if fleet >= max(ore_cost):
                return False
            return max(ore_cost) * time_left > mats + fleet * time_left 

        clay_cost = self.costs[2][1]
        if robot == 1:
            if fleet >= clay_cost:
                return False    
            return clay_cost * time_left > mats + fleet * time_left

        obsidian_cost = self.costs[3][1]
        if fleet >= obsidian_cost:
            return False
            
        return obsidian_cost * time_left > mats + fleet * time_left

    def theoretical_max(self, resource, time, endTime):
        time_left = endTime - time + 1
        result = [resource.robots[3]]
        for i in range(1, time_left):
            result.append(result[i-1] + 1)

        return resource.mats[3] + sum(result)

    def build_next_bot(self, resource, robot, time):
        cost = self.get_cost(robot)
        if resource.can_build(cost):
            for next_robot in range(4):
                new_r = update(resource, robot, cost)
                self.dfs(next_robot, time + 1, endTime, new_r)
            return True
        return False

def update(resource, robot, cost = [0,0,0,0]):
    new_r = deepcopy(resource)

    new_r.spend_minerals(cost)
    new_r.collect_minerals()

    if robot != -1:
        new_r.robots[robot] += 1

    return new_r 

def int_seq_sum(a, b):
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
            #blueprints[bp_id].find_max_geodes(1, endTime, Resource())
            blueprints[bp_id].find_max_geodes2(endTime)
            print(blueprints[bp_id].max)
        
    #Part 1
    # q = find_quality_levels(blueprints)

    # print(q, sum(q))

    #Part 2
    mult = 1
    for id in check_ids:
        mult *= blueprints[id].max

    print(mult)
    