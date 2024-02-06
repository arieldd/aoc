package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

func main() {

	filename := os.Args[1]

	file, err := os.Open(filename)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer file.Close()

	var lines []int
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		value, _ := strconv.Atoi(scanner.Text())
		lines = append(lines, value)
	}

	if err := scanner.Err(); err != nil {
		fmt.Println(err)
	}

	part1(lines)
	part2(lines)
}

func part1(lines []int) {
	prev_depth := lines[0]
	inc_count := 0
	for i := 1; i < len(lines); i++ {
		depth := lines[i]
		if depth > prev_depth {
			inc_count++
		}
		prev_depth = depth
	}

	fmt.Printf("\nPart 1: %d", inc_count)
}

func part2(lines []int) {
	prev_depth := lines[0] + lines[1] + lines[2]
	inc_count := 0
	for i := 1; i < len(lines)-2; i++ {
		depth := lines[i] + lines[i+1] + lines[i+2]
		if depth > prev_depth {
			inc_count++
		}
		prev_depth = depth
	}

	fmt.Printf("\nPart 2: %d", inc_count)
}
