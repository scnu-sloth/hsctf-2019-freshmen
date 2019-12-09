// flag{you_are_so_clever}
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
char* lost = "lbh ybfg,cyrnfr cynl ntnva!";
char* fail = "bu,lbh ybfg,guvf vf n snxr synt:";
char* real = "gdyyyy,guvf vf n erny synt:";
char* src = "synt{lbh_ner_fb_pyrire}";  // rot13
bool tag = 1;
int high, width;
int bird_x, bird_y;
int bar_y;
int bar_xTop, bar_xDown;
int score;

void HideCursor() {
  CONSOLE_CURSOR_INFO cursor_info = {1, 0};
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void gotoxy(int x, int y) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos;
  pos.X = x;
  pos.Y = y;
  SetConsoleCursorPosition(handle, pos);
}
void startup() {
  high = 18;
  width = 26;

  bird_x = high / 2;
  bird_y = width / 2;

  bar_y = width;

  bar_xTop = high / 2;
  bar_xDown = bar_xTop + 4;

  score = 0;

  HideCursor();
}
void show() {
  int i, j;
  gotoxy(0, 0);
  for (i = 0; i <= high + 1; i++) {
    for (j = 0; j <= width; j++) {
      if ((i == bird_x) && (j == bird_y))
        printf("@");
      else if (i == high + 1)
        printf("_");
      else if (j == width)
        printf("|");
      else if ((j == bar_y) && ((i <= bar_xTop) || (i >= bar_xDown)))
        printf("|");
      else
        printf(" ");
    }
    printf("\n");
  }
  printf("score:%d\n", score);
}
void updateWithoutInput() {
  if (bird_y == bar_y - 1) {
    if ((bird_x > bar_xTop) && (bird_x < bar_xDown))
      score++;
    else {
      tag = 0;  // die
    }
  }

  if (bar_y > 0)
    bar_y--;
  else {
    bar_y = width;
    bar_xTop = rand() % (high - 4);
    bar_xDown = bar_xTop + 4;
  }
  if (bird_x < high + 1)
    bird_x++;
  else {
    tag = 0;  // die
  }
  Sleep(50);
}
void updateWithInpute() {
  char input;

  if (kbhit()) {
    input = getch();
    if ((input == ' ') && (bird_x > 0) && (bird_x <= high))
      bird_x = bird_x - 2;
  }
}

void print(char* src) {
  int i = 0;
  for (i = 0; i < strlen(src); i++) {
    if (src[i] >= 'a' && src[i] <= 'z') {
      printf("%c", (src[i] - 0x61 + 13) % 26 + 0x61);
    } else {
      printf("%c", src[i]);
    }
  }
  puts("");
}

int main() {
  int i = 0;
  startup();
  while (tag) {
    show();
    updateWithoutInput();
    updateWithInpute();
  }
  if (score >= 70) {
    if (score < 2019) {
      print(fail);
      puts(src);
    } else {
      print(real);
      print(src);
    }
  } else {
    print(lost);
  }
  Sleep(3000);
  system("pause");
  return 0;
}
