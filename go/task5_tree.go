package main

import (
	"fmt"
	"strings"
)


type TreeNode struct {
	value int
	left  *TreeNode
	right *TreeNode
}

type BSTree struct {
	root *TreeNode
}

func (tree *BSTree) insert(value int) {
	if tree.root == nil {
		tree.root = &TreeNode{value: value}
		return
	}
	current := tree.root
	for {
		if value == current.value {
			return
		}
		if value < current.value {
			if current.left == nil {
				current.left = &TreeNode{value: value}
				return
			}
			current = current.left
		} else {
			if current.right == nil {
				current.right = &TreeNode{value: value}
				return
			}
			current = current.right
		}
	}
}

func (tree *BSTree) rightView() []int {
	var result []int
	if tree.root == nil {
		return result
	}
	queue := []*TreeNode{tree.root}
	for len(queue) > 0 {
		levelSize := len(queue)
		for i := 0; i < levelSize; i++ {
			current := queue[0]
			queue[0] = nil
			queue = queue[1:]
			if i == levelSize-1 {
				result = append(result, current.value)
			}
			if current.left != nil {
				queue = append(queue, current.left)
			}
			if current.right != nil {
				queue = append(queue, current.right)
			}
		}
	}
	return result
}

func runTreeRightEdge() {
	fmt.Print("Введите числа дерева (0 — конец): ")
	var tree BSTree
	for _, word := range strings.Fields(readLine()) {
		value, ok := parseInt32(word)
		if !ok {
			continue
		}
		if value == 0 {
			break
		}
		tree.insert(value)
	}
	fmt.Print("Правый край:")
	for _, value := range tree.rightView() {
		fmt.Print(" ", value)
	}
	fmt.Println()
}
