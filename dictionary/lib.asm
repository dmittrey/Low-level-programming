section .text

global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global print_err
global string_equals
global read_char
global read_word
global read_expression
global parse_uint
global parse_int
global string_copy
 
;Params:
;	rdi: Return status
;Out:
;	!exit
exit: 
	mov rax, 60		; Sys_exit code
	syscall 

;Params:
;	rdi: String link
;Out:
;	rax: String length
string_length:
	xor rax, rax		; Put 0 in rax
.loop:
	cmp byte [rdi+rax], 0	; Compare next symbol with zero
	je .exit		; Exit if equals
	inc rax			; Increment counter else
	jmp .loop		; Return to start of loop
.exit:
	ret

;Params:
;	rdi: String link
;Out:
;	!stdout
print_string:
	push rdi		; Save caller-saved register
	call string_length	; Calculate string size
				; String size in rax
	pop rdi			; Restore caller-saved register
	mov rdx, rax		; String size in bytes
	mov rsi, rdi		; Char buffer
	xor rdi, rdi
	inc rdi			; Stdout descriptor(1)
	xor rax, rax
	inc rax			; Sys_write code(1)
	syscall
	ret

;Params:
;	rdi: Code of char
;Out:
;	!stdout
print_char:
	xor rax, rax
	inc rax			; Sys_write code(1)     
	xor rdx, rdx
	inc rdx			; Char size in bytes(1)
	push rdi		; Put char on the stack
	xor rdi, rdi
	inc rdi			; Stdout descriptor(1)
	mov rsi, rsp		; Put char link
	syscall
	pop rdi
	ret

;Params:
;	-
;Out:
;	!stdout
print_newline:
	mov rdi, 0xA		; New line ascii code
	call print_char		
	ret

;Params:
;	rdi: Unsigned 8-byte decimal number
;Out:
;	!stdout
print_uint:
	mov r9, 10          	; Base num system
	mov rax, rdi		; Put parametr in rax	    
	xor rcx, rcx		; Clear symbols counter
	
	dec rsp			
	inc rcx
	mov byte [rsp], 0
.loop:
	xor rdx, rdx        	; Clear rdx
	div r9              	; Divide on 10
	add rdx, '0'        	; Transfer to ascii symbol
	dec rsp
	inc rcx
	mov byte [rsp], dl   	; Put symbol in buffer
	test rax, rax       	; Check the end of operation
	jnz .loop		; Return if not zero
	
	mov rdi, rsp
	push rcx
	call print_string
	pop rcx
	add rsp, rcx
	ret
;Params:
;	rdi: Unsigned 8-byte decimal number
;Out:
;	!stdout
print_int:
	xor rdx, rdx
	cmp rdi, 0		; Check if num have sign
	jns .print_unsign	; Print if not have
	push rdi		; Save rdi value
	mov rdi, '-'
	call print_char		; Print minus char
	pop rdi			
	neg rdi			; Take unsign form of rdi

.print_unsign:
	call print_uint
	ret

;Params:
;	rdi: Error string link
;Out:
;	!stderr
print_err:
	push rdi		; Save caller-saved register
	call string_length	; Calculate string size
				; String size in rax
	pop rdi			; Restore caller-saved register
	mov rdx, rax		; String size in bytes
	mov rsi, rdi		; Char buffer
	xor rdi, rdi
	inc rdi
	inc rdi			; Stderr descriptor(2)
	xor rax, rax
	inc rax			; Sys_write code(1)
	syscall
	ret
	
;Params:
;	rdi: First string link
;	rsi: Second string link
;Out:
;	rax: Result
string_equals:
.loop:
	mov al, byte [rsi]	; Take symbol from second string
	cmp al, byte [rdi]	; Compare with symbol from first string
	jne .extra_save		; Extrasave if not equals
	cmp al, 0		; Check for null-terminator
	je .save		; Save if exist
	inc rsi			
	inc rdi			; Switch to next symbols
	jmp .loop		; Jump to start of loop
.extra_save:
	xor rax, rax		; Put 0 in rax
	ret
.save:
	xor rax, rax		; Put 1 in rax
	inc rax
	ret

;Params:
;	!stdin
;Out:
;	rax: 	Char from stdin
;		0 if the end of the stream is reached
read_char:
	push 0			; 0 if end of stream
	xor rdx, rdx
	inc rdx			; Count of symbols(1)
	mov rsi, rsp		; Char link in stack
	xor rdi, rdi		; Stdin descriptor(0)
	xor rax, rax		; Sys_read code(0)
	syscall
	pop rax			; Take rax from stack
 	ret 

;Params:
;	rdi: Buffer string link
;	rsi: Buffer string size   
;Out:
;	rax:	Filled buffer link
;		0 if operation failed
;	rdx: Filled buffer size	
read_word:
	push r12		
	push r13
	push r14		; Callee-saved register
	xor r12, r12		; Clear r12
	mov r13, rdi		; Put link in r13(r8)
	mov r14, rsi		; Put size in r14(r9)
	dec r14			; Space for null-terminator

