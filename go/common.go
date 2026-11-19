package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const (
	minInt32 int64 = -1 << 31
	maxInt32 int64 = 1<<31 - 1
)

var input = bufio.NewReader(os.Stdin)
var inputEnded bool

func readLine() string {
	line, err := input.ReadString('\n')
	if err != nil {
		inputEnded = true
	}
	return strings.TrimRight(line, "\r\n")
}

func readNum() int64 {
	for {
		line := strings.TrimSpace(readLine())
		number, err := strconv.ParseInt(line, 10, 64)
		if err == nil {
			return number
		}
		if inputEnded {
			return 0
		}
		fmt.Print("Ожидалось целое число, попробуйте снова: ")
	}
}

func parseInt32(s string) (int, bool) {
	number, err := strconv.ParseInt(s, 10, 64)
	return int(number), err == nil && number >= minInt32 && number <= maxInt32
}
