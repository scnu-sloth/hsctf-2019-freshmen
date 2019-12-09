#include <cstdio>
#include <cstring>
#include <sm3.h>
#include <sm4.h>

constexpr int DIGEST_LENGTH = 32;

int main() {
    char pwd[BUFSIZ];
    unsigned char keyBuf[DIGEST_LENGTH];
    unsigned char tipBuf[DIGEST_LENGTH];
    unsigned char plain[BUFSIZ];
    unsigned char cipher[DIGEST_LENGTH];
    unsigned char realIV[SM4_BLOCK_SIZE];
    unsigned char iv[SM4_BLOCK_SIZE] = {
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    };
    sm4_key_t key;

    printf("Please input a password protected:\n");
    scanf("%s", plain); // HSCTF{C00l_Sm3_Cr@cker}

    printf("Please input a recovery password:\n");
    scanf("%s", pwd); // ng6324

    sm3(reinterpret_cast<const unsigned char *>(pwd), strlen(pwd), keyBuf);
    sm3(keyBuf, DIGEST_LENGTH, tipBuf);

    sm4_set_encrypt_key(&key, keyBuf);
    memcpy(realIV, iv, SM4_BLOCK_SIZE);
    sm4_cbc_encrypt(plain, cipher, DIGEST_LENGTH, &key, realIV, 1);

    FILE *fp = fopen("tips", "w");
    fwrite(tipBuf, 1, DIGEST_LENGTH, fp);
    fclose(fp);

    fp = fopen("password.bak", "w");
    fwrite(cipher, 1, DIGEST_LENGTH, fp);
    fclose(fp);

    return 0;
}