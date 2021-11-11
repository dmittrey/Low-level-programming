	; %1 - null-term... key word
	
	; %2 - mark name

%macro colon 2

%2:
	db %1, 0		; Null-term... string key word
	%ifdef dict_start
		dq dict_start
	%else 
		dq 0
	%endif			; next node link

	%define dict_start %2	; start of dictionary update
%endmacro
	
