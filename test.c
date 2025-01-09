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
void login_page(struct Human_data H[],struct Book_data B[], struct BR_data BR[]);
void signup_page(struct Human_data H[]);
void home_page(struct Human_data H[],struct Book_data B[], struct BR_data BR[]);
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
int index_human;

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
		printf("7Memory allocation failed\n");
		return 1;
	}

	while (fscanf(fp, " %c %d %s %llu %s %d \n",
            &H[Human_cnt].type, &H[Human_cnt].id,H[Human_cnt].password,&H[Human_cnt].phone_no,H[Human_cnt].name,&H[Human_cnt].nbook) == 6) // Ensure all fields are correctly read
	{
		Human_cnt++;
		H = realloc(H, (Human_cnt + 1) * sizeof(struct Human_data));
		if (!H) {
			printf("6Memory reallocation failed\n");
			free(H);
			return 1;
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
			return 1;  // Exit if file creation fails
		}
		fclose(fp);  // Close after creating the file
	}
	// Reopen the file for reading after creation
	fp = fopen("book_data.dat", "rb");
	if (fp == NULL) {
		printf("Failed to open the file for reading");
		return 1;  // Exit if the file can't be opened for reading
	}

	// Dynamically allocate memory for human data
	struct Book_data *B = malloc(sizeof(struct Book_data));  // Start with space for 1 record
	if (!B) {
		printf("5Memory allocation failed\n");
		return 1;
	}
	Book_cnt = 0;
	while (fscanf(fp, " %d %s %s %c \n",
	              &B[Book_cnt].book_id,
	              B[Book_cnt].author,
	              B[Book_cnt].name,
	              &B[Book_cnt].status) == 4) // Ensure all fields are correctly read
	{
		Book_cnt++;
		B = realloc(B, (Book_cnt + 1) * sizeof(struct Book_data));
		if (!B) {
			printf("4Memory reallocation failed\n");
			free(B);
			return 1;
		}

	}
	fclose(fp);


	//=================================================================================================================

	//Reading of BR_data.dat
	fp = fopen("BR_data.dat", "rb");
	if (fp == NULL) {
		fp = fopen("BR_data.dat", "wb");  // Creates the file if not present
		if (fp == NULL) {
			printf("Failed to open or create the file");
			return 1;  // Exit if file creation fails
		}
		fclose(fp);  // Close after creating the file
	}
	// Reopen the file for reading after creation
	fp = fopen("BR_data.dat", "rb");
	if (fp == NULL) {
		printf("Failed to open the file for reading");
		return 1;  // Exit if the file can't be opened for reading
	}

	// Dynamically allocate memory for human data
	struct BR_data *BR = malloc(sizeof(struct BR_data));  
if (!BR) {
    printf("Memory allocation failed\n");
    return 1;
}

BR_cnt = 0;
while (fscanf(fp, " %d \n %s \n %d \n %s \n",
              &BR[BR_cnt].book_id,
              BR[BR_cnt].B_T,
              &BR[BR_cnt].id,
              BR[BR_cnt].R_T) == 4) {
    BR_cnt++;
    printf("Reallocating memory for record: %d\n", BR_cnt);
    struct BR_data *temp = realloc(BR, (BR_cnt + 1) * sizeof(struct BR_data));
    if (!temp) {
        printf("Memory reallocation failed at count %d\n", BR_cnt);
        free(BR);
        return 1;
    }
    BR = temp;
}
	fclose(fp);



	//=================================================================================================================
	
	for(int i=0 ; i<Human_cnt; i++) {
		printf(" %c %d %s %llu %s %d\n",
		       H[i].type,
		       H[i].id,
		       H[i].password,
		       H[i].phone_no,
		       H[i].name,
		       H[i].nbook);
	}
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
	printf("Please select an option to proceed:\n1) Login\n2) Sign Up\n3) Exit\n\nYour Choice:");
	int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}

	if(choice == 1) {
		if(Human_cnt != 0) {
			login_page(H,B,BR);
		}
		else {          //if no data is present this is executed
			printf("\n--------NO DATA ARE STORED IN DATABASE, SIGN UP FIRST--------\n\n");
			goto re;
		}
	}
	else if(choice == 2) {
		signup_page(H);
		goto re;
	}
	else if(choice == 3) {
	    //Updating human_data.dat at end 
    	fp = fopen("human_data.dat","wb");
    	if(fp == NULL){
    	    printf("Error Opening File");
    	    return 0;
    	}
    	for(int i=0 ; i<Human_cnt ; i++){
    	    fprintf(fp," %c %d %s %llu %s %d\n",H[i].type, H[i].id,H[i].password,H[i].phone_no,H[i].name,H[i].nbook);
    	}
    	fclose(fp);
        getchar();   
        getchar();
		exit(0);
	}
	else {
		printf("\n--------CHOOSE AMONG 1, 2, 3--------\n\n");
		goto re;
	}
	return 0;
}

