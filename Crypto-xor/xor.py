from Crypto.Util.number import bytes_to_long, long_to_bytes
from Crypto.Random import get_random_bytes
from base64 import b64encode
from flag import flag

assert (len(flag) < 40)
assert (flag.startswith('flag'))

key = get_random_bytes(12) * 4
padding = '\x00' * (48 - len(flag) % 48)
message = (flag + padding).encode()
cipher = long_to_bytes(bytes_to_long(key) ^ bytes_to_long(message))
print(b64encode(cipher))
# zO+DR4Od/Z3z7/yJmuLXEs+d8Jag7v/ezrraE5uc+Z2ivP/bmP7iIPisya/G2Mvs
