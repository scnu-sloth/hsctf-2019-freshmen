// sssssdsssddsdddwwdwwaaaw
// flag{sssssdsssddsdddwwdwwaaaw}
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
char flag[30];
char maze[] =
    "*************"
    "*@***********"
    "*-***********"
    "*-***-**-****"
    "*--*****-****"
    "*-***#**-****"
    "*--**----****"
    "**-*****-****"
    "**-****--****"
    "**---**-*****"
    "**-*-----****"
    "**-------****"
    "*************";
bool check(char* flag) {
  char* cur = (maze + 14);
  while (*flag && *cur != '*') {
    switch (*flag++) {
      case 'w':
        cur -= 13;
        break;
      case 's':
        cur += 13;
        break;
      case 'a':
        cur -= 1;
        break;
      case 'd':
        cur += 1;
        break;
      default:
        return 0;
    }
  }
  if (*cur == '#') {
    return 1;
  }
  return 0;
}
int main(int argc, char const* argv[]) {
  puts("Please give me the shortest path!");
  scanf("%s", flag);
  if (strlen(flag) == 24 && check(flag)) {
    puts("gogogo! Now you can submit the flag and get the score!");
    printf("flag{%s}\n", flag);
  } else {
    puts("error!");
  }
  return 0;
}
