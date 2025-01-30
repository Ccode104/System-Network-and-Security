#include<stdio.h>
#include<stdlib.h>
#include<gmp.h>

void main(int argc, char* argv[])
{
	mpz_t m,temp;
	mpz_inits(m,temp,NULL);

	mpz_set_ui(m,atoi(argv[1]));

	// Check if m is 2,4 , prime^i or 2*prime^i for i!=2
	
	if((mpz_get_ui(m)==2)||(mpz_get_ui(m)==4))
	{
		// OK
	}
	else
	{
		mpz_t i;
		mpz_init_set_ui(i,3);
		
		mpz_set(temp,m);
		mp_bitcnt_t n;
		
		int flag=0;

		while(mpz_cmp(i,m)<=0)
		{
			n = mpz_remove(temp,temp,i);

			if(mpz_get_ui(temp)==2 || mpz_get_ui(temp)==1)
			{
				flag=1;
				break;
			}
			else
			{
				mpz_add_ui(i,i,2);
				mpz_set(temp,m);

			}
		}

		if(flag==0)
		{
			printf("No PRs!\n");
		}
	}
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
	    	
	mpz_t gcd;
	mpz_init(gcd);
	unsigned int arr[mpz_get_ui(phi)];
	
	for(int k =0;k<mpz_get_ui(phi);k++)
		arr[k]=0;

	mpz_set_ui(i,1);			
	while(mpz_cmp(i,phi)<=0)
	{
		mpz_gcd(gcd,i,m);

		if(mpz_cmp_ui(gcd,1)==0)
		{	
			mpz_t j;
			mpz_set_ui(j,2);

			while(mpz_cmp(j,phi)<=0)
			{
				if(mpz_divisible_p(phi,j)!=0)
				{
					mpz_powm(temp,i,j,m);
					if(arr[mpz_get_ui(temp)]==1)
						break;
					arr[mpz_get_ui(temp)]=1;
				}
				mpz_add_ui(j,j,1); // j is d(pow)
			}
			for(int k =0;k<mpz_get_ui(phi);k++)
				arr[k]=0;
			if(mpz_cmp(j,phi)>0)
			{
				printf("PR: %ld \n",mpz_get_ui(i));
			}
		}
		mpz_add_ui(i,i,1); // i belongs to RRSM
	}			
			
			
		
	
}



			
		
	
