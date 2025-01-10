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
void seat(struct Bus_data *B,int n);
void bus_status_page(struct Bus_data B[]);

//Secondary UDFs
void display_seats(struct Bus_data B);
void display_bus(struct Bus_data B);
void display_headings();



//Global Variable
unsigned long long phone_no;        //Stores Phone Number of the user
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
	if(generated_login_code == code) {      //If login code matches the entered one
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
    while (fscanf(fp, "%[^\n]\n%d\n%d\n", 
                  B[Bus_cnt].id, 
                  &B[Bus_cnt].T_seats, 
                  &B[Bus_cnt].A_seats) == 3) {
        // Read phone numbers for the available seats
        for (int i = 0; i < B[Bus_cnt].T_seats; i++) {
            if (fscanf(fp, "%llu", &B[Bus_cnt].array[i]) != 1) {
                B[Bus_cnt].array[i] = 0; // Default to 0 if missing
            }
        }
    
        // Read 'From' and 'To' locations
        if (fscanf(fp, "\n%[^\n]\n%[^\n]\n", B[Bus_cnt].from, B[Bus_cnt].to) != 2) {
            printf("Failed to read 'From' and 'To' locations\n");
            break;
        }
    
        // Read time and price
        if (fscanf(fp, "%[^\n]\n%d\n", B[Bus_cnt].Time, &B[Bus_cnt].price) != 2) {
            printf("Failed to read 'Time' and 'Price'\n");
            break;
        }
    
        // Increment the bus count and reallocate memory
        Bus_cnt++;
        B = realloc(B, (Bus_cnt + 1) * sizeof(struct Bus_data));
        if (!B) {
            printf("Memory reallocation failed\n");
            free(B);
            return;
        }
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
    printf("\nYour Choice:");
    int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}
	if(choice == 1) { booking_page(B); }
	else if(choice == 2) { refund_page(B); }
    else if(choice == 3){ bus_status_page(B); }      
	else if(choice > 4) {   
		printf("\n--------CHOOSE AMONG 1, 2, 3, 4--------\n\n");
	}

    // Updating Bus File Before Exiting or going to re:
    fp = fopen("Bus_data.dat", "wb");
    if (fp == NULL) { // Corrected condition
        printf("Error writing file\n");
        return;
    }
    
    for (int J = 0; J < Bus_cnt; J++) {
        // Writing id, total seats, and available seats in the file
        fprintf(fp, "%s\n%d\n%d\n", B[J].id, B[J].T_seats, B[J].A_seats);
    
        // Writing seat phone_no when booked; else it will default to zero
        for (int i = 0; i < B[J].T_seats; i++) {
            fprintf(fp, "%llu ", B[J].array[i]);
        }
    
        // Writing destination, time, and price
        fprintf(fp, "\n%s\n%s\n%s\n%d\n", B[J].from, B[J].to, B[J].Time, B[J].price);
    }
    
    fclose(fp); // Close the file

    
    if(choice == 4) { exit(0); }
	goto re;
}


/***************************************************************
 *                      PRIMARY UDFs                           *
 ***************************************************************/

void booking_page(struct Bus_data B[]){
    char from[50],to[50];
    printf("\n\n\n\n=======================================================\n");
    printf("|              BUS RESERVATION SYSTEM                 |\n");
    printf("=======================================================\n");
    printf("|                  BOOK A SEAT                        |\n");
    printf("-------------------------------------------------------\n");
    printf("| Please provide your travel details:                 |\n");
    printf("|                                                     |\n");
    printf("|    Departure City (From): ");
    scanf(" %[^\n]s", from);  // Read input with spaces
    printf("|                                                     |\n");
    printf("|    Destination City (To): ");
    scanf(" %[^\n]s", to);    // Read input with spaces
    printf("|                                                     |\n");
    printf("|    Number of seats You want To Book:");
    int seat_no = 0;        //stores no of seat user wants
    while(seat_no==0){
        seat_no = get_integer();
    }
    trimWhitespace(from);
    trimWhitespace(to);
    re:
    printf("-------------------------------------------------------\n");
    printf("|                                                     |\n");
    printf("| [1] -> Proceed to Check Available Buses             |\n");
    printf("| [2] -> Return to Main Menu                          |\n");
    printf("|                                                     |\n");
    printf("-------------------------------------------------------\n");
    printf("\nYour Choice: ");
    int choice = 0;
	while(choice == 0 )        //This Handles ValueError
	{
		choice = get_integer();
	}
	printf("\n\n-------------------------------------------------------\n");
    printf("| Checking available buses from %s to %s.......       \n", from, to);
    printf("-------------------------------------------------------\n\n");

	int bus_iteration[50];          //considering there may be max of 50 bus that user is searching for
	graphics_cnt = 0;
	if(choice == 1) { 
	    display_headings();
	    for(int i=0 ; i<Bus_cnt ; i++){
	        if(strcasecmp(from,B[i].from)==0 && strcasecmp(to,B[i].to)==0 && B[i].A_seats>=seat_no){
	            bus_iteration[graphics_cnt] = i;
	            display_bus(B[i]);
	        }
	    }
	    if(graphics_cnt == 0){
	        printf("----------Sorry,The Bus Departing from %s To %s is Not Available in Our Syatem----------\n\nHit Enter To Continue:",from,to);
	        getchar();
	        getchar();
	        return ;
	    }
	    again:
	    printf("\nChoose The Bus As per the Serial Number From the Table: ");
	    int choose = 0;
	    while(choose==0){
	        choose = get_integer();
	    }
	    if(choose > graphics_cnt){
	        printf("\n\n----------%d is invalid Choice------------\n",choose);
	        goto again;
	    }
	    else{
	        int bus_choice = bus_iteration[choose-1];
	        seat(&B[bus_choice],seat_no);
	        return;
	    }
	    
	}
	else if(choice == 2) { return ; }
	else{   
		printf("\n--------CHOOSE AMONG 1, 2--------\n\n");
		goto re;
	}
}


