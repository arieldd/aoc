package main

import (
	"aoc/go/utils"
	"bufio"
	"fmt"
	"math"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}

	lines := readLines(os.Args[1])
	grid := readGrid(lines)

	fmt.Println("Part 1: ", solve(grid, len(grid)))
	fmt.Println("Part 2: ", solve(grid, len(grid)*5))
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

func readGrid(lines []string) (grid [][]int) {
	grid = make([][]int, len(lines))
	for i, l := range lines {
		grid[i] = make([]int, len(l))
		for j, c := range l {
			risk := int(c - '0')
			grid[i][j] = risk
		}
	}
	return grid
}

func solve(grid [][]int, size int) int {
	start := node{0, 0, grid[0][0]}
	end := node{size - 1, size - 1, getRisk(grid, size-1, size-1, size)}
	return lowestRisk(grid, make([]node, 0), start, end, size)
}

type node struct {
	r, c, risk int
}

func lowestRisk(grid [][]int, visited []node, start, end node, size int) (lowestRisk int) {
	pq := make(utils.PQ[node], 0)
	pq.Init()

	start.risk = 0
	pq.TPush(start, start.risk)

	lowestRisk = math.MaxInt32
	for pq.Len() > 0 {
		current := pq.TPop()
		if contains(visited, current) {
			continue
		}

		if equals(current, end) && current.risk < lowestRisk {
			fmt.Println("Found end")
			lowestRisk = current.risk
			return lowestRisk
		}

		visited = append(visited, current)

		for k := 0; k < utils.Dirs; k += 2 {
			ni := current.r + utils.Dy[k]
			nj := current.c + utils.Dx[k]

			// if utils.IsValid(ni, nj, size) {
			// 	risk := grid[ni][nj]
			if risk := getRisk(grid, ni, nj, size); risk != -1 {
				next := node{ni, nj, risk + current.risk}
				pq.TPush(next, next.risk)
			}
		}
	}

	return lowestRisk
}

func getRisk(grid [][]int, i, j, size int) int {
	if !utils.IsValid(i, j, size) {
		return -1
	}
	n := len(grid)

	risk := grid[i%n][j%n]
	dx := j / n
	dy := i / n

	risk = (risk + dy + dx)

	if risk > 9 {
		risk %= 9
	}

	return risk
}

func equals(a, b node) bool {
	return a.r == b.r && a.c == b.c
}

func contains(list []node, x node) bool {
	for _, node := range list {
		if node.r == x.r && node.c == x.c {
			return true
		}
	}
	return false
}
