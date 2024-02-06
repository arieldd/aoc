package main

import (
	"bufio"
	"fmt"
	"os"
)

var dir_map = map[string]int{
	"forward": 0,
	"down":    1,
	"up":      2,
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Please provide a file name")
		return
	}

	file_name := os.Args[1]

	file, err := os.Open(file_name)

	defer file.Close()

	if err != nil {
		fmt.Println("Error opening file:", err)
		return
	}

	scanner := bufio.NewScanner(file)

	var lines []Move
	var dir_str string
	var value int
	for scanner.Scan() {
		fmt.Sscanf(scanner.Text(), "%s %d", &dir_str, &value)
		lines = append(lines, Move{dir_map[dir_str], value})
	}

	fmt.Printf("Part 1: %d\n", part1(lines))
	fmt.Printf("Part 2: %d\n", part2(lines))
}

func part1(lines []Move) (result int) {
	hpos := 0
	vpos := 0

	for _, line := range lines {
		switch line.dir {
		case 0:
			hpos += line.value
		case 1:
			vpos += line.value
		case 2:
			vpos -= line.value
		}
	}

	return hpos * vpos
}

func part2(lines []Move) (result int) {
	hpos := 0
	vpos := 0
	aim := 0

	for _, instr := range lines {
		switch instr.dir {
		case 0:
			hpos += instr.value
			vpos += aim * instr.value
		case 1:
			aim += instr.value
		case 2:
			aim -= instr.value
		}
	}

	return hpos * vpos
}

type Move struct {
	dir   int
	value int
}
