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

	for(int i = 0; i< 37;i++)
	{
		hash(key[i],key_hash);
		//printf("\n%dKey = %c hash = %d",i,key[i],mpz_get_ui(key_hash));
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


void encrypt(char *plaintext,char*ciphertext,mpz_t mapping[])
{
	
	int blocks = strlen(plaintext)/37 + 1;
	int empty = ((blocks)*37) - strlen(plaintext);

	//printf("e=%d",empty);
	int alphanumeric[37];

	for(int i = 0;i<37;i++)
		alphanumeric[i] = 0;

	mpz_t plaintext_hash;
	mpz_init(plaintext_hash);

	for(int i=0;i<strlen(plaintext);i++)	
	{
		hash(plaintext[i],plaintext_hash);
		alphanumeric[mpz_get_ui(plaintext_hash)] = 1;
	}

	//alphanumeric[10] = 1;
	if((alphanumeric[19]==1)||(alphanumeric[20]==1))
		alphanumeric[19]=alphanumeric[20]=1;
	

	// Append bogus characters
	int last_bogus = 0;
	int k = strlen(plaintext);
	mpz_t bogus;
	mpz_init(bogus);

	for(int i = 0; i < empty; i++)
	{
		while(alphanumeric[last_bogus]==1)
		{
			last_bogus++;
		}
		mpz_set_ui(bogus,last_bogus);
		plaintext[k] = character(bogus);
		alphanumeric[last_bogus] = 1; 
		k++;
	}

	plaintext[k] ='\0';

	
	k=0;
	//printf("p=%s l=%d k=%d b=%d",plaintext,strlen(plaintext),k,blocks);
	for(int i = 0;i<blocks;i++)
	{
		for(int j=0;j<37;j++)
		{
			ciphertext[mpz_get_ui(mapping[k])] = plaintext[k];
	//		printf("\n%d-place=%d-%c",k,mpz_get_ui(mapping[k]),ciphertext[mpz_get_ui(mapping[k])]);
			k++;
		}
	}
	ciphertext[37] = '\0';
	
	//printf("\n%d-cipher=%s",strlen(ciphertext),ciphertext);
	toUpper(ciphertext);
	/*for(int i=0;ciphertext[i]!='\0';i++)
		printf("\n%c",ciphertext[i]);
*/
}

void sort(mpz_t arr[])
{
	mpz_t temp;
	mpz_init(temp);

	for(int i =0;i<36;i++)
	{
		for(int j=0;j<36-i;j++)
		{
			if(mpz_cmp(arr[j],arr[j+1])>0)
			{
				mpz_set(temp,arr[j]);
				mpz_set(arr[j],arr[j+1]);
				mpz_set(arr[j+1],temp);
			}
		}
	}
}

void decrypt(char *decoded,char*ciphertext,mpz_t mapping[])
{
	mpz_t decrypt_mapping[37];

	for(int i=0;i<37;i++)
	{
		mpz_init(decrypt_mapping[i]);
	}
	for(int i=0;i<37;i++)
	{
		mpz_set_ui(decrypt_mapping[mpz_get_ui(mapping[i])],i);
	}

	
	//sort(decrypt_mapping);
	//for(int i=0;i<37;i++)
	//	printf("\n%d",mpz_get_ui(decrypt_mapping[i]));
	int blocks = strlen(ciphertext)/37;

	mpz_t ciphertext_hash;
	mpz_init(ciphertext_hash);

	
	int k=0;
	for(int i = 0;i < blocks;i++)
	{
		for(int j=0;j<37;j++)
		{
			decoded[mpz_get_ui(decrypt_mapping[j])] = ciphertext[k];
			k++;
		}
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
	trad_key[37]='\0';
	// The initialization is done
	
	char ciphertext[MAX_LENGTH];
	int length = strlen(plaintext);

	mpz_t mapping[37];
	for(int i=0;i<37;i++)
	{
		mpz_init(mapping[i]);
	}

	init_mapping(trad_key,mapping);
	/*for(int i=0;i<37;i++)
		printf("\n%d",mpz_get_ui(mapping[i]));
	*/
	//printf("\n Plaintext : %s",plaintext);
	encrypt(plaintext,ciphertext,mapping);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext,mapping);

	printf("\n Decoded : %s\n",decoded);

}
