#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "for_all.h"

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

//Primary UDF
void add_details();
void view_stored_data();
void edit_details();
void delete_details();
void find_details();

//Secondary UDF
void print_in_table(struct Students S[] , int num ,char sn);      // prints detail of students in a table
void table_head();
int if_no_data();                // Useful for checking if data is present in file or not
void send_stdNum();              //Updates num of students in file
void sort_data_f_name(struct Students S[]);     //Sort s data in file based on first name

//Global Variables
static int Total_std = 0;         //Stores Number of students in No_std.txt file
static int std = 0;               //stores Number of students added just now

int main(){
    while(1){
        printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------\n\nHOME PAGE\n");
        Total_std = 0;
        //Getting Total Number of Students from file, If no data is stored nthg is scaned hence total std remains 0
        fp = fopen("std_number.dat","rb");
        if (fp == NULL) {
    		fp = fopen("std_number.dat", "wb");  // Creates the file if not present
    		if (fp == NULL) {
    			printf("Failed to open or create the file");
    			return 1;  // Exit if file creation fails
    		}
    		fclose(fp);  // Close after creating the file
    	}
    	// Reopen the file for reading after creation
    	fp = fopen("std_number.dat", "rb");
    	    fscanf(fp,"%d",&Total_std);
    	fclose(fp);
        if(Total_std != 0){
            struct Students S[Total_std];
            fp = fopen("std_details.dat","rb");
            if (fp == NULL) {
    		fp = fopen("std_details.dat", "wb");  // Creates the file if not present
    		if (fp == NULL) {
    			printf("Failed to open or create the file");
    			return 1;  // Exit if file creation fails
    		}
    		fclose(fp);  // Close after creating the file
        	}
        	// Reopen the file for reading after creation
        	fp = fopen("std_details.dat", "rb");
            // Read the structure from the file
            fread(&S, sizeof(struct Students), Total_std, fp);
            fclose(fp);
            sort_data_f_name(S); //Sorting files at the beginning of program
        }
        int IfNoData = if_no_data();
        if(IfNoData == 0){
        printf("\nChoose:\n1) Add Student Details\n2) View Stored Data of Students \n3) Edit Student Details \n4) Delete Student Data\n5) Find Student\n6) Exit\n\nYour choice: ");    
        int choice = 0;
        while(choice == 0){
            choice = get_integer();
        }
            if(choice == 1){        add_details();      }
            else if(choice == 2){   view_stored_data(); }
            else if(choice == 3){   edit_details();     }
            else if(choice == 4){   delete_details();   }
            else if(choice == 5){   find_details();     }
            else if(choice == 6){   exit(0);            }
            else{
                printf("\n\n--------Invalid Choice--------\n");
                continue;       //Skips the Breaking below, Hence Continuing the Loop
            }
        }
        else if(IfNoData == 1){
            add_details();
        }
        else if(IfNoData == 2){
            continue;
        }
        printf("\n\nDo You want to Continue to HOME PAGE? (y/n): ");
        getchar();
        if(tolower(getchar())!='y'){
            break;
        }
    }
    send_stdNum();  //Send the number of students to the server
    return 0;

}



//Primary Function are coded below

void add_details()
{
    do{
        std = 0;
        while(std == 0){
            printf("\n\nEnter the number of Students, You want to add details of: ");
            std = get_integer();      //Returned Value is 0 unless integer is given
        }
        struct Students S[std];
        printf("\nEnter Details Of Students:\n");
        for(int i=0 ; i<std ; i++)
        {
            printf("\n\nFor Student %d:\n",i+1);
            printf("First Name: ");
            scanf(" %20[^\n]s", S[i].f_name);
            printf("Last Name: ");
            scanf(" %20[^\n]s", S[i].l_name);
            S[i].roll_no = 0;
            while(S[i].roll_no == 0) {       //This Handles ValueError
                printf("Roll Number: ");
                S[i].roll_no = get_integer();
            }
            S[i].class = 0;
            while(S[i].class == 0){      //This Handles ValueError
                printf("Class: ");
                S[i].class = get_integer();
            }
            printf("Section: ");
            scanf(" %5[^\n]s", S[i].section);
            printf("Course: ");
            scanf(" %30[^\n]s", S[i].course);
            trimWhitespace(S[i].f_name);
            trimWhitespace(S[i].l_name);
            trimWhitespace(S[i].section);
            trimWhitespace(S[i].course);
        }
        fp = fopen("std_details.dat","ab");
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
        print_in_table(S, std,'y');
        printf("\n\nDo you want to add details of more students? (y/n): ");
        getchar();
    }
    while(tolower(getchar())=='y');

}


