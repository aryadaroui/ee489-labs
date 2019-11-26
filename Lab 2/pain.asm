
; .data containing intitialized variables
; .bss containing unintitialized data
; .text containing program instructions

		.data
x	.word	0f32h
y	.word	0x1110
b1	.int	1011101b

		.bss	buffer1, 10

		.text
	.def	_Mult32bit

_Mult32bit:

	mov	#0, #ACO
	amov #x, XAR0
	ret

.end

