// 637 130 172 145 916 873 100 46 869
// flag{27d82ac91394369642e365}
#include <stdio.h>
#include <stdlib.h>
char* ptr[10];
int nums[9] = {0};
void init() {
  *ptr =
      "If you can guess the nine numbers correctly, you can get the flag, that "
      "each number is greater than 0 and less than 1000";
  *(ptr + 1) = "Les's go! Now give me the first number:";
  *(ptr + 2) = "Correct! Now give me the second number:";
  *(ptr + 3) = "Correct! Now give me the third number:";
  *(ptr + 4) = "Correct! Now give me the fourth number:";
  *(ptr + 5) = "Correct! Now give me the fifth number:";
  *(ptr + 6) = "Correct! Now give me the sixth number:";
  *(ptr + 7) = "Correct! Now give me the seventh number:";
  *(ptr + 8) = "Correct! Now give me the eight number:";
  *(ptr + 9) = "Correct! Now give me the ninth number:";
}
int main(int argc, char const* argv[]) {
  init();
  puts(*ptr);
  srand(2019);
  for (size_t i = 0; i < 9; i++) {
    printf("%s", *(ptr + i + 1));
    scanf("%d", nums + i);
    if (*(nums + i) != rand() % 1000) {
      puts("It's a fake number,you lost!");
      return 0;
    }
  }
  puts("You win! Now you can submit the flag and get the score!");
  printf("flag{");
  for (size_t i = 0; i < 9; i++) {
    printf("%x", *(nums + i));
  }
  puts("}");
  return 0;
}
