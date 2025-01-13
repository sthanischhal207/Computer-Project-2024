#include <stdio.h>		// Standard I/O library for basic input and output operations
#include <stdlib.h>		// Standard library for dynamic memory allocation and utility functions
#include <string.h>		// String library for operations like strcmp, strcpy, etc.
#include <ctype.h>		// Character type library for functions like isdigit, isspace
#include <math.h>		// Math library for mathematical operations like pow
#include <unistd.h>     	// for sleep()
#include <pthread.h>
#include <stdbool.h>
#include "for_all.h"	// Custom header file with utility functions and declarations

struct Telecom_data{
    unsigned long long by;      // Who is calling i.e. user
    unsigned long long to;      // To whome user is calling
    char start[50];             // Stores Time when call started or SMS was sent
    double duration;               // Stores Duration of the call
    char SMS[10000];            // Stores the SMS sent by the user
    float amount;               // Stores the amount Charged for either Call or SMS
};


//UDFs
void home_page();
void add_record(struct Telecom_data D[]);
void view_records(struct Telecom_data D[]);
void search_record(struct Telecom_data D[]);
void payment_overview(struct Telecom_data D[]);
void initiate_payment(struct Telecom_data D[]);

void display_time(int seconds);
void* check_input(void* arg);       // Function to handle user input in a separate thread
void display_headings(int n1,int n2);
void display_data(int n1, int n2,struct Telecom_data D);


//Global Variable
int Data_cnt = 0;           // Stores the Data Count in Data Base
float P_charge = 10;      // Stores Charge for Voice call per second
float S_charge = 20;      // Stores Charge for SMS per Character
unsigned long long phone_no;
// Global flag to detect when Enter is pressed
volatile bool stop = false;
char Time[50];              //Stores current Time
int graphics_cnt = 0;

int main(){
    //Connecting The Number in the database
    printf("\n\nBefore We Start,\nEnter Your Phone Number: ");
    phone_no = 0;
    while(phone_no == 0){
        phone_no = get_unsignedlonglong();
        int check = phone_no/pow(10,9);            //This Checks The phone_no
		if( check != 9) {
			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
			phone_no = 0;
		}
    }
    home_page();
    return 0;
}



