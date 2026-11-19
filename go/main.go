package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) > 1 {
		if !fileQuery(os.Args[1:]) {
			fmt.Println("Формат: go run main.go --file data.txt --query 'SETADD 10'")
		}
		return
	}
	for {
		fmt.Print("\n1. Стек и скобки\n" +
			"2. Множество\n" +
			"3. Пересечение множеств\n" +
			"4. Пирамида\n" +
			"5. Правый край дерева\n" +
			"6. Хеш-таблицы\n" +
			"7. Метод свёртки\n" +
			"8. LFU-кэш\n" +
			"0. Выход\nВыбор: ")
		number := readNum()
		if inputEnded || number == 0 {
			return
		}
		switch number {
		case 1:
			runStackParentheses()
		case 2:
			runSetADT()
		case 3:
			runSetIntersection()
		case 4:
			runPyramid()
		case 5:
			runTreeRightEdge()
		case 6:
			runHashTable()
		case 7:
			runHashFolding()
		case 8:
			runLFUCache()
		default:
			fmt.Println("Нет такого задания")
		}
	}
}
