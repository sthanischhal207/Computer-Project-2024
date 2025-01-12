#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "for_all.h"

struct Human_data {
	char type;              // u ->User   s-> Staff   this will store weather person is staff or normal user
	int id;
	char password[50];
	unsigned long long phone_no;
	char name[50];
	int nbook;              //Stores the no of books borrowed
};

struct Book_data {
	int book_id;
	char author[100];
	char name[100];
	char status;            //It Stores weather Book is available or not
};

struct BR_data {            //BR represents Borrow or return
	int book_id;
	int id;                 //Human id
	char B_T[50];           //Borrow Time
	char R_T[50];           //Return Time
};

//UDF
//Primary UDF
void login_page(struct Human_data H[]);
void signup_page(struct Human_data H[]);
void home_page(struct Human_data H[]);
void return_book(struct Human_data *H, struct Book_data B[],struct BR_data BR[]);
void borrow_book(struct Human_data *H, struct Book_data B[]);
void search_book(struct Book_data B[]);
void add_book(struct Human_data *H,struct Book_data B[]);

void forgot(struct Human_data *H);

// Global Variables
char Time[50]; // Array to store the time as string
int Human_cnt = 0;          //Counts number of data of humans in database
int Book_cnt = 0;           //Counts number of data of Books in database
int BR_cnt = 0;             //Counts number of data of Borrow or Return in database
int Human_index;

int main()
{
	Human_cnt = 0;
	FILE *fp = fopen("human_data.dat", "rb");
	if (fp == NULL) {
		fp = fopen("human_data.dat", "wb");  // Creates the file if not present
		if (fp == NULL) {
			printf("Failed to open or create the file");
			return 1;  // Exit if file creation fails
		}
		fclose(fp);  // Close after creating the file
	}
	// Reopen the file for reading after creation
	fp = fopen("human_data.dat", "rb");
	if (fp == NULL) {
		printf("Failed to open the file for reading");
		return 1;  // Exit if the file can't be opened for reading
	}

	// Dynamically allocate memory for human data
	struct Human_data *H = malloc(sizeof(struct Human_data));  // Start with space for 1 record
	if (!H) {
		printf("Memory allocation failed\n");
		return 1;
	}

	while (fscanf(fp, "%c\t%d\t%s\t%llu\t%s\t%d\n",
            &H[Human_cnt].type, &H[Human_cnt].id,H[Human_cnt].password,&H[Human_cnt].phone_no,H[Human_cnt].name,&H[Human_cnt].nbook) == 6) // Ensure all fields are correctly read
	{
		Human_cnt++;
		H = realloc(H, (Human_cnt + 1) * sizeof(struct Human_data));
		if (!H) {
			printf("Memory reallocation failed\n");
			free(H);
			return 1;
		}

	}
	fclose(fp);
	
	
	
	printf("============================================================================================================");
	printf("\n\n");
	print_space(13);                //prints space UDF in for_all.c
	printf("KATHMANDU INTERNATIONAL LIBRARY\n");
	print_space(19);                //prints space UDF in for_all.c
	printf("Jamal, Kathmandu");
	printf("\n\tYour Gateway to Knowledge and Inspiration\n");

	get_time(Time); // Pass the array to the function
	printf("\n\nDate: %.10s\nTime: %s\n",Time,Time + 11);            //Displays Date and Time of that day

re:                 //return to this using goto if any mistake is done by user

	printf("\n\nPlease select an option to proceed:\n1) Login\n2) Sign Up\n3) Exit\n\nYour Choice:");
	int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}

	if(choice == 1) {
		if(Human_cnt != 0) {
			login_page(H);
		}
		else {          //if no data is present this is executed
			printf("\n--------NO DATA ARE STORED IN DATABASE, SIGN UP FIRST--------\n\n");
		}
	}
	else if(choice == 2) {
		signup_page(H);
	}
	else if(choice == 3) {
		exit(0);
	}
	else {
		printf("\n--------CHOOSE AMONG 1, 2, 3--------\n\n");
	}
	goto re;
	return 0;
}

