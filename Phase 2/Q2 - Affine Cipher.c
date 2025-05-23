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


void encrypt(char *plaintext,char*ciphertext,char key1,char key2)
{
	mpz_t sum;
	mpz_init(sum);
	mpz_t plaintext_hash;
	mpz_t key1_hash,key2_hash;

	mpz_inits(plaintext_hash,key1_hash,key2_hash,NULL);
	hash(key1,key1_hash);
	hash(key2,key2_hash);

	for(int i = 0; i < strlen(plaintext); i++)
	{
		hash(plaintext[i],plaintext_hash);
		
		printf("\nPlaintext Char-Hash : %c-%d",plaintext[i],mpz_get_ui(plaintext_hash));

		mpz_mul(sum,plaintext_hash,key1_hash);
		mpz_add(sum,plaintext_hash,key2_hash);
		mpz_mod_ui(sum,sum,37);
		
		printf("\nCiphertext Char-Hash is %c -> (%d * %d + %d) mod 37 = %d",ciphertext[i],mpz_get_ui(plaintext_hash),mpz_get_ui(key1_hash),mpz_get_ui(key2_hash),mpz_get_ui(sum));
		ciphertext[i] = (char)(character(sum));
		
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);

}


void decrypt(char *decoded,char*ciphertext,char key1,char key2)
{
	mpz_t diff;
	mpz_init(diff);
	mpz_t ciphertext_hash;
	mpz_t key1_hash,key2_hash;

	mpz_inits(ciphertext_hash,key1_hash,key2_hash,NULL);
	hash(key1,key1_hash);
	hash(key2,key2_hash);

	mpz_t modulus;
	mpz_init_set_ui(modulus,37);

	for(int i = 0; i < strlen(ciphertext); i++)
	{
		hash(ciphertext[i],ciphertext_hash);
				
		mpz_sub(diff,ciphertext_hash,key2_hash);
		mpz_invert(key1_hash,key1_hash,modulus);
		mpz_mod_ui(diff,diff,37);
		decoded[i] = (char)(character(diff));
	}

	decoded[strlen(ciphertext)] = '\0';
	
	toUpper(decoded);

}
void main(int argc,char *argv[])
{
	char name[MAX_LENGTH];
	char roll_no[MAX_LENGTH];
	char plaintext[MAX_LENGTH];
	char decoded[MAX_LENGTH];

	strcpy(plaintext,argv[1]);

	removeSpaces(plaintext);
	toLowerCase(plaintext);
	

	printf("\nReading keys from keys.txt ....");
	char buffer[MAX_LENGTH];
	char trad_key[LENGTH_OF_KEY];
	char k_roll1;
	char k_roll2;

	FILE *file = fopen("output.txt","r");

	fgets(trad_key,LENGTH_OF_KEY,file);

	//printf("   %s  ",trad_key);
	fgetc(file);
	k_roll1 = fgetc(file);
	fgetc(file);
	k_roll2 = fgetc(file);

	printf("\n Key1 is  %c ",k_roll1);
	printf("\n Key2 is  %c ",k_roll2);
	// The initialization is done
	
	char ciphertext[MAX_LENGTH];
	int length = strlen(plaintext);

	k_roll2 = '3';
	encrypt(plaintext,ciphertext,k_roll1,k_roll2);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext,k_roll1,k_roll2);

	printf("\n Decoded : %s\n",decoded);

}
