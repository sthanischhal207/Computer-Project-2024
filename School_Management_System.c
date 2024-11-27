/*

PseudoCode:

Declear a Global Structure 
With parameters:
    ["f_Name","l_name","Roll_no","Class","Section","Course",""]

Function:
    1) main():
        ...
    
    2) delete(struct ...):
        ...
    
    3) Uptade(struct ...):
        ...

    4) Sorting(struct ...):
        sorting using f_name  , Section ,
        ...

    5) Edit(struct ...):
        edit any details of any student as per 

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>          // Include ctype.h for isdigit()
#include <stdlib.h>         // Include stdlib.h for atoi() and exit()


//Global Structure
struct Students{
    char f_name[20];
    char l_name[20];
    int roll_no;
    int class;
    char section[5];
    char course[30];
};


//UDF Declearation
int get_integer();      //for input error handelling
void print_in_table(struct Students S[] , int num);      // prints detail of students in a table
int cnt_digits(int num);        // Useful for printing data in table
void print_space(int n);         // Useful for printing data in table

void add_details();

//Global Variables
static int Total_std = 0;           //Stores Number of students in No_std.txt file
static int std = 0;                //stores Number of students added just now


int main(){
    printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------");
    while(1){
        printf("\n\nChoose:\n1) Add Student Details\n2) Edit Student Details\n3) Delete Student Data\n4)Exit\n");
        
        int choice;
        if((choice = get_integer())!=0)
        {
            if(choice == 1){
                add_details();
            }
            else if(choice == 2){
                //edit_details();
            }
            else if(choice == 3){
                //delete_details();
            }
            else if(choice == 4)
            {
                exit(0);
            }
            else{
                continue;       //Skips the Breaking below, Hence Continuing the Loop
            }
            break;
        }
    }

    return 0;
}


void add_details()
{
    std = 0;
    while(std == 0)
    {
        printf("Enter the number of Students, You want to add details of: ");
        std = get_integer();      //Returned Value is 0 unless integer is given
    }

    struct Students S[std];

    printf("\nEnter Details Of Students:\n");
    for(int i=0 ; i<std ; i++)
    {
        printf("\n\nFor Student %d:\n",i+1);
        
        printf("First Name: ");
        getchar();
        scanf("%20[^\n]", S[i].f_name);

        printf("Last Name: ");
        getchar();
        scanf("%20[^\n]", S[i].l_name);
        
        S[i].roll_no = 0;
        while(S[i].roll_no == 0)        //This Handles TypeError
        {
            printf("Roll Number: ");
            S[i].roll_no = get_integer();
        }

        S[i].class = 0;
        while(S[i].class == 0)      //This Handles TypeError
        {
            printf("Class: ");
            S[i].class = get_integer();
        }

        printf("Section: ");
        getchar();
        scanf("%5[^\n]s", S[i].section);

        printf("Course: ");
        getchar();
        scanf("%30[^\n]s", S[i].course);
    }

    printf("\n\nStored Data Of Students:\n");
    print_in_table(S, std );



}

int get_integer(){ // Returns the int if int was inputed else return 0

    char local_choice[30];      
    scanf(" %30[^\n]s", local_choice);   // space before %30[^\n]s to clear any new line // or we can use fflush(stdin);

    int is_valid = 1;

    for(int k=0 ;local_choice[k] != '\0'; k++)
    {
        if(!isdigit(local_choice[k]))
        {
            is_valid = 0; // if any enity except int is entered makes it invalid 
            break; 
        }
    }

    int num = atoi(local_choice);       //changes str to int

    if(is_valid == 1)   //Ensures Given data is an Integer
    {
        return num;
    }
    printf("\n\n-----Choose an Integer-----");
    return 0;
}


void print_in_table(struct Students S[], int num)
{
    printf("SN");
    print_space(4);

    printf("First Name");
    print_space(4);

    printf("Last Name");
    print_space(5);

    printf("Roll No.");
    print_space(3);

    printf("Class");
    print_space(3);

    printf("Section");
    print_space(3);

    printf("Course\n");


    for(int i=0 ; i<num ; i++)
    {
        printf("%d",i+1);
        print_space(4-cnt_digits(i+1));

        printf("%s",S[i].f_name);
        print_space(14-strlen(S[i].f_name));

        printf("%s",S[i].l_name);
        print_space(14-strlen(S[i].l_name));
        
        printf("%d",S[i].roll_no);
        print_space(9-cnt_digits(S[i].roll_no));

        printf("%d",S[i].class);
        print_space(9-cnt_digits(S[i].class));

        printf("%s",S[i].section);
        print_space(14-strlen(S[i].section));

        printf("%s", S[i].course);

        printf("\n");

    }
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