void home_page() {
    FILE *fp;

    // Reading data from the file
    fp = fopen("telecom_data.dat", "rb");
    if (fp == NULL) {
        // Create the file if not present
        fp = fopen("telecom_data.dat", "wb");
        if (fp == NULL) {
            printf("Failed to open or create the file\n");
            return;
        }
        fclose(fp);
    } else {
        fclose(fp);
    }

    // Reopen the file for reading
    fp = fopen("telecom_data.dat", "rb");
    if (fp == NULL) {
        printf("Failed to open the file for reading\n");
        return;
    }
    
    struct Telecom_data *D = malloc(sizeof(struct Telecom_data)); // Dynamic memory allocation
    if (!D) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }
    // Read data from the file using fread
    while (fread(&D[Data_cnt], sizeof(struct Telecom_data), 1, fp)) {
        Data_cnt++;
        D = realloc(D, (Data_cnt + 1) * sizeof(struct Telecom_data));
        if (!D) {
            printf("Memory reallocation failed\n");
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    
re:
    // Home Page Graphics
    printf("\n\n\n-------------------------------------------------------\n");
    printf("|                    TELECOM SYSTEM                   |\n");
    printf("-------------------------------------------------------\n");
    printf("|                                                     |\n");
    printf("|    [1] -> Add New Records                           |\n");
    printf("|                                                     |\n");
    printf("|    [2] -> View All Records                          |\n");
    printf("|                                                     |\n");
    printf("|    [3] -> Search Records                            |\n");
    printf("|                                                     |\n");
    printf("|    [4] -> Payment Overview                          |\n");
    printf("|                                                     |\n");
    printf("|    [5] -> Initiate Payment                          |\n");
    printf("|                                                     |\n");
    printf("|    [6] -> Exit the System                           |\n");
    printf("|                                                     |\n");
    printf("-------------------------------------------------------\n");
    printf("\nYour Choice: ");
    int choice = 0;

    while (choice == 0) { // Handles invalid input
        choice = get_integer();
    }

    if (choice == 1) { add_record(D); }
    else if(choice == 2){ view_records(D); } 
    else if(choice == 3){ search_record(D); } 
    else if(choice == 4){ payment_overview(D); }
    else if(choice == 5){ initiate_payment(D); }
    else if (choice > 6) {
        printf("\n--------Invalid Choice--------\n\n");
        goto re;
    }
    // Updating the file before exiting or going to re:
    fp = fopen("telecom_data.dat", "wb");
    if (fp == NULL) {
        printf("Error writing file\n");
        free(D);
        return;
    }

    for (int i = 0; i < Data_cnt; i++) {
        fwrite(&D[i], sizeof(struct Telecom_data), 1, fp);
    }
    fclose(fp);

    if (choice == 6) { exit(0); }
    goto re;
}




void add_record(struct Telecom_data D[]){
    re:
    printf("\n\n\n-------------------------------------------------------\n");
    printf("|                      ADD RECORDS                    |\n");
    printf("-------------------------------------------------------\n");
    printf("|                                                     |\n");
    printf("|    [1] -> Voice Call                                |\n");
    printf("|                                                     |\n");
    printf("|    [2] -> Send SMS                                  |\n");
    printf("|                                                     |\n");
    printf("-------------------------------------------------------\n");
    printf("\nYour Choice:");
    int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}
	if(choice == 1) {
	    
	    printf("\n\nEnter Phone Number You want To call: ");
        unsigned long long phone_call = 0;
        while(phone_call == 0){
            phone_call = get_unsignedlonglong();
            int check1 = phone_call/pow(10,9);            //This Checks The phone_no
    		if( check1 != 9) {
    			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
    			phone_call = 0;
    		}
        }
	    time_t start, end;
        double diff;
        int seconds = 1;  
        printf("Press Enter to Call: ");
        getchar();
        getchar(); // Wait for initial Enter key
        get_time(Time);         //Getting the Time
	    trimWhitespace(Time);
        time(&start); // Get the start time
    
        printf("Press Enter again to End Call:\n");
    
        // Create a thread to monitor user input
        pthread_t input_thread;
        pthread_create(&input_thread, NULL, check_input, NULL);
    
        // Main loop to display elapsed time
        while (!stop) {
            printf("\n");
            display_time(seconds);
            sleep(1); // Wait for 1 second
            seconds++;
        }
    
        time(&end); // Get the end time
        diff = difftime(end, start);
        
        stop = false;       
        
        printf("\n\nYou Have called %llu for %g seconds Costing you NRs.%g\n",phone_call,diff,diff*P_charge );
    
        // Join the input thread (cleanup)
        pthread_join(input_thread, NULL);
        
        printf("\n\nPress Enter To Continue:");
        getchar();
        
        D[Data_cnt].by = phone_no;
        D[Data_cnt].to = phone_call;
        strcpy(D[Data_cnt].start,Time);
        D[Data_cnt].duration = diff;
        D[Data_cnt].amount = diff*P_charge;
        strcpy(D[Data_cnt].SMS,"");
        
	}
	else if(choice == 2) {
	    printf("\n\nEnter Phone Number You want To call: ");
        unsigned long long phone_call = 0;
        while(phone_call == 0){
            phone_call = get_unsignedlonglong();
            int check1 = phone_call/pow(10,9);            //This Checks The phone_no
    		if( check1 != 9) {
    			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
    			phone_call = 0;
    		}
        }
        char SMS[10000];
        printf("Enter the Message: ");
        scanf(" %[^\n]s",SMS);
        
        get_time(Time);         //Getting the Time
	    trimWhitespace(Time);
        
        D[Data_cnt].by = phone_no;
        D[Data_cnt].to = phone_call;
        strcpy(D[Data_cnt].start,Time);
        D[Data_cnt].duration = 0;
        D[Data_cnt].amount = strlen(SMS)*S_charge;
        strcpy(D[Data_cnt].SMS,SMS);
	}
	else{   
		printf("\n--------Invalid Choice--------\n\n");
		goto re;
	}
	Data_cnt++;
    // Reallocate memory dynamically to adjust size
	D = realloc(D, (Data_cnt + 1) * sizeof(struct Telecom_data));
	if (!D) {
		printf("Memory reallocation failed\n");
		free(D);
		return ;
	}
}


void view_records(struct Telecom_data D[]){
    graphics_cnt = 0;
    printf("\n\nVoice Call History:\n");
    display_headings(0,0);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration != 0 && D[i].by == phone_no){
            display_data(0,0,D[i]);                
        }
    }
    graphics_cnt = 0;
    printf("\n\nSMS History:\n");
    display_headings(1,0);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration == 0 && D[i].by == phone_no){
            display_data(1,0,D[i]);                
        }
    }
    
    printf("\n\nPress Enter To Continue:");
    getchar();
    getchar();
}


