package main

// Односвязный список для корзин из заданий 2 и 6.
type ListNode struct {
	value int
	next  *ListNode
}

type SinglyList struct {
	head *ListNode
}

func (list *SinglyList) contains(value int) bool {
	for current := list.head; current != nil; current = current.next {
		if current.value == value {
			return true
		}
	}
	return false
}

func (list *SinglyList) pushFront(value int) {
	list.head = &ListNode{value: value, next: list.head}
}

func (list *SinglyList) removeValue(value int) bool {
	var previous *ListNode
	for current := list.head; current != nil; current = current.next {
		if current.value == value {
			if previous == nil {
				list.head = current.next
			} else {
				previous.next = current.next
			}
			return true
		}
		previous = current
	}
	return false
}

func (list *SinglyList) forEach(visit func(int)) {
	for current := list.head; current != nil; current = current.next {
		visit(current.value)
	}
}
