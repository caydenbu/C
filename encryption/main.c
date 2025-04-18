#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(char *data, char *password){
    	int dataLen = strlen(data);
    	int passLen = strlen(password);

    	for (int i = 0; i < dataLen; i++) {
        	data[i] ^= password[i % passLen]; 
	}	

}

int main()
{
	char string[] = "hello I am testing the decryption technique ive thought of my self";
	encrypt(string, "ThisIsMyTestPassword");
	encrypt(string, "ThisIsMyTestPassword");
	printf(string);
}