void search_record(struct Telecom_data D[]){
    printf("Enter The Number with whom You want to Search the history of: ");
    unsigned long long phone_search = 0;
    while(phone_search == 0){
        phone_search = get_unsignedlonglong();
        int check = phone_search/pow(10,9);            //This Checks The phone_no
		if( check != 9) {
			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
			phone_search = 0;
		}
    }
    graphics_cnt = 0;
    printf("\n\nVoice Call History:\n");
    display_headings(0,0);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration != 0 && D[i].by == phone_no && D[i].to == phone_search){
            display_data(0,0,D[i]);                
        }
    }
    graphics_cnt = 0;
    printf("\n\nSMS History:\n");
    display_headings(1,0);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration == 0 && D[i].by == phone_no && D[i].to == phone_search){
            display_data(1,0,D[i]);                
        }
    }
    
    printf("\n\nPress Enter To Continue:");
    getchar();
    getchar();
}


void payment_overview(struct Telecom_data D[]){
    float sum = 0;
    graphics_cnt = 0;
    printf("\n\nVoice Call Overview:\n");
    display_headings(0,3);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration != 0 && D[i].by == phone_no && D[i].amount != 0){
            display_data(0,3,D[i]);  
            sum += D[i].amount;
        }
    }
    graphics_cnt = 0;
    printf("\n\nSMS Overview:\n");
    display_headings(1,3);
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].duration == 0 && D[i].by == phone_no && D[i].amount != 0){
            display_data(1,3,D[i]);     
            sum += D[i].amount;
        }
    } 
    
    printf("\n\nYour Amount to be paid will be %g",sum);
    
    printf("\n\nPress Enter To Continue:");
    getchar();
    getchar();
}


void initiate_payment(struct Telecom_data D[]){
    float sum = 0;
    for(int i = 0 ; i<Data_cnt ; i++){
        if(D[i].by == phone_no && D[i].amount != 0){ 
            sum += D[i].amount;
        }
    }
    if(sum == 0){return;}
    printf("\n\n\n-------------------------------------------------------\n");
    printf("|                    TELECOM SYSTEM                   |\n");
    printf("-------------------------------------------------------\n");
    printf("|                                                     |\n");
    printf("|    Total Bill = NRs. %g\n",sum);
    if_more_pay:
    printf("|                                                     |\n");
    printf("|    Amount You would like to pay: NRs. ");
    float Amount_paid;
    scanf(" %f",&Amount_paid);             //Value Error might occour incase of input string
    if(Amount_paid > sum || Amount_paid<0){
        printf("---------Amount Must be less or equal to %g---------\n",sum);
        goto if_more_pay;
    }
    printf("|                                                     |\n");
    printf("-------------------------------------------------------\n");
    printf("|   Thank you for choosing our service!               |\n");
    printf("|                                                     |\n");
    printf("|   For further assistance, contact: 9876543210       |\n");
    printf("-------------------------------------------------------\n");

    for(int i=0 ; i<Data_cnt ; i++){
        if(D[i].by == phone_no){
        	//When Amount paid is less than required payment
        	if(Amount_paid - D[i].amount < 0){    
        		D[i].amount -= Amount_paid;
        		break;
        	}
        	else{
        		Amount_paid -= D[i].amount;
        		D[i].amount = 0;
        	}
        }
    }
}





/*********************************************************
 (0,0) -> SN To Time Call_Duration
 (1,0) -> Sn To Time SMS Length
 (0,3) -> SN To Time Call_Duration Amount
 (1,3) -> Sn To Time SMS Length Amount
 * ******************************************************/
void display_headings(int n1, int n2){
    printf("\n\n");
    printf("SN");
    print_space(3);
    printf("To");
    print_space(22);
    printf("Time");
    print_space(12);
    if(n1 == 0){printf("Call Duration");}
    else if(n1 == 1){printf("SMS Length");}
    print_space(2);
    if(n2 == 3){printf("Amount(NRs)[Not Paid]");}
}

void display_data(int n1, int n2,struct Telecom_data D){
    graphics_cnt++;
    printf("\n%d)",graphics_cnt);
    print_space(4-cnt_digits(graphics_cnt));
    printf("%llu",D.to);
    print_space(14-cnt_digits(D.to));
    printf("%s",D.start);
    print_space(29-strlen(D.start));
    if(n1 == 0){
        display_time(D.duration);
        print_space(7);
    }
    else if(n1 == 1){
        printf(" %ld",strlen(D.SMS));
        print_space(8-cnt_digits(strlen(D.SMS)));
    }
    print_space(2);
    if(n2 == 3){printf("%g",D.amount);}
    printf("\n");
}

void display_time(int seconds) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    printf("%02d:%02d", minutes, secs);
}

// Function to handle user input in a separate thread
void* check_input(void* arg) {
    getchar(); // Wait for Enter key
    stop = true; // Set the flag to true when Enter is pressed
    return NULL;
}
