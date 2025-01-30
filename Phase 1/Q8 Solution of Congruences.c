#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	int n = (argc-1)/2;

	mpz_t a[n];
	mpz_t m[n];

	for(int i = 0;i < n;i++)
	{
		mpz_init_set_ui(a[i],atoi(argv[i+1]));
		mpz_init_set_ui(m[i],atoi(argv[n+i+1]));
		printf("%ld %ld ",mpz_get_ui(a[i]),mpz_get_ui(m[i]));
	}

	// Check if all m are co-prime
	
	mpz_t gcd;
	mpz_init(gcd);
	
	int flag = 0;
	for(int i = 0;i < n; i++)
	{
		for(int j=i+1;j<n;j++)
		{
			mpz_gcd(gcd,m[i],m[j]);

			printf("\n %ld ",mpz_get_ui(gcd));
			
			if(mpz_cmp_ui(gcd,1)!=0)
			{
				flag = 1;
				break;
			}
		}
	}

	if(flag==1)
	{
		// relatively not prime
		return;
	}
	else
	{
		printf("asd");
		mpz_t M_prod;
		mpz_init_set_ui(M_prod,1);

		for(int i=0;i<n;i++)
		{
			mpz_mul(M_prod,M_prod,m[i]);
		}

		mpz_t M[n];

		for(int i=0;i<n;i++)
		{
			mpz_init(M[i]);
			mpz_fdiv_q(M[i],M_prod,m[i]);
		}
		mpz_t inv;
		mpz_init(inv);
		
		mpz_t x;
		mpz_init(x);

		mpz_t temp;
		mpz_init_set_ui(temp,1);

		for(int i=0;i<n;i++)
		{
			mpz_invert(inv,M[i],m[i]);
			mpz_mul(temp,M[i],inv);
			mpz_mul(temp,temp,a[i]);
			mpz_add(x,x,temp);
			mpz_set_ui(temp,0);
		}

		mpz_mod(x,x,M_prod);

		printf("x = %ld",mpz_get_ui(x));
	}
}

			

