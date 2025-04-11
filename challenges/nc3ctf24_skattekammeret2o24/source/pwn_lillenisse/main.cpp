/// NC3 CTF 2o24 - Skattekammeret2o24 - lillenisse - PWN bin
///
/// Compile med:
/// gcc main.cpp -o nc3ctf24_skattekammeret2o24_lillenisse.elf -s -O3 -m64
///

#include <stdio.h>
#include <string.h>
#include <stdlib.h>




void __attribute__ ((noinline)) __attribute__((optimize("O0"))) PwnJumpHere()
{
    system( "/bin/sh" );
}


int main(int argc, char **argv)
{
    char userInput[64] = {0};

    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);

	printf("-----------------------------------------------------------------------\n");
	printf(":: Skattekammeret - lillenisse bagdør 2o24 (C) 2024 Nisseværkstedet  ::\n");
	printf("-----------------------------------------------------------------------\n");

    printf("Skriv kodeordet:> ");

    /// Vuln: Buffer overflow
    fgets(userInput, sizeof(userInput) * 2, stdin);
    userInput[strlen(userInput) - 1] = 0;

    if (!strcmp(userInput, "hemmelig_pakke_shh"))
    {
        printf("V1ZWb1UwMUhUa2xVVkZwTlpWUnNOVmRzWkV0bFZteFlUbGQwVFdKWVp6RlVSRTVoWXpKV1NXRkVWbUZWVkRBNQ==");
    }

    return 0;
}
