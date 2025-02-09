#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define LENGTH_OF_KEY 38
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


void shift_mapping(mpz_t mapping[],int shift)
{
	mpz_t temp[37];

	for(int i=0;i<37;i++)
		mpz_init(temp[i]);
		
	
	for(int i = 0; i< 37;i++)
	{
		mpz_set(temp[(i+1)%37],mapping[i]);
	}

	for(int i = 0; i< 37;i++)
	{
		mpz_set(mapping[i],temp[i]);
	}



	// We get the the mapping : 
}

void init_mapping(char* key,mpz_t mapping[])
{
	mpz_t key_hash,map_val;
	mpz_inits(key_hash,map_val,NULL);

	for(int i = 0; i< strlen(key);i++)
	{
		hash(key[i],key_hash);
		mpz_set(mapping[i],key_hash);
	}

	// We get the the mapping : 
}



void rotor_mapping(char c,mpz_t hash_val,mpz_t mapping[])
{
	mpz_t key_hash;
	mpz_init(key_hash);

	hash(c,key_hash);
	mpz_set(hash_val,mapping[mpz_get_ui(key_hash)]);
}



void toUpper(char *str)
{
	for(int i=0;str[i]!='\0';i++)
		str[i] = toupper(str[i]);
}


void encrypt(char *plaintext,char*ciphertext,char *key)
{
	
	mpz_t rotor_mapping_hash,mapping[37];
	mpz_init(rotor_mapping_hash);

	for (int i = 0; i < 37; ++i)
	{
		mpz_init(mapping[i]);
	}
	
	init_mapping(key,mapping);

	for(int i=0;i<37;i++)
		printf("%c",character(mapping[i]));

	for(int i = 0; i < strlen(plaintext); i++)
	{
		rotor_mapping(plaintext[i],rotor_mapping_hash,mapping);
		ciphertext[i] = (char)(character(rotor_mapping_hash));
		shift_mapping(mapping,1);
		for(int i=0;i<37;i++)
			printf("%c",character(mapping[i]));	
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);

}


void decrypt(char *decoded,char*ciphertext,char *key)
{
	mpz_t mapping[37],ciphertext_hash;

	mpz_init(ciphertext_hash);

	for (int i = 0; i < 37; ++i)
	{
		mpz_init(mapping[i]);
	}
	
	init_mapping(key,mapping);
	mpz_t k; 
	mpz_init(k);

	for(int i = 0; i < strlen(ciphertext); i++)
	{
		hash(ciphertext[i],ciphertext_hash);
		mpz_set_ui(k,0);
		while(mpz_cmp(mapping[mpz_get_ui(k)],ciphertext_hash)!=0)
			mpz_add_ui(k,k,1);
		decoded[i] = (char)(character(k));
		shift_mapping(mapping,1);
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
	k_roll1 = fgetc(file);
	fgetc(file);
	k_roll2 = fgetc(file);

	printf("\n Key is  %s ",trad_key);
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
