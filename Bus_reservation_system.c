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
    char Time[10];          //Stores Time of Departure
    int price;              //Stores the Price of Bus Ticket
};

//UDFs
void home_page();



//Global Variable
unsigned long long phone_no;        //Stores Phone Number of the user
int Bus_cnt;            //Stores Total Number of Bus



int main(){
    printf("-------------------------------------------------------\n");
    printf("|               BUS RESERVATION SYSTEM                |\n");
    printf("-------------------------------------------------------\n");
    printf("\nEnter Your Phone Number to proceed: ");
    
    home_page();
    
    
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
    while (fscanf(fp, " %s\n%d\t%d\n", 
                   B[Bus_cnt].id, 
                   &B[Bus_cnt].T_seats, 
                   &B[Bus_cnt].A_seats) == 3)  // Read ID, total seats, available seats
    {
        // Read 50 phone numbers (or 0 if not booked)
        for (int i = 0; i < 50; i++) {
            fscanf(fp, "%llu", &B[Bus_cnt].array[i]);
        }
    
        // Read From and To locations
        fscanf(fp, " %s\t%s\n", B[Bus_cnt].from, B[Bus_cnt].to);
    
        // Read time and price
        fscanf(fp, " %s\t%d", B[Bus_cnt].Time, &B[Bus_cnt].price);
    
        Bus_cnt++;
        B = realloc(B, (Bus_cnt + 1) * sizeof(struct Bus_data));
        if (!B) {
            printf("Memory reallocation failed\n");
            free(B);
            return ;
        }
    }
    
    fclose(fp);

    
    
    
    
    
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
}