void view_stored_data()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat","rb");
    if(fp == NULL) {
        printf("\nError opening file!\nError");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);
    table_head();
    print_in_table(S,Total_std,'y');
}


void edit_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat", "rb");
    if (fp == NULL) {
        printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
        exit(1);
    }
    // Read the structure from the file
    fread(S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    do
    {    
        printf("\n\n----------Stored Student Details----------\n\n");
        table_head();
        print_in_table(S, Total_std, 'y');
        
        char f_name[20];
        printf("\nEnter First Name of student you want to edit: ");
        getchar(); // Clear buffer
        scanf("%20[^\n]s", f_name);
        
        int roll_no = 0;
        while (roll_no == 0) {
            printf("His/Her Roll Number: ");
            roll_no = get_integer();
        }
        
        for (int i = 0; i < Total_std; i++)
        {
            if (S[i].roll_no == roll_no && strcasecmp(S[i].f_name, f_name) == 0)
            {
                printf("\n\nEnter New Details:\n");
                printf("First Name: ");
                scanf(" %20[^\n]s", S[i].f_name);
                printf("Last Name: ");
                scanf(" %20[^\n]s", S[i].l_name);
                
                S[i].roll_no = 0;
                while (S[i].roll_no == 0) {
                    printf("Roll Number: ");
                    S[i].roll_no = get_integer();
                }
                
                S[i].class = 0;
                while (S[i].class == 0) {
                    printf("Class: ");
                    S[i].class = get_integer();
                }
                
                printf("Section: ");
                scanf(" %5[^\n]s", S[i].section);
                printf("Course: ");
                scanf(" %30[^\n]s", S[i].course);
                
                trimWhitespace(S[i].f_name);
                trimWhitespace(S[i].l_name);
                trimWhitespace(S[i].section);
                trimWhitespace(S[i].course);
                
                fp = fopen("std_details.dat", "wb");
                if (fp == NULL) {
                    printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
                    exit(1);
                }
                fwrite(S, sizeof(struct Students), Total_std, fp);
                fclose(fp);
                
                printf("\n\n-----Details Updated-----\n");
                break;
            }
            else if (i == Total_std - 1) {
                printf("\n\n--------Student Not Found--------\n");
            }
        }
        
        sort_data_f_name(S);
        printf("\n\nDo you want to edit another student? (y/n): ");
        getchar(); // Clear buffer
    }
    while (tolower(getchar()) == 'y');
}



void delete_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat", "rb");
    if (fp == NULL) {
        printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
        exit(1);
    }
    // Read the structure from the file
    fread(S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    do
    {    
        if (Total_std == 0){
            printf("\n\n-----All the Students' Details have been deleted-----\n\n");
            break;
        }
        
        printf("\n\n----------Stored Student Details----------\n\n");
        table_head();
        print_in_table(S, Total_std, 'y');
        
        char f_name[20];
        printf("\nEnter First Name of student you want to delete: ");
        getchar(); // Clear buffer
        scanf(" %20[^\n]s", f_name);
        trimWhitespace(f_name);
        
        int roll_no = 0;
        while (roll_no == 0) {
            printf("His/Her Roll Number: ");
            roll_no = get_integer();
        }
        
        for (int i = 0; i < Total_std; i++)
        {
            if (S[i].roll_no == roll_no && strcasecmp(S[i].f_name, f_name) == 0)
            {
                for (int j = i; j < Total_std - 1; j++) {
                    S[j] = S[j + 1];
                }
                Total_std--;
                
                fp = fopen("std_details.dat", "wb");
                if (fp == NULL) {
                    printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
                    exit(1);
                }
                fwrite(S, sizeof(struct Students), Total_std, fp);
                fclose(fp);
                
                printf("\n\n-----Details Deleted-----\n\n");
                break;
            }
            else if (i == Total_std - 1) {
                printf("\n\n--------Student Not Found--------\n");
            }
        }
        
        sort_data_f_name(S);
        printf("\n\nDo you want to delete details of another student? (y/n): ");
        getchar(); // Clear buffer
    }
    while (tolower(getchar()) == 'y');
    
    send_stdNum();
}