void login_page(struct Human_data H[]) {
    re:
    	printf("\n\nEnter Your ID: ");
    	int id = 0;
    	while(id == 0 ){            //This Handles ValueError
    		id = get_integer();
    	}
    	
    	int i;
    	for(i = 0 ; i<Human_cnt ; i++) {    //Checks if entered id is present in database or not
    		if (id == H[i].id) {
    			break;                  //breaks the loop when id matches
    		}
    	}
    	if(i == Human_cnt){         //When Id is not found in Database
    	    printf("\n\n------Invalid ID, SIGN UP FIRST---------\n\n");
    	    main();
    	}
    	else{
    	    printf("Enter Your Password: ");
        	char pass[50];
        	scanf(" %[^\n]s", pass);
        	//trimWhitespace(pass);               //removes all the white space in the beginning and end (function from for_all.c)
            if(strcmp(pass,H[i].password) == 0) {          //Confirms the password from Database
                Human_index = i;
            	home_page(H);
        	}
        	else if(strcasecmp(pass, "forgot") == 0) {      //if id is correct and password was forgotten, will go to forgot() function
    			forgot(&H[i]);
    		}
        	else{
        		printf("\n\n------Invalid PASSWORD------\n\n");
        		printf("FORGOT YOUR PASSWORD?? ENTER 'forgot' IN PASSWORD TO RECOVER YOUR PASSWORD");
        		goto re;
        	}
    	}
}


void signup_page(struct Human_data H[])
{
	int generated_id = generate_number(5);          //Generates 5 digit number between 1-9 (function from for_all.c)
	int i;
check_again:
	for(i=0 ; i<Human_cnt ; i++)
	{
		if(H[i].id == generated_id) {        // Generated Id is already present, then generated new id again
			generated_id = generate_number(5);
			goto check_again;                   // Ensures that generated id is not present
		}
	}
	H[Human_cnt].id = generated_id;
	printf("\n\nSystem Generated Your Id as %d\n(Please kindly remember it carefully)",generated_id);

	printf("\nSet Your Password: ");
	scanf(" %[^\n]s", H[Human_cnt].password);
	trimWhitespace(H[Human_cnt].password);          //removes all the white space in the beginning and end (function from for_all.c)
    
	printf("Enter Your Name: ");
	scanf(" %[^\n]s", H[Human_cnt].name);
	trimWhitespace(H[Human_cnt].name);          //removes all the white space in the beginning and end (function from for_all.c)

	printf("Phone Number: ");
	unsigned long long phoneNo = 0;
	while(phoneNo == 0){         //it handel ValueError
		phoneNo = get_unsignedlonglong();
		int check =phoneNo/pow(10,9);
		if( check != 9) {
			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
			phoneNo = 0;
		}
	}
	H[Human_cnt].phone_no = phoneNo;

	H[Human_cnt].nbook = 0;

	char decision;
	printf("Are You Sure You want to Sign Up (y/n): ");
	scanf(" %c", &decision);
	fflush(stdin);

	if(decision == 'y') { H[Human_cnt].type = 'u'; }
	else if(decision == 's') { H[Human_cnt].type = 's'; }
	else { return; }

	//open the file for appending
	FILE *fp = fopen("human_data.dat", "ab");
	if (fp == NULL) {
		printf("Failed to open the file for appending");
		return ;  // Exit if the file can't be opened for appending
 	}
 	fprintf(fp,"%c\t%d\t%s\t%llu\t%s\t%d\n",H[Human_cnt].type, H[Human_cnt].id, H[Human_cnt].password, H[Human_cnt].phone_no, H[Human_cnt].name, H[Human_cnt].nbook);
	printf("\n\nYour Data has been Sucessfully Added to the System Database.\nThank You For Choosing Us\n");
 	fclose(fp);
	Human_cnt++;
	H = realloc(H, (Human_cnt + 1) * sizeof(struct Human_data));
	if (!H) {
		printf("Memory reallocation failed\n");
		free(H);
		return ;
	}
	main();
}





