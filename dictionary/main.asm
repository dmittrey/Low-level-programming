%include "lib.inc"
extern find_word

global _start

section .rodata

%include "colon.inc"
%include "words.inc"

overflow_text: db "->Length of string is longer than buffer", 0
not_found_text: db "->There is no such string in the dictionary as this", 0
found_text: db "->Value: ", 0

section .text

_start:	
	sub rsp, 256	    	; Space for key buffer
		
	mov rdi, rsp
	mov rsi, 256
	call read_expression	; Read key in buffer

	test rax, rax
	jz .overflow_err	    ; If buffer is ended

	mov rsi, rsp
	mov rdi, dict_start
	call find_word		    ; Find key in dict

	add rsp, 256            ; Restore rsp state	
		
	test rax, rax
	jz .not_found		    ; Test for operation fail status

.found:
    push rax
	mov rdi, found_text
	call print_string	    ; Print message "Value: "
    pop rdi
    call print_string       ; Print node value
    call print_newline	    ; Print new line for UI

	call exit

.not_found:
	mov rdi, not_found_text
	call print_string	    ; Print error msg
	call print_newline	    ; Print new line for UI

	call exit

.overflow_err:
    add rsp, 256            ; Restore rsp state

	mov rdi, overflow_text
	call print_string	    ; Print error msg
	call print_newline	    ; Print new line for UI

	call exit
