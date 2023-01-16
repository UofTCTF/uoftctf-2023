#include <stdint.h>
#include <stdio.h>

typedef uint64_t u64;
typedef uint16_t u16;
typedef uint8_t u8;
typedef uint64_t ulong;
typedef uint32_t uint;
typedef uint16_t ushort;

uint8_t local_4d = '!';
char solution[64];
int sol_i = 0;

// solution for the tower of hanoi problem
void solve(int disk, u8 dst, u8 src, u8 helper) {
  if (disk == 0) {
    return;
  }

  solve(disk - 1, dst, helper, src);

  solution[sol_i++] = (u8)(local_4d + dst);
  solution[sol_i++] += (u8)(local_4d + src);
  local_4d += 3;

  solve(disk - 1, helper, src, dst);
}

u64 memo[100000] = {0};

// fibonacci but with half-adder algorithm for addition
// patched to use memoization to avoid crazy recursion depths
// and to make it faster
uint64_t fib(uint64_t n) {

  if (memo[n])
    return memo[n];

  ulong uVar1;
  ulong local_28;
  ulong local_20;

  local_28 = n;
  if (1 < n) {
    local_28 = fib(n - 1);
    local_20 = fib(n - 2);
    while (local_20 != 0) {
      uVar1 = local_28 & local_20;
      local_28 = local_28 ^ local_20;
      local_20 = uVar1 * 2;
    }
  }

  memo[n] = local_28;

  return local_28;
}

int main() {
  solve(5, 0, 2, 1);

  // yes, Ghidra output is ugly, but it's easy to make it work

  uint16_t local_58[36];

  local_58[0] = 0x2038;
  local_58[1] = 0x333a;
  local_58[2] = 0x434;
  local_58[3] = 0x2d22;
  local_58[4] = 0x312;
  local_58[5] = 0x1904;
  local_58[6] = 0x1738;
  local_58[7] = 0x301c;
  local_58[8] = 0x3502;
  local_58[9] = 0x320c;
  local_58[10] = 0x2128;
  local_58[11] = 8;
  local_58[12] = 0x422;
  local_58[13] = 0x2204;
  local_58[14] = 0x1e1a;
  local_58[15] = 0x2802;
  local_58[16] = 0x2204;
  local_58[17] = 0x3710;
  local_58[18] = 0x371c;
  local_58[19] = 0x333c;
  local_58[20] = 0x73a;
  local_58[21] = 0x2930;
  local_58[22] = 0x2030;
  local_58[23] = 0x381a;
  local_58[24] = 0x221e;
  local_58[25] = 0x222;
  local_58[26] = 0x2802;
  local_58[27] = 0x1330;
  local_58[28] = 0x1220;
  local_58[29] = 0x3030;
  local_58[30] = 0x1b16;
  local_58[31] = 0x2b2c;
  local_58[32] = 0x1618;
  local_58[33] = 0x3834;
  local_58[34] = 0x190c;
  local_58[35] = 0x3c18;

  int i;
  u64 uVar2;

  // this is the actual flag being printed
  for (i = 0; i < 0x24; i = i + 1) {
    uVar2 = fib((ulong) * (ushort *)(solution + *(char *)(local_58 + i)));
    putchar(
        (uint)(uVar2 >> (*(u8 *)((long)local_58 + (long)i * 2 + 1) & 0x3f)) &
        0xff);
  }
}
