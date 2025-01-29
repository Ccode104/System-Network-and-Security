#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	mpz_t a,x,b,m;
	mpz_inits(a,x,b,m,NULL);

	mpz_set_ui(a,atoi(argv[1]));
	mpz_set_ui(b,atoi(argv[2]));
	mpz_set_ui(m,atoi(argv[3]));

	mpz_t g;
	mpz_init(g);

	mpz_gcd(g,a,m);

	if((mpz_divisible_p(b,g) == 0)||(mpz_cmp_ui(g,1)>0))
	{
		printf("%ld",mpz_get_ui(g));
		return;
	}
	else
	{
		// x is MI of a
		mpz_invert(x,a,m);
		mpz_t i;
		mpz_init_set_ui(i,0);
		
		mpz_t soln;
		mpz_init_set(soln,x);

		mpz_t m_by_g;
		mpz_init(m_by_g);

		mpz_fdiv_q(m_by_g,m,g);

		while(mpz_cmp(i,g)<=0)
		{
			printf("%ld ",mpz_get_ui(soln));

			mpz_add(soln,soln,m_by_g);	
			mpz_add_ui(i,i,1);
		}
	}
}

