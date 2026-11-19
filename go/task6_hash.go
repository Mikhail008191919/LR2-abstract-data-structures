package main

import (
	"fmt"
	"strings"
	"time"
)


func magnitude(number int64) uint64 {
	if number >= 0 {
		return uint64(number)
	}
	return uint64(-(number + 1)) + 1
}

func fold(number int64) uint64 {
	rest := magnitude(number)
	var sum uint64
	for {
		sum += rest % 1000
		rest /= 1000
		if rest == 0 {
			return sum
		}
	}
}

type ChainTable struct {
	buckets     []SinglyList
	comparisons uint64
}

func newChainTable(capacity int) *ChainTable {
	return &ChainTable{buckets: make([]SinglyList, capacity)}
}

func (table *ChainTable) has(key int) bool {
	index := int(fold(int64(key)) % uint64(len(table.buckets)))
	return table.buckets[index].contains(key)
}

func (table *ChainTable) add(key int) {
	index := int(fold(int64(key)) % uint64(len(table.buckets)))
	found := false
	table.buckets[index].forEach(func(current int) {
		table.comparisons++
		if current == key {
			found = true
		}
	})
	if !found {
		table.buckets[index].pushFront(key)
	}
}

type Cell struct {
	key  int64
	used bool
}

type Cuckoo struct {
	first         []Cell
	second        []Cell
	keys          []int64
	displacements uint64
	rebuilds      uint64
}

func newCuckoo(capacity int) *Cuckoo {
	if capacity < 3 {
		capacity = 3
	}
	return &Cuckoo{first: make([]Cell, capacity), second: make([]Cell, capacity)}
}

func (table *Cuckoo) hash1(key int64) int {
	return int(fold(key) % uint64(len(table.first)))
}

func (table *Cuckoo) hash2(key int64) int {
	number := uint64(key)
	return int((number*1315423911 + (number >> 7) + 7) % uint64(len(table.second)))
}

func (table *Cuckoo) has(key int64) bool {
	a := table.first[table.hash1(key)]
	b := table.second[table.hash2(key)]
	return (a.used && a.key == key) || (b.used && b.key == key)
}

func (table *Cuckoo) place(key int64) bool {
	useFirst := true
	for attempt := 0; attempt < 2*len(table.first); attempt++ {
		var cell *Cell
		if useFirst {
			cell = &table.first[table.hash1(key)]
		} else {
			cell = &table.second[table.hash2(key)]
		}
		if !cell.used {
			cell.key = key
			cell.used = true
			return true
		}
		if cell.key == key {
			return true
		}
		key, cell.key = cell.key, key
		table.displacements++
		useFirst = !useFirst
	}
	return false
}

func (table *Cuckoo) rebuild() error {
	nextSize := len(table.first)*2 + 1
	for {
		if nextSize > 2_000_001 {
			return fmt.Errorf("не удалось перестроить хеш-таблицу")
		}
		table.rebuilds++
		table.first = make([]Cell, nextSize)
		table.second = make([]Cell, nextSize)
		success := true
		for _, key := range table.keys {
			if !table.place(key) {
				success = false
				break
			}
		}
		if success {
			return nil
		}
		nextSize = nextSize*2 + 1
	}
}

func (table *Cuckoo) add(key int64) error {
	if table.has(key) {
		return nil
	}
	table.keys = append(table.keys, key)
	if !table.place(key) {
		return table.rebuild()
	}
	return nil
}

func runHashTable() {
	fmt.Print("Размер таблицы: ")
	size := readNum()
	if size < 3 || size > 100000 {
		fmt.Println("Неверный размер")
		return
	}
	fmt.Print("Ключи через пробел (32-битные целые): ")
	var keys []int
	for _, word := range strings.Fields(readLine()) {
		if key, ok := parseInt32(word); ok {
			keys = append(keys, key)
		}
	}
	chain := newChainTable(int(size))
	cuckoo := newCuckoo(int(size))
	started := time.Now()
	for _, key := range keys {
		chain.add(key)
	}
	chainTime := time.Since(started).Nanoseconds()
	started = time.Now()
	for _, key := range keys {
		if err := cuckoo.add(int64(key)); err != nil {
			fmt.Println("Ошибка хеширования:", err)
			return
		}
	}
	cuckooTime := time.Since(started).Nanoseconds()
	fmt.Printf("Цепочки: сравнений %d, время %d нс\n", chain.comparisons, chainTime)
	fmt.Printf("Кукушка: вытеснений %d, перестроений %d, время %d нс\n", cuckoo.displacements, cuckoo.rebuilds, cuckooTime)
	for _, key := range keys {
		if !chain.has(key) || !cuckoo.has(int64(key)) {
			fmt.Println("ОШИБКА поиска:", key)
		}
	}
}
