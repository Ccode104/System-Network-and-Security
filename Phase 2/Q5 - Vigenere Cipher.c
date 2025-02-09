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


void encrypt(char *plaintext,char*ciphertext,char* key)
{
	mpz_t sum;
	mpz_init(sum);
	mpz_t plaintext_hash;
	mpz_t key_hash[strlen(key)];

	mpz_init(plaintext_hash);

	for (int i = 0; i < strlen(key); ++i)
	{
		mpz_init(key_hash[i]);
		hash(key[i],key_hash[i]);
	}

	mpz_t k; 
	mpz_init_set_ui(k,0);

	for(int i = 0; i < strlen(plaintext); i++)
	{
		hash(plaintext[i],plaintext_hash);
		
		mpz_add(sum,plaintext_hash,key_hash[mpz_get_ui(k)]);
		mpz_mod_ui(sum,sum,37);
		
		ciphertext[i] = (char)(character(sum));

		mpz_mod_ui(k,k,strlen(key));
		
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);

}


void decrypt(char *decoded,char*ciphertext,char* key)
{
	mpz_t diff;
	mpz_init(diff);
	mpz_t ciphertext_hash;
	mpz_t key_hash[strlen(key)];

	mpz_init(ciphertext_hash);
	
	for (int i = 0; i < strlen(key); ++i)
	{
		mpz_init(key_hash[i]);
		hash(key[i],key_hash[i]);
	}

	mpz_t k; 
	mpz_init_set_ui(k,0);

	for(int i = 0; i < strlen(ciphertext); i++)
	{
		hash(ciphertext[i],ciphertext_hash);
		
		mpz_sub(diff,ciphertext_hash,key_hash[mpz_get_ui(k)]);
		mpz_mod_ui(diff,diff,37);
		decoded[i] = (char)(character(diff));
		mpz_mod_ui(k,k,strlen(key));
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

	printf("\n Key is  %c ",k_roll1);
	// The initialization is done
	
	char ciphertext[MAX_LENGTH];
	int length = strlen(plaintext);


	encrypt(plaintext,ciphertext,trad_key);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext,trad_key);

	printf("\n Decoded : %s\n",decoded);

}
