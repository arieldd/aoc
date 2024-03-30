package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type Octopus struct {
	energy  int
	flashed bool
}

const gridSize = 10
const dirs = 8

var dx = [dirs]int{1, 1, 0, -1, -1, -1, 0, 1}
var dy = [dirs]int{0, 1, 1, 1, 0, -1, -1, -1}

func main() {
	if len(os.Args) < 2 {
		fmt.Print("Please provide an input file")
		return
	}

	input, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Printf("failed to open input file with error %s", err)
		return
	}
	defer input.Close()

	scanner := bufio.NewScanner(input)
	lines := []string{}
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	grid := parseGrid(lines)

	flashes := part1(grid, 100)
	fmt.Printf("Part 1: %d \n\n", flashes)

	steps := part2(grid)
	fmt.Printf("Part 2: %d \n\n", steps+100)
	return
}

func part1(grid [][]Octopus, steps int) (flashes int) {
	flashes = 0

	printGrid(grid)
	for range steps {
		flashes += simulateStep(grid)
	}
	printGrid(grid)
	return flashes
}

func part2(grid [][]Octopus) (step int) {
	step = 1
	printGrid(grid)
	for {
		stepFlashes := simulateStep(grid)
		if stepFlashes == gridSize*gridSize {
			break
		}
		step++
	}
	printGrid(grid)
	return step
}

func simulateStep(grid [][]Octopus) (flashCount int) {
	flashCount = 0
	for i, row := range grid {
		for j := range row {
			grid[i][j].energy++
		}
	}

	for i, row := range grid {
		for j, oct := range row {
			if oct.energy > 9 && !oct.flashed {
				flashCount += flash(grid, i, j)
			}
		}
	}

	for i, row := range grid {
		for j, oct := range row {
			if oct.flashed {
				grid[i][j].energy = 0
				grid[i][j].flashed = false
			}
		}
	}
	return flashCount
}

func flash(grid [][]Octopus, i, j int) (flashes int) {
	grid[i][j].flashed = true

	flashes = 1
	for k := range dirs {
		ni := i + dy[k]
		nj := j + dx[k]
		if isValid(ni, nj, gridSize) {
			oct := &grid[ni][nj]
			oct.energy++
			if oct.energy > 9 && !oct.flashed {
				flashes += flash(grid, ni, nj)
			}
		}
	}

	return flashes
}

func isValid(ni, nj, gridSize int) bool {
	return ni >= 0 && nj >= 0 && ni < gridSize && nj < gridSize
}

func printGrid(grid [][]Octopus) {
	for _, row := range grid {
		for _, oct := range row {
			if oct.energy == 0 {
				fmt.Print(bold(oct.energy))
			} else {
				fmt.Printf("%d", oct.energy)
			}
		}
		fmt.Println()
	}
	fmt.Println()
}

func parseGrid(lines []string) (grid [][]Octopus) {
	grid = make([][]Octopus, gridSize)
	for i := range gridSize {
		grid[i] = make([]Octopus, gridSize)
	}

	for i, l := range lines {
		for j, c := range l {
			grid[i][j] = Octopus{energy: int(c - '0')}
		}
	}
	return grid
}

func bold(e int) string {
	return "\033[31m" + strconv.Itoa(e) + "\033[0m"
}
