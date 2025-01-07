#include "for_all.h"
#include <stdio.h>
#include <ctype.h>  // For isdigit
#include <stdlib.h> // For atoi, rand, srand
#include <time.h>   // For time, ctime


/***************************************************************
 * return 0 if user is entering entity other than integer      *
 * return Integer entered bu user if user is entering integer  *
 ***************************************************************/

int get_integer(){ 
    char local_choice[30];
    scanf(" %29[^\n]s", local_choice);   // space before %29[^\n]s to clear any new line // or we can use fflush(stdin);

    for(int k=0 ;local_choice[k] != '\0'; k++)
    {
        if(!isdigit(local_choice[k]))
        {
            printf("\n\n-----Choose an Integer-----\n\n");
            return 0;
        }
    }
    int num = atoi(local_choice);       //changes str to int
    if(num == 0)
    {
        printf("\n--------Element cannot be 0--------\n");   
        return 0;
    }
    return num;
}

unsigned long long get_unsignedlonglong() {
	char local_choice[50];
	scanf(" %49[^\n]s", local_choice);   // space before %49[^\n]s to clear any new line // or we can use fflush(stdin);
	trimWhitespace(local_choice);

	for(int k=0 ; local_choice[k] != '\0'; k++)
	{
		if(!isdigit(local_choice[k]))
		{
			printf("\n\n-----Choose an Integer-----\n\n");
			return 0;
		}
	}
	unsigned long long num = strtoull(local_choice, NULL , 10);;       //changes str to int
	if(num == 0)
	{
		printf("\n--------Element cannot be 0--------\n");
		return 0;
	}
	return num;
}


int generate_number(int n) {
	srand(time(NULL));     // Seed the random number generator
	int number = 0;
	// Ensure n is a positive integer
	if (n <= 0) {
		return -1;  // Invalid input, return error code
	}
	// Generate a random number with n digits
	for (int i = 0; i < n; i++) {
		// Generate a random digit between 1 and 9
		int digit = rand() % 9 + 1;
		number = number * 10 + digit;
	}
	return number;
}



void trimWhitespace(char *str) {
	int start = 0;
	int end = 0;

	// Find the index of the first non-whitespace character
	while (isspace(str[start])) {
		start++;
	}

	// Find the index of the last non-whitespace character
	end = start;
	while (str[end] != '\0') {
		end++;
	}
	end--;  // Move to the last valid character

	// Remove trailing whitespace
	while (end >= start && isspace(str[end])) {
		end--;
	}

	// Shift the string to the front, and terminate it properly
	int i;
	for (i = 0; start + i <= end; i++) {
		str[i] = str[start + i];
	}
	str[i] = '\0';  // Null-terminate the string
}

void get_time(char *t)
{
	// Get the current time
	time_t current_time = time(NULL);

	// Check if time() returned an error
	if (current_time == ((time_t)-1)) {
		printf("Error getting the current time.\n");
		return;
	}

	// Convert the time to a string
	char *time_str = ctime(&current_time);

	// Check if ctime() returned NULL
	if (time_str == NULL) {
		printf("Error converting time to string.\n");
		return;
	}

	// Copy the string into the provided buffer
	snprintf(t, 50, "%s", time_str); // Use snprintf to safely copy
}


int cnt_digits(int num)
{
	int cnt=0;
	while(num!=0)
	{
		num/=10;
		cnt+=1;
	}
	return cnt;
}

void print_space(int n)
{
	for(int i=0; i<n ; i++)
	{
		printf(" ");
	}
}

