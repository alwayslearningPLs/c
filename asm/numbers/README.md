# Numbers

## Binary numbers

The least significant bit is 0 (the 1 at the end) and the most significant bit is 7 (the 1 at the beginning).

`175 = 10101111`

`741 = 00000010 11100101`

## Two complement

In signed numbers, if the first bit is 1, then the number is negative:

- `01111111` -> 127         -> 0x7f
- `10000000` -> 128 (-128)  -> 0x80

Let's convert -1 to its two's complement format:

- Sign bit:                     `10000000`
- Absolute value of -1 is:      ` 0000001`
- Complement of 1 is:           ` 1111110`
- Adding one to the complement: ` 1111111`
- Adding the sign bit with last:`11111111`

Let's convert -59 to its two's complement format:

- Sign bit:                     `10000000`
- Absolute value of 59 is:      ` 0111011`
- Complement of 59 is:          ` 1000100`
- Add one to it:                ` 1000101`
- Adding the Sign bit:          `11000101` -> -59 -> 197

Let's convert -750 to its two's complement format:

- Sign bit:                     `10000000 00000000`
- Absolute value of 750:        ` 0000010 11101110`
- Two complement:               ` 1111101 00010001`
- Add one to the complement:    ` 1111101 00010010`
- Add the sign bit:             `11111101 00010010` -> -750 -> 64786

## The floating point numbers

The x86-64 architecture supports 3 different varieties of floating point numbers: 32 bit, 64 bit and 80 bit numbers.

| variety | bits | Exponent | eponent bias | fraction | Precision |
| ------- | ---- | -------- | ------------ | -------- | --------- |
| float | 32 | 8 | 127 | 23 | ~7 digits |
| double | 64 | 11 | 1023 | 52 | ~16 digits |
| long double | 80 | 15 | 16383 | 64 | 19 digits |

Float type:

- First bit is the sign bit.
- The first 7 bits are the exponent segment.
- The last 23 bits are the fraction segment.

## Segment data types

word = 16-bit unsigned integer

- `db` -> data byte (8 bits)
- `dw` -> data word (16 bits)
- `dd` -> data double word (32 bits)
- `dq` -> data quad-word (64 bits)

## Reference

- [word meaning](http://www.asmcommunity.net/forums/topic/?id=30592)
- [pointers in c](https://stackoverflow.com/questions/24805673/declare-and-initialize-pointer-concisely-i-e-pointer-to-int)
- [endianess in c](https://stackoverflow.com/questions/1024951/does-my-amd-based-machine-use-little-endian-or-big-endian)
- [floating point](https://softwareengineering.stackexchange.com/questions/215065/can-anyone-explain-representation-of-float-in-memory)
- [another floating point](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html)