.start_space:
	call read_char		; Read char
	cmp rax, 0x0
	je .save		; Save if end of the stream reached 
	cmp rax, 0x20
	je .start_space
	cmp rax, 0x09
	je .start_space
	cmp rax, 0xA
	je .start_space		; Checking for a whitespace character

.loop:
	cmp rax, 0x0
	je .save 
        cmp rax, 0x20
        je .save
        cmp rax, 0x09
        je .save
        cmp rax, 0xA
        je .save      		; Save if we take whitespace character or end of the stream

	cmp r12, r14
	jae .extra_save
	mov [r13 + r12], al	; Put symbol in buffer
	inc r12			; Increase counter
	call read_char 		; Read next symbol
	jmp .loop		; Return to start

.save:	
	mov rax, r13		; Filled bufer link
	mov rdx, r12		; Counter
	mov [r13 + r12], byte 0	; Null-terminator at the end
	pop r14			
	pop r13
	pop r12			; Callee-saved register save
	ret 

.extra_save:
	mov rax, 0		; Result of failed operation
	pop r14
        pop r13
        pop r12                 ; Callee-saved register save	
	ret

;Params:
;	rdi: Buffer string link
;	rsi: Buffer string size   
;Out:
;	rax:	Filled buffer link
;		0 if operation failed
;	rdx: Filled buffer size	
read_expression:
	push r12		
	push r13
	push r14		; Callee-saved register
	xor r12, r12		; Clear r12
	mov r13, rdi		; Put link in r13(r8)
	mov r14, rsi		; Put size in r14(r9)
	dec r14			; Space for null-terminator

.start_space:
	call read_char		; Read char
	cmp rax, 0x0
	je .save		; Save if end of the stream reached 
	cmp rax, 0x20
	je .start_space
	cmp rax, 0x09
	je .start_space
	cmp rax, 0xA
	je .start_space		; Checking for a whitespace character

.loop:
	cmp rax, 0x0
	je .save 
        cmp rax, 0x09
        je .save
        cmp rax, 0xA
        je .save      		; Save if we take whitespace character or end of the stream

	cmp r12, r14
	jae .extra_save
	mov [r13 + r12], al	; Put symbol in buffer
	inc r12			; Increase counter
	call read_char 		; Read next symbol
	jmp .loop		; Return to start

.save:	
	mov rax, r13		; Filled bufer link
	mov rdx, r12		; Counter
	mov [r13 + r12], byte 0	; Null-terminator at the end
	pop r14			
	pop r13
	pop r12			; Callee-saved register save
	ret 

.extra_save:
	mov rax, 0		; Result of failed operation
	pop r14
        pop r13
        pop r12                 ; Callee-saved register save	
	ret


;Params:
;   	rdi: String link
;Out:
;	rax:	Decimal number length
;	rdx: 	Decimal number length
;		0 if operation failed
parse_uint:
	mov r8, 0xA		; Base of number system
	xor rcx, rcx		; Clear rcx
	xor rax, rax		; Clear rax
.loop:    
	movzx r9, byte[rdi + rcx]	; Move with size extend
	cmp r9b, 0			; Save if string is end
 	je .save
 	cmp r9b, '0'
 	jb .save
	cmp r9b, '9'
	ja .save
	mul r8			; Shift 1 bit left
	sub r9b, '0'		; Transfer from ascii in dec numbers
	add rax, r9		; Add dec number in result
	inc rcx			; Counter increase
	jmp .loop		; Return to start
.save:
	mov rdx, rcx		; Save counter in rdx
	ret

;Params:
;	rdi: String link
;Out:
;	rdx: 	Number length
;		0 if operation failed
parse_int:
	xor rax, rax		; Put 0 in rax
	mov al, byte [rdi]	; Take next symbol
	inc rdi			; Shift pointer
	cmp al, '-'		; Sign check
	je .negative		; Sign_save if true
	dec rdi			; Decrease counter
	call parse_uint		; Parse uint number
	ret

.negative:
	call parse_uint		; Parse uint number
	neg rax			; Invert result
	inc rdx			; Keep sign in count
	ret 

;Params:
;	rdi: String link
;	rsi: Buffer string link
;	rdx: Buffer string size	
;Out:
;	rax:	String length
;		0 if buffer end reached
string_copy:
	push rdi
	push rsi
	push rdx		; Save caller-saved registers
	call string_length	; Calculate string length
	pop rdx
	pop rsi
	pop rdi			; Restore caller-saved registers
	inc rax			; Null-terminator space
	cmp rax, rdx		; Compare string and buffer length
	ja .extra_save		; Extra save if string longer
	xor rcx, rcx		; Clear rcx
.loop:
	mov dl, byte [rdi]	
	mov byte [rsi], dl	; Move symbol from string to buffer
        inc rdi
        inc rsi			; Increase pointers
        inc rcx			; Increase counter
	cmp rcx, rax		; Check buffer status
	jna .loop		; If not filled then continue
	dec rax			; String length without nul-terminator
	ret
.extra_save:
	mov rax, 0		; Put 0 in rax
	ret
