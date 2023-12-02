
def calculate_score(data):
    elf_ecore = {"A" : 1, "B" : 2, "C": 3}
    my_score = {"X" : 1, "Y": 2, "Z" : 3}
    
    score = 0
    for p1, p2 in data:
        points = my_score[p2]
        result = points - elf_ecore[p1]

        score += points

        if result == 0:
            score += 3 #draw
        
        if result > 0:
            if result == 2:
                continue #lost
            
            score += 6 # won

        if result == -2:
            score += 6 # won

    return score

def follow_guide(data):
    plays = {
        "A" : {"X" : 3, "Y": 4, "Z" : 8}, #rock 1
        "B" : {"X" : 1, "Y": 5, "Z" : 9}, #paper 2
        "C":  {"X" : 2, "Y": 6, "Z" : 7}} #scissors 3
    
    score = 0
    for p1, p2 in data:
        score += plays[p1][p2]

    return score
         

if __name__ == "__main__":
    with open("input.txt", "r") as file:
        data = [line.strip().split() for line in file.readlines()]
        print("score: " + str(calculate_score(data)))
        print("score: " + str(follow_guide(data)))
