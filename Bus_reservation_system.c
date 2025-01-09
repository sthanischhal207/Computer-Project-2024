#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "for_all.h"


struct Bus_data{
    char id[15];            
    int T_seats;            // T Respresents Total
    int A_seats;            // A respresents Available
    unsigned long long array[50];         //Stores the array of phone no in the seat which are booked else stores 0
    char from[50];          //Stores Start Destination of Bus
    char to[50];            //Stores End destination of Bus 
    char Time[15];          //Stores Time of Departure
    int price;              //Stores the Price of Bus Ticket
};

//Primary UDFs
void home_page();
void booking_page(struct Bus_data B[]);
void refund_page(struct Bus_data B[]);

//Secondary UDFs
void display_seats(struct Bus_data B);
void display_bus(struct Bus_data B);
void display_headings();



//Global Variable
unsigned long long phone_no = 0;        //Stores Phone Number of the user
int Bus_cnt;            //Stores Total Number of Bus
int graphics_cnt;        //Used For Displaying Bus Data  


int main(){
    printf("-------------------------------------------------------\n");
    printf("|               BUS RESERVATION SYSTEM                |\n");
    printf("-------------------------------------------------------\n");
    printf("\nEnter Your Phone Number to proceed: ");
    while(phone_no == 0){
        phone_no = get_unsignedlonglong();
        /*int check =phone_no/pow(10,9);
		if( check != 9) {
			printf("-------------Phone Number Must Contain 10 digits and Start with 9-------------\n\nPhone Number: ");
			phone_no = 0;
		}*/
    }
    int generated_login_code;
	re:
	generated_login_code = generate_number(4);             //generates 4 digit login code
	printf("\n\nLOGIN CODE HAS BEEN SENT TO YOU, IN PHONE NUMBER: %llu",phone_no);
	printf("\n\n\nThis is How SMS in Phone Number %llu Looks like:\n",phone_no);
	printf("----------------------------------------------------------------\n");
	printf("Thank You for choosing Us\nYour Login Code is %d ",generated_login_code);
	printf("\n----------------------------------------------------------------\n");
	printf("\nEnter The Login Code: ");
	int code = 0;
	while( code == 0){
        code = get_integer();
    }
	if(generated_login_code == code) {
		home_page();
	}
	else {
		printf("-----------Invalid Login Code-----------");
		goto re;
	}    
    
}