void seat(struct Bus_data *B,int n){
    int seats[n];       //stores the seat number user choosed
    re:
    display_seats(*B);
    printf("\n\nEnter %d seat numbers:\n",n);
    for(int i=0 ; i<n ; i++){
        int user_seat_choice=0;
        while(user_seat_choice==0){
            user_seat_choice = get_integer();
            if(user_seat_choice > B->A_seats){
                printf("\n\n-------Invalid seat Choice-------\n");
                user_seat_choice = 0;
            }
            if(B->array[user_seat_choice-1] != 0){
                printf("\n-------Seat Is already Booked, Please Kindly Choose Another Seat---------\n");
                user_seat_choice = 0;
            }
        }
        seats[i] = user_seat_choice;
    }
    printf("\n\nYour Total Will be %d\nAre You Sure You want To Book Seat Number: ",B->price*n);
    for(int i=0 ; i<n ; i++){
        printf("%d,",seats[i]);
    }
    printf("(y/n): ");
    getchar();
    if(getchar() == 'y'){
        for(int i=0 ; i<n ; i++){
            int array_iteration = seats[i]-1;
            B->array[array_iteration] = phone_no;
            B->A_seats--;
        }  
        
        //Providing The bill to the User
        printf("\n\n\n=======================================================\n");
        printf("|                 BUS RESERVATION BILL                |\n");
        printf("=======================================================\n");
        printf("|                                                     |\n");
        printf("|   Phone Number:   %llu                  \n", phone_no);
        printf("|                                                     |\n");
        printf("|   Bus ID:         %s  \n", B->id);
        printf("|                                                     |\n");
        printf("|   Departure Time: %s  \n", B->Time);
        printf("|                                                     |\n");
        printf("|   Route:          %s to %s \n", B->from, B->to);
        printf("|                                                     |\n");
        printf("|   Seat Numbers:   "); 
        for (int i = 0 ; i < n ; i++) {
            printf("%d,", seats[i]);
        }
        printf("\n|                                                     |\n");
        printf("|   Total Fare:     %d             \n",B->price*n);
        printf("|                                                     |\n");
        printf("=======================================================\n");
        printf("|   Thank you for choosing our service!               |\n");
        printf("|                                                     |\n");
        printf("|   For further assistance, contact: 9876543210       |\n");
        printf("=======================================================\n");
        printf("\nNOTICE: PLEASE KEEP SCREENSHOT OF THE BILL\n");
    
    }
    else{ goto re; }
    return ;
}


