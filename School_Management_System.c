#include <stdio.h>
#include <string.h>
#include <ctype.h>          // Include ctype.h for isdigit() , tolower()
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
void edit_details();
void delete_details();
void find_details();


int get_integer();      //for input error handelling
void print_in_table(struct Students S[] , int num);      // prints detail of students in a table
void table_head();
int cnt_digits(int num);        // Useful for printing data in table
void print_space(int n);         // Useful for printing data in table
int if_no_data();              // Useful for checking if data is present in file or not
void send_stdNum();             //Updates num of students in file

void sort_data_f_name(struct Students S[]);

//Global Variables
static int Total_std = 0;           //Stores Number of students in No_std.txt file
static int std = 0;                //stores Number of students added just now




int main(){
    while(1){
        printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------\n\nHOME PAGE\n");
        Total_std = 0;
        //Getting Total Number of Students from file, If no data is stored nthg is scaned hence total std remains 0
        fp = fopen("std_number.txt","r");
        if (fp == NULL) {
            printf("Error opening file!\n");
            return 1;
        }
            fscanf(fp,"%d",&Total_std);
        fclose(fp);

        if(Total_std != 0)
        {
            struct Students S[Total_std];

            fp = fopen("std_details.txt","rb");
            if(fp == NULL) {
                printf("Error opening file!\n");
                exit(1);
            }
            // Read the structure from the file
            fread(&S, sizeof(struct Students), Total_std, fp);
            fclose(fp);
            sort_data_f_name(S); //Sorting files at the beginning of program
        }       
    
        printf("\nChoose:\n1) Add Student Details\n2) View Stored Data of Students \n3) Edit Student Details \n4) Delete Student Data\n5) Find Student\n6) Exit\n");    
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
                else{add_details();}
            }
            else if(choice == 3){
                if(if_no_data() == 0)
                {
                    edit_details();
                }
                else{add_details();}
                
            }
            else if(choice == 4)
            {
                if(if_no_data() == 0)
                {
                    delete_details();
                }
                else{add_details();}
            }
            else if(choice == 5)
            {
                if(if_no_data() == 0)
                {
                    find_details();
                }
                else{add_details();}
            }
            else if(choice == 6)
            {
                exit(0);
            }
            else{
                printf("\n\n--------Invalid Choice--------\n");
                continue;       //Skips the Breaking below, Hence Continuing the Loop
            }
            printf("Do You want to Continu to HOME PAGE? (y/n): ");
            getchar();
            if(tolower(getchar())!='y')
            {
                break;
            }
        }
    }

    send_stdNum();
    return 0;

}



//Primary Function are coded below

