#!/usr/bin/env python

with open('./data','rb') as file:
    data = file.read()

plain = ''.join([chr(ord(x)^0xef) for x in data])

print(plain)


