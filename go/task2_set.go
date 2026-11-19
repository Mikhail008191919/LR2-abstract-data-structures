package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

// Задание 2. Множество на хеш-таблице с цепочками.
type IntSet struct {
	buckets []SinglyList
	count   int
}

func newIntSet() *IntSet {
	return &IntSet{buckets: make([]SinglyList, 101)}
}

func normalizedIndex(value, capacity int) int {
	index := value % capacity
	if index < 0 {
		index += capacity
	}
	return index
}

func (set *IntSet) has(value int) bool {
	return set.buckets[normalizedIndex(value, len(set.buckets))].contains(value)
}

func (set *IntSet) add(value int) bool {
	if set.has(value) {
		return false
	}
	set.buckets[normalizedIndex(value, len(set.buckets))].pushFront(value)
	set.count++
	if set.count > len(set.buckets)*2 {
		set.grow()
	}
	return true
}

func (set *IntSet) del(value int) bool {
	if !set.buckets[normalizedIndex(value, len(set.buckets))].removeValue(value) {
		return false
	}
	set.count--
	return true
}

func (set *IntSet) grow() {
	newBuckets := make([]SinglyList, len(set.buckets)*2+1)
	for i := range set.buckets {
		set.buckets[i].forEach(func(value int) {
			index := normalizedIndex(value, len(newBuckets))
			newBuckets[index].pushFront(value)
		})
	}
	set.buckets = newBuckets
}

func (set *IntSet) values() []int {
	result := make([]int, 0, set.count)
	for i := range set.buckets {
		set.buckets[i].forEach(func(value int) {
			result = append(result, value)
		})
	}
	sort.Ints(result)
	return result
}

func formatSet(set *IntSet) string {
	items := set.values()
	parts := make([]string, len(items))
	for i, item := range items {
		parts[i] = strconv.Itoa(item)
	}
	return "{" + strings.Join(parts, ", ") + "}"
}

func runSetADT() {
	set := newIntSet()
	fmt.Println("SETADD x, SETDEL x, SET_AT x, PRINT, EXIT")
	for {
		fmt.Print("SET> ")
		command := strings.Fields(readLine())
		if inputEnded && len(command) == 0 {
			return
		}
		if len(command) == 0 {
			continue
		}
		name := strings.ToUpper(command[0])
		if name == "EXIT" {
			return
		}
		if name == "PRINT" {
			fmt.Println(formatSet(set))
			continue
		}
		if len(command) != 2 {
			fmt.Println("Нужно целое число")
			continue
		}
		key, ok := parseInt32(command[1])
		if !ok {
			fmt.Println("Нужно целое число")
			continue
		}
		switch name {
		case "SETADD":
			fmt.Println(strings.ToUpper(strconv.FormatBool(set.add(key))))
		case "SETDEL":
			fmt.Println(strings.ToUpper(strconv.FormatBool(set.del(key))))
		case "SET_AT":
			fmt.Println(strings.ToUpper(strconv.FormatBool(set.has(key))))
		default:
			fmt.Println("Неизвестная команда")
		}
	}
}

func fileQuery(args []string) bool {
	if len(args) != 4 || args[0] != "--file" || args[2] != "--query" {
		return false
	}
	command := strings.Fields(args[3])
	if len(command) != 2 {
		fmt.Fprintln(os.Stderr, "Формат: SETADD/SETDEL/SET_AT целое_число")
		return true
	}
	key, ok := parseInt32(command[1])
	if !ok {
		fmt.Fprintln(os.Stderr, "Формат: SETADD/SETDEL/SET_AT целое_число")
		return true
	}
	set := newIntSet()
	if content, err := os.ReadFile(args[1]); err == nil {
		for _, word := range strings.Fields(string(content)) {
			if saved, valid := parseInt32(word); valid {
				set.add(saved)
			}
		}
	}
	changed := false
	switch strings.ToUpper(command[0]) {
	case "SETADD":
		changed = set.add(key)
		fmt.Println(strings.ToUpper(strconv.FormatBool(changed)))
	case "SETDEL":
		changed = set.del(key)
		fmt.Println(strings.ToUpper(strconv.FormatBool(changed)))
	case "SET_AT":
		fmt.Println(strings.ToUpper(strconv.FormatBool(set.has(key))))
	default:
		fmt.Fprintln(os.Stderr, "Неизвестная команда")
		return true
	}
	if changed {
		var result strings.Builder
		for _, value := range set.values() {
			fmt.Fprintln(&result, value)
		}
		if err := os.WriteFile(args[1], []byte(result.String()), 0644); err != nil {
			fmt.Fprintln(os.Stderr, "Не удалось записать файл:", err)
		}
	}
	return true
}
