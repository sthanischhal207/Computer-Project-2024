#include <stdio.h>		// Standard I/O library for basic input and output operations
#include <stdlib.h>		// Standard library for dynamic memory allocation and utility functions
#include <string.h>		// String library for operations like strcmp, strcpy, etc.
#include <ctype.h>		// Character type library for functions like isdigit, isspace
#include <math.h>		// Math library for mathematical operations like pow
#include "for_all.h"		// Custom header file with utility functions and declarations

/* Structure definition for student records
 * Contains basic student information like name, roll number, class details
 */
struct Students{
    char f_name[20];        // First name of student
    char l_name[20];        // Last name of student
    int roll_no;            // Roll number
    int class;              // Class/grade of student
    char section[5];        // Section identifier
    char course[30];        // Course name
};

// Global file pointer for file operations
FILE *fp;

/* Primary Function Declarations
 * Core functionalities of the school management system
 */
void add_details();         // Add new student records
void view_stored_data();    // Display all stored student records
void edit_details();        // Modify existing student records
void delete_details();      // Remove student records
void find_details();        // Search for specific student records

/* Secondary Function Declarations
 * Support functions for data manipulation and display
 */
void print_in_table(struct Students S[] , int num ,char sn);      // Prints student details in tabular format
void table_head();                                                // Prints table headers
int if_no_data();                                                // Checks if data exists in file
void send_stdNum();                                              // Updates total number of students in file
void sort_data_f_name(struct Students S[]);                      // Sorts student data by first name

// Global Variables
static int Total_std = 0;   // Tracks total number of students in database
static int std = 0;         // Tracks number of students being added in current session

/* Main function
 * Provides menu-driven interface for school management system
 */
int main(){
    while(1){
        printf("\n\t-------------WELCOME TO SCHOOL MANAGEMENT SYSYTEM-------------\n\nHOME PAGE\n");
        Total_std = 0;
        
        // Initialize/open file for storing number of students
        fp = fopen("std_number.dat","rb");
        if (fp == NULL) {
    		fp = fopen("std_number.dat", "wb");  // Creates file if not present
    		if (fp == NULL) {
    			printf("Failed to open or create the file");
    			return 1;
    		}
    		fclose(fp);
    	}
    	
    	// Read total number of students from file
    	fp = fopen("std_number.dat", "rb");
    	    fscanf(fp,"%d",&Total_std);
    	fclose(fp);

        // If students exist, load their data and sort by first name
        if(Total_std != 0){
            struct Students S[Total_std];
            fp = fopen("std_details.dat","rb");
            if (fp == NULL) {
    		fp = fopen("std_details.dat", "wb");
    		if (fp == NULL) {
    			printf("Failed to open or create the file");
    			return 1;
    		}
    		fclose(fp);
        	}
        	fp = fopen("std_details.dat", "rb");
            fread(&S, sizeof(struct Students), Total_std, fp);
            fclose(fp);
            sort_data_f_name(S);
        }

        // Check if data exists and handle menu options
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
                continue;
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
    send_stdNum();
    return 0;
}

/* Function to add new student details
 * Allows adding multiple students in one session
 * Stores data in binary file and updates total count
 */
void add_details()
{
    do{
        std = 0;
        while(std == 0){
            printf("\n\nEnter the number of Students, You want to add details of: ");
            std = get_integer();
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
            while(S[i].roll_no == 0) {
                printf("Roll Number: ");
                S[i].roll_no = get_integer();
            }
            S[i].class = 0;
            while(S[i].class == 0){
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
        send_stdNum();
        table_head();
        print_in_table(S, std,'y');
        printf("\n\nDo you want to add details of more students? (y/n): ");
        getchar();
    }
    while(tolower(getchar())=='y');
}

/* Function to display all stored student records
 * Reads data from file and displays in tabular format
 */
void view_stored_data()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat","rb");
    if(fp == NULL) {
        printf("\nError opening file!\nError");
        exit(1);
    }
    fread(&S, sizeof(struct Students), Total_std, fp);
    fclose(fp);
    table_head();
    print_in_table(S,Total_std,'y');
}

/* Function to edit existing student records
 * Allows modification of all fields for a specific student
 * Identifies student by first name and roll number
 */
void edit_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat", "rb");
    if (fp == NULL) {
        printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
        exit(1);
    }
    fread(S, sizeof(struct Students), Total_std, fp);
    fclose(fp);

    do
    {    
        printf("\n\n----------Stored Student Details----------\n\n");
        table_head();
        print_in_table(S, Total_std, 'y');
        
        char f_name[20];
        printf("\nEnter First Name of student you want to edit: ");
        getchar();
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
        getchar();
    }
    while (tolower(getchar()) == 'y');
}

/* Function to delete student records
 * Removes specific student data and updates file
 * Identifies student by first name and roll number
 */
void delete_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat", "rb");
    if (fp == NULL) {
        printf("\nError opening file!\n Make sure you have created std_number.dat and std_details.dat in the same folder.\n");
        exit(1);
    }
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
        getchar();
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
        getchar();
    }
    while (tolower(getchar()) == 'y');
    
    send_stdNum();
}

/* Function to search for student records
 * Allows searching by multiple criteria:
 * - First Name
 * - Last Name
 * - Roll Number
 * - Class
 * - Section
 * - Course
 */
void find_details()
{
    struct Students S[Total_std];
    fp = fopen("std_details.dat","rb");
    if(fp == NULL) {
        printf("\nError opening file!\n Make Sure you have created std_number.dat  and std_details.dat in the same Folder.\n");
        exit(1);
    }
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
                while(roll_no == 0){        // Input validation loop
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
                while(class == 0){        // Input validation loop
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
                continue;       // Skip to next iteration if invalid choice
            }
            printf("\nDo You want to find another student? (y/n): ");
            getchar();
            if(tolower(getchar())!='y'){
                break;
            }
        }
    }
}

/* Secondary Functions
 * These functions provide support for the primary operations
 */

/* Function to sort student data by first name
 * Uses bubble sort algorithm
 * Updates the sorted data in the file
 */
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

/* Function to print table headers
 * Displays column names for student data table
 */
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

/* Function to print student data in tabular format
 * Parameters:
 * S[] - Array of student structures
 * num - Number of students to print
 * sn - Flag to determine if serial number should be printed ('y'/'n')
 */
void print_in_table(struct Students S[], int num, char sn){
    for(int i=0 ; i<num ; i++){
        if(sn == 'y'){   // Print serial number if flag is 'y'
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

/* Function to check if data exists in the file
 * Returns:
 * 1 - No data exists and user wants to add data
 * 0 - Data exists
 * Exits program if no data and user doesn't want to add
 */
int if_no_data(){
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

/* Function to update the total number of students in file
 * Writes current Total_std value to std_number.dat
 */
void send_stdNum(){
    fp = fopen("std_number.dat","wb");
    if(fp == NULL) {
        printf("\nError opening file!\n Make Sure you have created std_number.dat  and std_details.dat in the same Folder.\n");
        exit(1);
    }
    fprintf(fp,"%d", Total_std);
    fclose(fp);
}
