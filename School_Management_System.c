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

//Global FILE Pointer
FILE *fp;

//UDF Declearation
void add_details();
void view_stored_data();


int get_integer();      //for input error handelling
void print_in_table(struct Students S[] , int num);      // prints detail of students in a table
int cnt_digits(int num);        // Useful for printing data in table
void print_space(int n);         // Useful for printing data in table
int if_no_data();
void send_stdNo();



//Global Variables
static int Total_std = 0;           //Stores Number of students in No_std.txt file
static int std = 0;                //stores Number of students added just now




int main(){
    printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------");
    while(1){

        Total_std = 0;
        //Getting Total Number of Students from file, If no data is stored nthg is scaned hence total std remains 0
        fp = fopen("std_number.txt","r");
        if (fp == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
            fscanf(fp,"%d",&Total_std);
        fclose(fp);

        again:
            printf("\n\nChoose:\n1) Add Student Details\n2) View Stored Data of Students \n3) Edit Student Details \n4) Delete Student Data\n5)Exit\n");    
            int choice;
            if((choice = get_integer())!=0)
            {
                if(choice == 1){
                    add_details();
                }
                else if(choice == 2){
                    if(if_no_data() == 0)
                    {
                        view_stored_data();
                    }
                    else{goto again;}
                }
                else if(choice == 3){
                    if(if_no_data() == 0)
                    {
                        //edit_details();
                    }
                    else{goto again;}
                    
                }
                else if(choice == 4)
                {
                    if(if_no_data() == 0)
                    {
                        //delete_details();
                    }
                    else{goto again;}         
                }
                else if(choice == 5)
                {
                    exit(0);
                }
                else{
                    continue;       //Skips the Breaking below, Hence Continuing the Loop
                }
                break;
            }
    }


    send_stdNo();

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
        while(S[i].roll_no == 0)        //This Handles ValueError
        {
            printf("Roll Number: ");
            S[i].roll_no = get_integer();
        }

        S[i].class = 0;
        while(S[i].class == 0)      //This Handles ValueError
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

    fp = fopen("std_details.txt","a");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    for(int i=0 ; i<Total_std ; i++)
    {
        fprintf(fp ,"%s %s %d %d %s %s \n", S[i].f_name , S[i].l_name , S[i].roll_no , S[i].class , S[i].section , S[i].course);
    }

    fclose(fp);

    printf("\n\nStored Data Of Students:\n");
    Total_std += std;
    send_stdNo();       //Update Number of std to file
    print_in_table(S, std );
}



void view_stored_data()
{
    struct Students S[Total_std];

    fp = fopen("std_details.txt","r");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    for(int i=0 ; i<Total_std ; i++)
    {
          fscanf(fp ,"%s %s %d %d %s %s \n", S[i].f_name , S[i].l_name , &S[i].roll_no , &S[i].class , S[i].section , S[i].course);     
    }
    fclose(fp);

    print_in_table(S,Total_std);

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
        print_space(6-cnt_digits(i+1));

        printf("%s",S[i].f_name);
        print_space(14-strlen(S[i].f_name));

        printf("%s",S[i].l_name);
        print_space(14-strlen(S[i].l_name));
        
        printf("%d",S[i].roll_no);
        print_space(11-cnt_digits(S[i].roll_no));

        printf("%d",S[i].class);
        print_space(8-cnt_digits(S[i].class));

        printf("%s",S[i].section);
        print_space(10-strlen(S[i].section));

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


int if_no_data()
{
    if(Total_std == 0){
        printf("\nNo Data Are Stored.\nDo you Want to add data?(y/n)");
        getchar();
        if(getchar() == 'y'){
            add_details();
            return 1;
        }
    }
    return 0;
}


void send_stdNo()
{
    //Sending Total no of std to std_number.txt
    fp = fopen("std_number.txt","w");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(fp,"%d", Total_std);
    fclose(fp);
}