package hashFunction

import (
	"hash/crc32"
	"sort"
	"strconv"
)

type Hash func(data []byte) uint32

type Map struct {
	hash    Hash
	copies  int
	keys    []int
	hashMap map[int]string
}

func New(copy int, f Hash) *Map {
	m := &Map{
		copies:  copy,
		hash:    f,
		hashMap: make(map[int]string),
	}
	if m.hash == nil {
		m.hash = crc32.ChecksumIEEE
	}
	return m
}

func (m *Map) IsEmpty() bool {
	return len(m.keys) == 0
}

func (m *Map) Add(keys ...string) {
	for _, key := range keys {
		for i := 0; i < m.copies; i++ {
			hash := int(m.hash([]byte(strconv.Itoa(i) + key)))
			m.keys = append(m.keys, hash)
			m.hashMap[hash] = key
		}
	}
	sort.Ints(m.keys)
}

func (m *Map) Get(key string) string {
	if m.IsEmpty() {
		return ""
	}
	hash := int(m.hash([]byte(key)))
	index := sort.Search(len(m.keys), func(i int) bool { return m.keys[i] > hash })
	if index == len(m.keys) {
		index = 0
	}
	return m.hashMap[m.keys[index]]
}
