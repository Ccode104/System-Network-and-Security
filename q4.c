#include<stdio.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	mpz_t num;
        mpz_init_set_ui(num,atoi(argv[1]));

	mpz_t i;
	
	mpz_t gcd;
	
	mpz_init(gcd);
	int j = 1;
	
	printf("1 ");
	for(mpz_set_ui(i,2);mpz_cmp(i,num)<0;mpz_add_ui(i,i,1))
	{
		mpz_gcd(gcd,num,i);
		if(mpz_cmp_ui(gcd,1)==0)
		{
			printf("%ld ",mpz_get_ui(i));
			j++;
		}
	}

	printf("\n Euler Quotient = %d ",j);
}
