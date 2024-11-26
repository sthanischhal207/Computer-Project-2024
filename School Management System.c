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


//UDF Declearation
int get_integer();

//Global Structure
struct Students{
    char f_name[20];
    char l_name[20];
    int Roll_no;
    int Class;
    char section[5];
    char course;
};

//Global Variables
static int Total_std;


int main(){
    //printf("Enter")
    printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------");
    while(1){
        printf("\n\nChoose:\n1) Add Student Details\n2) Edit Student Details\n3) Delete Student Data\n");
        
        int choice;
        if((choice = get_integer())!=0)
        {
            if(choice == 1){
                add_details();
            }
            else if(choice == 2){
                edit_details();
            }
            else if(choice == 3){
                delete_details();
            }
            else{
                continue;
            }
            break;
        }

    }
    return 0;
}


int get_integer(){ // Returns the int if int was inputed else return 0

    char local_choice[30];      
    scanf(" %30[^\n]s", local_choice);   //Takes a single char // space before %c to clear any new line // or we can use fflush(stdin);

    int is_valid = 1;

    for(int k=0 ;local_choice[k] != '\\0'; k++)
    {
        if(!isdigit(local_choice[k]))
        {
            is_valid = 0; // if any enity except int is entered makes it invalid 
            break; 
        }
    }

    if(is_valid == 1)   //Ensures Given data is an Integer
    {
        return atoi(local_choice);      //changes str to int
    }
    printf("\n\n-----Choose an Integer-----");
    return 0;
}
