#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "for_all.h"

struct Human_data{
    char type;              // u ->User   s-> Staff   this will store weather person is staff or normal user
    int id;
    char password[50];
    long long phone_no;
    char name[50];
    int nbook;              //Stores the no of books borrowed
};

struct Book_data{
    int book_id;
    char author[100];
    char name[100];
    char status;            //It Stores weather Book is available or not
};

struct BR_data{             //BR represents Borrow or return
    int book_id;
    int id;                 //Human id
    char B_T[50];           //Borrow Time
    char R_T[50];           //Return Time
};

//UDF 
//Primary UDF
void login_page(struct Human_data H[]);
void signup_page(struct Human_data H[]);
void home_page();
void return_book();
void borrow_book();
void search_book();
void add_book();

// Global Variables
char Time[50]; // Array to store the time as string
int Human_cnt=0;

int main()
{
    Human_cnt = 0;
    int i = 0;
    
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
    struct Human_data *H = malloc((Human_cnt+1) * sizeof(struct Human_data));  // Start with space for 1 records initially
    while(fscanf(fp,"%c %d %s %lld %s %d \n",&H[i].type, &H[i].id, H[i].password, &H[i].phone_no, H[i].name, &H[i].nbook) != EOF)
    {
        Human_cnt++;
        i++;
        H = realloc(H,(Human_cnt+1) * sizeof(struct Human_data));           // Reallocate memory
    }
    // Free the dynamically allocated memory
    free(H);
    fclose(fp);
    
    get_time(Time); // Pass the array to the function
    printf("\n\n-------------KATHMANDU INTERNATIONAL LIBRARY-------------\n");
    print_space(19);
    printf("Jamal, Kathmandu");
    printf("\n\nDate: %.10s\n\n",Time);            //Displays Date of that day
    re:                 //return to this using goto if any mistake is done by user
        printf("CHOOSE:\n1) Login\n2) Sign Up\n3) Exit\n");
        int choice = 0;
        while(choice == 0 )        //This Handles ValueError
        {
            choice = get_integer();
        }
        
        if(choice == 1){
            if(Human_cnt != 0){
                login_page(H);
            }
            else{           //if no data is present this is executed
                printf("\n--------NO DATA ARE STORED IN DATABASE, SIGN UP FIRST--------\n\n");
                goto re;
            }
        }
        else if(choice == 2){
            signup_page(H);
        }
        else if(choice == 3){
            exit(0);
        }
        else{
            printf("\n--------CHOOSE AMONG 1, 2, 3--------\n\n");
            goto re;
        }
    
    return 0;
}

void login_page(struct Human_data H[]){
    re:
        printf("\n\nEnter Your ID: ");
        int id = 0;
        while(id == 0 )        //This Handles ValueError
        {
            id = get_integer();
        }
        printf("___%d__",id);
        printf("Enter Your Password: ");
        char pass[50];
        getchar();          //removes \n 
        scanf("%[^\n]s", pass);
        
        trimWhitespace(pass);               //removes all the white space in the beginning and end (function from for_all.c)
        printf("__%s__",pass);
        int i;
        for(i = 0 ; i<Human_cnt ; i++){     //Checks if entered id is present in database or not
        printf("Type: %c\n", H[i].type);
        printf("ID: %d\n", H[i].id);
        printf("Password: %s\n", H[i].password);
        printf("Phone No: %lld\n", H[i].phone_no);
        printf("Name: %s\n", H[i].name);
        printf("Nbook: %d\n", H[i].nbook);
            if (id == H[i].id){      
                if (strcasecmp(pass, "forgot") == 0) {      //if id is correct and password was forgotten, will go to forgot() function
                    //forgot(H);
                    goto re;
                }
                if(strcmp(pass,H[i].password) == 0) {
                    break;
                }
            }
        }
        if(i == Human_cnt)          //if data is not avilable in database user is asked to re-enter the id
        {
            printf("\n\n------Invalid ID OR PASSWORD------\n\n");
            printf("FORGOT YOUR PASSWORD?? ENTER 'forgot' IN PASSWORD TO RECOVER YOUR PASSWORD");
            goto re;
        }
        //home_page();
}


void signup_page(struct Human_data H[])
{
    FILE *fp = fopen("human_data.dat", "ab");
    if (fp == NULL) {
        fp = fopen("human_data.dat", "wb");  // Creates the file if not present
        if (fp == NULL) {
            printf("Failed to open or create the file");
            return;
        }
        fclose(fp);  // Close after creating the file
    }
    
    int generated_id = generate_number(5);          //Generates 5 digit number between 1-9 (function from for_all.c)
    int i;
    check_again:
        for(i=0 ; i<Human_cnt ; i++)
        {
            if(H[i].id == generated_id){         // Generated Id is already present, then generated new id again 
                generated_id = generate_number(5);
                goto check_again;                   // Ensures that generated id is not present
            }
        }
    struct Human_data temp;
    temp.id = generated_id;
    printf("\n\nSystem Generated Your Id as %d\n(Please kindly remember it carefully)",generated_id);
    
    printf("\nSet Your Password: ");
    getchar();          //fflush(stdin);
    scanf("%[^\n]s", temp.password);
    trimWhitespace(temp.password);          //removes all the white space in the beginning and end (function from for_all.c)
    
    printf("Enter Your Name: ");
    getchar();
    scanf("%[^\n]s", temp.name);
    trimWhitespace(temp.name);          //removes all the white space in the beginning and end (function from for_all.c)
    
    printf("Phone Number: ");
    int phoneNo = 0;
    while(phoneNo == 0)         //it handel ValueError
    {
        phoneNo = get_integer();
    }
    temp.phone_no = phoneNo;
    
    temp.nbook = 0;
    
    char decision;
    printf("Are You Sure You want to Sign Up (y/n): ");
    getchar();
    scanf("%c", &decision);
    fflush(stdin);
    
    if(decision == 'y'){ temp.type = 'u'; }
    else if(decision == 's'){ temp.type = 's'; }
    else{return; }
    
    
    //open the file for appending 
    fp = fopen("human_data.dat", "ab");
    if (fp == NULL) {
        printf("Failed to open the file for appending");
        return ;  // Exit if the file can't be opened for appending
    }
    fprintf(fp,"%c %d %s %lld %s %d \n",temp.type, temp.id, temp.password, temp.phone_no, temp.name, temp.nbook);

    fclose(fp);
}
/*
void home_page();
void return_book();
void borrow_book();
void search_book();
void add_book();
*/










