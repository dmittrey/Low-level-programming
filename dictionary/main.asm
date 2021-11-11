%include "lib.inc"
extern find_word

global _start

section .data

%include "colon.inc"
%include "words.inc"

buff: times 256 db 0
overflow_text: db "->Length of string is longer than buffer", 0
not_found_text: db "->There is no such string in the dictionary as this", 0
found_text: db "->Value: ", 0

section .text

_start:	
	push r12
	push r13		; Save callee-saved registers
	
	mov r13, rsp
	sub rsp, 256		; Space for key buffer
		
	mov rdi, rsp
	mov rsi, 256
	call read_expression	; Read key in buffer

	test rax, rax
	jz .overflow_err	; If buffer is ended

	mov rsi, rsp
	mov rdi, dict_start
	call find_word		; Find key in dict

	mov rsp, r13            ; Restore rsp state	
		
	test rax, rax
	jz .not_found		; Test for operation fail status

.found:
	mov r12, rax		; Start from fetched node
	
	mov rdi, found_text
	call print_string	; Print message "Value: "

	mov rdi, r12
	call string_length	; Calculate key length in node

	add r12, rax
	add r12, 9		; Put in r12 value address
     	
	mov rdi, r12 
        call print_string	; Print value

        call print_newline	; Print new line for UI

	pop r13
	pop r12			; Restore calee-saved registers
	call exit

.not_found:
	mov rdi, not_found_text
	call print_string	; Print error msg

	call print_newline	; Print new line for UI

	pop r13
	pop r12			; Restore callee-saved registers
	call exit

.overflow_err:
	mov rsp, r13            ; Restore rsp state

	mov rdi, overflow_text
	call print_string	; Print error msg
	
	call print_newline	; Print new line for UI

	pop r13
	pop r12			; Restore callee-saved registers
	call exit