void login_page(struct Human_data H[],struct Book_data B[], struct BR_data BR[]) {
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
            	index_human = i;
            	home_page(H,B,BR);
        	}
        	else if(strcasecmp(pass, "forgot") == 0) {      //if id is correct and password was forgotten, will go to forgot() function
    			forgot(&H[i]);
    			goto re;
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
	struct Human_data Temp;
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
	Human_cnt++;
	Temp.id = generated_id;
	printf("\n\nSystem Generated Your Id as %d\n(Please kindly remember it carefully)",generated_id);

	printf("\nSet Your Password: ");
	scanf(" %[^\n]s", Temp.password);
	trimWhitespace(Temp.password);          //removes all the white space in the beginning and end (function from for_all.c)
    
	printf("Enter Your Name: ");
	scanf(" %[^\n]s", Temp.name);
	trimWhitespace(Temp.name);          //removes all the white space in the beginning and end (function from for_all.c)

	printf("Phone Number: ");
	unsigned long long phoneNo = 0;
	while(phoneNo == 0)         //it handel ValueError
	{
		phoneNo = get_unsignedlonglong();
		/*int check =phoneNo/pow(10,9);
		if( check != 9)
		{
			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
			phoneNo = 0;
		}*/
	}
	Temp.phone_no = phoneNo;

	Temp.nbook = 0;

	char decision;
	printf("Are You Sure You want to Sign Up (y/n): ");
	scanf(" %c", &decision);
	fflush(stdin);

	if(decision == 'y') {
		Temp.type = 'u';
	}
	else if(decision == 's') {
		Temp.type = 's';
	}
	else {
		return;
	}


	//open the file for appending
	FILE *fp = fopen("human_data.dat", "ab");
	if (fp == NULL) {
		printf("Failed to open the file for appending");
		return ;  // Exit if the file can't be opened for appending
	}
	if (fprintf(fp, "%c %d %s %llu %s %d\n", Temp.type, Temp.id, Temp.password, Temp.phone_no, Temp.name, Temp.nbook) < 0) {
    perror("Failed to write data to the file");
} else {
    printf("\n\nYour Data has been Successfully Added to the System Database.\nThank You For Choosing Us\n");
}
	printf("\n\nYour Data has been Sucessfully Added to the System Database.\nThank You For Choosing Us\n");
	fclose(fp);
}



