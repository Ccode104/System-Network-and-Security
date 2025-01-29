#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	mpz_t a;
        mpz_init_set_ui(a,atoi(argv[1]));
	mpz_t x;
	mpz_init_set_ui(x,atoi(argv[2]));
	mpz_t p;
	mpz_init_set_ui(p,atoi(argv[3]));

	printf("Initial %ld^%ld(mod%ld)\n",mpz_get_ui(a),mpz_get_ui(x),mpz_get_ui(p));

	printf("Check if p is prime...\n");
	
	if(mpz_probab_prime_p(p,50)==2)
		printf("p is prime!\n");
	else
	{
		printf("p is not prime. We cannot apply fermat theorem.\n");
		return;
	}
	
	mpz_t res;
	mpz_init(res);

	if(mpz_cmp(x,p)<0)
	{
		mpz_powm(res,a,x,p);
		printf("The answer is %ld",mpz_get_ui(res));
		return;
	}
	
	mpz_t k;
	mpz_init(k);
	
	mpz_t r;
	mpz_t part1,part2;
	mpz_init(part1);
	mpz_init(part2);

	mpz_init(r);
	mpz_fdiv_qr(k,r,x,p);
		
	printf("We have (%ld^(%ld*%ld)+%ld)(mod %ld)\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(r),mpz_get_ui(p));
	printf("i.e ((%ld^%ld)^%ld)*(%ld^%ld)(mod %ld)\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p));
	printf("i.e( ((%ld^%ld)^%ld)(mod %ld))*((%ld^%ld)(mod %ld))(mod %ld)\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(p),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p),mpz_get_ui(p));
	printf("Using Fermat Theorem : a^p modp = a\n");
	mpz_powm(part1,a,k,p);
        printf("i.e %ld * (%ld^%ld)mod%ld\n",mpz_get_ui(part1),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p));
	
	mpz_powm(part2,a,r,p);
	mpz_mul(res,part1,part2);
	printf("Result = %ld\n",mpz_get_ui(res));
}	

	
