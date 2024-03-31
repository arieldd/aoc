package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"slices"
	"strconv"
	"strings"
)

type foldInstr struct {
	dim   int
	value int
}

type point struct {
	x int
	y int
}

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}
	lines := readLines(os.Args[1])
	points, folds := readInstr(lines)

	fmt.Println("Part 1: ", part1(points, folds[0]))
	fmt.Println("Part 2: ")
	part2(points, folds)
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
func readInstr(lines []string) (points map[point]struct{}, folds []foldInstr) {
	points = make(map[point]struct{}, 0)

	sep := slices.Index(lines, "")
	for _, l := range lines[:sep] {
		coords := strings.Split(l, ",")
		x, _ := strconv.Atoi(coords[0])
		y, _ := strconv.Atoi(coords[1])
		points[point{x, y}] = struct{}{}
	}

	for _, l := range lines[sep+1:] {
		parts := strings.Split(l, " ")
		fold := strings.Split(parts[2], "=")
		axis := 0
		if fold[0] == "y" {
			axis = 1
		}
		val, _ := strconv.Atoi(fold[1])

		folds = append(folds, foldInstr{dim: axis, value: val})
	}

	return points, folds
}

func doFold(points map[point]struct{}, fold foldInstr) {
	for k := range points {
		p := k
		if fold.dim == 0 && p.x > fold.value {
			p.x = fold.value - (p.x - fold.value)
		} else if fold.dim == 1 && p.y > fold.value {
			p.y = fold.value - (p.y - fold.value)
		}
		delete(points, k)
		points[p] = struct{}{}
	}
}

func part1(points map[point]struct{}, fold foldInstr) int {
	doFold(points, fold)
	return len(points)
}

func part2(points map[point]struct{}, folds []foldInstr) {
	for _, fold := range folds {
		doFold(points, fold)
	}

	minX, maxX, minY, maxY := math.MaxInt, math.MinInt, math.MaxInt, math.MinInt
	for p := range points {
		if p.x > maxX {
			maxX = p.x
		}

		if p.y > maxY {
			maxY = p.y
		}

		if p.x < minX {
			minX = p.x
		}

		if p.y < minY {
			minY = p.y
		}
	}
	sizeX, sizeY := maxX-minX+1, maxY-minY+1

	grid := make([][]int, sizeY)
	for i := range sizeY {
		grid[i] = make([]int, sizeX)
	}

	for p := range points {
		grid[p.y-minY][p.x-minX] = 1
	}

	for i := range sizeY {
		for j := range sizeX {
			if grid[i][j] == 1 {
				fmt.Print("#")
			} else {
				fmt.Print(".")
			}
		}
		fmt.Println()
	}
}
