#include <stdio.h>
#include <string.h>
char flag[30];  // flag{this_is_a_simple_rev}
char arr[] =
    "\x11\x09\x0d\x04\x14\x19\x0d\x36\x07\x30\x36\x41\x6f\x50\x66\x00\x0a\x03"
    "\x05\x0f\x11\x39\x53\x44\x57\x5c";
char* src = "welcome_to_2019scnuctf!!!!";
void xor (char* a, char* b) {
  size_t len = strlen(a);
  for (size_t i = 0; i < strlen(a); i++) {
    *(b+i) ^= *(a+i);
  }
  return;
} 

int main(int argc, char const* argv[]) {
  puts(src);
  puts("Give me your flag!");
  scanf("%s", flag);
  xor(flag, arr);
  if (strlen(flag) == 26 && !strcmp(arr, src)) {
    puts("correct!");
  } else {
    puts("incorrect,try again!");
  }
  return 0;
}