void find_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat","rb");
    if(fp == NULL) {
        printf("\nError opening file!\n Make Sure you have created std_number.dat  and std_details.dat in the same Folder.\n");
        exit(1);
    }
    // Read the structure from the file
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    int choice, i, cnt;
    while(1){
        printf("\n\nWhich element do you want use to search for student?\n");
        printf("1) First Name\n2) Last Name\n3) Roll Number\n4) Class\n5) Section\n6) Course\n");
        if((choice = get_integer())!=0){
            if(choice == 1){
                char f_name[20];
                printf("\nEnter First Name of std you want to Find: ");
                scanf(" %20[^\n]s", f_name);
                trimWhitespace(f_name);
                printf("\n\n");
                table_head();
                cnt =0;
                for(i=0 ; i<Total_std ; i++){
                    if(strcasecmp(S[i].f_name,f_name)==0){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 ){
                    printf("\n\n--------Student Not Found--------\n");
                }
            }

            else if(choice == 2){
                char l_name[20];
                printf("\nEnter Last Name of std you want to Find: ");
                scanf(" %20[^\n]s", l_name);
                trimWhitespace(l_name);
                printf("\n\n");
                table_head();
                cnt=0;
                for(i=0 ; i<Total_std ; i++){
                    if(strcasecmp(S[i].l_name,l_name)==0){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 ){
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 3){
                int roll_no = 0;
                while(roll_no == 0){        //This Handles ValueError
                    printf("\nEnter Roll Number of std you want to Find: ");
                    roll_no = get_integer();
                }
                printf("\n\n");
                table_head();
                cnt=0;
                for(i=0 ; i<Total_std ; i++){
                    if(S[i].roll_no == roll_no){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 ){
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 4){
                int class = 0;
                while(class == 0){        //This Handles ValueError
                    printf("\nEnter Class of Students you want to Find: ");
                    class = get_integer();
                }
                printf("\n\n");
                table_head();
                cnt=0;
                for(i=0 ; i<Total_std ; i++){
                    if(S[i].class == class){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 ){
                    printf("\n\n--------Student Not Found--------\n");
                }

            }
            else if(choice == 5){
                char section[20];
                printf("\nEnter Section of std you want to Find: ");
                scanf(" %20[^\n]s", section);
                trimWhitespace(section);
                printf("\n\n");
                table_head();
                cnt=0;
                for(i=0 ; i<Total_std ; i++){
                    if(strcasecmp(S[i].section,section)==0){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 )
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else if(choice == 6){
                char course[20];
                printf("\nEnter Course of std you want to Find: ");
                scanf(" %20[^\n]s", course);
                trimWhitespace(course);
                printf("\n\n");
                table_head();
                cnt=0;
                for(i=0 ; i<Total_std ; i++){
                    if(strcasecmp(S[i].course,course)==0){
                        cnt++;
                        printf("%d",cnt);
                        print_space(6-cnt_digits(cnt));
                        print_in_table(&S[i],1,'n');
                    }
                }
                if(cnt ==0 )
                {
                    printf("\n\n--------Student Not Found--------\n");
                }
            }
            else{
                printf("\n\n--------Invalid Choice--------\n");
                continue;       //Skips the Breaking below, Hence Continuing the Loop
            }
            printf("\nDo You want to find another student? (y/n): ");
            getchar();
            if(tolower(getchar())!='y'){
                break;
            }
        }
    }
}

//Secondary Functions are coded Below
void sort_data_f_name(struct Students S[]){
    int i,j;
    struct Students temp;
    for(i=0 ; i<Total_std ; i++){
        for(j=0 ; j<Total_std ; j++){
            if(strcasecmp(S[i].f_name,S[j].f_name)<0){
                temp = S[i];
                S[i] = S[j];
                S[j] = temp;
            }
        }
    }
    fp = fopen("std_details.dat","wb");
    if(fp == NULL) {
        printf("\nError opening file!\n Make Sure you have created std_number.dat  and std_details.dat in the same Folder.\n");
        exit(1);
    }
    fwrite(S, sizeof(struct Students), Total_std, fp);
    fclose(fp);
}

void table_head(){
    printf("\nSN");
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

void print_in_table(struct Students S[], int num, char sn){
    for(int i=0 ; i<num ; i++){
        if(sn == 'y'){   //Added to fix SN display while finding students
            printf("%d",i+1);
            print_space(6-cnt_digits(i+1));
        }
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

int if_no_data(){    //returns 1 if no data is present in the file else 0
    if(Total_std == 0){
        printf("\nNo Data Are Stored.\nDo you Want to add data?(y/n)");
        getchar();
        if(getchar() == 'y'){
            return 1;
        }
        else{
            exit(0);
        }
    }
    return 0;
}

void send_stdNum(){
    //Sending Total no of std to std_number.dat
    fp = fopen("std_number.dat","wb");
    if(fp == NULL) {
        printf("\nError opening file!\n Make Sure you have created std_number.dat  and std_details.dat in the same Folder.\n");
        exit(1);
    }
    fprintf(fp,"%d", Total_std);
    fclose(fp);
}
