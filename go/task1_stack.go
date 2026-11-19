package main

import (
	"fmt"
	"strings"
)


func fixParentheses(text string) string {
	chars := []rune(text)
	stack := make([]int, 0)
	remove := make([]bool, len(chars))
	for i, ch := range chars {
		switch ch {
		case '(':
			stack = append(stack, i)
		case ')':
			if len(stack) == 0 {
				remove[i] = true
			} else {
				stack = stack[:len(stack)-1]
			}
		}
	}
	for _, index := range stack {
		remove[index] = true
	}
	var result strings.Builder
	for i, ch := range chars {
		if !remove[i] {
			result.WriteRune(ch)
		}
	}
	return result.String()
}

func runStackParentheses() {
	fmt.Print("Введите строку: ")
	fmt.Println("Результат:", fixParentheses(readLine()))
}
