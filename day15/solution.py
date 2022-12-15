import numpy as np

class Sensor:
    def __init__(self,x, y, beacon) -> None:
        self.x = x
        self.y = y
        self.beacon = beacon
        self.distance = abs(self.x - self.beacon[0]) + abs(self.y - self.beacon[1])

    def __str__(self) -> str:
        return str((self.x, self.y)) + ':' + str(self.beacon) + '->' + str(self.distance)
    
    def can_see(self, x, y):
        if x == self.x and y == self.y:
            return True
        
        d = abs(y - self.y)
        if d > self.distance:
            return False

        cols = self.distance - d + 1
        if (x, y) == (self.x, y):
            return True

        if x in range(self.x - cols, self.x + cols + 1):
            return True
        
        return False

    def get_row_interval(self, y):
        d = abs(y - self.y)
        if d > self.distance:
            return []

        cols = self.distance - d
        return [self.x - cols, self.x + cols]

def get_row_cover(sensors, beacons, row, min = -5e6, max = 5e6):
    y = row
    covered = {}
    for s in sensors:
        d = abs(y - s.y)
        if d > s.distance:
            continue

        cols = s.distance - d + 1
        pos = [(s.x, y)]

        for j in range(1, cols):
            if s.x - j >= min:
                pos.append((s.x - j, y))
            if s.x + j <= max:
                pos.append((s.x + j, y))

        for p in pos:
            if p not in covered:
                covered[p] = 3
    
    return covered

def get_row_cover_interval(sensors, row, min = -5e6, max = 5e6):
    y = row

    intervals = []
    for s in sensors:
        it = s.get_row_interval(y)
        if not it:
            continue
        if it[0] <= min and it[1] >= max:
            return [it] 
        intervals.append(it)
    
    intervals.sort(key=lambda x: x[0])
    result = [intervals[0]]
    for i in range(1, len(intervals)):
        current = intervals[i]
        last = result[-1]
        if last[1] > current[1]:
            continue
        if last[1] - current[0] >= -1:
            last[1] = current[1]
            continue
        result.append(current) 

    return result

def cover_map(map, sensors, beacons, size):
    for b in beacons:
        x = b[0]
        y = b[1]
        if is_valid_pos(x, size) and is_valid_pos(y, size):
            map[y][x] = 2

    for s in sensors:
        map[s.y][s.x] = 1
        for i in range(s.distance + 2):
            for y in [s.y - i, s.y + i]:
                if is_valid_pos(y, size):
                    for j in range(s.distance - i + 1):
                        x = s.x - j
                        if is_valid_pos(x, size) and not map[y][x]:
                                map[y][x] = 3

                        x = s.x + j
                        if is_valid_pos(x, size) and not map[y][x]:
                                map[y][x] = 3

def draw_map(map):
    for row in map:
        for value in row:
            text = '.'
            if value == 1:
                text = 'S'
            elif value == 2:
                text = 'B'
            elif value == 3:
                text = '#'
            print(text, end='')
        print()
    print('\n')

def is_valid_pos(pos, size):
    return 0 <= pos <= size

def find_signal(sensors, beacons, max):
    map = np.zeros([max +1, max + 1])
    
    cover_map(map, sensors, beacons, max)

    for i in range(max + 1):
        for j in range(max + 1):
            if not map[i][j]:
                return (j, i)

    return (0,0)

def find_signal_2(sensors, beacons, max):
    for y in range(max + 1):
        it = get_row_cover_interval(sensors, y, 0, max)
        print(y, it)
        if len(it) > 1:
            return (it[1][0]-1, y)

    return (0,0)

with open("input.txt", "r") as file:
    
    data = [line.strip() for line in file.readlines()]

    sensors = []
    beacons = []

    for line in data:
        split = line.split(':')

        for i in range(2):
            values = split[i].split('at')[-1].split(',')
            x = int(values[0].split('=')[-1])
            y = int(values[1].split('=')[-1])

            if i == 0: 
                sensor = (x, y)
            else:
                beacon = (x,y)
                sensors.append(Sensor(sensor[0], sensor[1], beacon))
                beacons.append(beacon)

    #Part 1     
    # row = 2000000
    # covered = get_row_cover(sensors, beacons, row, 0, 4000000)
    # #print(covered)
    # print(covered)

    #Part 2
    limit = 4000000
    mult = 4000000
    pos = find_signal_2(sensors, beacons, limit)
    print(pos)
    print(pos[0] * mult + pos[1])