void home_page(){
    FILE *fp;
    // Reading of Bus_data.dat
    fp = fopen("Bus_data.dat", "rb");
    if (fp == NULL) {
        fp = fopen("Bus_data.dat", "wb");  // Creates the file if not present
        if (fp == NULL) {
            printf("Failed to open or create the file");
            return;  // Exit if file creation fails
        }
        fclose(fp);  // Close after creating the file
    }
    // Reopen the file for reading after creation
    fp = fopen("Bus_data.dat", "rb");
    if (fp == NULL) {
        printf("Failed to open the file for reading");
        return;  // Exit if the file can't be opened for reading
    }
    
    // Dynamically allocate memory for bus data
    struct Bus_data *B = malloc(sizeof(struct Bus_data));  // Start with space for 1 record
    if (!B) {
        printf("Memory allocation failed\n");
        return ;
    }
    Bus_cnt = 0;
    while (fscanf(fp, "%s\n%d\n%d\n", 
                   B[Bus_cnt].id, 
                   &B[Bus_cnt].T_seats, 
                   &B[Bus_cnt].A_seats) == 3)  // Read ID, total seats, available seats
    {
        // Read 50 phone numbers (or 0 if not booked)
        for (int i = 0; i < 50; i++) {
            fscanf(fp, "%llu", &B[Bus_cnt].array[i]);
        }
    
        // Read From and To locations
        fscanf(fp, "%s\n%s\n", B[Bus_cnt].from, B[Bus_cnt].to);
    
        // Read time and price
        fscanf(fp, "%s\n%d\n", B[Bus_cnt].Time, &B[Bus_cnt].price);
    
        Bus_cnt++;
        B = realloc(B, (Bus_cnt + 1) * sizeof(struct Bus_data));
        if (!B) {
            printf("Memory reallocation failed\n");
            free(B);
            return ;
        }
    }
    
    fclose(fp);

    
    
    printf("%d",Bus_cnt);
    
    
    
    // Print all the bus data stored in the structure
    for (int i = 0; i < Bus_cnt; i++) {
    printf("Bus ID: %s\n", B[i].id);
    printf("Total Seats: %d\n", B[i].T_seats);
    printf("Available Seats: %d\n", B[i].A_seats);
    
    // Print the phone numbers (if any are booked)
    printf("Booked Phone Numbers: ");
    for (int j = 0; j < 50; j++) {
        if (B[i].array[j] != 0) {  // Only print non-zero (booked) phone numbers
            printf("%llu ", B[i].array[j]);
        }
    }
    printf("\n");

    // Print from and to locations
    printf("From: %s\n", B[i].from);
    printf("To: %s\n", B[i].to);

    // Print the departure time
    printf("Time: %s\n", B[i].Time);
    
    // Print the price
    printf("Price: %d\n", B[i].price);
    printf("\n");
}

    
    
    
    
    
    
    re:
    printf("\n-------------------------------------------------------\n");
    printf("|               BUS RESERVATION SYSTEM                |\n");
    printf("-------------------------------------------------------\n");
    printf("|                                                     |\n");
    printf("|    [1] -> Book Seats                                |\n");
    printf("|                                                     |\n");
    printf("|    [2] -> Get a Refund                              |\n");
    printf("|                                                     |\n");
    printf("|    [3] -> Check Bus Status                          |\n");
    printf("|                                                     |\n");
    printf("|    [4] -> Exit the System                           |\n");
    printf("|                                                     |\n");
    printf("-------------------------------------------------------\n");
    printf("Your Choice:");
    int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}
	if(choice == 1) { booking_page(B); }
	else if(choice == 2) { refund_page(B); }
    else if(choice == 3){ }
	else if(choice > 4) {   
		printf("\n--------CHOOSE AMONG 1, 2, 3, 4--------\n\n");
	}

    if(choice == 4) { exit(0); }
	goto re;
}




void display_headings(){
    printf("\n\nSN");
    print_space(4);
    printf("Bus ID");
    print_space(8);
    printf("Destination");
    print_space(20);
    printf("Available Seats");
    print_space(3);
    printf("Price");
    print_space(3);
    printf("Time of Departure");
}

void display_bus(struct Bus_data B)
{
    graphics_cnt++;                 //increase graphics count
    printf("\n%d",graphics_cnt);
    print_space(6 - cnt_digits(graphics_cnt));
    printf("%s",B.id);
    print_space(14 - strlen(B.id));
    printf("%s to %s",B.from,B.to);
    print_space(28 - strlen(B.from) - strlen(B.to));
    printf("%d",B.A_seats);
    print_space(18 - cnt_digits(B.A_seats));
    printf("%s",B.Time);
}


void display_seats(struct Bus_data B){
    char result[15];
    for(int i=0; i<(B.T_seats - 5) ; i++){    //Looping through Bus's Seats
        B.array[i] = 0 ? strcpy(result,"Available") : strcpy(result,"Booked");
        printf("%d) %s",i+1,result);
        print_space(20 - strlen(result));
        if((i+1) % 4 == 0){ printf("\n"); }
        else if((i+1) % 2 == 0){ printf("\t\t"); }
    }
    for(int i=(B.T_seats - 5); i<5 ; i++){    //Looping through Bus's last 5 Seats
        B.array[i] = 0 ? strcpy(result,"Available") : strcpy(result,"Booked");
        printf("%d) %s",i+1,result);
        print_space(20 - strlen(result));
    }
    
}