void refund_page(struct Bus_data B[]){
    int bus_iteration[20];          //Stores the iteration of the bus in database that user has booked seat of
    int cnt=0;            //count number of bus user has booked
    //Find the Matching Phone Numbers
    for(int i = 0 ; i<Bus_cnt ; i++){
        for(int j=0 ; j<B[i].T_seats ; j++){
            if(B[i].array[j] == phone_no){
                bus_iteration[cnt] = i;
                cnt++;
                break;
            }
        }
    }
    if(cnt == 0){
        printf("\n\n----------Sorry, No data are available of Your booking-----------\n");
        return;
    }
    printf("\nPlease Kindly Choose Which One Would You like To Cancel:\n\n");
    for(int i=0 ; i<cnt ; i++){
        int iteration = bus_iteration[i];
        printf("%d)  Bus ID: %s \nDeparture Time: %s \nRoute: %s to %s \nSeat Numbers: ",i+1, B[iteration].id, B[iteration].Time, B[iteration].from, B[iteration].to);
        for(int j=0 ; j<B[iteration].T_seats ; j++){ if(B[iteration].array[j] == phone_no){printf("%d, ",j+1);} }       //printing seat numbers which are booked by user
        printf("\nPrice per Seat: %d \n",B[iteration].price);
    }
    
    printf("\nYour Cancellation Choice: ");
    int choice = 0;
	while(choice == 0 ){        //This Handles ValueError
		choice = get_integer();
		if(choice > cnt){ 
		    choice = 0; 
		    printf("\n\n-----------Invalid Choice-----------\n\n");
		}
	}
	int generated_refund_code;
	re:
	printf("\nSending Refund Code in Phone No: %llu",phone_no);
	generated_refund_code = generate_number(4);             //generates 4 digit login code
	printf("\n\n\nThis is How SMS in Phone Number %llu Looks like:\n",phone_no);
	printf("----------------------------------------------------------------\n");
	printf("Thank You for choosing Us\nYour Refund Code is %d ",generated_refund_code);
	printf("\n----------------------------------------------------------------\n");
	printf("\nEnter The Refund Code: ");
	int code = 0;
	while( code == 0){
        code = get_integer();
    }
	if(generated_refund_code == code) {      //If Refund code matches the entered one
		int iteration = bus_iteration[choice-1];
		int seat_cnt = 0;           //counts no of seat canclled
		for(int j=0 ; j<B[iteration].T_seats ; j++){
            if(B[iteration].array[j] == phone_no){
                B[iteration].array[j] = 0;
                B[iteration].A_seats++;         //Incresase no of Available seats
                seat_cnt++;
            }
        }
		printf("\n\n%d Has been refunded to Your Account\nThank You for valuable Time, We hope for your Good Day.\n\n",B[iteration].price*seat_cnt);
	}
	else {
		printf("\n\n-----------Invalid Login Code-----------\n\n");
		goto re;
	}
	printf("\nHit Enter To Continue:");
    getchar();
    getchar();
}


void bus_status_page(struct Bus_data B[]){
    printf("\n\n\n=======================================================\n");
    printf("|                       BUS STATUS                    |\n");
    printf("=======================================================\n\n");
    graphics_cnt = 0;
    display_headings();
    for(int i=0 ; i<Bus_cnt ; i++){
        display_bus(B[i]);
    }
    again:
    printf("\nChoose The Bus You want to View Status of,\nAs per the Serial Number From the Table: ");
    int choose = 0;
    while(choose==0){
        choose = get_integer();
    }
    if(choose > graphics_cnt){
        printf("\n\n----------%d is invalid Choice------------\n",choose);
        goto again;
    }
    else{
        display_seats(B[choose-1]);
    }
    printf("\nHit Enter To Continue:");
    getchar();
    getchar();
}



/***************************************************************
 *                      SECONDARY UDFs                         *
 ***************************************************************/

void display_headings(){
    printf("\n\nSN");
    print_space(4);
    printf("Bus ID");
    print_space(8);
    printf("Destination");
    print_space(15);
    printf("Available Seats");
    print_space(3);
    printf("Price");
    print_space(3);
    printf("Time of Departure\n");
}

void display_bus(struct Bus_data B)
{
    graphics_cnt++;                 //increase graphics count
    printf("%d",graphics_cnt);
    print_space(6 - cnt_digits(graphics_cnt));
    printf("%s",B.id);
    print_space(14 - strlen(B.id));
    printf("%s to %s",B.from,B.to);
    print_space(28 - strlen(B.from) - strlen(B.to));
    printf("%d",B.A_seats);
    print_space(12 - cnt_digits(B.A_seats));
    printf("%d",B.price);
    print_space(11- cnt_digits(B.price));
    printf("%s\n\n",B.Time);
}


void display_seats(struct Bus_data B){
    char result[15];
    printf("\n\n\n------------------------Bus Number Plate: %s------------------------\n\n\n",B.id);
    for(int i=0; i<(B.T_seats - 5) ; i++){    //Looping through Bus's Seats
        B.array[i] == 0 ? strcpy(result,"Available") : strcpy(result,"Booked");
        printf("%d)",i+1);
        print_space(3 - cnt_digits(i+1));
        printf("%s",result);
        print_space(12 - strlen(result));
        if((i+1) % 4 == 0){ printf("\n\n"); }
        else if((i+1) % 2 == 0){ printf("\t\t"); }
    }
    for(int i=(B.T_seats - 5); i<B.T_seats ; i++){    //Looping through Bus's last 5 Seats
        B.array[i] == 0 ? strcpy(result,"Available") : strcpy(result,"Booked");
        printf("%d)",i+1);
        print_space(3 - cnt_digits(i+1));
        printf("%s",result);
        print_space(12 - strlen(result));
    }
}
