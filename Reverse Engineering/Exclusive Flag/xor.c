#include <stdio.h>
#include <string.h>

char flag[] = "\x61\x7b\x72\x60\x6f\x4d\x24\x61\x4b\x20\x46\x27\x4b\x20\x4b\x5c\x20\x4c\x24\x46\x69";

void xor_string(char *input) {
	for (int i = 0; i < strlen(input); i++) {
		input[i] ^= 20;
	}
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Usage: %s <flag>\n", argv[0]);
		return 0;
	}
	
	xor_string(argv[1]);
	if (!strncmp(argv[1], flag, strlen(flag))) { // Substitute bytes
		printf("You guessed the flag!\n");
	}
	else {
		printf("Incorrect :(\n");
	}
	
	return 0;
}