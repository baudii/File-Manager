.code
;---------------------------------------------------
Make_Sum proc
; extern "C" int Make_Sum(int a, int b);
; RCX - a
; RDX - b
; return: RAX
	
	push rax

	mov eax, ecx
	add eax, edx

	pop rax

	ret

Make_Sum endp
;---------------------------------------------------
Get_Pointer_Offset proc
; Set RDI to screen buffer pointer at {line.x, line.y}
; Params:
; RCX - screen_buffer
; RDX - (-)(Screen_Width)(Y)(X) - 2 bytes each
; return - RDI

	push rax
	push rbx
	
; 1. formula: memory_offset = (line.Y * line.Screen_Width + line.X) * 4

; 1.1 setup registers
	movzx rdi, dx ; RDI = line.X
	mov rbx, rdx
	shr rbx, 16 ; BX = line.Y
	movzx rax, bx ; RAX = line.Y
	shr rbx, 16 ; BX = line.Screen_Width

; 1.2 implement formula
	imul ax, bx ; RAX = line.Y * line.Screen_Width
	shr rbx, 16 ; BX = line.Len
	add rax, rdi ; RAX = line.Y * line.Screen_Width + line.X
	shl rax, 2 ; RAX = (line.Y * line.Screen_Width + line.X) * 4, 4 - size of CHAR_INFO

; 1.3 saving pointer offset to RDI
	mov rdi, rcx
	add rdi, rax

	pop rbx
	pop rax

	ret

Get_Pointer_Offset endp
;---------------------------------------------------
Get_Screen_Width proc
; Calculate the line offset in bytes
; Params:
; RCX - screen_buffer
; RDX - line_data or fill_data
; return - RBX

	mov rbx, rdx
	shr rbx, 32
	movzx rbx, bx
	shl rbx, 2 
	
	; RBX = BX = screen width in bytes
	
	ret

Get_Screen_Width endp
;---------------------------------------------------
Draw_Line_Horizontal proc
; extern "C" void Draw_Line_Horizontal(CHAR_INFO* screen_buffer, SLine_Data line, CHAR_INFO symbol_data);
; Params:
; RCX - screen_buffer
; RDX - line
; R8 - symbol_data
; return: none

	push rax
	push rcx
	push rdi
	push r10

	call Get_Pointer_Offset ; RDI = pointer offset

	; 1. Print first symbol
	mov eax, r8d
	mov r10, r8
	shr r10, 32
	mov ax, r10w
	stosd

	; 2. Print line
	mov eax, r8d
	mov rcx, rdx
	shr rcx, 48

	rep stosd

	; 3. Print last symbol
	shr r10, 16
	mov ax, r10w
	stosd

	pop r10
	pop rdi
	pop rcx
	pop rax

	ret

Draw_Line_Horizontal endp
;---------------------------------------------------
Draw_Line_Vertical proc
; extern "C" void Draw_Line_Vertical(CHAR_INFO *screen_buffer, SLine_Data line, SPanelSymbol symbol_data);
; Params:
; RCX - screen_buffer
; RDX - line
; R8 - symbol_data
; return: none

	push rax
	push rbx
	push rcx
	push rdi
	push r10

	call Get_Pointer_Offset ; RDI = pointer offset

	; 1. Setup parameters
	
	; 1.1 Calculate the line offset and put at RBX
	call Get_Screen_Width ; RBX = screen width in bytes
	sub rbx, 4 ; adjust offset for vertical lines
	
	; 1.2 Put line length at RCX
	mov rcx, rdx
	shr rcx, 48 ; CX = line.Len

	; 2. Print first symbol
	mov eax, r8d
	mov r10, r8
	shr r10, 32
	mov ax, r10w
	stosd
	add rdi, rbx

	; 3. Loop to print the line
	mov eax, r8d
_1:
	stosd
	add rdi, rbx

	loop _1

	; 4. Print last symbol
	shr r10, 16
	mov ax, r10w
	stosd

	pop r10
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret

Draw_Line_Vertical endp
;---------------------------------------------------
Fill_Area proc
; extern "C" void Fill_Area(CHAR_INFO * screen_buffer, SFill_Data fill_data, SPanelSymbol symbol_data);
; Params:
; RCX - screen_buffer
; RDX - fill_data
; R8 - symbol_data
; return: none

	push rax
	push rbx
	push rcx
	push rdi
	push r10
	push r11

	call Get_Pointer_Offset ; RDI = pointer offset


	; 1 Put fill width at RCX
	mov rcx, rdx
	shr rcx, 48 ; RCX = CX = { fill_data.Width, fill_data.Height }. CL = fill_data.Width, CH = fill_data.Height

	; 2. Setup screen width
	call Get_Screen_Width ; RBX = screen width in bytes

	; 2.1 Adjust for fill_data.Width
	movzx r10, cl
	shl r10, 2
	sub rbx, r10

	; 3. Loop to print the result
	; 3.1 Setup counters
	movzx r10, cx
	shr r10, 8 ; save CH, CH = i
	xor ch, ch ; remove CH from RCX
	mov r11, rcx ; save R11, R11 = j

	; 3.2 Loop body
	mov eax, r8d
_1:
	rep stosd

	add rdi, rbx
	mov ecx, r11d
	dec r10
	jnz _1

	pop r11
	pop r10
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret

Fill_Area endp
;---------------------------------------------------
Show_Colors proc
; extern "C" void Show_Colors(CHAR_INFO * screen_buffer, SLine_Data line, CHAR_INFO symbol);
; Params:
; RCX - screen_buffer
; RDX - line
; R8 - symbol
; return: none

	push rax
	push rbx
	push rcx
	push rdi
	push r8
	push r10

	call Get_Pointer_Offset ; RDI = pointer offset

	; Get screen width in bytes
	call Get_Screen_Width ; RBX = screen width in bytes
	sub rbx, 64

	and r8, 0ffffh

	; setup counters
	mov r10, 16

	xor rcx, rcx
_outer:
	mov cl, 16
_inner:
	mov eax, r8d
	add r8, 010000h
	stosd
	loop _inner

	add rdi, rbx

	dec r10
	jnz _outer

	pop r10
	pop r8
	pop rdi
	pop rcx
	pop rbx
	pop rax

	ret

Show_Colors endp
;---------------------------------------------------
Draw_Text proc
; extern "C" int Draw_Text(CHAR_INFO * screen_buffer, SText_Data text_data, const char* text);
; Params:
; RCX - screen_buffer
; RDX - text_data
; R8 - text
; return - RAX = string length

	push rbx
	push rdi
	push r8

	call Get_Pointer_Offset ; RDI = pointer offset
	
	mov rax, rdx
	shr rax, 32
	xor rbx, rbx

_1:
	mov ax, [ r8 ]
	add r8, 2

	cmp ax, 0
	je _exit

	inc rbx
	stosd

	jmp _1

_exit:
	mov rax, rbx

	pop r8
	pop rdi
	pop rbx

	ret

Draw_Text endp
;---------------------------------------------------
Draw_Limited_Text proc
; extern "C" void Draw_Limited_Text(CHAR_INFO * screen_buffer, SText_Data text_data, const wchar_t* text, unsigned short limit);
; Params:
; RCX - screen_buffer
; RDX - text_data
; R8 - text
; R9 - limit
; return - none

	push rax
	push rcx
	push rdi
	push r8
	push r9

	call Get_Pointer_Offset ; RDI = pointer offset
	
	mov rax, rdx
	shr rax, 32

_1:
	
	mov ax, [ r8 ]
	add r8, 2

	cmp ax, 0
	je _fill_spaces

	cmp r9, 0
	je _exit

	stosd
	dec r9

	jmp _1

_fill_spaces:
	mov rcx, r9
	mov ax, 020h ; 20 = hex value of ' ' character
	rep stosd

_exit:
	pop r9
	pop r8
	pop rdi
	pop rcx
	pop rax

	ret

Draw_Limited_Text endp
;---------------------------------------------------
end