void home_page(struct Human_data H[])
{
    FILE *fp;
    //Reading of BR_data.dat
	fp = fopen("BR_data.dat", "rb");
	if (fp == NULL) {
		fp = fopen("BR_data.dat", "wb");  // Creates the file if not present
		if (fp == NULL) {
			printf("Failed to open or create the file");
			return;  // Exit if file creation fails
		}
		fclose(fp);  // Close after creating the file
	}
	// Reopen the file for reading after creation
	fp = fopen("BR_data.dat", "rb");
	if (fp == NULL) {
		printf("Failed to open the file for reading");
		return;  // Exit if the file can't be opened for reading
	}

	// Dynamically allocate memory for human data
	struct BR_data *BR = malloc(sizeof(struct BR_data));  // Start with space for 1 record
	if (!BR) {
		printf("Memory allocation failed\n");
		return ;
	}
	BR_cnt = 0;
	while (fscanf(fp, " %d %49[^\t] %d %49[^\n]",&BR[BR_cnt].book_id,BR[BR_cnt].B_T,&BR[BR_cnt].id,BR[BR_cnt].R_T) == 4) // Ensure all fields are correctly read
	{ 
		BR_cnt++;
		BR = realloc(BR, (BR_cnt + 1) * sizeof(struct BR_data));
		if (!BR) {
			printf("Memory reallocation failed\n");
			free(BR);
			return ;
		}
	}
	fclose(fp);


	//=================================================================================================================
    
	//Reading of book_data.dat
	fp = fopen("book_data.dat", "rb");
	if (fp == NULL) {
		fp = fopen("book_data.dat", "wb");  // Creates the file if not present
		if (fp == NULL) {
			printf("Failed to open or create the file");
			return;  // Exit if file creation fails
		}
		fclose(fp);  // Close after creating the file
	}
	// Reopen the file for reading after creation
	fp = fopen("book_data.dat", "rb");
	if (fp == NULL) {
		printf("Failed to open the file for reading");
		return;  // Exit if the file can't be opened for reading
	}

	// Dynamically allocate memory for human data
	struct Book_data *B = malloc(sizeof(struct Book_data));  // Start with space for 1 record
	if (!B) {
		printf("Memory allocation failed\n");
		return ;
	}
	Book_cnt = 0;
	while (fscanf(fp, "%d\t%99[^\t]\t%99[^\t]\t%c\n",
                  &B[Book_cnt].book_id,
                  B[Book_cnt].author,
                  B[Book_cnt].name,
                  &B[Book_cnt].status) == 4)
    {
        Book_cnt++;
        struct Book_data *new_ptr = realloc(B, (Book_cnt + 1) * sizeof(struct Book_data));
        if (!new_ptr) {
            printf("Memory reallocation failed\n");
            free(B);
            fclose(fp);
            return;
        }
        B = new_ptr;
    }

	fclose(fp);

	//=================================================================================================================

	


	//The Following Codes are for Graphics
	printf("============================================================================================================");
	printf("\n\n\t\tWELCOME TO KATHMANDU INTERNATIONAL LIBRARY\n\t\tYour Gateway to Knowledge and Inspiration\n\n\n");
	printf("Hello %s, We are dedicated to providing you with a seamless library experience.",H[Human_index].name);

	get_time(Time); // Pass the array to the function
	printf("\n\nDate: %.10s\nTime: %s\n",Time,Time + 11);            //Displays Date and Time of that day

	if(Book_cnt == 0 && H[Human_index].type == 'u') {        //If No Book are available IN CASE OF USER
		printf("\n\nSorry, no books are currently available.\n");
		printf("Our staff needs to add book records to the system first.\n");
		printf("Please check back later. Thank you for your understanding!\n");
		exit(0);
	}
	if(Book_cnt == 0 && H[Human_index].type == 's') {        //If No Book are available IN CASE OF STAFF
		printf("\n\nNo books are currently available.\nGoing to Add Book Feature............\n\n");
		add_book(&H[Human_index],B);
	}

re:
	for(int i=0 ; i<Book_cnt ; i++){
	    printf( "1 %d  %s %s %c \n",B[i].book_id,B[i].author,B[i].name,B[i].status);
	}
	printf("\n\nPlease select an option to proceed:\n1) Return a Book\n2) Borrow a Book\n3) Search for a Book\n");
	//The Below code Displays Different UI For User and a Staff
	H[Human_index].type == 'u' ? printf("4) Log Out\n\nYour Choice: ") : printf("4) Add a Book\n5) Log Out\n\nYour Choice: ");
	int choice = 0;
	while(choice == 0) {
		choice = get_integer();
	}
	if(choice == 1) { return_book(&H[Human_index],B,BR);}
	else if(choice == 2) { borrow_book(&H[Human_index],B);}
	else if(choice == 3) { search_book(B);}
	else if(choice == 4 && H[Human_index].type == 'u') {               //Staff has LogOut in option 4
	    //Updating human_data.dat
    	fp = fopen("human_data.dat","wb");
    	if(fp == NULL){
    	    printf("Error Opening File");
    	    return ;
    	}
    	for(int i=0 ; i<Human_cnt ; i++){
    	    fprintf(fp,"%c\t%d\t%s\t%llu\t%s\t%d\n",H[i].type, H[i].id,H[i].password,H[i].phone_no,H[i].name,H[i].nbook);
    	}
    	fclose(fp);
        getchar();   
		exit(0);
	}
	else if(choice == 4 && H[Human_index].type == 's') { add_book(&H[Human_index],B); }          //Staff has Add a Book in option 4
	else if(choice == 5 && H[Human_index].type == 's') {               //Staff has LogOut in option 5
	    //Updating human_data.dat
    	fp = fopen("human_data.dat","wb");
    	if(fp == NULL){
    	    printf("Error Opening File");
    	    return ;
    	}
    	for(int i=0 ; i<Human_cnt ; i++){
    	    fprintf(fp,"%c\t%d\t%s\t%llu\t%s\t%d\n",H[i].type, H[i].id,H[i].password,H[i].phone_no,H[i].name,H[i].nbook);
    	}
    	fclose(fp);
        getchar();   
		exit(0);
	}
	else {
		printf("\n-----------%d is Invalid Input-----------",choice);
		goto re;
	}

	//=================================================================================================================
    
    //Updating book_data.dat
	fp = fopen("book_data.dat","wb");
	if(fp == NULL){
	    printf("Error Opening File");
	    return ;
	}
	for (int i = 0; i < Book_cnt; i++) {
    fprintf(fp, "%d\t%s\t%s\t%c\n",
                B[i].book_id,
                B[i].author,
                B[i].name,
                B[i].status);
    }

	fclose(fp); 
	goto re;           //Wont end until User LogOut
}


