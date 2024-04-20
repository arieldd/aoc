package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}
	lines := readLines(os.Args[1])
	fmt.Println("Part 1: ", part1(lines))
	fmt.Println("Part 2: ", part2(lines))
}

func readLines(filename string) (lines []string) {
	file, err := os.Open(filename)
	if err != nil {
		panic("Failed to open input file")
	}
	defer file.Close()
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	return lines
}

func readGrid(lines []string) (grid [][]rune) {
	grid = make([][]rune, len(lines))
	for i, l := range lines {
		grid[i] = make([]rune, len(l))
		for j, c := range l {
			grid[i][j] = c
		}
	}
	return grid
}

func part1(lines []string) int {
	return 0
}

func part2(lines []string) int {
	return 0
}
