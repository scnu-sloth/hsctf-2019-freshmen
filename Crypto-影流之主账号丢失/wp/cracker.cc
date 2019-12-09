#include <cstdio>
#include <cstring>
#include <ctime>
#include <sm3.h>

constexpr int DIGEST_LENGTH = 32;

int main(int argc, const char *argv[]) {
    char pwd[DIGEST_LENGTH];
    unsigned char tipBuf[DIGEST_LENGTH];
    unsigned char buf[DIGEST_LENGTH];

    if (argc < 2) {
        printf("cracker <tips path>");
        return 0;
    }

    // 读取tips
    FILE *fp = fopen(argv[1], "r");
    if (fp == nullptr) {
        printf("Failed to open %s\n", argv[1]);
        return -1;
    }
    fread(tipBuf, 1, DIGEST_LENGTH, fp);
    fclose(fp);

    // 开始遍历
    pwd[6] = 0;
    time_t t = time(nullptr);
    for (int i = 0; i < 26; ++i) {
        pwd[0] = 'a' + i;
        for (int j = 0; j < 26; ++j) {
            pwd[1] = 'a' + j;
            for (int k = 0; k < 10; ++k) {
                pwd[2] = '0' + k;
                for (int l = 0; l < 10; ++l) {
                    pwd[3] = '0' + l;
                    for (int m = 0; m < 10; ++m) {
                        pwd[4] = '0' + m;
                        for (int n = 0; n < 10; ++n) {
                            pwd[5] = '0' + n;

                            sm3(reinterpret_cast<const unsigned char *>(pwd), strlen(pwd), buf);
                            sm3(buf, DIGEST_LENGTH, buf);
                            if (memcmp(buf, tipBuf, DIGEST_LENGTH) == 0) {
                                printf("Found! password is '%s'\n", pwd);
                                printf("cost %ld sec\n", time(nullptr) - t);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("Not found!\n");

    return 0;
}
