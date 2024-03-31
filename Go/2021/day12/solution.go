package main

import (
	"bufio"
	"fmt"
	"os"
	"slices"
	"strings"
)

type Cave struct {
	name string
	size int
	conn []string
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Please provide an input file")
		return
	}

	input, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println("Unable to read input file, error: ", err)
		return
	}

	lines := []string{}

	scanner := bufio.NewScanner(input)

	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	caves := readCaves(lines)
	fmt.Println("Part 1: ", part1(caves))
	fmt.Println("Part 2: ", part2(caves))
}

func part1(caves map[string]Cave) int {
	path := make([]string, 0)
	return findPaths(caves, path, "start", "end")
}

func part2(caves map[string]Cave) int {
	path := make([]string, 0)
	return findPaths2(caves, path, "start", "end", "start", false)
}

func findPaths(caves map[string]Cave, path []string, current, end string) int {
	c := caves[current]

	path = append(path, current)

	result := 0
	for _, next := range c.conn {
		if next == end {
			result++
		} else if caves[next].size == 1 || !slices.Contains(path, next) {
			result += findPaths(caves, path, next, end)
		}
	}

	path = path[:len(path)-1]
	return result
}

func findPaths2(caves map[string]Cave, path []string, current, end, start string, doubleVisit bool) int {
	c := caves[current]

	path = append(path, current)

	result := 0
	for _, next := range c.conn {
		if next == end {
			result++
		} else {
			if caves[next].size == 1 || !slices.Contains(path, next) {
				result += findPaths2(caves, path, next, end, start, doubleVisit)
			} else if next != start && !doubleVisit && slices.Contains(path, next) {
				result += findPaths2(caves, path, next, end, start, true)
			}
		}
	}

	path = path[:len(path)-1]
	return result
}

func readCaves(lines []string) (caves map[string]Cave) {
	caves = make(map[string]Cave)
	for _, l := range lines {
		path := strings.Split(l, "-")
		for i, label := range path {
			size := 0
			if label[0] < 'a' {
				size = 1
			}
			if _, exists := caves[label]; !exists {
				caves[label] = Cave{
					name: label,
					size: size,
					conn: []string{path[(i+1)%len(path)]},
				}
			} else {
				c := caves[label]
				c.conn = append(c.conn, path[(i+1)%len(path)])
				caves[label] = c
			}
		}
	}
	return caves
}

func printCaves(caves map[string]Cave) {
	for k, v := range caves {
		fmt.Print(k, ":")
		for _, c := range v.conn {
			fmt.Print(" ", c)
		}
		fmt.Println()
	}
	fmt.Println()
}
