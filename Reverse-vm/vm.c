// flag = "flag{It_is_an_amazing_vm}"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
bool tag = 0;
__uint8_t* ptr =
    //   start vm
    //   init regs
    "\x03\x00\x00\x00\x00\x00"  // mov $0,%reg0
    "\x03\x00\x00\x00\x00\x01"  // mov $0,%reg1
    "\x03\x00\x00\x00\x00\x02"  // mov $0,%reg2
    "\x03\xe3\x07\x00\x00\x03"  // mov $2019,%reg3
    "\x03\x25\x5b\x00\x00\x04"  // mov $23333,%reg4
    "\x03\x91\xe5\x25\x01\x05"  // mov $19260817,%reg5
    "\x03\x7f\x00\x00\x00\x06"  // mov $127,%reg6
    "\x03\x19\x00\x00\x00\x07"  // mov $24,%reg7
    //   check
    "\x02\x02\x00"  // mov (%reg2),%reg0
    "\x09\x02"      // inc %reg2
    "\x01\x03\x01"  // mov %reg3,%reg1
    "\x06\x04\x01"  // mul %reg4,%reg1
    "\x04\x05\x01"  // add %reg5,%reg1
    "\x08\x06\x01"  // mod %reg6,%reg1
    "\x01\x01\x03"  // mov %reg1,%reg3
    "\x0A\x00\x01"  // cmp %reg0,%reg1
    "\x0B\x0A"      // jnz exit
    "\x0A\x02\x07"  // cmp %reg2,%reg7
    "\x0B\xde"      // jnz check
    "\x0C"          // tag = 1
    //   end vm
    "\x00\x00";
__uint8_t dest[25] = {0x21, 0x58, 0x33, 0x57, 0x24, 0x2c, 0x66, 0x5d, 0x5f,
                      0x7f, 0x0a, 0x27, 0x39, 0x5f, 0x1d, 0x0b, 0x0f, 0x2e,
                      0x00, 0x6b, 0x2b, 0x5b, 0x19, 0x5c, 0x41};

void vm(unsigned char* opcode) {
  __uint32_t tmp = 0;
  __uint32_t reg[8] = {0};
  __uint8_t* ptr = opcode;
  __uint8_t vflag = 1;
  __uint8_t zf = 0;
  __int8_t level = 0;
  while (vflag) {
    switch (*ptr) {
      case 0x00:
        vflag = 0;
        // puts("exit");
        break;
      case 0x01:
        tmp = reg[*++ptr];
        reg[*++ptr] = tmp;
        // puts("mov %%reg,%%reg");
        break;
      case 0x02:
        tmp = *(dest + reg[*++ptr]);
        reg[*++ptr] = tmp;
        // puts("mov (%%reg),%%reg");
        break;
      case 0x03:
        tmp = *++ptr + (*++ptr << 8) + (*++ptr << 16) + (*++ptr << 24);
        reg[*++ptr] = tmp;
        // puts("mov $imm,%%reg");
        break;
      case 0x04:
        reg[*++ptr] += reg[*++ptr];
        // puts("add %%reg,%%reg");
        break;
      case 0x05:
        reg[*++ptr] -= reg[*++ptr];
        // puts("sub %%reg,%%reg");
        break;
      case 0x06:
        reg[*++ptr] *= reg[*++ptr];
        // puts("mul %%reg,%%reg");
        break;
      case 0x07:
        reg[*++ptr] /= reg[*++ptr];
        // puts("div %%reg,%%reg");
        break;
      case 0x08:
        reg[*++ptr] %= reg[*++ptr];
        // puts("mod %%reg,%%reg");
        break;
      case 0x09:
        reg[*++ptr]++;
        // puts("inc %%reg");
        break;
      case 0x0A:
        zf = reg[*++ptr] - reg[*++ptr];
        // printf("cmp %%reg,%%reg (zf = %d)\n", zf);
        break;
      case 0x0B:
        level = *++ptr;
        if (zf != 0) {
          level = level < 0x80 ? level : level - 0x100;
          ptr += level - 2;
        }
        // printf("jnz (level = %d),ptr = %x\n", level, *ptr);
        break;
      case 0x0C:
        // puts("gogogo");
        tag = 1;
        break;
      default:
        // printf("Invalid opcode %x\n", *ptr);
        break;
    }
    ++ptr;
  }
}

int main(int argc, char const* argv[]) {
  char flag[30];
  char* s =
      ""
      "V         V        M         M\n"
      " V       V        M M       M M\n"
      "  V     V        M   M     M   M\n"
      "   V   V        M     M   M     M\n"
      "    V V        M       M M       M\n"
      "     V        M         M         M\n";
  puts(s);
  puts("this is a simple vm,please give me your flag:");
  scanf("%s", flag);
  if (strlen(flag) != 25) {
    puts("incorrect!");
  } else {
    for (size_t i = 0; i < 25; i++) {
      dest[i] ^= flag[i];
    }
    vm(ptr);
    if (tag) {
      puts("Wow,you are tqlllll!");

    } else {
      puts("incorrect!");
    }
  }
  return 0;
}
