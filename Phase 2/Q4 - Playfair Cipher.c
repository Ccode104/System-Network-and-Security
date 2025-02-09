#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define LENGTH_OF_KEY 26
#define MAX_LENGTH 100

char key_matrix[5][5];

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


void makeMatrix(char *key,char key_matrix[5][5])
{
	/*
	Pattern : (0,n-1),(0,n-2),(1,n-1),(0,n-3),(1,n-2),(2,n-1)....
	We have 2*n diagonals
	We fill each diagonal starting from top right corner to bottom left corner
	*/

	toLowerCase(key);
	int n = 5,row,col;
	int index = 0;
/*
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			mpz_init(key_matrix[i][j]);
		}
	}
*/
	int col_start = n - 1; 
	row = 0;
	col = n - 1;
	for(int i=n-1;i>=0;i--)
	{
		// We have n-i elements

		for(int j = 0;j < n - i;j++)
		{
			
			
			//printf("\nrow = %d col = %d",row,col);
			//hash(key[index],key_hash);
			//mpz_set(key_matrix[row][col],key_hash);

			key_matrix[row][col] = key[index];
			printf("%c",key[index]);
			row ++;
			col ++;
			index++;
		} 
		col_start --;
		col = col_start; 
		row = 0;
		
	}

	int row_start = 1;
	col = 0;
	row = 1;

	for(int i = 1;i < n;i++)
	{
		// We have n-i elements
		
		for(int j = 0;j < n - i;j++)
		{	
			//printf("\nrow = %d col = %d",row,col);
			//hash(key[index],key_hash);
			//mpz_set(key_matrix[row][col],key_hash);
			key_matrix[row][col] = key[index];
			printf("%c",key[index]);
			row ++;
			col ++;
			index++;
		} 

		row_start++;
		row = row_start;
		col = 0;
	
	
	}

	
}

void find(char c,mpz_t row,mpz_t col,char key_matrix[5][5])
{
	for (int i = 0; i < 5; ++i)
	{
		//printf("\n");
		for (int j = 0; j < 5; ++j)
		{
			//printf(" %c ",key_matrix[i][j]);
			if(key_matrix[i][j] == c)
			{
				mpz_set_ui(row,i);
				mpz_set_ui(col,j);
				break;
			}
		}
	}
}
void encrypt(char *plaintext,char*ciphertext,char* key)
{

	mpz_t plaintext_hash;


	mpz_init(plaintext_hash);

	// Find potential bogus characters

	int alphabets[37];

	for(int i =0;i<25;i++)
	{
		hash(plaintext[i],plaintext_hash);
		alphabets[mpz_get_ui(plaintext_hash)] = 1;
		printf(" %d ",mpz_get_ui(plaintext_hash));
	}

	if((alphabets[19]==1)||(alphabets[20]==1))
		alphabets[19]=alphabets[20]=1;

	char bogus;

	mpz_t k;
	mpz_init(k);
	mpz_set_ui(k,11);

	while(alphabets[mpz_get_ui(k)]!=0)
	{
		mpz_add_ui(k,k,1);
	}

	bogus = character(k);
	printf("\n bogus=%c ",bogus);
	
	// Add bogus character in between duplicates

	char plaintext_copy[1 + strlen(plaintext)];
	strcpy(plaintext_copy,plaintext);
	int d = 0;

	if(strlen(plaintext)%2==0)
	{
		for(int i = 0;i < strlen(plaintext_copy) - 1;i = i + 2)
		{
			printf("\n Plaintext = %s",plaintext);
			if((plaintext_copy[i] == plaintext_copy[i+1])||(plaintext_copy[i]=='i' && plaintext_copy[i+1] == 'j')||(plaintext_copy[i]=='j' && plaintext_copy[i+1] == 'i'))
			{
				// Add bogus in between
				plaintext[d] = plaintext_copy[i];
				plaintext[d+1] = bogus;
				plaintext[d+2] = plaintext_copy[i+1];
				d=d+3;
			}
			else
			{

				plaintext[d] = plaintext_copy[i];
				plaintext[d+1] = plaintext_copy[i+1];
				d=d+2;
			}

		}
		plaintext[d] ='\0';
	}
	else{

		for(int i = 0;i < strlen(plaintext_copy) - 2;i = i + 2)
		{
			printf("\n Plaintext = %s",plaintext);
			if(plaintext_copy[i] == plaintext_copy[i+1])
			{
				// Add bogus in between
				plaintext[d] = plaintext_copy[i];
				plaintext[d+1] = bogus;
				plaintext[d+2] = plaintext_copy[i+1];
				d=d+3;
			}
			else
			{
				plaintext[d] = plaintext_copy[i];
				plaintext[d+1] = plaintext_copy[i+1];
				d=d+2;
			}

		}
		plaintext[d+1] ='\0';
	}

	


	// Check for even length

	if(strlen(plaintext)%2!=0)
	{
		// Append a bogus character

		// Check if current bogus is same as last char or not

		mpz_t bogus_hash;
		mpz_init(bogus_hash);

		if(plaintext[strlen(plaintext)-1] == bogus)
		{
			hash(bogus,bogus_hash);
			for(int i = 1 + mpz_get_ui(bogus_hash);i < 37;i++)
			{
				if(alphabets[i]==0)
				{
					bogus = character(bogus_hash);
					break;
				}
			}
		}
		
		plaintext[1 + strlen(plaintext)] = '\0';
		plaintext[strlen(plaintext)] = bogus;

	}
	//printf(" \n p=%s",plaintext);
	mpz_t row1,col1,col2,row2;
	mpz_inits(row1,row2,col2,col1,NULL);
	
	for(int i = 0; i < strlen(plaintext) - 1; i = i + 2)
	{
		
		// Take pairs

		// Check if in same row

		

		find(plaintext[i],row1,col1,key_matrix);
		find(plaintext[i+1],row2,col2,key_matrix);
//printf("\n 1%c %d %d 2%c %d %d",plaintext[i],row1,col1,plaintext[i+1],row2,col2);

		if(mpz_cmp(row1,row2)==0)
		{
			mpz_add_ui(col1,col1,1);
			mpz_mod_ui(col1,col1,5);
			mpz_add_ui(col2,col2,1);
			mpz_mod_ui(col2,col2,5);
			ciphertext[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col1)];
			ciphertext[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col2)];
		}
		else if(mpz_cmp(col1,col2)==0)
		{
			mpz_add_ui(row1,row1,1);
			mpz_mod_ui(row1,row1,5);
			mpz_add_ui(row2,row2,1);
			mpz_mod_ui(row2,row2,5);
			ciphertext[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col1)];
			ciphertext[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col2)];
		}
		else
		{
			ciphertext[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col2)];
			ciphertext[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col1)];
		}

		
		
		
		
	}

	ciphertext[strlen(plaintext)] = '\0';
	
	toUpper(ciphertext);

}


