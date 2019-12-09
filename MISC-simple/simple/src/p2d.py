#!/usr/bin/env python

with open('./plain','rb') as file:
    data = file.read()

cipher = ''.join([chr(ord(x)^0xef) for x in data])

with open('./data','wb') as file:
    file.write(cipher)


