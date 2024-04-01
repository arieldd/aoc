package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}
	lines := readLines(os.Args[1])
	tmpl := lines[0]
	pairs := readPairs(lines[2:])
	fmt.Println("Part 1: ", solve(tmpl, pairs, 10))
	fmt.Println("Part 2: ", solve(tmpl, pairs, 40))
}

func readPairs(lines []string) (pairs map[string]string) {
	pairs = make(map[string]string)

	for _, l := range lines {
		parts := strings.Split(l, " ")
		pairs[parts[0]] = parts[len(parts)-1]
	}

	return pairs
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

func solve(tmpl string, pairs map[string]string, steps int) int64 {
	polymer := tmpl
	for i := range steps {
		fmt.Println("Step: ", i, " - ", polymer)
		polymer = doStep(polymer, pairs)
	}
	counts := make(map[rune]int64)
	for _, char := range polymer {
		counts[char]++
	}

	maxCount, minCount := int64(0), int64(len(polymer))
	for _, v := range counts {
		if v < minCount {
			minCount = v
		}

		if v > maxCount {
			maxCount = v
		}
	}
	fmt.Println(maxCount, minCount)
	return maxCount - minCount
}

func doStep(polymer string, pairs map[string]string) string {
	size := len(polymer)

	var result strings.Builder
	for i := range size - 1 {
		p := polymer[i : i+2]

		if insert, exists := pairs[p]; exists {
			var temp strings.Builder
			temp.WriteByte(p[0])
			temp.WriteString(insert)
			if i == size-2 {
				temp.WriteByte(p[1])
			}

			result.WriteString(temp.String())
		} else {
			result.WriteString(p)
		}
	}

	return result.String()
}
