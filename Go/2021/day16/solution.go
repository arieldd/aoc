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
	packets := parsePackets(bits, 100)
	sum := 0
	for _, packet := range packets {
		sum += toDecimal(packet.version[:])
	}
	return sum
}

func part2(lines []string) int {
	return 0
}

type packet_t struct {
	version  [3]byte
	typeId   [3]byte
	contents []byte
	value    int
}

func getBitEncoding(hex string) []byte {
	bits := make([]byte, 0)

	for _, c := range hex {
		bits = append(bits, hexToBytes[c]...)
	}
	return bits
}

func toPacket(bits []byte) (p packet_t) {
	p.version = ([3]byte)(bits[:3])
	p.typeId = ([3]byte)(bits[3:6])
	p.contents = bits[6:]
	return p
}

func toDecimal(bits []byte) int {
	decimal := 0

	for _, b := range bits {
		decimal <<= 1
		decimal += int(b)
	}
	return decimal
}

func parsePackets(bits []byte, limit int) (packets []packet_t) {
	if len(bits) < 7 {
		return packets
	}

	packet := toPacket(bits)
	typeId := toDecimal(packet.typeId[:])
	packets = append(packets, packet)

	switch typeId {
	case 4: // Literal
		value := parseLiteral(packet.contents)
		if len(value) < len(packet.contents) {
			skip := len(value) + (len(value) / 4)
			packets = append(packets, parsePackets(packet.contents[skip:], limit-1)...)
		}
	default: // Operand
		if packet.contents[0] == 0 {
			length := toDecimal(packet.contents[1:16])
			packets = append(packets, parsePackets(packet.contents[16:16+length], limit-1)...)
		} else {
			valid := toDecimal(packet.contents[1:12])
			packets = append(packets, parsePackets(packet.contents[12:], valid)...)
		}
	}
	return packets
}

func parseLiteral(bits []byte) (result []byte) {
	result = append(result, bits[1:5]...)
	if bits[0] == 1 {
		result = append(result, parseLiteral(bits[5:])...)
	}
	return result
}
