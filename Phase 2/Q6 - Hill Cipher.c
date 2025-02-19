#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define LENGTH_OF_KEY 26
#define MAX_LENGTH 100

mpz_t matrix_key[5][5];




void appendChar(char str[], char ch) {
	int len = strlen(str);
	str[len] = ch;  // Add the character at the next position
     	str[len + 1] = '\0';  // Null-terminate the string
}


#define MOD 37
#define SIZE 5

void determinant(mpz_t result, mpz_t matrix[SIZE][SIZE], int n) {
    mpz_t temp[SIZE][SIZE], subdet, sign, mod;
    
    // Initialize temporary matrix for minors
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            mpz_init(temp[i][j]);

    // Initialize variables
    mpz_init(subdet);
    mpz_init_set_ui(sign, 1);
    mpz_init_set_ui(mod, MOD);
    mpz_set_ui(result, 0); // result = 0 initially

    // Base case: Determinant of 1x1 matrix is just matrix[0][0] % MOD
    if (n == 1) {
        mpz_mod(result, matrix[0][0], mod);
        return;
    }

    // Expand along the first row
    for (int f = 0; f < n; f++) {
        int subi = 0;
        
        // Generate submatrix by skipping row 0 and column f
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == f) continue;
                mpz_set(temp[subi][subj], matrix[i][j]);
                subj++;
            }
            subi++;
        }

        // Compute determinant of the submatrix
        determinant(subdet, temp, n - 1);

        // Apply the formula: result += (matrix[0][f] * subdet * sign) % MOD
        mpz_mul(subdet, subdet, matrix[0][f]); // subdet *= matrix[0][f]
        mpz_mul(subdet, subdet, sign);         // subdet *= sign
        mpz_add(result, result, subdet);       // result += subdet
        mpz_mod(result, result, mod);          // result %= MOD (to avoid large values)

        // Flip sign: (-1)^column
        mpz_neg(sign, sign);
    }

    // Ensure determinant is positive in modulo space
    mpz_mod(result, result, mod);

    // Clear memory
    mpz_clear(subdet);
    mpz_clear(sign);
    mpz_clear(mod);
}


// Function to compute the modular inverse of a determinant using GMP
int modInverse(mpz_t inverse, mpz_t det, mpz_t mod) {
    return mpz_invert(inverse, det, mod) != 0; // Returns 1 if inverse exists, 0 otherwise
}

// Function to compute cofactor matrix
void cofactorMatrix(mpz_t matrix[SIZE][SIZE], mpz_t cofactors[SIZE][SIZE], int n) {
    mpz_t temp[SIZE][SIZE], subdet, sign;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            mpz_init(temp[i][j]);

    mpz_init(subdet);
    mpz_init(sign);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int subi = 0, subj = 0;
            for (int row = 0; row < n; row++) {
                if (row == i) continue;
                subj = 0;
                for (int col = 0; col < n; col++) {
                    if (col == j) continue;
                    mpz_set(temp[subi][subj], matrix[row][col]);
                    subj++;
                }
                subi++;
            }
            determinant(subdet, temp, n - 1);
            mpz_mul_si(cofactors[i][j], subdet, ((i + j) % 2 == 0 ? 1 : -1));

            mpz_t mod;
            mpz_init_set_ui(mod,MOD);
            mpz_mod(cofactors[i][j], cofactors[i][j], mod);
        }
    }

    mpz_clear(subdet);
    mpz_clear(sign);
}

// Function to transpose a matrix
void transpose(mpz_t matrix[SIZE][SIZE], mpz_t transposed[SIZE][SIZE], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mpz_set(transposed[j][i], matrix[i][j]);
        }
    }
}

// Function to compute modular inverse of a matrix mod MOD
int matrixModInverse(mpz_t matrix[SIZE][SIZE], mpz_t inverse[SIZE][SIZE]) {
    mpz_t det, det_inv, mod;
    mpz_init(det);
    mpz_init(det_inv);
    mpz_init_set_ui(mod, MOD);

    determinant(det, matrix, SIZE);
    if (!modInverse(det_inv, det, mod)) {
        printf("Matrix is not invertible under mod %d\n", MOD);
        return 0;
    }

    mpz_t cofactors[SIZE][SIZE], adjugate[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mpz_init(cofactors[i][j]);
            mpz_init(adjugate[i][j]);
        }
    }

    cofactorMatrix(matrix, cofactors, SIZE);
    transpose(cofactors, adjugate, SIZE);

    // Compute inverse matrix: (det_inv * adjugate) % MOD
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            mpz_mul(inverse[i][j], det_inv, adjugate[i][j]);
            mpz_mod(inverse[i][j], inverse[i][j], mod);
        }
    }

    mpz_clear(det);
    mpz_clear(det_inv);
    mpz_clear(mod);
    return 1;
}

// Function to print a matrix
void printMatrix(mpz_t matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            gmp_printf("%Zd ", matrix[i][j]);
        }
        printf("\n");
    }
}


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


