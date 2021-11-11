%include "lib.inc"

global find_word

;In:
;	rsi - Null-term... string link
;	rdi - Dictionary start link
;Out:
;	rax - Entry string link or 0(failed status)
find_word:
	push r14	; Key length in node
	push r13	; Null-term... string link
	push r12	; Dictionary node link	
	; Save callee-saved registers
	
	; In nodes:
	; 1) node key
	; 2) next node link
	; 3) node value

	mov r13, rsi
	mov r12, rdi

.loop:	
	mov rdi, r12
	call string_length	; Find key length in node
	mov r14, rax
	
	mov rsi, r13
	mov rdi, r12
	call string_equals	; rdi and rsi decrements 
	test rax, rax		; Test for 0 in rax(not equals)
	jnz .found
	
	mov r12, [r12 + r14 + 1]
	test r12, r12		; Check if next node exist
	jz .end
	jmp .loop

.end:
	xor rax, rax
	pop r12
	pop r13
	pop r14
	ret

.found:
	mov rax, r12
	add rax, 9
	pop r12
	pop r13
	pop r14
	ret
	


