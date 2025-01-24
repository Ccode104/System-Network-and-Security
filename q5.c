#include<stdio.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	mpz_t a;
        mpz_init_set_ui(a,atoi(argv[1]));
	mpz_t x;
	mpz_init_set_ui(x,atoi(argv[2]));
	mpz_t p;
	mpz_init_set_ui(p,atoi(argv[3]));

	printf("Initial %ld^%ld(mod%ld)",mpz_get_ui(a),mpz_get_ui(x),mpz_get_ui(p));

	printf("Check if p is prime...\n");
	
	if(mpz_probab_prime(p)==2)
		printf("p is prime!\n");
	else
	{
		printf("p is not prime. We cannot apply fermat theorem.\n");
		return 0;
	}
	
	mpz_t res;
	mpz_init(res);

	if(mpz_cmp(x,p)<0)
	{
		mpz_powm(res,a,x,p);
		printf("The answer is %ld",mpz_get_ui(res));
		return 0;
	}
	
	mpz_t k;
	mpz_init(k);
	
	mpz_t r;
	mpz_init(r);
	mpz_div_qr(k,r,x,p);
		
	printf("We have (%ld^(%ld*%ld)+%ld)(mod %ld)\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(r),mpz_get_ui(p));
	printf("i.e ((%ld^%ld)^%ld)*(%ld^%ld)(mod %ld)\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p));
	printf("i.e( ((%ld^%ld)^%ld)(mod %ld))*((%ld^%ld)(mod %ld))\n",mpz_get_ui(a),mpz_get_ui(k),mpz_get_ui(p),mpz_get_ui(p),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p));
	printf("Using Fermat Theorem : a^p modp = a\n");
        printf("i.e %ld * (%ld^%ld)mod%ld",mpz_get_ui(a),mpz_get_ui(a),mpz_get_ui(r),mpz_get_ui(p));
	
	mpz_powm(res,a,r,p);
	mpz_mul(res,a,res);
	printf("Result = %ld",mpz_get_ui(res));
}	

	
