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
#include <ctype.h>


//UDF Declearation
int get_integer();      //for input error handelling
void print_in_table(struct Students S[]);      // prints detail of students in a table
int cnt_digits(int num);        // Useful for printing data in table
void add_details();

//Global Structure
struct Students{
    char f_name[20];
    char l_name[20];
    int Roll_no;
    int Class;
    char section[5];
    char course[30];
};

//Global Variables
static int Total_std = 0;


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
    while(Total_std == 0)
    {
        printf("Enter the number of Students, You want to add details of: ");
        Total_std = get_integer();      //Returned Value is 0 unless integer is given
    }

    struct Students S[Total_std];

    printf("\nEnter Details Of Students:\n");
    for(int i=0 ; i<Total_std ; i++)
    {
        printf("For Student %d",i+1);
        
        printf("First Name: ");
        scanf("%20[^\n]s", S[i].f_name);

        printf("Last Name: ");
        scanf("%20[^\n]s", S[i].l_name);

        S[i].Roll_no = 0;
        while(S[i].Roll_no == 0)        //This Handles TypeError
        {
            S[i].Roll_no = get_integer();
        }

        S[i].Class = 0;
        while(S[i].Class == 0)      //This Handles TypeError
        {
            S[i].Class = get_integer();
        }

        printf("Section: ");
        scanf("%5[^\n]s", S[i].section);

        printf("Course: ");
        scanf("%30[^\n]s", S[i].course);
    }

    printf("\n\nStored Data Of Students:\n");
    print_in_table(S);



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


void print_in_table(struct Students S[])
{
    //...
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