void decrypt(char *decoded,char*ciphertext,char* key)
{
	mpz_t ciphertext_hash;

	mpz_init(ciphertext_hash);
	
	mpz_t row1,col1,col2,row2;
	mpz_inits(row1,row2,col2,col1,NULL);

	for(int i = 0; i < strlen(ciphertext) - 1; i = i + 2)
	{
		
		// Take pairs

		// Check if in same row

		find(ciphertext[i],row1,col1,key_matrix);
		find(ciphertext[i+1],row2,col2,key_matrix);
//printf("\n 1%c %d %d 2%c %d %d",plaintext[i],row1,col1,plaintext[i+1],row2,col2);


		if(mpz_cmp(row1,row2)==0)
		{
			mpz_sub_ui(col1,col1,1);
			mpz_mod_ui(col1,col1,5);
			mpz_sub_ui(col2,col2,1);
			mpz_mod_ui(col2,col2,5);
			decoded[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col1)];
			decoded[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col2)];
		}
		else if(mpz_cmp(col1,col2)==0)
		{
			mpz_sub_ui(row1,row1,1);
			mpz_mod_ui(row1,row1,5);
			mpz_sub_ui(row2,row2,1);
			mpz_mod_ui(row2,row2,5);
			decoded[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col1)];
			decoded[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col2)];
		}
		else
		{
			decoded[i] = key_matrix[mpz_get_ui(row1)][mpz_get_ui(col2)];
			decoded[i+1] = key_matrix[mpz_get_ui(row2)][mpz_get_ui(col1)];
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
	printf("   %s  ",trad_key);
	k_roll1 = fgetc(file);
	fgetc(file);
	k_roll2 = fgetc(file);

	printf("\n Key is  %c ",k_roll1);
	// The initialization is done
	

	char ciphertext[MAX_LENGTH];
	int length = strlen(plaintext);
	
	makeMatrix(trad_key,key_matrix);

	encrypt(plaintext,ciphertext,trad_key);

	printf("\n Plaintext : %s",plaintext);
	printf("\n Ciphertext : %s",ciphertext);
	
	toLowerCase(ciphertext);

	decrypt(decoded,ciphertext,trad_key);

	printf("\n Decoded : %s\n",decoded);

	for (int i = 0; i < 5; i++)
	{
		printf("\n %d  ",i);
		for (int j = 0; j < 5; j++)
		{
			printf("%c",key_matrix[i][j]);
			
		}
	}

}