void add_book(struct Human_data *H,struct Book_data B[]) {
	FILE *fp = fopen("book_data.dat","ab");
	struct Book_data temp;
	printf("Enter The Following Data of The Added Book:\n");
	printf("Name: ");
	scanf(" %99[^\n]s",temp.name);
    trimWhitespace(temp.name);
	printf("Author: ");
	scanf(" %99[^\n]s",temp.author);
    trimWhitespace(temp.author);
	printf("Number of '%s' Being Added in the Library: ",temp.name);
	int book_num = 0;
	while(book_num == 0) {
		book_num = get_integer();
	}
	temp.status = 'y';
	int stored_id[1000];        //stored generated_id
	for(int i=0 ; i<book_num ; i++)
	{
		int generated_id = generate_number(7);          //Generates 7 digit number between 1-9 (function from for_all.c)
        check_again:
    		for(int j=0 ; j<Book_cnt ; j++)
    		{
    			if(B[j].book_id == generated_id) {        // Generated Id is already present, then generated new id again
    				generated_id = generate_number(7);
    				goto check_again;                   // Ensures that generated id is not present
    			}
    		}
    		for(int j=0 ; j<book_num ; j++)                //checks if generated number is already present
    		{
    			if(stored_id[j] == generated_id) {        // Generated Id is already present, then generated new id again
    				generated_id = generate_number(7);
    				goto check_again;                   // Ensures that generated id is not present
    			}
    		}
    		stored_id[i] = generated_id;
    		temp.book_id = generated_id;
		fprintf(fp, "%d\t%s\t%s\t%c\n",temp.book_id,temp.author,temp.name,temp.status);
		
		B[Book_cnt] = temp;
		Book_cnt++;
		B = realloc(B, (Book_cnt + 1) * sizeof(struct Book_data));
	}
	fclose(fp);
    
    for(int i=0 ; i<Book_cnt ; i++){
	    printf( "2 %d  %s %s %c \n",B[i].book_id,B[i].author,B[i].name,B[i].status);
	}
}

