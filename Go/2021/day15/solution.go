package main

import (
	"aoc/go/utils"
	"bufio"
	"fmt"
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
	visited := map[struct{ i, j int }]struct{}{}
	return lowestRisk(grid, visited, start, end, size)
}

type node struct {
	r, c, risk int
}

func lowestRisk(grid [][]int, visited map[struct{ i, j int }]struct{}, start, end node, size int) int {
	pq := make(utils.PQ[node], 0)
	pq.Init()

	start.risk = 0
	pq.TPush(start, start.risk)

	for pq.Len() > 0 {
		current := pq.TPop()
		if _, found := visited[struct {
			i int
			j int
		}{current.r, current.c}]; found {
			continue
		}

		if equals(current, end) {
			return current.risk
		}

		visited[struct{ i, j int }{current.r, current.c}] = struct{}{}

		for k := 0; k < utils.Dirs; k += 2 {
			ni := current.r + utils.Dy[k]
			nj := current.c + utils.Dx[k]

			if risk := getRisk(grid, ni, nj, size); risk != -1 {
				next := node{ni, nj, risk + current.risk}
				pq.TPush(next, next.risk)
			}
		}
	}

	return -1
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
