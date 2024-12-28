[bits 16]           
[global _start]     

_start:
    cli                     
    xor ax, ax              
    mov ds, ax              
    mov es, ax              
    mov fs, ax              
    mov gs, ax              

    mov si, welcome_message
print_message:
    lodsb                   
    or al, al               
    jz done_printing
    mov ah, 0x0E            
    int 0x10                
    jmp print_message

done_printing:
    mov ax, 0x1000          
    mov es, ax              
    mov bx, 0x200           
    mov ah, 0x02            
    int 0x13                
    jz error_disk_read      

    jmp 0x1000:0            

error_disk_read:
    mov si, error_message
    jmp print_message

welcome_message db 'Welcome to basickernel ', 0
error_message db 'Error: Disk read failed!', 0

times 510 - ($ - $$) db 0 
dw 0xAA55                 