void return_book(struct Human_data *H, struct Book_data B[],struct BR_data BR[])
{
    re:
        printf("Please Select The Book You want to Return:\n");
        int i,j;
        int book_index[6];      //To store index of books borrowed
        int BR_index[6];        //To store index of Borrow and return Data
        int cnt = 0;            //counts no of book borrowed
        for (i = 0; i < BR_cnt; i++) {
            trimWhitespace(BR[i].R_T);
            if (H->id == BR[i].id && strcmp(BR[i].R_T, "abc") == 0) {  // If User has Borrowed the book
                for (j = 0; j < Book_cnt; j++) {
                    if (BR[i].book_id == B[j].book_id) {
                        book_index[cnt] = j;  // Store the index
                        break;
                    }
                }
                if (j == Book_cnt) {  // Book not found
                    printf("Error: Book ID %d not found in book database.\n", BR[i].book_id);
                    continue;  // Skip this entry
                }
                BR_index[cnt] = i;
                cnt++;  // Increment borrowed book count
                if (cnt >= 6) {  // Prevent overflow
                    printf("Error: Too many borrowed books to process.\n");
                    break;
                }
                printf("%d) %s By %s\n\tBook ID: %d\n", cnt, B[j].name, B[j].author, B[j].book_id);
            }
        }
        if (cnt == 0) {
            printf("\n\n-----------No Book Has Been Borrowed----------\n\n");
            home_page(H);
            return;
        }
        printf("\nYour Choice: ");
        int choice = 0;
        while (choice == 0) {
            choice = get_integer();
        }
        if (choice < 1 || choice > cnt) {
            printf("Invalid choice. Please try again.\n");
            goto re;  // Go back for a valid input
        }
        
        int return_index = book_index[choice - 1];
        int br_index = BR_index[choice - 1];
        
        printf("Thank you for returning the book: %s by %s.\nWe hope you enjoyed reading it! Have a great day!", B[return_index].name, B[return_index].author);
        
        get_time(Time);  // Get current time
        trimWhitespace(Time);
        
        // Update the borrow data
        H->nbook--;
        B[return_index].status = 'y';
        strcpy(BR[br_index].R_T, Time);  // Use br_index safely
        
        // Update BR_data.dat
        FILE *fp = fopen("BR_data.dat", "wb");
        if (fp == NULL) {
            printf("Error Opening File");
            return;
        }
        for (int i = 0; i < BR_cnt; i++) {
            fprintf(fp, " %d \t %s \t %d \t %s \n", BR[i].book_id, BR[i].B_T, BR[i].id, BR[i].R_T);
        }
        fclose(fp);
}

void borrow_book(struct Human_data *H, struct Book_data B[]){
    if(H->nbook>5){          //Checks if the user has reached the limit of brrowing 5 books
        printf("\n\nYou Have Reached Your Limit of Borrowing Books i.e. 5 Books");
        return;
    }
    get_time(Time);         //get Current Time and Stores it in variable Time
    printf("Have You Obtained ID of the Book You want To Borrow From Search Book Section (y/n): ");
    getchar();
    if(getchar() == 'y'){
        re:
            printf("Enter the ID of the Book:");
            int book_id = 0;
        	while(book_id == 0) {
        		book_id = get_integer();
        	}
        	int i;
        	for(i=0 ; i<Book_cnt ; i++){
        	    if(B[i].book_id == book_id){
        	        break;
        	    }
        	}
        	if(i == Book_cnt){              //If all the data were read and book id was Not found
        	    printf("\n\n------------The Entered ID was Not Found, Can you Re-enter the ID------------");
        	    goto re;
        	}
        	else{               //When Book was found
        	    printf("You have successfully borrowed the book:\n");
        	    printf("%s By %s\n",B[i].name,B[i].author);
        	    printf("The transaction has been recorded in the database. Thank you for using Kathmandu International Library!\n\n");
        	    
        	    H->nbook++;           //Number of Book brrowed by user increased by 1
        	    B[i].status = 'n';      //Updating Avaibality of Book
        	    
        	    FILE *fp = fopen("BR_data.dat","ab");
	            struct BR_data temp;
	            BR_cnt++;           //updating number of BR data
	            temp.book_id = B[i].book_id;        
	            temp.id = H->id;
	            trimWhitespace(Time);
	            strcpy(temp.B_T,Time);
	            strcpy(temp.R_T,"abc");
	            fprintf(fp, " %d \t %s \t %d \t %s \n",temp.book_id,temp.B_T,temp.id,temp.R_T);
	            fclose(fp);
        	}
    }
}


