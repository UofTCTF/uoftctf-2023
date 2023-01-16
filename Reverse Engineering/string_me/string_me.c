#include <stdio.h>
#include <string.h>

char flag[] = "uoft{5Tr1N95_1n_TH3_81NaR135}";

int flag_check(char *check) {
	return !strncmp(flag, check, strlen(flag));
}

int main() {
	char input[32];

	printf("Guess the flag: ");
	fgets(input, sizeof(input), stdin);

	if (flag_check(input)) {
		printf("You're correct!!!\n");
	}
	else {
		printf("Sorry, that's not correct :(\n");
	}
	return 0;
}