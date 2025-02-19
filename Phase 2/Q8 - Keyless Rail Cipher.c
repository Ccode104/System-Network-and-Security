#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define LENGTH_OF_KEY 26
#define MAX_LENGTH 100


void toLowerCase(char str[]) 
{
	for (int i = 0; str[i] != '\0'; i++) 
	{
		str[i] = tolower(str[i]);
	}
}


void removeSpaces(char *str) 
{

	int i, j = 0;
	int len = strlen(str);
		    
		    
	// Iterate through the string
		         
	for (i = 0; i < len; i++) 
	{
		               
	    if (str[i] != ' ') 
	    {
		                             
		    str[j++] = str[i];  // Copy non-space characters
		                                     
	    }
		                                         
	}
		                                             
	str[j] = '\0';  // Null-terminate the modified string
		                                             
}

char character(mpz_t h)
{
	char c;
	mpz_mod_ui(h,h,37);

	if (mpz_cmp_ui(h , 10)<0)
	{
		c = (char)(mpz_get_ui(h) + (unsigned int)('0'));
	}
	else if(mpz_cmp_ui( h , 10) > 0)
	{
		c = (char)((int)('a') + (int)mpz_get_ui(h) - 11);
	}
	else
	{
		c = ' ';
	}

	return c;
}

void hash(char c,mpz_t hash)
{

	if(isdigit(c))
		mpz_set_ui(hash,(unsigned int)(c - '0'));
	else if(isalpha(c))
		mpz_set_ui(hash ,(unsigned int)(c - 'a') + 11);
	else
		mpz_set_ui(hash,10);

}
void toUpper(char *str)
{
	for(int i=0;str[i]!='\0';i++)
		str[i] = toupper(str[i]);
}


void encrypt(char *plaintext,char *ciphertext)
{

	int l1 = strlen(plaintext)/2;
	int l2;
	if(strlen(plaintext)%2!=0)
	{
		l2=l1;
		l1=l1+1;
	}
	else
	{
		l2=l1;
	}

	char zigzag[2][l1];

	int k = 0;
	for(int i = 0; i < strlen(plaintext) - 1 ; i = i + 2)
	{
		zigzag[0][k] = plaintext[i];
		zigzag[1][k] = plaintext[i+1];
		k++;
	}

	if(strlen(plaintext)%2!=0)
		zigzag[0][strlen(plaintext)/2] = plaintext[strlen(plaintext)-1];

	for(int i = 0; i < l1; i++)
	{
		ciphertext[i] = zigzag[0][i];
	}

	k = l1;
	for(int i = 0; i < l2; i++)
	{
		ciphertext[k] = zigzag[1][i];
		k++;
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);
	/*
	printf("\n");
	for(int i=0;i<l1;i++)
	{
		printf("%c",zigzag[0][i]);
	}
	printf("\n");
	for(int i=0;i<l2;i++)
	{
		printf("%c",zigzag[1][i]);
	}
	*/

}


void decrypt(char *decoded,char*ciphertext)
{

	int l1 = strlen(ciphertext)/2;
	int l2;
	if(strlen(ciphertext)%2!=0)
	{
		l2=l1;
		l1=l1+1;
	}
	else
	{
		l2=l1;
	}

	char zigzag[2][l1];

	for(int i = 0; i < l1; i++)
	{
		zigzag[0][i] = ciphertext[i];
	}

	int k = 0;
	for(int i = l1; i < strlen(ciphertext); i++)
	{
		zigzag[1][k] = ciphertext[i];
		k++;
	}
/*
	printf("\n");
	for(int i=0;i<l1;i++)
	{
		printf("%c",zigzag[0][i]);
	}
	printf("\n");
	for(int i=0;i<l2;i++)
	{
		printf("%c",zigzag[1][i]);
	}
*/
	k=0;
	for(int i = 0; i < l2; i++)
	{
		decoded[k] = zigzag[0][i];
		decoded[k+1] = zigzag[1][i];
		k=k+2;
	}


	if(l1!=l2)
		decoded[k] = zigzag[0][l1-1];

	decoded[k+1] = '\0';
	
	toUpper(decoded);

}
void main(int argc,char *argv[])
{
	char name[MAX_LENGTH];
	char roll_no[MAX_LENGTH];
	char plaintext[MAX_LENGTH];
	char decoded[MAX_LENGTH];
	char ciphertext[MAX_LENGTH];

	strcpy(plaintext,argv[1]);

	removeSpaces(plaintext);
	toLowerCase(plaintext);
	

	printf("\nReading keys from keys.txt ....");
	char buffer[MAX_LENGTH];
	char trad_key[LENGTH_OF_KEY];
	char k_roll1;
	char k_roll2;
/*
	FILE *file = fopen("output.txt","r");

	fgets(trad_key,LENGTH_OF_KEY,file);

	//printf("   %s  ",trad_key);
	k_roll1 = fgetc(file);
	fgetc(file);
	k_roll2 = fgetc(file);

	printf("\n Key is  %c ",k_roll1);
	// The initialization is done
	
	
	int length = strlen(plaintext);

*/
	encrypt(plaintext,ciphertext);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext);

	printf("\n Decoded : %s\n",decoded);

}
