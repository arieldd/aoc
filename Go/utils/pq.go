package utils

import "container/heap"

type PQ[T any] []pqi[T]

func (q *PQ[T]) Init() {
	heap.Init(q)
}

func (q *PQ[T]) TPop() T {
	return heap.Pop(q).(pqi[T]).v
}

func (q *PQ[T]) TPush(x T, priority int) {
	heap.Push(q, pqi[T]{v: x, p: priority})
}

// Pop implements heap.Interface.
func (q *PQ[T]) Pop() any {
	old := *q
	n := len(old)
	item := old[n-1]
	*q = old[:n-1]
	return item
}

// Push implements heap.Interface.
func (q *PQ[T]) Push(x any) {
	*q = append(*q, x.(pqi[T]))
}

func (q PQ[_]) Len() int           { return len(q) }
func (q PQ[_]) Less(i, j int) bool { return q[i].p < q[j].p }
func (q PQ[_]) Swap(i, j int)      { q[i], q[j] = q[j], q[i] }

type pqi[T any] struct {
	v T
	p int
}