void home_page(struct Human_data H[],struct Book_data B[], struct BR_data BR[])
{
    for(int i=0 ; i<Human_cnt; i++) {
		printf(" %c %d %s %llu %s %d\n",
		       H[i].type,
		       H[i].id,
		       H[i].password,
		       H[i].phone_no,
		       H[i].name,
		       H[i].nbook);
	}
    FILE *fp;
re:
	//The Following Codes are for Graphics
	printf("============================================================================================================");
	printf("\n\n\t\tWELCOME TO KATHMANDU INTERNATIONAL LIBRARY\n\t\tYour Gateway to Knowledge and Inspiration\n\n\n");
	printf("Hello %s, We are dedicated to providing you with a seamless library experience.",H[index_human].name);

	get_time(Time); // Pass the array to the function
	printf("\n\nDate: %.10s\nTime: %s\n",Time,Time + 11);            //Displays Date and Time of that day

	if(Book_cnt == 0 && H[index_human].type == 'u') {        //If No Book are available IN CASE OF USER
		printf("\n\nSorry, no books are currently available.\n");
		printf("Our staff needs to add book records to the system first.\n");
		printf("Please check back later. Thank you for your understanding!\n");
		exit(0);
	}
	if(Book_cnt == 0 && H[index_human].type == 's') {        //If No Book are available IN CASE OF STAFF
		printf("\n\nNo books are currently available.\nGoing to Add Book Feature............\n\n");
		add_book(&H[index_human],B);
		goto re;
	}
	printf("\n\nPlease select an option to proceed:\n1) Return a Book\n2) Borrow a Book\n3) Search for a Book\n");
	//The Below code Displays Different UI For User and a Staff
	H[index_human].type == 'u' ? printf("4) Log Out\n\nYour Choice: ") : printf("4) Add a Book\n5) Log Out\n\nYour Choice: ");
	int choice = 0;
	while(choice == 0)
	{
		choice = get_integer();
	}
	if(choice == 1) { return_book(&H[index_human],B,BR);}
	else if(choice == 2) { borrow_book(&H[index_human],B);}
	else if(choice == 3) { search_book(B);}
	else if(choice == 4 && H[index_human].type == 'u') {               //User has LogOut in option 4
	//Updating human_data.dat at end 
    	fp = fopen("human_data.dat","wb");
    	if(fp == NULL){
    	    printf("Error Opening File");
    	    return ;
    	}
    	for(int i=0 ; i<Human_cnt ; i++){
    	    fprintf(fp," %c %d %s %llu %s %d\n",H[i].type, H[i].id,H[i].password,H[i].phone_no,H[i].name,H[i].nbook);
    	}
    	fclose(fp);
        getchar();   
        getchar();
		exit(0);
	}
	else if(choice == 4 && H[index_human].type == 's') { add_book(&H[index_human],B); }          //Staff has Add a Book in option 4
	else if(choice == 5 && H[index_human].type == 's') {            //Staff has LogOut in option 5
	//Updating human_data.dat at end 
    	fp = fopen("human_data.dat","wb");
    	if(fp == NULL){
    	    printf("Error Opening File");
    	    return ;
    	}
    	for(int i=0 ; i<Human_cnt ; i++){
    	    fprintf(fp," %c %d %s %llu %s %d\n",H[i].type, H[i].id,H[i].password,H[i].phone_no,H[i].name,H[i].nbook);
    	}
    	fclose(fp);
        getchar();   
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
	for(int i=0 ; i<Book_cnt ; i++){
	    fprintf(fp, " %d %s %s %c \n",B[i].book_id,B[i].author,B[i].name,B[i].status);
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
		Book_cnt++;
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
		fprintf(fp, " %d %s %s %c \n",temp.book_id,temp.author,temp.name,temp.status);
	}
	fclose(fp);
}

void return_book(struct Human_data *H, struct Book_data B[],struct BR_data BR[])
{
    re:
        printf("Please Select The Book You want to Return:\n");
        int i,j;
        int book_index[6];      //To store index of books borrowed
        int BR_index[6];        //To store index of Borrow and return Data
        int cnt = 0;            //counts no of book borrowed
        for(i=0 ;i<BR_cnt ; i++){
            if(H->id == BR[i].id && strcasecmp(BR[i].R_T,"abc") == 0 ){           //If User has Borrowed the book
                for(j=0 ; j<Book_cnt ; j++){
                    if(BR[i].book_id == B[j].book_id){
                        book_index[cnt]=j ;       //Stores Books index in book_index for future
                        break;
                    }
                } 
                BR_index[cnt] = i;
                cnt++;                      //Updating Book cnt
                printf("%d) %s By %s\n\tBook ID: %d\n",cnt, B[j].name, B[j].author, B[j].book_id);
            }
        }
        if(cnt ==  0){
            printf("\n\n-----------No Book Has Been Borrowed----------\n\n");
            home_page(H,B,BR);
        }
        
        printf("\nYour Choice: ");
        int choice = 0;
    	while(choice == 0)
    	{
    		choice = get_integer();
    	}
    	if(choice>cnt){ goto re;}
    	int return_index = book_index[choice-1];
	    printf("Thank you for returning the book: %s by %s.\nWe hope you enjoyed reading it! Have a great day!",B[return_index].name, B[return_index].author);
	    
	    get_time(Time);         //get Current Time and Stores it in variable Time
	    trimWhitespace(Time);
	    //Updating the changes that occours after returning book
	    H->nbook--;
	    B[return_index].status = 'y';
	    strcpy(BR[BR_index[choice]].R_T,Time);
	    
    //Updating BR_data.dat
	FILE *fp = fopen("BR_data.dat","wb");
	if(fp == NULL){
	    printf("Error Opening File");
	    return ;
	}
	for(int i=0 ; i<BR_cnt ; i++){
	    fprintf(fp, " %d \n %s \n %d \n %s \n",BR[i].book_id,BR[i].B_T,BR[i].id,BR[i].R_T);
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
        FILE *fp = fopen("BR_data.dat","ab");
        struct BR_data temp;
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
	            BR_cnt++;           //updating number of BR data
	            temp.book_id = B[i].book_id;        
	            temp.id = H->id;
	            trimWhitespace(Time);
	            strcpy(temp.B_T,Time);
	            strcpy(temp.R_T,"abc");
	            fprintf(fp, " %d \n %s \n %d \n %s \n",temp.book_id,temp.B_T,temp.id,temp.R_T);
        	}
        fclose(fp);
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
        
        printf("\nAuthor : ");
        scanf(" %99[^\n]s",author);
        trimWhitespace(name);
        
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
    	    printf("Id of the Book you are searching for is %d\n(Please remember this Id To Borrow The Book)\n",B[required_book_index].book_id);
    	}
}



void forgot(struct Human_data *H)
{
	int generated_login_code = generate_number(6);
	printf("\n\nLOGIN CODE HAS BEEN SENT TO YOU, IN PHONE NUMBER: %lld",H->phone_no);
	printf("\n\n\nThis is How SMS in Phone Number %lld Looks like:\n",H->phone_no);
	printf("----------------------------------------------------------------\n");
	printf("Hello %s, Your Login Code is %d ",H->name,generated_login_code);
	printf("\n----------------------------------------------------------------\n");
	printf("\nEnter The Login Code: ");
	int code;
	scanf("%d",&code);
	if(generated_login_code == code)
	{
		printf("Your password is: %s\n(Don't Forget that Again)", H->password);
	}
	else
	{
		printf("-----------Invalid Login Code-----------");
	}
}
