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


void encrypt(char *plaintext,char *ciphertext,int number_of_col)
{

	int rows = strlen(plaintext)/number_of_col;

	if(strlen(plaintext)%4!=0)
		rows++; 
	
	char zigzag[rows][number_of_col];

	int empty = ((rows+1)*(number_of_col) - strlen(plaintext))%4;

	for(int i = 0; i < strlen(plaintext); i++)
	{
		zigzag[i/number_of_col][i%number_of_col] = plaintext[i];
	}

	int k = 0;
	//printf("cols=%d",number_of_col-empty);
	for(int i = 0; i < number_of_col - empty; i++)
	{
		for(int j =0;j<rows;j++)
		{
			ciphertext[k] = zigzag[j][i];
			k++;
		}
	}

	for(int i = number_of_col - empty;i<number_of_col;i++)
	{
		for(int j =0;j<rows-1;j++)
		{
			ciphertext[k] = zigzag[j][i];
			k++;
		}
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);

	for (int i = 0; i < strlen(plaintext)/4; ++i)
	{
		printf("\n");
		for(int j =0;j<number_of_col;j++)
		{
			printf("%c",zigzag[i][j]);
		}
	}

	printf("\n");
	for(int i =0;i<number_of_col-empty;i++)
		printf("%c",zigzag[rows-1][i]);
	

}


void decrypt(char *decoded,char*ciphertext,int number_of_col)
{


	int rows = strlen(ciphertext)/number_of_col;

	if(strlen(ciphertext)%4!=0)
		rows++; 
	
	char zigzag[rows][number_of_col];

	int empty = ((rows+1)*(number_of_col) - strlen(ciphertext))%4;

	int k = 0;
	//printf("numberofcol-empty= %d rows = %d",number_of_col-empty,rows);
	for (int i = 0; i < number_of_col - empty; ++i)
	{
		for(int j = 0;j < rows; j++)
		{
			zigzag[j][i] = ciphertext[k];
			k++;
		}
	}

	for(int i = number_of_col - empty ; i < number_of_col; i++)
	{
		for (int j = 0; j < rows - 1; ++j)
		{
			zigzag[j][i] = ciphertext[k];
			k++;
		}
	}

	//printf("cols = %d row = %d",number_of_col,strlen(ciphertext)/4);
	for (int i = 0; i < strlen(ciphertext)/4; ++i)
	{
		printf("\n");
		for(int j =0;j<number_of_col;j++)
		{
			printf("%c",zigzag[i][j]);
		}
	}

	printf("\n");
	for(int i = 0;i<number_of_col-empty;i++)
		printf("%c",zigzag[rows-1][i]);


	k = 0;
	for(int i = 0; i < strlen(ciphertext)/4; i++)
	{
		for(int j =0;j<number_of_col;j++)
		{
			decoded[k] = zigzag[i][j];
			k++;
		}
	}

	for(int j =0;j<number_of_col;j++)
	{
			decoded[k] = zigzag[rows-1][j];
			k++;
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
	char ciphertext[MAX_LENGTH];
	int number_of_col = atoi(argv[2]);

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
	encrypt(plaintext,ciphertext,number_of_col);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext,number_of_col);

	printf("\n Decoded : %s\n",decoded);

}
