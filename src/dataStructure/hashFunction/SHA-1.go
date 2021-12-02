package hashFunction

import (
	"bytes"
	"encoding/binary"
)

var (
	H0            uint32 = 0x67452301
	H1            uint32 = 0xEFCDAB89
	H2            uint32 = 0x98BADCFE
	H3            uint32 = 0x10325476
	H4            uint32 = 0xC3D2E1F0
	A, B, C, D, E uint32
	H             [5]uint32
	W             [80]uint32
	TEMP          uint32
)

func K(t int) uint32 {
	if 0 <= t && t <= 19 {
		return 0x5A827999
	} else if 20 <= t && t <= 39 {
		return 0x6ED9EBA1
	} else if 40 <= t && t <= 59 {
		return 0x8F1BBCDC
	} else {
		return 0xCA62C1D6
	}
}

func F(t int, B uint32, C uint32, D uint32) uint32 {
	if 0 <= t && t <= 19 {
		return (B & C) | ((^B) & D)
	} else if 20 <= t && t <= 39 {
		return B ^ C ^ D
	} else if 40 <= t && t <= 59 {
		return (B & C) | (B & D) | (C & D)
	} else {
		return B ^ C ^ D
	}
}

func initial() {
	A = 0x00000000
	B = 0x00000000
	C = 0x00000000
	D = 0x00000000
	E = 0x00000000
	H[0] = H0
	H[1] = H1
	H[2] = H2
	H[3] = H3
	H[4] = H4
	TEMP = 0x00000000
	for i := 0; i < 80; i++ {
		W[i] = 0x00000000
	}
}

func Compute(M []byte) []byte {
	initial()
	for i := 0; i < 64; i = i + 4 {
		bytesBuffer := bytes.NewBuffer(M[i : i+3])
		tmp := uint32(0)
		binary.Read(bytesBuffer, binary.BigEndian, &tmp)
		W[i/4] = uint32(tmp)
	}
	for i := 16; i < 80; i++ {
		tmp := W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16]
		W[i] = (tmp << 1) | (tmp >> (32 - 1))
	}
	A = H[0]
	B = H[1]
	C = H[2]
	D = H[3]
	E = H[4]
	for i := 0; i < 80; i++ {
		TEMP = ((A << 5) | (A >> (32 - 5))) + F(i, B, C, D) + E + W[i] + K(i)
		E = D
		D = C
		C = ((B << 30) | (B >> (32 - 30)))
		B = A
		A = TEMP
	}
	H[0] = H[0] + A
	H[1] = H[1] + B
	H[2] = H[2] + C
	H[3] = H[3] + D
	H[4] = H[4] + E
	tmp := H[0]
	bytesBuffer := bytes.NewBuffer([]byte{})
	binary.Write(bytesBuffer, binary.BigEndian, &tmp)
	ans := bytesBuffer.Bytes()
	for i := 1; i <= 4; i++ {
		tmp = H[i]
		bytesBuffer = bytes.NewBuffer([]byte{})
		binary.Write(bytesBuffer, binary.BigEndian, &tmp)
		ans = append(ans, bytesBuffer.Bytes()...)
	}
	return ans
}

func Complement(s string) []byte {
	ans := []byte(s)
	if len(ans) == 56 {
		tmp := uint64((1 << 15) + len(ans))
		bytesBuffer := bytes.NewBuffer([]byte{})
		binary.Write(bytesBuffer, binary.BigEndian, &tmp)
		ans = append(ans, 0x1)
		ans = append(ans, bytesBuffer.Bytes()...)
	} else {
		tmp := uint64(len(ans))
		bytesBuffer := bytes.NewBuffer([]byte{})
		binary.Write(bytesBuffer, binary.BigEndian, &tmp)
		ans = append(ans, 0x1)
		for {
			ans = append(ans, 0x0)
			if len(ans) == 56 {
				break
			}
		}
		ans = append(ans, bytesBuffer.Bytes()...)
	}
	return ans
}
