package main

import (
	"fmt"
	"strings"
)


type LFUCache struct {
	keys        []int
	values      []int
	frequencies []int
	lastUsed    []int64
	capacity    int
	clock       int64
}

func newLFUCache(capacity int) *LFUCache {
	return &LFUCache{capacity: capacity}
}

func (cache *LFUCache) find(key int) int {
	for i, current := range cache.keys {
		if current == key {
			return i
		}
	}
	return -1
}

func (cache *LFUCache) removeLeastUsed() {
	victim := 0
	minFrequency := cache.frequencies[0]
	oldestTime := cache.lastUsed[0]
	for i := 1; i < len(cache.keys); i++ {
		frequency := cache.frequencies[i]
		last := cache.lastUsed[i]
		if frequency < minFrequency || (frequency == minFrequency && last < oldestTime) {
			victim, minFrequency, oldestTime = i, frequency, last
		}
	}
	cache.keys = append(cache.keys[:victim], cache.keys[victim+1:]...)
	cache.values = append(cache.values[:victim], cache.values[victim+1:]...)
	cache.frequencies = append(cache.frequencies[:victim], cache.frequencies[victim+1:]...)
	cache.lastUsed = append(cache.lastUsed[:victim], cache.lastUsed[victim+1:]...)
}

func (cache *LFUCache) get(key int) int {
	position := cache.find(key)
	if position == -1 {
		return -1
	}
	value := cache.values[position]
	cache.frequencies[position]++
	cache.clock++
	cache.lastUsed[position] = cache.clock
	return value
}

func (cache *LFUCache) set(key, value int) {
	if cache.capacity <= 0 {
		return
	}
	position := cache.find(key)
	if position != -1 {
		cache.values[position] = value
		cache.frequencies[position]++
		cache.clock++
		cache.lastUsed[position] = cache.clock
		return
	}
	if len(cache.keys) == cache.capacity {
		cache.removeLeastUsed()
	}
	cache.clock++
	cache.keys = append(cache.keys, key)
	cache.values = append(cache.values, value)
	cache.frequencies = append(cache.frequencies, 1)
	cache.lastUsed = append(cache.lastUsed, cache.clock)
}

func runLFUCache() {
	fmt.Print("Ёмкость кэша: ")
	capacity := readNum()
	fmt.Print("Количество команд: ")
	commands := readNum()
	if capacity < 0 || capacity > 100000 || commands < 0 || commands > 100000 {
		fmt.Println("Неверные параметры")
		return
	}
	cache := newLFUCache(int(capacity))
	fmt.Println("Команды: SET ключ значение / GET ключ")
	for i := int64(0); i < commands; {
		cmd := strings.Fields(readLine())
		if inputEnded && len(cmd) == 0 {
			return
		}
		if len(cmd) == 0 {
			continue
		}
		name := strings.ToUpper(cmd[0])
		valid := (name == "GET" && len(cmd) == 2) || (name == "SET" && len(cmd) == 3)
		var key, value int
		if valid {
			key, valid = parseInt32(cmd[1])
		}
		if valid && name == "SET" {
			value, valid = parseInt32(cmd[2])
		}
		if !valid {
			fmt.Println("Неверная команда")
			continue
		}
		if name == "GET" {
			fmt.Println(cache.get(key))
		} else {
			cache.set(key, value)
		}
		i++
	}
}
