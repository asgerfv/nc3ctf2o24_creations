; https://www.muppetlabs.com/~breadbox/software/tiny/teensy.html
; nasm -f bin -o tiny.elf tiny.asm && ls -la tiny.elf && ndisasm -b 32 tiny.elf

BITS 32

                org     0x10000

file_start:
                db      0x7F, "ELF"             ; e_ident
                dd      1                                       ; p_type
                dd      0                                       ; p_offset
                dd      $$                                      ; p_vaddr 
                dw      2                       ; e_type        ; p_paddr
                dw      3                       ; e_machine
                dd      _start                  ; e_version     ; p_filesz
                dd      _start                  ; e_entry       ; p_memsz
                dd      4                       ; e_phoff       ; p_flags
  _start:
                inc     esi                     ; 1
                shl     esi, 16                 ; 3     esi = 0x10000

                lea     edi, [esp-4]    ; 4   Giv mig 4 bytes på stakken

                ;dw      0x34                    ; e_ehsize
                jmp     DoTheShow               ; 2

                dw      0x20                    ; e_phentsize = SKAL være korrekt
e_phnum:
                db      1                       ; e_phnum = SKAL være 1
                                                ; e_shentsize
                                                ; e_shnum
                                                ; e_shstrndx
                dw      0                       ; e_shentsize
                dw      0                       ; e_shnum
                ;dw      0                       ; e_shstrndx
                db "Lgnd"
flag:                
                ;db      "nc3{sh3llc0d3_h3le_vinter3n}"
                db      0x4c,0xfe,0x7a,0x22,0xb4,0xfc,0xa9,0x30,0x30,0xe1,0x13,0x99,0xa4,0x41,0x52,0xc7,0x02,0x64,0x02,0x66,0xc5,0x0a,0xce,0x88,0x06,0x3b,0x91,0x9d
flag_end:       equ     $ - flag


DoTheShow:
    ; Hent et 1 tal ind
    mov     ecx, [esi + (e_phnum - file_start)]     ; 3
    shl     ecx, 3      ; 3
    dec     cl          ; 2    cl == 7

    LODSd               ; 1     eax = 0x464C457F (XOR key)
    mov    edx, eax     ; 1     edx = XOR key

    ; Lokalisér flaget (i krypteret form)
    add     esi, (flag - file_start - 4)   ; -4 pga. LODSD

    mov     bl, 1       ; 2     stdout

looper:
    LODSd               ; 1     eax = 4 bytes af Krypteret flag

; Encrypt
;    XOR     EAX, EDX    ; 
;    ror     eax, cl     ; 

; Decrypt
    rol     eax, cl     ; 
    XOR     EAX, EDX    ; 


    STOSD
    sub     edi, 4


    mov [esp-8], ecx
    mov [esp-12], edx

    mov     eax, 4 ; sys_write
    mov     edx, 4 ; length
    mov     ecx, edi

    jmp     do_syscall

    ; Obfuscate : Corrupt disassembly'en
    db 0xE8
;    int 0x80           ; Trigger interrupt to invoke syscall
back_again:

    mov edx, [esp-12]
    mov ecx, [esp-8]

; Obfuscate
    jns obf1
    call $+6+5
    db 0xE9
    db "Lgnd"
obf1:
    db 0xE9

    ;dec ecx
    sub ecx, 7
    jnz looper

    ; exit(0) syscall in x86
    xor ebx, ebx           ; Set exit code to 0 (EBX = 0)
    mov eax, 1             ; Syscall number for exit in 32-bit (eax = 1)
    ;int 0x80               ; Trigger interrupt to invoke syscall

do_syscall:
    int 0x80           ; Trigger interrupt to invoke syscall
    jmp back_again


filesize      equ     $ - $$