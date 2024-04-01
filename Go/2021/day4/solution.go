package main

import (
	"bufio"
	"fmt"
	"os"
	"slices"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}
	lines := readLines(os.Args[1])
	game := readGame(lines)

	fmt.Println("Part 1: ", part1(game))
	fmt.Println("Part 2: ", part2(game))
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

func part1(game game) int {
	for _, n := range game.numbers {
		for i := range game.boards {
			if game.boards[i].mark(n) {
				return game.boards[i].score(n)
			}
		}
	}
	return 0
}

func part2(game game) int {
	winningBoards := make([]int, 0)
	for _, n := range game.numbers {
		for i := range game.boards {
			if !slices.Contains(winningBoards, i) && game.boards[i].mark(n) {
				winningBoards = append(winningBoards, i)
				if len(winningBoards) == len(game.boards) {
					return game.boards[i].score(n)
				}
			}
		}
	}
	return 0
}

func readGame(lines []string) (game game) {
	numberLine := strings.Split(lines[0], ",")
	for _, str := range numberLine {
		draw, _ := strconv.Atoi(str)
		game.numbers = append(game.numbers, draw)
	}

	b := board{}
	r := 0
	for _, l := range lines[2:] {
		if l == "" {
			game.boards = append(game.boards, b)
			b = board{}
			r = 0
			continue
		}

		row := strings.Split(l, " ")
		for c, v := range row {
			n, _ := strconv.Atoi(v)
			b.card[r][c] = number{value: n}
		}
		r++
	}
	game.boards = append(game.boards, b)

	return game
}

func printGame(game game) {
	for _, n := range game.numbers {
		fmt.Print(n, " ")
	}

	fmt.Println()

	for _, b := range game.boards {
		for _, row := range b.card {
			for _, n := range row {
				fmt.Print(n.value, " ")
			}
			fmt.Println()
		}
		fmt.Println()
	}
}

type number struct {
	value  int
	marked bool
}

type board struct {
	card [5][5]number
}

func (b *board) mark(draw int) bool {
	for i, row := range b.card {
		for j, n := range row {
			if n.value == draw {
				b.card[i][j].marked = true

				return b.bingo(i, j)
			}
		}
	}
	return false
}

func (b *board) score(draw int) int {
	sum := 0

	for _, row := range b.card {
		for _, n := range row {
			if !n.marked {
				sum += n.value
			}
		}
	}

	return sum * draw
}

func (b *board) bingo(i, j int) bool {
	won := true
	for k := range len(b.card) {
		if !b.card[k][j].marked {
			won = false
			break
		}
	}

	if won {
		return true
	}

	for k := range len(b.card) {
		if !b.card[i][k].marked {
			return false
		}
	}
	return true
}

type game struct {
	numbers []int
	boards  []board
}
