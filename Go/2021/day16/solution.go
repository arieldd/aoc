package main

import (
	"bufio"
	"fmt"
	"os"
)

var hexToBytes = map[rune][]byte{
	'0': {0, 0, 0, 0},
	'1': {0, 0, 0, 1},
	'2': {0, 0, 1, 0},
	'3': {0, 0, 1, 1},
	'4': {0, 1, 0, 0},
	'5': {0, 1, 0, 1},
	'6': {0, 1, 1, 0},
	'7': {0, 1, 1, 1},
	'8': {1, 0, 0, 0},
	'9': {1, 0, 0, 1},
	'A': {1, 0, 1, 0},
	'B': {1, 0, 1, 1},
	'C': {1, 1, 0, 0},
	'D': {1, 1, 0, 1},
	'E': {1, 1, 1, 0},
	'F': {1, 1, 1, 1},
}

func main() {
	if len(os.Args) < 2 {
		panic("Please provide an input file")
	}
	lines := readLines(os.Args[1])

	bits := getBitEncoding(lines[0])

	fmt.Println(toPacket(bits))

	fmt.Println("Part 1: ", part1(bits))
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

func part1(bits []byte) int {
	packet := toPacket(bits)
	version := toDecimal(packet.version[:])
	typeId := toDecimal(packet.typeId[:])
	if typeId == 4 {
		fmt.Println("Literal")
	}

	fmt.Println(version)
	return 0
}

func part2(lines []string) int {
	return 0
}

type packet struct {
	version  [3]byte
	typeId   [3]byte
	contents []byte
}

func getBitEncoding(hex string) []byte {
	bits := make([]byte, 0)

	for _, c := range hex {
		bits = append(bits, hexToBytes[c]...)
	}
	return bits
}

func toPacket(bits []byte) (p packet) {
	p.version = ([3]byte)(bits[:3])
	p.typeId = ([3]byte)(bits[3:6])
	p.contents = bits[6:]
	return p
}

func toDecimal(bits []byte) int {
	decimal := 0

	for _, b := range bits {
		decimal *= 2
		decimal += int(b)
	}
	return decimal
}

func parsePackets(bits []byte) []packet {
	return nil
}
