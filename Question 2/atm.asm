; A 32-bit x86 Assembly ATM program for Linux (NASM)
; Uses pure Linux syscalls for I/O

section .data
    balance     dd 1000         ; Initial balance
    
    ; Menu and Prompts
    menu        db "ATM Menu:", 0xA
                db "1. Deposit", 0xA
                db "2. Withdraw", 0xA
                db "3. Check Balance", 0xA
                db "4. Exit", 0xA
                db "Enter choice: ", 0
    menu_len    equ $ - menu

    prompt_amt  db "Enter amount: ", 0
    prompt_len  equ $ - prompt_amt

    msg_bal     db "Current balance: ", 0
    msg_bal_len equ $ - msg_bal

    msg_err     db "Error: Insufficient funds.", 0xA, 0
    msg_err_len equ $ - msg_err

    msg_bye     db "Goodbye!", 0xA, 0
    msg_bye_len equ $ - msg_bye

    newline     db 0xA
    
section .bss
    buffer      resb 10         ; Buffer for user input (numbers)
    num_buffer  resb 11         ; Buffer for printing numbers (itoa)

section .text
    global _start

; --- Helper Functions ---

; print_string(ecx: string, edx: length)
print_string:
    mov eax, 4          ; sys_write
    mov ebx, 1          ; stdout
    int 0x80            ; kernel interrupt
    ret

; print_string_z(ecx: null-terminated string)
; Requires a helper to get length
print_string_z:
    push ebx
    mov ebx, ecx
    .len_loop:
        cmp byte [ebx], 0
        je .found_len
        inc ebx
        jmp .len_loop
    .found_len:
        sub ebx, ecx    ; ebx now holds length
        mov edx, ebx
        call print_string
    pop ebx
    ret

; read_input(ecx: buffer, edx: max_length)
read_input:
    mov eax, 3          ; sys_read
    mov ebx, 0          ; stdin
    int 0x80
    ; eax contains length read. We need to null-terminate it.
    dec eax             ; Remove newline
    mov byte [ecx + eax], 0 ; Add null terminator
    ret

; read_int() -> returns in eax
read_int:
    push ebx
    push ecx
    push edx
    
    ; Read string from user
    mov ecx, buffer
    mov edx, 10
    call read_input
    
    ; Convert ASCII to int (atoi)
    mov eax, 0          ; Accumulator
    mov ecx, buffer     ; String pointer
    .atoi_loop:
        movzx ebx, byte [ecx]
        cmp ebx, 0
        je .atoi_done   ; End of string
        cmp ebx, '0'
        jl .atoi_done   ; Not a digit
        cmp ebx, '9'
        jg .atoi_done   ; Not a digit

        sub ebx, '0'    ; Convert char '0'-'9' to int 0-9
        imul eax, 10    ; Multiply accumulator by 10
        add eax, ebx    ; Add new digit
        inc ecx
        jmp .atoi_loop
    .atoi_done:
    
    pop edx
    pop ecx
    pop ebx
    ret

; print_int(eax: number to print)
print_int:
    push eax
    push ebx
    push ecx
    push edx
    
    mov ecx, num_buffer + 10 ; Point to end of buffer
    mov byte [ecx], 0xA    ; Add newline
    dec ecx
    
    mov ebx, 10         ; Divisor
    
    ; Handle case for 0
    cmp eax, 0
    jne .itoa_loop
    mov byte [ecx], '0'
    jmp .itoa_print
    
    .itoa_loop:
        xor edx, edx        ; Clear edx for division
        div ebx             ; eax = eax / 10, edx = eax % 10
        add dl, '0'         ; Convert remainder (0-9) to char ('0'-'9')
        mov [ecx], dl       ; Store char in buffer
        dec ecx
        cmp eax, 0          ; Is quotient zero?
        jne .itoa_loop      ; If not, loop
        
    .itoa_print:
        inc ecx             ; Point to first char
        
        ; Calculate length
        push ecx            ; Save start pointer
        mov ebx, num_buffer + 11
        sub ebx, ecx
        mov edx, ebx        ; edx = length
        pop ecx             ; ecx = string
        
        call print_string
    
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret
    
; --- ATM Functions ---

proc_deposit:
    mov ecx, prompt_amt
    call print_string_z
    
    call read_int       ; Amount is in eax
    add [balance], eax  ; Add to balance
    ret

proc_withdraw:
    mov ecx, prompt_amt
    call print_string_z
    
    call read_int       ; Amount to withdraw is in eax
    
    mov ebx, [balance]
    cmp ebx, eax        ; Compare balance >= amount
    jl .insufficient    ; Jump if less
    
    sub [balance], eax  ; Subtract from balance
    ret
    
    .insufficient:
        mov ecx, msg_err
        call print_string_z
        ret

proc_check_balance:
    mov ecx, msg_bal
    call print_string_z
    
    mov eax, [balance]
    call print_int      ; print_int will handle newline
    ret

proc_exit:
    mov ecx, msg_bye
    call print_string_z
    
    mov eax, 1          ; sys_exit
    mov ebx, 0          ; exit code 0
    int 0x80

; --- Main Program ---

_start:
    ; Main loop
.main_loop:
    ; Print menu
    mov ecx, menu
    mov edx, menu_len
    call print_string
    
    ; Read choice
    mov ecx, buffer
    mov edx, 3          ; Max 2 chars + newline
    call read_input
    
    ; Compare choice
    cmp byte [buffer], '1'
    je .do_deposit
    
    cmp byte [buffer], '2'
    je .do_withdraw
    
    cmp byte [buffer], '3'
    je .do_balance
    
    cmp byte [buffer], '4'
    je .do_exit
    
    ; Invalid choice, just loop again
    jmp .main_loop

.do_deposit:
    call proc_deposit
    jmp .main_loop
    
.do_withdraw:
    call proc_withdraw
    jmp .main_loop
    
.do_balance:
    call proc_check_balance
    jmp .main_loop

.do_exit:
    call proc_exit
    