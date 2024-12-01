#include <stdio.h>
#include <ctype.h>          // Include ctype.h for isdigit() , tolower()
#include <stdlib.h>         // Include stdlib.h for atoi() and exit()


//UDF DECLEARATION
void graphics();
void game();
void replace_box(int i, int j);
char win_loose_draw();
void board_reset();
int num_check(int n);
int get_integer();


//Global 2D Array representing Board
static char board[5][65] = {
    "   |   |                    1 | 2 | 3 ",
    "-----------                -----------",
    "   |   |                    4 | 5 | 6 ",
    "-----------                -----------",
    "   |   |                    7 | 8 | 9 ",
};

static int data[9];     // Stores the number which have already been choosen
static int cnt=0;       // Stores how many number have been choosen


int main()
{
    do{
        board_reset();      //resets the board every time game starts
        printf("\n\n\t------WELCOME TO TIC TAC TOE------\n\n");
        graphics();
        printf("\n\nWE WILL BE STARTING FROM 'O'\n");
        game();
        printf("\n\nDo You want To Play Again? (y/n): ");
        fflush(stdin);      //flushes out any new lines
        //getchar();        //Use this if Play again Code Doesn't runs
    }
    while(tolower(getchar())=='y');
    return 0;
}



void game()     //Contains Main Game Logic
{
    int i,j;
    while(cnt!=9){

        printf("\n\nPlayer ");
        if(cnt % 2 == 0){       //Since we started from player O, Player O's trun will always be such that cnt % 2 == 0
            printf("O:\n");
        }
        else{
            printf("X:\n");
        }

        int choice = 0;
        while(choice == 0)      //Ensures Given data is an Integer
        {
            printf("Enter the Integer between 1-9: ");
            choice = get_integer();
        }
        
        if(choice >= 1 && choice <= 9)    
        {
            if(num_check(choice)==0)        //Ensures choosen Integer is not choosen before
            {

                data[cnt] = choice;         //Storing the number choosen by the players

                //Setting up row and column where the data is to be stored
                cnt++;
                if(choice >= 1 && choice <= 3){
                    i=0;
                    j=4*choice-3;
                }
                else if(choice >= 4 && choice <= 6){
                    i=2;
                    j=4*(choice-3)-3;
                }
                else if(choice >= 7 && choice <= 9){
                    i=4;
                    j=4*(choice-6)-3;
                }
                
                replace_box(i,j);       // i represent row and j represent column
            }
            else{
                printf("\n%d has already been choosen, Choose another Number",choice);
            }
            continue;
        }
    }
}

void replace_box(int i, int j)
{
    if( cnt % 2 == 0)
    {
        board[i][j] = 'X';
    }
    else
    {
        board[i][j] = 'O';
    }

    graphics();

    
    char result = 'N';
    if(cnt > 4)         //Makes Code More Efficient 
    {
        result = win_loose_draw();
    }

    if(result != 'N')
    {
        printf("\n\nPlayer %c Wins!", result);
        cnt = 9;

    }
    else if(result == 'N' && cnt==9)
    {
        printf("\n\nGame is Draw");
    }
}

char win_loose_draw()   // Checks all the rows and comlumn for any potential winners if(yes) returns Either "O" or "X" else return "N" 
{
    int i;
    for(i=0;i<=5;i+=2)
    {
        if(board[i][1]!=' ' && board[i][1] == board[i][5] && board[i][1] == board[i][9]) //Check Horizontal Rows
        {
            return board[i][1];
        }
    }
    for(i=1;i<=9;i+=4)
    {
        if(board[0][i]!=' ' && board[0][i] == board[2][i] && board[0][i] == board[4][i]) //Check Vertical Columns
        {
            return board[0][i];
        }
    }
    if(board[0][1]!=' ' && board[0][1] == board[2][5] && board[0][1] == board[4][9]) //Check main diagonal
        {
            return board[0][1];
        }
    else if(board[0][9]!=' ' && board[0][9] == board[2][5] && board[0][9] == board[4][1]) //Check secondary diagonal
        {
            return board[0][9];
        }
    return 'N';     //Neither "O" nor "X" has won 
}

void graphics()     //Prints the Game's Graphics
{
    int i,j;
    printf("\n\n");
    for(i = 0 ; i < 5 ; i++ )
    {
        printf("\t");
        for(j = 0 ; j < 65 ; j++)
        {
            printf("%c",board[i][j]);
        }
        printf("\n");
    }
}


int num_check(int n) //returns 1 if number has already be choosen else return 0
{
    int i;
    for(i=0;i<9;i++)
    {
        if(data[i]==n)
        {
            return 1;
        }
    }
    return 0;
}



void board_reset() //Resets all the data stored
{
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 65; j++) {
            if (board[i][j] == 'X' || board[i][j] == 'O'){
                board[i][j] = ' '; // Reset character to space
            }
        }
    }
    for(int i=0;i<9;i++)
    {
        data[i] = ' ' ;   //reset which number have been choosen
    }
    cnt=0;       //reset how many number have been choosen
}



int get_integer(){ // Returns the int if int was inputed else return 0
    char local_choice[30];
    scanf(" %30[^\n]s", local_choice);   // space before %30[^\n]s to clear any new line // or we can use fflush(stdin);

    int is_valid = 1;

    for(int k=0 ;local_choice[k] != '\0'; k++)
    {
        if(!isdigit(local_choice[k]))
        {
            is_valid = 0;        // if any enity except int is entered makes it invalid 
            break; 
        }
    }

    int num = atoi(local_choice);       //changes str to int

    if(is_valid == 1)           //Ensures Given data is an Integer
    {
        return num;
    }
    printf("\n\n-----Choose an Integer-----\n\n");
    return 0;
}
