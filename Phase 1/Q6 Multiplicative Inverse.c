#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc,char *argv[])
{
	mpz_t gcd;
	mpz_init(gcd);

	mpz_t num;
	mpz_init_set_ui(num,atoi(argv[1]));

	mpz_t mod_val;
	mpz_init_set_ui(mod_val,atoi(argv[2]));

	/* void mpz_gcdext (mpz t g, mpz t s, mpz t t, const mpz t a, const mpz t b) [Function]
	 * Set g to the greatest common divisor of a and b, and in addition set s and t to coefficients
	 * satisfying as + bt = g. The value in g is always positive, even if one or both of a and b
	 * are negative (or zero if both inputs are zero). The values in s and t are chosen such that
	 * normally, |s| < |b|/(2g) and |t| < |a|/(2g), and these relations define s and t uniquely. There
	 * are a few exceptional cases:
	 * If |a| = |b|, then s = 0, t = sgn(b).
	 * Otherwise, s = sgn(a) if b = 0 or |b| = 2g, and t = sgn(b) if a = 0 or |a| = 2g.
	 * In all cases, s = 0 if and only if g = |b|, i.e., if b divides a or a = b = 0.
	 * If t or g is NULL then that value is not computed. */


	// Calculate the gcd(num,mod_val)
	
	mpz_t s,t;
	mpz_inits(s,t,NULL);

	mpz_gcdext(gcd,s,t,num,mod_val);
		
	printf("%ld %ld\n",mpz_get_ui(s),mpz_get_ui(t));
	if(mpz_cmp_ui(gcd,1)<=0)
	{
		printf("The multiplicative inverse of %ld (mod %ld) is %ld (mod %ld)\n",mpz_get_ui(num),mpz_get_ui(mod_val),mpz_get_ui(s),mpz_get_ui(mod_val));	      }
	else
	{
		printf("The multiplicative inverse does not exist!\n");
	}
}


