/**
 * Lift Day by @kako57
 *
 * compiled using `gcc -s -O0 -g0 -o lift_day lift_day.c`
 *
 * Thanks to @Tyler3412 for helping me name the challenge
 * and for regularly dragging me to go to the gym :skull:
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Linked list that represents a tower of plates => a stack
// Hopefully you made use of struct features in your Ghidra/IDA decompiler!
typedef struct __node {
  u8 size;
  struct __node *below;
} plate;

// people who know C will know what the following functions do

plate *new_plate(u8 size) {
  plate *new_p = (plate *)calloc(1, sizeof(plate));
  new_p->size = size;
}

void push(plate **tower, plate *n_plate) {
  n_plate->below = *tower;
  *tower = n_plate;
}

plate *pop(plate **tower) {
  plate *popped = *tower;
  *tower = (*tower)->below;
  return popped;
}

// the move function follows tower of hanoi rules
void move(plate **dst, plate **src, bool *messed_up) {
  if (*src == NULL) {
    *messed_up = 1;
    return;
  }
  if (*dst != NULL && (*dst)->size < (*src)->size) {
    *messed_up = 1;
    return;
  }
  if (*dst == *src) {
    *messed_up = 1;
    return;
  }
  plate *plate_to_move = pop(src);
  push(dst, plate_to_move);
}

void init(plate **towers, u8 num_plates) {
  for (u8 i = 0; i < 3; i++) {
    towers[i] = NULL;
  }

  for (u8 i = num_plates; i > 0; i--) {
    push(&towers[0], new_plate(i));
  }
}

plate *return_plates(plate *head) {
  while (head) {
    plate *tmp = head;
    head = head->below;
    free(tmp);
  }
  return head;
}

void finish_workout(plate **towers) {
  for (int i = 0; i < 3; i++) {
    towers[i] = return_plates(towers[i]);
  }
}

// this add macro is used to add two numbers together
// it's a bit of a hack, but it works
#define add(x, y)                                                              \
  {                                                                            \
    typeof(y) save_addend = y;                                                 \
    while (y != 0) {                                                           \
      typeof(x) carry = x & y;                                                 \
      x ^= y;                                                                  \
      y = carry << 1;                                                          \
    }                                                                          \
    y = save_addend;                                                           \
  }

// this is the fibonacci function with the add macro
u64 fib(u64 n) {
  if (n <= 1)
    return n;

  u64 a = n - 1;
  u64 b = n - 2;

  a = fib(a);
  b = fib(b);

  add(a, b);

  return a;
}

#define get_byte(x, n) (x >> n) & 0xff

void generate_flag(char *answer) {
  char weeee[36][2] = {{0x38, 0x20}, {0x3a, 0x33}, {0x34, 0x04}, {0x22, 0x2d},
                       {0x12, 0x03}, {0x04, 0x19}, {0x38, 0x17}, {0x1c, 0x30},
                       {0x02, 0x35}, {0x0c, 0x32}, {0x28, 0x21}, {0x08, 0x00},
                       {0x22, 0x04}, {0x04, 0x22}, {0x1a, 0x1e}, {0x02, 0x28},
                       {0x04, 0x22}, {0x10, 0x37}, {0x1c, 0x37}, {0x3c, 0x33},
                       {0x3a, 0x07}, {0x30, 0x29}, {0x30, 0x20}, {0x1a, 0x38},
                       {0x1e, 0x22}, {0x22, 0x02}, {0x02, 0x28}, {0x30, 0x13},
                       {0x20, 0x12}, {0x30, 0x30}, {0x16, 0x1b}, {0x2c, 0x2b},
                       {0x18, 0x16}, {0x34, 0x38}, {0x0c, 0x19}, {0x18, 0x3c}};

  for (int i = 0; i < 36; i++) {
    size_t k = weeee[i][0];
    u64 x = *(u16 *)(answer + k);
    putchar(get_byte(fib(x), weeee[i][1]));
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s argument", argv[0]);
    return 1;
  }

  plate *towers[3];
  u8 num_plates = 5;
  init(towers, num_plates);

  char *answer = argv[1];
  size_t answer_length = strlen(answer);

  u8 st_char = '!'; // starting character
  size_t i;
  bool messed_up = false;

  for (i = 0; i < answer_length; i += 2) {
    u8 src_idx = answer[i] - st_char;
    u8 dst_idx = answer[i + 1] - st_char;

    if (src_idx > 2 || dst_idx > 2) {
      finish_workout(towers);
      return 1;
    }

    move(&towers[dst_idx], &towers[src_idx], &messed_up);

    if (messed_up) {
      finish_workout(towers);
      return 1;
    }

    st_char += 3; // increment starting character
  }

  // check if there are any plates left for the first two towers
  for (i = 0; i < 2; i++) {
    if (towers[i] != NULL) {
      finish_workout(towers);
      return 1;
    }
  }

  plate *it = towers[2];

  // check if the stack is sorted
  while (it) {
    u8 next_size = it->below ? it->below->size : num_plates + 1;
    if (it->size + 1 != next_size) {
      finish_workout(towers);
      return 1;
    }
    it = it->below;
  }

  // assert that the answer is the optimal solution = 2 ** n - 1 moves
  if (((1 << num_plates) - 1) * 2 != answer_length) {
    finish_workout(towers);
    return 1;
  }

  puts(
      "Nice one! I'll put the flag below. It might be done in 49 hours tho XD");

  generate_flag(answer);

  finish_workout(towers);

  return 0;
}
