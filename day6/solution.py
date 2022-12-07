from collections import deque

def first_marker(data, marker_size):
    size = len(data)

    marker = deque(data[:marker_size - 1])
    for i in range(marker_size-1, size):
        
        char = data[i]

        if len(marker) == marker_size and char not in marker:
            return i

        if char in marker:
            pos = marker.index(char)
    
            for j in range(0, pos+1):
                marker.popleft()
    
        marker.append(char)
        
    return 0

with open("input.txt", "r") as file:

    data = [char for char in file.read()]

    size = 14 #4 for P1
    print(first_marker(data, size))
