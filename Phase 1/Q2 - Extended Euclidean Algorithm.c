#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc,char * argv[])
{
	mpz_t a,b;

	mpz_init(a);
	mpz_init(b);

	mpz_set_ui(a,atoi(argv[1]));
	mpz_set_ui(b,atoi(argv[2]));

	mpz_t r1,r2,r,t1,t2,s1,s2,s,t,q;

	mpz_inits(r1,r2,r,t1,t2,s1,s2,s,t,q,NULL);

	mpz_set(r1,a);
	mpz_set(r2,b);
	mpz_set_ui(s1,1);
	mpz_set_ui(t2,1);

	printf("q r1 r2 r s1 s2 s t1 t2 t \n");

	do
	{
		printf("%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",mpz_get_ui(q),mpz_get_ui(r1),mpz_get_ui(r2),mpz_get_ui(r),mpz_get_ui(s1),mpz_get_ui(s2),mpz_get_ui(s),mpz_get_ui(t1),mpz_get_ui(t2),mpz_get_ui(t));

		mpz_cdiv_q(q,r1,r2);
		mpz_mul(r,r2,q);
		mpz_sub(r,r1,r);

		mpz_set(r1,r2);
		mpz_set(r2,r);

		mpz_mul(t,t2,q);
		mpz_sub(t,t1,t);

		mpz_set(t1,t2);
		mpz_set(t2,t);

		mpz_mul(s,s2,q);
		mpz_sub(s,s1,t);

		mpz_set(s1,s2);
		mpz_set(s2,s);

		
	}while(mpz_get_ui(r)!=0);

	printf("GCD is : %ld  x = %ld  y = %ld ",mpz_get_ui(r1),mpz_get_ui(s1),mpz_get_ui(t1));

}
