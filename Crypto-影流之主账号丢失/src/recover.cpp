#include <cstdio>
#include <cstring>
#include <sm3.h>
#include <sm4.h>

constexpr int DIGEST_LENGTH = 32;

int main(int argc, const char *argv[]) {
    unsigned char keyBuf[DIGEST_LENGTH];
    unsigned char tipsBuf[DIGEST_LENGTH];
    unsigned char validateBuf[DIGEST_LENGTH];
    unsigned char cipher[DIGEST_LENGTH];
    unsigned char result[DIGEST_LENGTH];
    unsigned char realIV[SM4_BLOCK_SIZE];
    unsigned char iv[SM4_BLOCK_SIZE] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    };
    sm4_key_t key;

    if (argc < 4) {
        printf("recover <cipher path> <tips path> <recovery password>");
        return 0;
    }

    // 读取密码密文
    FILE *fp = fopen(argv[1], "r");
    if (fp == nullptr) {
        printf("Failed to open %s\n", argv[1]);
        return -1;
    }
    fread(cipher, 1, DIGEST_LENGTH, fp);
    fclose(fp);

    // 读取tips
    fp = fopen(argv[2], "r");
    if (fp == nullptr) {
        printf("Failed to open %s\n", argv[2]);
        return -1;
    }
    fread(tipsBuf, 1, DIGEST_LENGTH, fp);
    fclose(fp);

    // 计算一波密码和tips
    sm3(reinterpret_cast<const unsigned char *>(argv[3]), strlen(argv[3]), keyBuf);
    sm3(keyBuf, DIGEST_LENGTH, validateBuf);

    if (memcmp(validateBuf, tipsBuf, DIGEST_LENGTH) != 0) {
        printf("Recovery password '%s' is wrong!\n", argv[3]);
        return -1;
    }

    // 密码对了！开始解密：
    sm4_set_decrypt_key(&key, keyBuf);
    memcpy(realIV, iv, SM4_BLOCK_SIZE);
    sm4_cbc_encrypt(cipher, result, DIGEST_LENGTH, &key, realIV, 0);

    printf("%s\n", result);

    return 0;
}