package main

import (
	"fmt"
	"strconv"
	"strings"
)

func runHashFolding() {
	fmt.Print("Введите целый ключ: ")
	key := readNum()
	rest := magnitude(key)
	var parts []uint64
	for {
		parts = append(parts, rest%1000)
		rest /= 1000
		if rest == 0 {
			break
		}
	}
	for i, j := 0, len(parts)-1; i < j; i, j = i+1, j-1 {
		parts[i], parts[j] = parts[j], parts[i]
	}
	fmt.Print("Части: ")
	for i, part := range parts {
		if i > 0 {
			fmt.Print(" + ")
		}
		fmt.Print(part)
	}
	fmt.Println(" =", fold(key))
	fmt.Println("Индекс корзины:", fold(key)%1009)
	cuckoo := newCuckoo(1009)
	if err := cuckoo.add(key); err != nil {
		fmt.Println("Ошибка хеширования:", err)
		return
	}
	fmt.Println("Поиск в кукушке:", strings.ToUpper(strconv.FormatBool(cuckoo.has(key))))
	if key >= minInt32 && key <= maxInt32 {
		chain := newChainTable(1009)
		chain.add(int(key))
		fmt.Println("Поиск в цепочках:", strings.ToUpper(strconv.FormatBool(chain.has(int(key)))))
	} else {
		fmt.Println("Цепочки из ЛР1 хранят int, проверка пропущена")
	}
}
