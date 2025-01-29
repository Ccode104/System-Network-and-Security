#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

void main(int argc,char* argv[])
{

    int i = argc; // number of integers
    mpz_t *integers; // store the integers
    integers =(mpz_t*)malloc(sizeof(mpz_t)*argc);

    for(int n = 0 ; n < argc ; n++)
    {
        mpz_init(integers[n]);
    }

    int flag = 0; // 1 if a divisor is a common divisor

    mpz_t common_divisors[argc]; // list of common divisors
    
    for(int n = 0 ; n < argc ; n++)
    {
        mpz_init(common_divisors[n]);
    }

    mpz_set_ui(common_divisors[0],1);

    int no_of_common_divisors = 0; // number of common divisors

    mpz_t divisor;
    mpz_init(divisor);
    mpz_set_ui(divisor,2); // 1 is a trivial common divisor
 
    no_of_common_divisors ++ ;

    // Store the integers
    i = i - 1;
    while(i > 0)
    {
        mpz_set_ui(integers[i],argv[i][0] - '0');
        i = i - 1;
    } 

    argc -- ;

    mpz_t min;
    mpz_init(min);
    mpz_set(min , integers[0]); // divisor <= max always to be a common_divisor

    // Find the min
    for(int k = 0;k < argc;k++)
    {
        if(mpz_get_ui(min) < mpz_get_ui(integers[k]))
        {
            mpz_set(min,integers[k]);
        }
    }

    // Find common divisors
    while(mpz_get_ui(divisor) <= mpz_get_ui(min))
    {   
        flag = 1;
        for(int j = 0;j < argc;j++)
        {
            if(mpz_get_ui(integers[j]) % mpz_get_ui(divisor) == 0)
            {
                // continue
            }
            else
            {
                flag = 0;
                break;
            }
        }
        
        if(flag == 1)
        {
            mpz_set(common_divisors[no_of_common_divisors],divisor);
            no_of_common_divisors ++;
        }

        mpz_add_ui(divisor,divisor,1);
    }

    printf("Common divisor are : "); 
    for(int k = 0;k < no_of_common_divisors;k++)
    {
        printf("%ld ",mpz_get_ui(common_divisors[k]));
    }
}
