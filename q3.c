#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>

void main(int argc, char *argv[])
{
	mpz_t num,factor;

	mpz_init_set_ui(num ,atoi(argv[1]));
	mpz_init_set_ui(factor,2);
	
	while(mpz_cmp(factor,num) <= 0)
	{
		if(mpz_divisible_p(num,factor))
		{
			printf("%d ",mpz_get_ui(factor));
			mpz_div(num,num,factor);
		}
		else
		{
			mpz_add_ui(factor,factor,1);
		}
	}
}