void search_book(struct Book_data B[])
{   
    int required_book_index = -1;           //Stores the index of required book
    char name[100];
    char author[100];
    re:
        printf("\nEnter the Following Details of the book you are looking for: ");
        printf("\nName : ");
        scanf(" %99[^\n]s",name);
        trimWhitespace(name);
        
        printf("Author : ");
        scanf(" %99[^\n]s",author);
        trimWhitespace(author);
        
        for(int i=0 ; i<Book_cnt ; i++)
        {
            if(strcasecmp(B[i].name,name)==0 && strcasecmp(B[i].author,author)==0 && B[i].status == 'y'){
                required_book_index = i;
                break;
            }
        }
    	    
    	if(required_book_index == -1){
    	    printf("\nSorry, The Book You are searching for is Currently not available in our Library.\nPlease try again.\n");
    	    goto re;
    	}
    	else{
    	    printf("\nId of the Book you are searching for is %d\n(Please remember this Id To Borrow The Book)\n",B[required_book_index].book_id);
    	}
}



void forgot(struct Human_data *H)
{
	int generated_login_code;
	int chance = 0;
	re:
	generated_login_code = generate_number(6);
	printf("\n\nLOGIN CODE HAS BEEN SENT TO YOU, IN PHONE NUMBER: %llu",H->phone_no);
	printf("\n\n\nThis is How SMS in Phone Number %llu Looks like:\n",H->phone_no);
	printf("----------------------------------------------------------------\n");
	printf("Hello %s, Your Login Code is %d ",H->name,generated_login_code);
	printf("\n----------------------------------------------------------------\n");
	printf("\nEnter The Login Code: ");
	int code = 0;
	while( code == 0){
        code = get_integer();
    	}
	if(generated_login_code == code) {
		printf("Your password is: %s\n(Don't Forget that Again)", H->password);
	}
	else {
		chance++;
	    if(chance>5){           //Auto cancel if code is wrong more thann 5 times
		printf("\n\n-------Invalid Code Multiple Time--------\n\n");
		return ;
	    }
		printf("-----------Invalid Login Code-----------");
		goto re;
	}
}


9395428	robert kiyosaki rich dad poor dad	y
8736867	robert kiyosaki	rich dad poor dad	y
9729769	robert kiyosaki	rich dad poor dad	y
1211121	robert kiyosaki	rich dad poor dad	y
6264763	robert kiyosaki	rich dad poor dad	y
5971573	robert kiyosaki	rich dad poor dad	y
7541688	robert kiyosaki	rich dad poor dad	y
6252965	robert kiyosaki	rich dad poor dad	y
2244436	robert kiyosaki	rich dad poor dad	y
1261875	robert kiyosaki	rich dad poor dad	y
3991145	robert kiyosaki	rich dad poor dad	y
9235162	robert kiyosaki	rich dad poor dad	y
7731756	robert kiyosaki	rich dad poor dad	y
6222649	robert kiyosaki	rich dad poor dad	y
8129299	robert kiyosaki	rich dad poor dad	y
1762972	robert kiyosaki	rich dad poor dad	y
2359115	robert kiyosaki	rich dad poor dad	y
9342583	robert kiyosaki	rich dad poor dad	y
8354258	robert kiyosaki	rich dad poor dad	y
7932781	robert kiyosaki	rich dad poor dad	y
1833732	robert kiyosaki	rich dad poor dad	y
3668855	robert kiyosaki	rich dad poor dad	y
7488299	robert kiyosaki	rich dad poor dad	y
5244958	robert kiyosaki	rich dad poor dad	y
8158376	robert kiyosaki	rich dad poor dad	y
9697267	robert kiyosaki	rich dad poor dad	y
5349577	robert kiyosaki	rich dad poor dad	y
1629883	robert kiyosaki	rich dad poor dad	y
8698915	robert kiyosaki	rich dad poor dad	y
3149638	robert kiyosaki	rich dad poor dad	y
9126664	robert kiyosaki	rich dad poor dad	y
1516492	robert kiyosaki	rich dad poor dad	y
2317628	robert kiyosaki	rich dad poor dad	y
8912177	robert kiyosaki	rich dad poor dad	y
4952546	robert kiyosaki	rich dad poor dad	y
3658124	robert kiyosaki	rich dad poor dad	y
8688353	robert kiyosaki	rich dad poor dad	y
2498159	robert kiyosaki	rich dad poor dad	y
8655517	robert kiyosaki	rich dad poor dad	y
9257293	robert kiyosaki	rich dad poor dad	y


u	64527	pass1234	9847690579	Nischhal	0
s	33591	pass1234	9876543211	Ram	0