void add_details()
{
    do{
        std = 0;
        while(std == 0)
        {
            printf("\n\nEnter the number of Students, You want to add details of: ");
            std = get_integer();      //Returned Value is 0 unless integer is given
        }

        struct Students S[std];

        printf("\nEnter Details Of Students:\n");
        for(int i=0 ; i<std ; i++)
        {
            printf("\n\nFor Student %d:\n",i+1);
            
            printf("First Name: ");
            getchar();
            scanf("%20[^\n]s", S[i].f_name);

            printf("Last Name: ");
            getchar();
            scanf("%20[^\n]s", S[i].l_name);
            
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

        fp = fopen("std_details.txt","ab");
        if(fp == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
        fwrite(S, sizeof(struct Students), std, fp);
        fclose(fp);

        printf("\n\nStored Data Of Students:\n");
        Total_std += std;
        send_stdNum();       //Update Number of std to file

        table_head();
        print_in_table(S, std);
        printf("\n\nDo you want to add details of more students? (y/n): ");
        getchar();
    }
    while(tolower(getchar())=='y');

}



void view_stored_data()
{
    struct Students S[Total_std];

    fp = fopen("std_details.txt","rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    table_head();
    print_in_table(S,Total_std);
}



void edit_details()
{
   struct Students S[Total_std];

    fp = fopen("std_details.txt","rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);


    fp = fopen("std_details.txt","wb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    do
    {    
        printf("\n\n----------Stored Student Details----------\n\n");
        table_head();
        print_in_table(S,Total_std);

        char f_name[20];
        printf("\nEnter First Name of std you want to edit: ");
        getchar();
        scanf("%20[^\n]s", f_name);
            
        int roll_no = 0;
        while(roll_no == 0)        //This Handles ValueError
        {
            printf("His/Her Roll Number: ");
            roll_no = get_integer();
        }

        for(int i=0 ; i<Total_std ; i++)
        {
            if(S[i].roll_no == roll_no && strcasecmp(S[i].f_name,f_name) == 0)
            {
                printf("\n\nEnter New Details:\n");
                printf("First Name: ");
                getchar();
                scanf("%20[^\n]s", S[i].f_name);

                printf("Last Name: ");
                getchar();
                scanf("%20[^\n]s", S[i].l_name);
                
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

                fwrite(S, sizeof(struct Students), Total_std, fp);
                printf("\n\n-----Details Updated-----\n");
                break;
            }
            else if(i == Total_std-1)
            {
                printf("\n\n--------Student Not Found--------\n");
            }
        }
        sort_data_f_name(S);
        printf("\n\nDo you want to edit another student? (y/n): ");
        getchar();
    }
    while(tolower(getchar())=='y');
    fclose(fp);
    
}



void delete_details()
{
    struct Students S[Total_std];

    fp = fopen("std_details.txt","rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    fp = fopen("std_details.txt","wb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    do
    {    
        printf("\n\n----------Stored Student Details----------\n\n");
        table_head();
        print_in_table(S,Total_std);

        char f_name[20];
        printf("\nEnter First Name of std you want to delete: ");
        getchar();
        scanf("%20[^\n]s", f_name);
            
        int roll_no = 0;
        while(roll_no == 0)        //This Handles ValueError
        {
            printf("His/Her Roll Number: ");
            roll_no = get_integer();
        }

        for(int i=0 ; i<Total_std ; i++)
        {
            if(S[i].roll_no == roll_no && strcasecmp(S[i].f_name,f_name) == 0)
            {
                for(int j=i ; j<(Total_std-i) ; j++)
                {
                    S[j] = S[j+1];
                }
                Total_std--;
                fwrite(S, sizeof(struct Students), Total_std, fp);
                printf("\n\n-----Details deleted-----\n\n");
                break;
            }
            else if(i == Total_std-1)
            {
                printf("\n\n--------Student Not Found--------\n");
            }
        }
        sort_data_f_name(S);
        printf("\n\nDo you want to Delete detail of another student? (y/n): ");
        getchar();
    }
    while(tolower(getchar())=='y');
    send_stdNum();
    fclose(fp);
}



void find_details()
{
    struct Students S[Total_std];

    fp = fopen("std_details.txt","rb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);


    int choice, i, found=0;
    while(1){
        printf("\n\nWhich element do you want use to search for student?\n");
        printf("1) First Name\n2) Last Name\n3) Roll Number\n4) Class\n5) Section\n6) Course\n");
        if((choice = get_integer())!=0)
        {
            if(choice == 1){
                char f_name[20];
                printf("\nEnter First Name of std you want to Find: ");
                getchar();
                scanf("%20[^\n]s", f_name);
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(strcasecmp(S[i].f_name,f_name)==0)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }

            else if(choice == 2){
                char l_name[20];
                printf("\nEnter Last Name of std you want to Find: ");
                getchar();
                scanf("%20[^\n]s", l_name);
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(strcasecmp(S[i].l_name,l_name)==0)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 3){
                int roll_no = 0;
                while(roll_no == 0)        //This Handles ValueError
                {
                    printf("\nEnter Roll Number of std you want to Find: ");
                    roll_no = get_integer();
                }
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(S[i].roll_no == roll_no)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 4)
            {
                int class = 0;
                while(class == 0)        //This Handles ValueError
                {
                    printf("\nEnter Class of Students you want to Find: ");
                    class = get_integer();
                }
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(S[i].class == class)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }

            }
            else if(choice == 5)
            {
                char section[20];
                printf("\nEnter Section of std you want to Find: ");
                getchar();
                scanf("%20[^\n]s", section);
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(strcasecmp(S[i].section,section)==0)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 6)
            {
                char course[20];
                printf("\nEnter Course of std you want to Find: ");
                getchar();
                scanf("%20[^\n]s", course);
                printf("\n\n");
                table_head();
                for(i=0 ; i<Total_std ; i++)
                {
                    if(strcasecmp(S[i].course,course)==0)
                    {
                        print_in_table(&S[i],1);
                        found = 1;
                    }
                }
                if(found != 1)
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else{
                printf("\n\n--------Invalid Choice--------\n");
                continue;       //Skips the Breaking below, Hence Continuing the Loop
            }
            printf("Do You want to find another student? (y/n): ");
            getchar();
            if(tolower(getchar())!='y')
            {
                break;
            }
        }
    }
}


//Secondary Functions are coded Below


void sort_data_f_name(struct Students S[])
{
    int i,j;
    struct Students temp;
    for(i=0 ; i<Total_std-1 ; i++)
    {
        for(j=0 ; j<Total_std ; j++)
        {
            if(strcasecmp(S[i].f_name,S[j].f_name)<0)
                {
                    temp = S[i];
                    S[i] = S[j];
                    S[j] = temp;
                }
        }
    }
    fp = fopen("std_details.txt","wb");
    if(fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fwrite(S, sizeof(struct Students), Total_std, fp);
    fclose(fp);
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

    if(num == 0)
    {
        printf("\n--------Element cannot be 0--------\n");   
        return 0;
    }
    if(is_valid == 1)   //Ensures Given data is an Integer
    {
        return num;
    }
    printf("\n\n-----Choose an Integer-----\n\n");
    return 0;
}


void table_head()
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
}


void print_in_table(struct Students S[], int num)
{
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


int if_no_data()    //returns 1 if no data is present in the file else 0
{
    if(Total_std == 0){
        printf("\nNo Data Are Stored.\nDo you Want to add data?(y/n)");
        getchar();
        if(getchar() == 'y'){
            return 1;
        }
    }
    return 0;
}


void send_stdNum()
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
