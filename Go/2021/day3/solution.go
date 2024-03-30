package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Please provide an input file")
		return
	}

	file, err := os.Open(os.Args[1])

	if err != nil {
		fmt.Println("Couldn't open file input file", err)
		return
	}

	defer file.Close()

	scanner := bufio.NewScanner(file)

	lines := []string{}
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	fmt.Printf("Part 1: %d\n", part1(lines))
	fmt.Printf("Part 1: %d\n", part2(lines))
}

func part1(lines []string) int {
	var (
		gamma   []byte
		epsilon []byte
	)

	for i := 0; i < len(lines[0]); i++ {
		var (
			ones, zeros int
		)
		for _, line := range lines {
			if line[i] == '1' {
				ones++
			} else {
				zeros++
			}
		}
		if ones > zeros {
			gamma = append(gamma, '1')
			epsilon = append(epsilon, '0')
		} else {
			gamma = append(gamma, '0')
			epsilon = append(epsilon, '1')
		}
	}

	return toDecimal(string(gamma)) * toDecimal(string(epsilon))
}

func part2(lines []string) int {
	oxygen := filter(lines, 0, '1')
	co2 := filter(lines, 0, '0')

	return toDecimal(oxygen) * toDecimal(co2)
}

func filter(lines []string, index int, value byte) string {
	if len(lines) == 1 {
		return lines[0]
	}

	var (
		ones, zeros []string
	)

	for _, line := range lines {
		if line[index] == '1' {
			ones = append(ones, line)
		} else {
			zeros = append(zeros, line)
		}
	}

	if len(ones) >= len(zeros) {
		return filter(ones, index+1, value)
	} else {
		return filter(zeros, index+1, value)
	}
}

func toDecimal(binary string) int {
	decimal := 0
	for i, c := range binary {
		if c == '1' {
			decimal += 1 << (len(binary) - i - 1)
		}
	}
	return decimal
}
