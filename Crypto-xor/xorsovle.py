from Crypto.Util.number import bytes_to_long, long_to_bytes
from base64 import b64decode

cipher = b'zO+DR4Od/Z3z7/yJmuLXEs+d8Jag7v/ezrraE5uc+Z2ivP/bmP7iIPisya/G2Mvs'
cipher = b64decode(cipher)
subcipher = cipher[:4] + cipher[-8:]
message = 'flag' + '\x00' * 8
key = long_to_bytes(bytes_to_long(message.encode()) ^ bytes_to_long(subcipher))
flag = long_to_bytes(bytes_to_long(cipher) ^ bytes_to_long(key * 4))
print(flag.decode())