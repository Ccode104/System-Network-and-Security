#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


#define NO_OF_CHAR 37

void toLowerCase(char str[]) 
{
	for (int i = 0; str[i] != '\0'; i++) 
	{
		str[i] = tolower(str[i]);
	}
}

void appendChar(char str[], char ch) {
	int len = strlen(str);
	str[len] = ch;  // Add the character at the next position
     	str[len + 1] = '\0';  // Null-terminate the string
}

void removeSpaces(char *str) 
{

	int i, j = 0;
	int len = strlen(str);
		    
		    
	// Iterate through the string
		         
	for (i = 0; i < len; i++) 
	{
		               
	    if (str[i] != ' ') 
	    {
		                             
		    str[j++] = str[i];  // Copy non-space characters
		                                     
	    }
		                                         
	}
		                                             
	str[j] = '\0';  // Null-terminate the modified string
		                                             
}
void main(int argc, char *argv[])
{
	
	int length = strlen(argv[1]);
	int roll_length = strlen(argv[2]);

	char name[length + 1];
	char roll_no[roll_length + 1];

	int is_IandJ_distinct = atoi(argv[3]);

	strcpy(name,argv[1]);
	strcpy(roll_no,argv[2]);

	//Convert to lowercase
	toLowerCase(name);
	toLowerCase(roll_no);
	
	// Remove spaces for the traditional cipher
	removeSpaces(name);

	int alphabet[NO_OF_CHAR];

	for(int i=0;i<11;i++)
		alphabet[i]=1;

	for (int i = 11; i < NO_OF_CHAR; ++i)
	{
		alphabet[i]=0;
	}

	int no_of_distinct = 0;
	char key[26];
	int hash = 0;
	int length_of_key = 0;

	for(int i=0;i<length;i++)
	{
		if(isdigit(name[i]))
			hash = (int)(name[i] -'0');
		else if(isalpha(name[i]))
			hash = (int)(name[i] -'a') + 11;
		else
			hash = 10;
		hash = hash % NO_OF_CHAR;

		if(alphabet[hash] == 1)
		{
			// Repeated a letter
			// Do not consider
		}
		else
		{

			// Distinct
			// Consider

			if((is_IandJ_distinct == 0)&&((hash == 19)||(hash == 20)))
				alphabet[19] = alphabet[20] = 1;
			else
				alphabet[hash]=1;
			
			appendChar(key,name[i]);
			length_of_key++;

			if(length_of_key==25)
				break;

		}
	}



	for (int i = 11; i < NO_OF_CHAR; ++i)
	{
		printf(" %d ",alphabet[i]);
	}
	
	if(length_of_key == 25)
	{
		// Generated the key
	}
	else
	{
		// Look for remaining characters
		
		for(int i=0;i<length_of_key;i=(i+1)%length_of_key)
		{
			
			if(isdigit(name[i]))
				hash = (int)(key[i] -'0');
			else if(isalpha(key[i]))
				hash = (int)(key[i] -'a') + 11;
			else
				hash = 10;
			hash = hash % NO_OF_CHAR;
			while(alphabet[hash] == 1)
			{
				// Repeated a letter
				// Increment again
				hash = (hash + 1)% NO_OF_CHAR;
			}
			
				// Distinct
				// Consider

				for (int i = 11; i < NO_OF_CHAR; ++i)
				{
					printf(" %c-%d ",(char)(i - 11 +'a'), alphabet[i]);
				}

				alphabet[hash]=1;
			
				if(hash == 10)	
					appendChar(key,' ');
				else if(hash <10)
					appendChar(key,(char)(hash + '0'));
				else
					appendChar(key,(char)(hash - 11 + 'a'));
				
				length_of_key++;

				printf("\n%s %ld",key,length_of_key);

				
				if(((length_of_key==25)&&(is_IandJ_distinct==0))||((length_of_key==26)&&(is_IandJ_distinct==1)))
					break;
				
			
		}

		if(is_IandJ_distinct == 1)
		{
			appendChar(key,' ');
			for(int i = 0; i < 10 ;i++)
				appendChar(key,(char)(i + '0'));
		}
	}



	FILE *file = fopen("output.txt", "w");  // Open in write 						
   	if (file == NULL) {
							    		
		printf("Error opening file!\n");				                        
		return;
							                 
	}
							    						                                                             
	fputs(key, file);  // Write Key
	fputc('\n',file);
	fputc(roll_no[roll_length-1],file);
						
	fputc('\n',file);
	fputc(roll_no[roll_length-2],file);

							    
							                                         
	fclose(file);  // Close the file
	printf("\n Key = %s\n",key);
							  	                                           
	printf("Key Generated successfully.\n");
}
							    
							                                                 
							                   
							    


			

