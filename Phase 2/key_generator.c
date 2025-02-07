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

void removeSpaces(char *str) {
	    int i, j = 0;
	        int len = strlen(str);
		    
		    // Iterate through the string
		         for (i = 0; i < len; i++) {
		                 if (str[i] != ' ') {
		                             str[j++] = str[i];  // Copy non-space characters
		                                     }
		                                         }
		                                             str[j] = '\0';  // Null-terminate the modified string
		                                             }
void main(int argc, char *argv[])
{

	char name[argc];

	strcpy(name,argv[1]);
	
	//Convert to lowercase
	toLowerCase(name);
	removeSpaces(name);

	int length = strlen(name);

	int alphabet[NO_OF_CHAR];

	for(int i=0;i<NO_OF_CHAR;i++)
		alphabet[i]=0;

	int no_of_distinct = 0;
	char key[25];
	int hash = 0;
	char toAppend[2];
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
			alphabet[hash]=1;
			
			appendChar(key,name[i]);
			length_of_key++;

			if(length_of_key==25)
				break;

		}
	}



	
	if(length_of_key == 25)
	{
		// Generated the key
	}
	else
	{
		// Look for remaining characters
		
		for(int i=0;i<length;i++)
		{
			
			if(isdigit(name[i]))
				hash = (int)(name[i] -'0');
			else if(isalpha(name[i]))
				hash = (int)(name[i] -'a') + 11;
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
				alphabet[hash]=1;
			
				if(hash == 10)	
					appendChar(key,' ');
				else if(hash <10)
					appendChar(key,(char)(hash + '0'));
				else
					appendChar(key,(char)(hash - 11 + 'a'));
				length_of_key++;

				if(length_of_key==25)
					break;
				
			
		}
	}



	FILE *file = fopen("output.txt", "w");  // Open in write 						
   	if (file == NULL) {
							    		
		printf("Error opening file!\n");				                        
		return;
							                 
	}
							    						                                                             
	fputs(key, file);  // Write Key
							    
							                                         
	fclose(file);  // Close the file
							                                             
	printf("Key Generated successfully.\n");
}
							    
							                                                 
							                   
							    


			

