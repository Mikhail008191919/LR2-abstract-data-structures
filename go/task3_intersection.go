package main

import (
	"fmt"
	"strings"
)

// Задание 3. Поиск пары множеств с максимальным пересечением.
func runSetIntersection() {
	fmt.Print("Сколько множеств? ")
	count := readNum()
	if count < 2 || count > 100 {
		fmt.Println("Нужно от 2 до 100 множеств")
		return
	}
	sets := make([]*IntSet, int(count))
	for i := range sets {
		sets[i] = newIntSet()
		fmt.Printf("Множество %d (числа через пробел): ", i+1)
		for _, word := range strings.Fields(readLine()) {
			value, ok := parseInt32(word)
			if ok && value > 0 {
				sets[i].add(value)
			}
		}
	}
	elements := make([][]int, len(sets))
	for i := range sets {
		elements[i] = sets[i].values()
	}
	first, second, best := 0, 1, -1
	for i := 0; i < len(sets); i++ {
		for j := i + 1; j < len(sets); j++ {
			common := 0
			for _, value := range elements[i] {
				if sets[j].has(value) {
					common++
				}
			}
			if common > best {
				best, first, second = common, i, j
			}
		}
	}
	fmt.Printf("Множество %d: %s\n", first+1, formatSet(sets[first]))
	fmt.Printf("Множество %d: %s\n", second+1, formatSet(sets[second]))
	fmt.Println("Общих элементов:", best)
}
