package main

import (
	"fmt"

	"./hashFunction"
)

func main() {
	s := "Hello World!"
	fmt.Println(hashFunction.Compute(hashFunction.Complement(s)))
}
