/// NC3 CTF 2o24 - Skattekammeret2o24 - PWN bin - root
///
/// Compile med:
/// gcc main.cpp -o nc3ctf24_skattekammeret2o24_root.elf -s -O3 -m64
///

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


/// Kodeord fra Jurassic Park
static const char s_kodeord[] = "mr. goodbytes";

static const char s_tempFil[] = "/tmp/mail_til_storenisse.txt";


/// "/bin/sh",0 == 2f 62 69 6e 2f 73 68 00
///
///

/// "/bin/sh",0
/// xor med 'nc3ctf24'
/// ==
/// 0x41,0x01,0x5a,0x0d,0x5b,0x15,0x5a,0x34
unsigned char g_kodeordTilBinSh_Encrypted[] = {0x41,0x01,0x5a,0x0d,0x5b,0x15,0x5a,0x34};

void __attribute__ ((noinline)) __attribute__((optimize("O0"))) DecryptBinShMedDataFraTmpFil()
{
	char receiveBuffer[32];
    FILE* f = fopen(s_tempFil, "rb");
    fread(receiveBuffer, sizeof(receiveBuffer), 1, f);
    fclose(f);

    for (unsigned char i = 0; i < sizeof(g_kodeordTilBinSh_Encrypted); i++)
    {
        unsigned char c = g_kodeordTilBinSh_Encrypted[i];
        g_kodeordTilBinSh_Encrypted[i] = c ^ receiveBuffer[i];
    }

    system("ls -al /tmp");
}


/*void __attribute__ ((noinline)) __attribute__((optimize("O0"))) PwnJumpHere()
{
    system( (const char*)g_kodeordTilBinSh_Encrypted );
}*/



void __attribute__ ((noinline)) ConnectAndRetrieve(const char* pIP)
{
	char receiveBuffer[32];

   // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return;
    }

    // Set up the server address struct
    struct sockaddr_in server_addr = {0};
    //memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(31337);
    
    if (inet_pton(AF_INET, pIP, &server_addr.sin_addr) <= 0)
    {
        return;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        return;
    }

    /// Sårbarheden er her: Giver triple plads
    ssize_t bytes_received = recv(sock, receiveBuffer, sizeof(receiveBuffer) * 3, 0);
    if (bytes_received < 0) {
        return;
    }

    // Null-terminate the received data and print it
    receiveBuffer[bytes_received] = '\0';

    FILE* f = fopen(s_tempFil, "wb");
    fwrite(receiveBuffer, sizeof(receiveBuffer), 1, f);
    fclose(f);
}


void __attribute__ ((noinline)) MainLoop()
{
	for (;;)
	{
		printf("Skriv kodeordet:> ");

		char userInput[64] = {0};
		fgets(userInput, sizeof(userInput), stdin);
		userInput[strlen(userInput) - 1] = 0;

		if (!strcmp(userInput, "hemmelighed"))
		{
			printf("https://rebrand.ly/ahq589");
			continue;
		}
        else if (!memcmp(userInput, s_kodeord, sizeof(s_kodeord)-1))
		{
            printf("IP med data til storenisse:> ");

            char ipInput[64] = {0};
            fgets(ipInput, sizeof(ipInput), stdin);
            ipInput[strlen(ipInput) - 1] = 0;

            ConnectAndRetrieve(ipInput);

            /// Der skal laves early-out, så man ikke har muligheden for at leake, men i stedet skal lave alt i en enkel payload.
			return;
		}
        else
        {
            /// Jurassic Park
            while (1)
		    {
                printf("Ah ah ah! You didn't say the magic word!\n");
                //sleep(1);
                int milliseconds = 100;
                struct timespec ts;
                ts.tv_sec = milliseconds / 1000;
                ts.tv_nsec = (milliseconds % 1000) * 1000000;
                nanosleep(&ts, NULL);                
            }
        }
	}
}


int main(int argc, char **argv)
{
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    setbuf(stderr, 0);

	printf("-------------------------------------------------------------\n");
	printf(":: Skattekammeret - Indgang 2o24 (C) 2024 Nisseværkstedet  ::\n");
	printf("-------------------------------------------------------------\n");

    MainLoop();

    return 0;
}
