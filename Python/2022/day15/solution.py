import numpy as np

class Sensor:
    def __init__(self,x, y, beacon) -> None:
        self.x = x
        self.y = y
        self.beacon = beacon
        self.distance = abs(self.x - self.beacon[0]) + abs(self.y - self.beacon[1])

    def __str__(self) -> str:
        return str((self.x, self.y)) + ':' + str(self.beacon) + '->' + str(self.distance)
    
    def get_row_interval(self, y):
        d = abs(y - self.y)
        if d > self.distance:
            return []

        cols = self.distance - d
        return [self.x - cols, self.x + cols]

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

def find_signal(sensors, max):
    for y in range(max + 1):
        it = get_row_cover_interval(sensors, y, 0, max)
        if y % 10000 == 0:
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
    row = 2000000
    covered = get_row_cover_interval(sensors, row)
    print(covered[0][1] - covered[0][0])

    #Part 2
    limit = 4000000
    mult = 4000000
    pos = find_signal(sensors, limit)
    print(pos)
    print(pos[0] * mult + pos[1])
