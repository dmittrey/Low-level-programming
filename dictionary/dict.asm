%include "lib.inc"

global find_word

;In:
;	rsi - Null-term... string link
;	rdi - Dictionary start link
;Out:
;	rax - Entry string link or 0(failed status)
find_word:	
	; In nodes:
	; 1) next node link
	; 2) node key
	; 3) node value

.loop:
    push rdi
	push rsi

    add rdi, 8          ; Jmp to key address
	
    call string_equals  ; Compare keys

	test rax, rax		; Test for 0 in rax(not equals)
	jnz .found
	
    pop rsi
    pop rdi
	mov rdi, [rdi]      ; Jump to next node
	test rdi, rdi		; Check if next node exist
	jz .end
	jmp .loop

.end:
	xor rax, rax
	ret

.found:
    pop rdi             ; Put key in rdi

    call string_length  ; Calculate key size without Null-terminator

    pop rdi             ; Convention
    add rax, rdi
    add rax, 9          ; Jump to node value
    ret
