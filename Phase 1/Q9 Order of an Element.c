#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc, char* argv[])
{
	mpz_t a,m,h,temp;
	mpz_inits(a,m,h,temp,NULL);

	mpz_set_ui(a,atoi(argv[1]));
	mpz_set_ui(m,atoi(argv[2]));

	// Check if gcd(a,m) == 1
	
	mpz_t gcd;
	mpz_init(gcd);

	mpz_gcd(gcd,a,m);

	if(mpz_cmp_ui(gcd,1)!=0)
		return;
	else
	{
		// Case 1 : m is prime
		if(mpz_probab_prime_p(m,50)==2)
		{
			// Apply Fermat theorem
			mpz_t p;
			mpz_init_set(p,m);

			mpz_sub_ui(p,p,1);

			mpz_t i;
			mpz_init_set_ui(i,1);

			mpz_set(temp,a);

			while(mpz_cmp(i,p)<=0)
			{
				if(mpz_cmp_ui(temp,1)==0)
				{
					printf("The order is %ld \n",mpz_get_ui(i));
					break;
				}
				else
				{
					mpz_mul(temp,temp,a);
					mpz_mod(temp,temp,m);
					mpz_add_ui(i,i,1);
				}
			}
		}
		// Case 2 : m is not prime
		else
		{
			mpz_t phi;
			mpz_init_set_ui(phi,1);
			
			mpz_t temp2;
			mpz_init(temp2);

			mp_bitcnt_t n;

			mpz_t i;
			mpz_init_set_ui(i,2);
			mpz_set(temp,m);

			while(mpz_cmp_ui(temp,1)!=0)
			{
				n = mpz_remove(temp,temp,i);
				
				//printf(" %ld ",mpz_get_ui(temp));
				if(n==0)
				{
					mpz_add_ui(i,i,1);
					continue;
				}
				mpz_pow_ui(temp2,i,n-1);
				mpz_submul(temp2,temp2,i);
				mpz_neg(temp2,temp2);
					
				mpz_mul(phi,phi,temp2);
			}  
	    		
			mpz_set_ui(i,1);
			mpz_set(temp,a);			
			while(mpz_cmp(i,phi)<=0)
			{
				if(mpz_cmp_ui(temp,1)==0)
				{	
					printf("The order is %ld \n",mpz_get_ui(i));	
		     			break;																									   }
				else
				{		
					mpz_mul(temp,temp,a);
					mpz_mod(temp,temp,m);
					mpz_add_ui(i,i,1);
				}					
			}
			
		}
	}
}



			
		
	
