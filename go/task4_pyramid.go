package main

import (
	"fmt"
	"sort"
	"strconv"
	"strings"
)

// Задание 4. Жадный выбор самого высокого блока каждой ширины.
type Block struct {
	width  int
	height int
}

func runPyramid() {
	fmt.Print("Количество блоков: ")
	count := readNum()
	if count < 1 || count > 100000 {
		fmt.Println("Неверное количество блоков")
		return
	}
	blocks := make([]Block, 0, int(count))
	fmt.Println("Вводите ширину и высоту каждой строкой:")
	for int64(len(blocks)) < count {
		line := strings.Fields(readLine())
		if inputEnded && len(line) == 0 {
			return
		}
		if len(line) != 2 {
			fmt.Println("Ошибка ввода, повторите строку")
			continue
		}
		width, err1 := strconv.ParseInt(line[0], 10, 64)
		height, err2 := strconv.ParseInt(line[1], 10, 64)
		if err1 != nil || err2 != nil || width <= 0 || height <= 0 || width > 1_000_000_000 || height > 1_000_000_000 {
			fmt.Println("Ошибка ввода, повторите строку")
			continue
		}
		blocks = append(blocks, Block{int(width), int(height)})
	}
	sort.Slice(blocks, func(i, j int) bool {
		if blocks[i].width != blocks[j].width {
			return blocks[i].width > blocks[j].width
		}
		return blocks[i].height > blocks[j].height
	})
	previousWidth := 0
	var totalHeight int64
	for _, block := range blocks {
		if block.width != previousWidth {
			totalHeight += int64(block.height)
			previousWidth = block.width
		}
	}
	fmt.Println("Максимальная высота:", totalHeight)
}
