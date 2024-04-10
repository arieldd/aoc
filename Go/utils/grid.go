package utils

const Dirs = 8

var Dx = [Dirs]int{1, 1, 0, -1, -1, -1, 0, 1}
var Dy = [Dirs]int{0, 1, 1, 1, 0, -1, -1, -1}

func IsValid(ni, nj, gridSize int) bool {
	return ni >= 0 && nj >= 0 && ni < gridSize && nj < gridSize
}
