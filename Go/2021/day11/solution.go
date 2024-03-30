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

	grid := parsegrid(lines)
	printGrid(grid)

	flashes := part1(&grid, 2)
	fmt.Printf("Flashed %d times\n", flashes)

	printGrid(grid)
	return
}

func part1(grid *[gridSize][gridSize]Octopus, steps int) (flashes int) {
	flashes = 0

	for range steps {
		flashes += simulateStep(grid)
	}
	return flashes
}

func simulateStep(grid *[gridSize][gridSize]Octopus) (flashes int) {
	flashes = 0
	for i, row := range grid {
		for j := range row {
			grid[i][j].energy++
		}
	}

	for i, row := range grid {
		for j, oct := range row {
			if oct.energy > 9 && !oct.flashed {
				flashes += flash(grid, i, j)
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
	return flashes
}

func flash(grid *[gridSize][gridSize]Octopus, i, j int) (flashes int) {
	grid[i][j].flashed = true

	flashes++
	for k := range dirs {
		ni := i + dy[k]
		nj := j + dx[k]
		if isValid(ni, nj, gridSize) {
			oct := grid[ni][nj]
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

func printGrid(grid [gridSize][gridSize]Octopus) {
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
}

func parsegrid(lines []string) (grid [gridSize][gridSize]Octopus) {
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