void encrypt(mpz_t plaintext_matrix[5][5],mpz_t ciphertext_matrix[5][5],mpz_t matrix_key[5][5],int l)
{
	for (int i = 0; i < l; ++i)
	{
		for(int j=0;j<5;j++)
		{
			mpz_set_ui(ciphertext_matrix[i][j],0);
			for(int k=0;k<5;k++)
			{
				mpz_addmul(ciphertext_matrix[i][j],plaintext_matrix[i][k],matrix_key[k][j]);
			}
		}
	}

}


void decrypt(mpz_t decoded_matrix[5][5],mpz_t ciphertext_matrix[5][5],mpz_t key[5][5],int l)
{	

	mpz_t key_inverse[5][5];

	for(int i=0;i<5;i++)
    {
    	for(int j=0;j<5;j++)
    	{
    		mpz_init(key_inverse[i][j]);
    	}
    }

	matrixModInverse(key,key_inverse);

	for (int i = 0; i < l; ++i)
	{
		for(int j=0;j<5;j++)
		{
			mpz_set_ui(decoded_matrix[i][j],0);
			for(int k=0;k<5;k++)
			{
				mpz_addmul(decoded_matrix[i][j],ciphertext_matrix[i][k],key_inverse[k][j]);
			}
		}
	}


}

void makeMatrix(mpz_t matrix[5][5],char *key,int l)
{
	mpz_t key_hash;
	mpz_init(key_hash);

	for(int i = 0;i < 5;i++)
	{
		for(int j=0;j<5;j++)
		{
			mpz_init(matrix[i][j]);
		}
	}

	for(int i = 0;i <= l;i++)
	{
		for(int j=0;j<5;j++)
		{
			hash(key[(i*5)+j],key_hash);
			mpz_set(matrix[i][j],key_hash); 
		}
	}
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
	

	printf("\nReading keys from output.txt ....");
	char buffer[MAX_LENGTH];
	char trad_key[LENGTH_OF_KEY];
	char k_roll1;
	char k_roll2;

	FILE *file = fopen("output.txt","r");

	fgets(trad_key,LENGTH_OF_KEY,file);

	printf("\n Key is   %s  ",trad_key);
	fgetc(file);
	k_roll1 = fgetc(file);
	fgetc(file);
	k_roll2 = fgetc(file);

	//printf("\n Key is  %c ",k_roll1);
	// The initialization is done
	
	char ciphertext[MAX_LENGTH];
	int length = strlen(plaintext);

	makeMatrix(matrix_key,trad_key,5);

	int l = strlen(plaintext) / 5;
	int rem = strlen(plaintext) % 5 == 0 ? 0 : (l+1)*5 - strlen(plaintext);

	int bogus[37];

	for(int i = 0; i < 37;i++)
	{
		bogus[i] = 1;
	}

	mpz_t plaintext_hash;
	mpz_init(plaintext_hash);

	for(int i = 0;i<strlen(plaintext);i++)
	{
		hash(plaintext[i],plaintext_hash);
		bogus[mpz_get_ui(plaintext_hash)] = 0;
	}

	int j = 11;
	mpz_t c;
	mpz_init(c);

	for(int i=0;i<rem;i++)
	{
		while(j<37)
		{
			if(bogus[j]==1)
			{
				bogus[j]=0;
				j++;
				break;
			}
			j++;
		}

		mpz_init_set_ui(c,j-1);
		appendChar(plaintext,character(c));
	}
	mpz_t plaintext_matrix[5][5],ciphertext_matrix[5][5],decoded_matrix[5][5];

	for(int i=0;i<5;i++)
    {
    	for(int j=0;j<5;j++)
    	{
    		mpz_init(plaintext_matrix[i][j]);
    		mpz_init(ciphertext_matrix[i][j]);
    		mpz_init(decoded_matrix[i][j]);
    	}
    }


	makeMatrix(plaintext_matrix,plaintext,l);

	printf("\n Plaintext : ");
	for(int i=0;i<=l;i++)
    {
    	for(int j=0;j<5;j++)
    	{
    		printf("%c",character(plaintext_matrix[i][j]));
    	}
    }

    if(rem != 0)
    	l++;
	encrypt(plaintext_matrix,ciphertext_matrix,matrix_key,l);

	printf("\n Ciphertext : ");
	for(int i=0;i<l;i++)
    {
    	for(int j=0;j<5;j++)
    	{
    		printf("%c",character(ciphertext_matrix[i][j]));
    	}
    }

	
	
	
	toLowerCase(ciphertext);

	decrypt(decoded_matrix,ciphertext_matrix,matrix_key,l);

	printf("\n Decoded : ");

	for(int i=0;i<l;i++)
    {
    	for(int j=0;j<5;j++)
    	{
    		printf("%c",character(decoded_matrix[i][j]));
    	}
    }

    printf("\n");
 

    
}
