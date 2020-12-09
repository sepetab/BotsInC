// intensity.c
// Assignment 2, COMP1511 18s1: Intensity
//
// This program by ARAVIND VENKATESWARAN (z5208102) on 21/04/18
//
// Version 1.0.1: Minor changes to wording of comments.
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <assert.h>

// SOME USEFUL #defines - YOU WILL NEED MORE

#define ACTION_PLAYER_NAME    0
#define ACTION_DISCARD        1
#define ACTION_PLAY_CARD      2
#define ACTION_UNIT_TESTS     3

#define N_CARDS              40
#define N_CARDS_INITIAL_HAND 10
#define N_PLAYERS             4
#define N_CARDS_DISCARDED     3

#define CARD_MIN             10
#define CARD_MAX             49


// ADD EXTRA #defines HERE

//These are array indexes!
#define first_array_value 0
#define fourties 3
#define thirties 2

//These are values
#define TENS 10
#define TWENTIES 20
#define CALVES 30
#define FOURTIES 40
#define BUFFALO 47

void print_player_name(void);
void choose_discards(void);
void choose_card_to_play(void);
void run_unit_tests(void);


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE

//Returns the max index in an array.
int Max_Array(int array[],int array_length);
//Returns the min index in an array.
int Min_Array(int array[],int array_length);
//Contains logic to discard cards.
void discard_logic(int input[],int count[],int length[],
                  int card_suits[4][10],int discard[],int num_discarded);
//contains logic to play cards.
int play_logic(int num_cards_inhand,int num_cards_played_inround,
               int cards_inhand[],int cards_played_inround[],int round,
               int num_cards_played,int cards_played[]);
//Returns the index with the least second digit of the cards in hand.
int least_second_digit(int cards_inhand[],int num_cards_inhand);
//Returns the index with the biggest second digit.
int most_second_digit(int cards_inhand[],int num_cards_inhand);


// You should not need to change this main function

int main(void) {

    int which_action = 0;
    scanf("%d", &which_action);
    if (which_action == ACTION_PLAYER_NAME) {
        print_player_name();
    } else if (which_action == ACTION_DISCARD) {
        choose_discards();
    } else if (which_action == ACTION_PLAY_CARD) {
        choose_card_to_play();
    } else {
        run_unit_tests();
    }

    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("Ryuu");
}

void choose_discards() {
    // All variables declaration
    //This is the input array which holds the cards in hand.
    int input[10] = {0};
    //This array stores the number of cards in each suit
    //(4 because 4 suits in total)
    int count[4] = {0};
    //This array holds the values in count array as it will get changed
    int length[4];
    //The 4 and the 10 are the sizes of the two dimensional array where 
    //the 4 is the suit{10's,20's,30's,40's} and the 10 represents the 
    //input card  in that suit.
    int card_suits[4][10];
    //This array holds the discarded values to be printed
    int discard[3] = {0};
    //This is zero at the beginning.
    int num_discarded = 0;
    int suit;
    
    //Getting input through array
    for(int i=0;i<10;i++){
        scanf("%d",&input[i]);
        //The suit is gonna be input/10 for eg. 31/10 =3rd suit but in 
        //an array the address is 3-1 which is 2.
        suit = (input[i] / 10) - 1;
        card_suits[suit][count[suit]] = input[i];
        count[suit]++;
    }
    
   //Logic is implemented here
   discard_logic(input,count,length,card_suits,discard,num_discarded);
   printf("%d %d %d\n",discard[0], discard[1], discard[2]);

}


void choose_card_to_play(void) {

    //All variables declaration
    int num_cards_inhand;
    int num_cards_played_inround;
    int table_position;
    scanf("%d %d %d",&num_cards_inhand,&num_cards_played_inround,
           &table_position);
    int counter_temp=0;
    int cards_inhand[num_cards_inhand];
    
    //If the condition is true, then the array size below would be zero, 
    //this would be invalid,so I changed it to 1.       
    if(num_cards_played_inround==0){ 
        num_cards_played_inround=1;
    //As one is a valid input,I choose a flag to mark it that it was 
    //originally zero.
        counter_temp++;
     }
     
    int cards_played_inround[num_cards_played_inround];
    //This would change it back to zero because it would produce errors 
    //in the program and would ask for an extra input.
    if(num_cards_played_inround==1 && counter_temp==1){
        num_cards_played_inround=0;
        
     }
    //The round varies from 0 to 9 and it is the max cards(10)-num cards 
    //in hand. 
    int round=N_CARDS_INITIAL_HAND-num_cards_inhand;
    //This is the num of previously played cards.
    int num_cards_played=round*4;
    //Same purpose as above!
    if(num_cards_played==0){
    //Not a valid number as it is round*4! so i dont need a flag
        num_cards_played=1;
    }
    
    int cards_played[num_cards_played];
    //Same purpose as above
    if(num_cards_played==1){
        num_cards_played=0;
    }
    
   //Getting inputs
    
    for(int i=0;i<num_cards_inhand;i++){
        scanf("%d",&cards_inhand[i]);
        }
        
    for(int i=0;i<num_cards_played_inround;i++){
        scanf("%d",&cards_played_inround[i]);
        }
    
    for(int i=0;i<num_cards_played;i++){
        scanf("%d",&cards_played[i]);
        } 
        
    int discard1,discard2,discard3,recieved1,recieved2,recieved3;
    scanf("%d %d %d",&discard1,&discard2,&discard3);
    scanf("%d %d %d",&recieved1,&recieved2,&recieved3);
    
    //Logic is implemented here
    int result=play_logic(num_cards_inhand,num_cards_played_inround,
                          cards_inhand,cards_played_inround,round,
                          num_cards_played,cards_played);
    printf("%d\n",result);

}

//NORMAL FUNCTIONS

//First I check if the normal functions(other than logic functions) are 
//working correctly.Method below.

//FOR DISCARDS
//>Firstly,I check if the 47 48 49 elimination is preferred over 
//the 35 36 37 38 39 elimination
//>Secondly,I check if the thirties elimination is preferred over 
//the normal cards
//>Lastly I check if the biggest number of the suit with the most number 
//of cards is eliminated.

//FOR PLAYS
//>Check If My program is legal with 2 extreme cases
//>Check whether it returns the smalles of the suit that is played
//>Whether it plays the smallest second digit in first plays.
//>Whether it reserves least fourties for 47's play.
//>Test if it plays least 40's for 47 round.
//>If 49 or 48 is played and the first card in round in fourties,play 47
//If I have it.
//When free to play anything discard unwanted high number cards such as 
//49,48 or 39,38. 


void run_unit_tests(void) {
    
    //NORMAL FUNCTIONS
    
    //First,I did a basic check
    int array1[10]={20,21,22,23,24,25,26,27,28,29};
    int array_length=10;
    //All these functions return an array index.
    assert(Min_Array(array1,array_length)==0);
    assert(Max_Array(array1,array_length)==9);
    assert(least_second_digit(array1,array_length)==0);
    assert(most_second_digit(array1,array_length)==9);
    
    //I checked the least second digit index more thourougly
    int array2[10]={17,18,19,21,28,31,32,33,39,49};
    assert(Min_Array(array2,array_length)==0);
    assert(Max_Array(array2,array_length)==9);
    assert(least_second_digit(array2,array_length)==3);
    assert(most_second_digit(array2,array_length)==9);
    
    //I checked the Min Max logic thouroughly
    int array3[10]={21,22,23,24,29,31,32,33,48,49};
    assert(Min_Array(array3,array_length)==0);
    assert(Max_Array(array3,array_length)==9);
    assert(least_second_digit(array3,array_length)==0);
    assert(most_second_digit(array3,array_length)==9);
    
    //Complex check with ranges of numbers
    int array4[10]={78,76,75,70,12,11,13,14,99,32};
    assert(Min_Array(array4,array_length)==5);
    assert(Max_Array(array4,array_length)==8);
    assert(least_second_digit(array4,array_length)==3);
    assert(most_second_digit(array4,array_length)==8);
    
    //Another Complex check that checks most second digit throughly.
    int array5[10]={29,25,13,45,22,33,34,35,43,44};
    assert(Min_Array(array5,array_length)==2);
    assert(Max_Array(array5,array_length)==3);
    assert(least_second_digit(array5,array_length)==4);
    assert(most_second_digit(array5,array_length)==0);
    
    //DISCARD LOGIC

    //This test checks if the program eliminates fourties over thirties.
    int input[] = {14,15,17,19,21,29,38,39,48,49};
    int card_suits[4][10]= {{14, 15, 17,19}, {21,29}, {38,39}, {48,49}};
    int count[4]={4,2,2,2};
    int num_discarded=0;
    int length[4];
    int discard[3];
    discard_logic(input,count,length,card_suits,discard,num_discarded);
    assert(discard[0] == 48 && discard[1] == 49 && discard[2] == 39);

    //This test checks if the program eliminates thirties over normal 
    //numbers.    
    int input2[] = {14,15,17,19,21,29,36,37,38,39};
    int card_suits2[4][10]= {{14, 15, 17,19}, {21,29}, {36,37,38,39}, {}};
    int count2[4]={4,2,4,0};
    int num_discarded2=0;
    int length2[4];
    int discard2[3];
    discard_logic(input2,count2,length2,card_suits2,discard2,num_discarded2);
    assert(discard2[0] == 39 && discard2[1] == 38 && discard2[2] == 37);

    //Checks if the biggest number of the biggest suit gets eliminated.    
    int input3[] = {14,15,17,19,21,22,23,24,31,41};
    int card_suits3[4][10]= {{14, 15, 17,19}, {21,22,23,24}, {31}, {41}};
    int count3[4]={4,4,1,1};
    int num_discarded3=0;
    int length3[4];
    int discard3[3];
    discard_logic(input3,count3,length3,card_suits3,discard3,num_discarded3);
    assert(discard3[0] == 24 && discard3[1] == 19 && discard3[2] == 23);

    //Checks if the biggest number of the biggest suit gets eliminated but 
    //also checks if thirties gets eliminated over other numbers.    
    int input4[] = {14,15,17,19,21,31,32,33,34,41};
    int card_suits4[4][10]= {{14, 15, 17,19}, {21}, {31,32,33,34}, {41}};
    int count4[4]={4,1,4,1};
    int num_discarded4=0;
    int length4[4];
    int discard4[3];
    discard_logic(input4,count4,length4,card_suits4,discard4,num_discarded4);
    assert(discard4[0] == 34 && discard4[1] == 19 && discard4[2] == 33);

    //Checks all of those above conditions in one go.    
    int input5[] = {14,15,17,19,21,22,23,24,39,49};
    int card_suits5[4][10]= {{14, 15, 17,19}, {21,22,23,24}, {38}, {49}};
    int count5[4]={4,4,1,1};
    int num_discarded5=0;
    int length5[4];
    int discard5[3];
    discard_logic(input5,count5,length5,card_suits5,discard5,num_discarded5);
    assert(discard5[0] == 49 && discard5[1] == 38 && discard5[2] == 24);
    
    //Checks for legal play
    int num_cards_inhand=10;
    int num_cards_played_inround=3;
    int cards_inhand[10]={12,14,18,21,22,23,24,25,38,39};
    int cards_played_inround[3]={39,27,36};
    int round=0;
    int num_cards_played=0;
    int cards_played[1]={0};
   assert(play_logic(num_cards_inhand,num_cards_played_inround,
                      cards_inhand,cards_played_inround,round,
                      num_cards_played,cards_played)==38); 
               
    //Another  check for legal play
    int num_cards_inhand2=7;
    int num_cards_played_inround2=2;
    int cards_inhand2[7]={24,25,26,27,28,29,49};
    int cards_played_inround2[2]={40,36};
    int round2=3;
    int num_cards_played2=12;
    int cards_played2[12]={12,13,14,15,16,17,18,19,20,21,22,23};
   assert(play_logic(num_cards_inhand2,num_cards_played_inround2,
                      cards_inhand2,cards_played_inround2,round2,
                      num_cards_played2,cards_played2)==49);  
    
    //Checks if it returns smallest of the suit played
    int num_cards_inhand3=7;
    int num_cards_played_inround3=2;
    int cards_inhand3[7]={20,21,22,23,24,25,26};
    int cards_played_inround3[2]={27,49};
    int round3=3;
    int num_cards_played3=12;
    int cards_played3[12]={12,13,14,15,16,17,18,19,39,37,36,35};
    assert(play_logic(num_cards_inhand3,num_cards_played_inround3,
                      cards_inhand3,cards_played_inround3,round3,
                      num_cards_played3,cards_played3)==20);
    
    //Checks if the least 40 isn't returned unless a 47 is played.
    int num_cards_inhand4=7;
    int num_cards_played_inround4=2;
    int cards_inhand4[7]={40,41,42,43,44,45,46};
    int cards_played_inround4[2]={48,49};
    int round4=3;
    int num_cards_played4=12;
    int cards_played4[12]={12,13,14,15,16,17,18,19,39,37,36,35};
    assert(play_logic(num_cards_inhand4,num_cards_played_inround4,
                      cards_inhand4,cards_played_inround4,round4,
                      num_cards_played4,cards_played4)==41);
                      
    //Checks if the least 40 is played if a 47 is played
    int num_cards_inhand5=7;
    int num_cards_played_inround5=2;
    int cards_inhand5[7]={40,41,42,43,44,45,46};
    int cards_played_inround5[2]={48,47};
    int round5=3;
    int num_cards_played5=12;
    int cards_played5[12]={12,13,14,15,16,17,18,19,39,37,36,35};
    assert(play_logic(num_cards_inhand5,num_cards_played_inround5,
                      cards_inhand5,cards_played_inround5,round5,
                      num_cards_played5,cards_played5)==40);
                       
    //Checks if 47 is played if 48 or 49 is played.
    int num_cards_inhand6=7;
    int num_cards_played_inround6=2;
    int cards_inhand6[7]={40,41,42,43,44,45,47};
    int cards_played_inround6[2]={48,45};
    int round6=3;
    int num_cards_played6=12;
    int cards_played6[12]={12,13,14,15,16,17,18,19,20,21,22,23};
    assert(play_logic(num_cards_inhand6,num_cards_played_inround6,
                      cards_inhand6,cards_played_inround6,round6,
                      num_cards_played6,cards_played6)==47);
    
    //If free to play,it discards highest number. 
    int num_cards_inhand7=7;
    int num_cards_played_inround7=2;
    int cards_inhand7[7]={30,31,32,33,34,35,37};
    int cards_played_inround7[2]={18,45};
    int round7=3;
    int num_cards_played7=12;
    int cards_played7[12]={12,13,14,15,16,17,18,19,20,21,22,23};
    assert(play_logic(num_cards_inhand7,num_cards_played_inround7,
                      cards_inhand7,cards_played_inround7,round7,
                      num_cards_played7,cards_played7)==37); 
                      
    //If free to play,it discards highest number. 
    int num_cards_inhand8=7;
    int num_cards_played_inround8=2;
    int cards_inhand8[7]={30,31,32,33,34,37,49};
    int cards_played_inround8[2]={18,45};
    int round8=3;
    int num_cards_played8=12;
    int cards_played8[12]={12,13,14,15,16,17,18,19,20,21,22,23};
    assert(play_logic(num_cards_inhand8,num_cards_played_inround8,
                      cards_inhand8,cards_played_inround8,round8,
                      num_cards_played8,cards_played8)==49);
    
    //If free to play,it discards highest number but 30's are prefered
    //over 20's.
    int num_cards_inhand9=7;
    int num_cards_played_inround9=2;
    int cards_inhand9[7]={17,18,19,20,22,29,39};
    int cards_played_inround9[2]={41,45};
    int round9=3;
    int num_cards_played9=12;
    int cards_played9[12]={12,13,14,15,16,17,18,19,30,31,32,33};
    assert(play_logic(num_cards_inhand9,num_cards_played_inround9,
                      cards_inhand9,cards_played_inround9,round9,
                      num_cards_played9,cards_played9)==39);
                      
    //If free to play it chooses the biggest second digit and the biggest 
    //first digit                                                                     
    int num_cards_inhand10=7;
    int num_cards_played_inround10=2;
    int cards_inhand10[7]={25,26,27,29,39,41,42};
    int cards_played_inround10[2]={12,13};
    int round10=3;
    int num_cards_played10=12;
    int cards_played10[12]={12,13,14,15,16,17,18,19,30,31,32,33};
    assert(play_logic(num_cards_inhand10,num_cards_played_inround10,
                      cards_inhand10,cards_played_inround10,round10,
                      num_cards_played10,cards_played10)==39);                              
               
              

}


// ADD YOUR FUNCTIONS HERE

//To get index of the max number in an array
int Max_Array(int array[],int array_length){

    int max=0;
    
    for(int i=1;i<array_length;i++){
       if(array[i]>=array[max]){
            max=i;
       }
    }
    return max;
}

//To get index of the min number in an array
int Min_Array(int array[],int array_length){

    int min=Max_Array(array,array_length);
    
    for(int i=0;i<array_length;i++){
       if(array[i]<array[min]){
            min=i;
       }
    }
    return min;
}


int least_second_digit(int cards_inhand[],int num_cards_inhand){
    //Created an array to store the second digits of the cards in hand 
    //(it is in increasing order)    
    int second_digit_array[num_cards_inhand];
    for(int n=0;n<num_cards_inhand;n++){
        //This is the formula for the second digit,where i divide it by  
        //10 and multiply it again to get the tens value(eg.10,20,30||40) 
        //and I subtract this from the original value(eg.49-40=9) for 
        //the second digit.
        int second_digit=cards_inhand[n]-((cards_inhand[n]/10)*10);
        //I do this because you are not supposed to play the thirties in  
        //the first round unless it is played before.
        if((cards_inhand[n]/10)==3){
            //If the card number is in the thirties,i turn it into a big 
            //impossible second digit value,in this case 10.
            second_digit=10;
        }
        second_digit_array[n]=second_digit;
    }
    //I want the minimum second digit to be played.
    int index = Min_Array(second_digit_array,num_cards_inhand);
    return index;
}

int most_second_digit(int cards_inhand[],int num_cards_inhand){
    //Created an array to store the second digits of the cards in hand 
    //(it is in increasing order)
    int index=0;    
    int second_digit_array[num_cards_inhand];
    for(int n=0;n<num_cards_inhand;n++){
        //This is the formula for the second digit,where i divide it by  
        //10 and multiply it again to get the tens value(eg.10,20,30||40) 
        //and I subtract this from the original value(eg.49-40=9) for 
        //the second digit.
        int second_digit=cards_inhand[n]-((cards_inhand[n]/10)*10);
        second_digit_array[n]=second_digit;
    }
    //I want the maximum second digit to be played.
    index = Max_Array(second_digit_array,num_cards_inhand);
    
    //This function is used in playing cards if I am free to play,
    //If I had these cards,I would want it to get disposed first.
    for(int n=num_cards_inhand-1;n>=0;n--){
        if(cards_inhand[n]==49||cards_inhand[n]==48||
           cards_inhand[n]==47||cards_inhand[n]==39||
           cards_inhand[n]==38||cards_inhand[n]==37){
            index=n;
            break;
        }
    }    
        
    return index;
}



//The 4 and the 10 are the sizes of the two dimensional array where 
//the 4 is the suit{10's,20's,30's,40's} and the 10 
//represents the input card in that suit.
void discard_logic(int input[],int count[],int length[],
                  int card_suits[4][10],int discard[],
                  int num_discarded){ 

    //In this step,I want to discard any cards that are the buffalo or 
    //higher.

    for(int k=0;k<count[fourties];k++){ 
        if((card_suits[fourties][k]==48) || 
           (card_suits[fourties][k]==49) || 
           (card_suits[fourties][k]==BUFFALO )){
            discard[num_discarded]=card_suits[fourties][k];
            //-1 to replace addres with the least value so it dosent 
            //cause me problems in the third step(line 212).
            card_suits[fourties][k]=-1; 
            num_discarded++;
        }
    }
    
   
   //After checking for the buffalo,I want to get rid of any high thirty 
   //values that can make me get penalty points.
   for(int k=count[thirties]-1;k>=0&&(num_discarded<3);k--){
        if((card_suits[thirties][k]==39) || 
           (card_suits[thirties][k]==38) || 
           (card_suits[thirties][k]==37) || 
           (card_suits[thirties][k]==36)){
            discard[num_discarded]=card_suits[thirties][k];
            //-1 to replace addres with the least value so it dosent  
            //cause problems in the third step(line 212).
            card_suits[thirties][k]=-1; 
            num_discarded++;
        }
   }
   
   //Getting the index of the suit which has the most number of cards.
   int max_index=Max_Array(count,4);
   
   //Copying length array to count array so we have a copy of count array 
   //for the next step.
   for(int i=0; i<4; i++){
      length[i] = count[i];
   }
   
   //If my cards dont contain any of the cards above, it discards the 
   //highest card from the suit which has the most number of cards.
   while(num_discarded < 3){
    for(int i=0;i<4;i++){
        while(max_index==i && num_discarded < 3){
            int to_discard = Max_Array(card_suits[i],length[i]);
            discard[num_discarded] = card_suits[i][to_discard];
            count[i]--;
            //-1 to replace addres with the least value so it dosent 
            //cause problems in the next loop.
            card_suits[i][to_discard]=-1;
            num_discarded++;
            max_index=Max_Array(count,4);   
        }
    } 
   }

}

int play_logic(int num_cards_inhand,int num_cards_played_inround,
               int cards_inhand[],int cards_played_inround[],int round,
               int num_cards_played,int cards_played[]){
    //Array index with the largest card
    int biggest_card = num_cards_inhand-1;
    //Acts like a marker for multipurposes
    int checker=0;
    int checker2=0;
    int checker3=0;
    // To Count number of stuff in an array when needed
    int counter_temp=0;
    //This is the variable that is returned.
    int result=0;
    //Temp index variable
    int i=0;
    
//All numbers here can be understood literally through the naming of 
//variables,any other will be annotated to help with markings!
    
    //The first step I did was to find if there was a need for me to play 
    //first,in other words ,if I had won the previous round or if I start.
    if(num_cards_played_inround==0){
        //If I had only thirties,play the least thirty.
        if(cards_inhand[first_array_value]>=30 && 
           cards_inhand[biggest_card]<=39){
            result=cards_inhand[first_array_value];
        //If I had cards ranging from 30's to 40's only.
        }else if(cards_inhand[first_array_value]>=30 && 
                (cards_inhand[biggest_card]<=49 && 
                 cards_inhand[biggest_card]>=40)){
            //This is to seperate out the cards played in the previous 
            //round.
            int last_round_cards = num_cards_played-4;
            for(int k=last_round_cards;k<num_cards_played;k++){
                //if someone played the thirties in the previous round,I 
                //play the least thirty I have.
                if(cards_played[k]>=30 && cards_played[k]<=39){
                    //A flag that is triggered to check in the next if 
                    //else statement.
                    checker=1;
                }
            }
            if(checker==1){
                //first value is going to be in the thirties or fourties
                //(refer above)and is going to be the least.
                result=cards_inhand[first_array_value];
            
            }else{
                int n=0;
                //Checking the first array index that contains the first 
                //integer in the 40's and returning that.
                while((n<biggest_card)&&(cards_inhand[n]<FOURTIES)){
                    n++;
                }
                //The index is stored in n for this loops and relevant 
                //loops below.
                result=cards_inhand[n];
            }
        }else{
            //If I had 10's or 20's, I first check if the previous round 
            //had played in the thirties so I can play 30's to get rid 
            //of mine.
            if(round>0){
                //This is to seperate out the cards played in the previous 
                //round.
                int last_round_cards = num_cards_played-4;
                for(int k=last_round_cards;k<num_cards_played;k++){
                    //if someone played the thirties in the previous 
                    //round,I play the least thirty I have.
                    if(cards_played[k]>=30 && cards_played[k]<=39){
                        //A flag that is triggered to check in the next 
                        //if else statement.
                        checker=1;
                    }
                }
            }
            if(checker==1){
                int n=0;
                //Checking the first array index that contains the first 
                //integer in the 30's and returning that.
                while((n<biggest_card)&&(cards_inhand[n]<30)){
                    n++;
                }
                //If I had thirties,I would rather play my least one!
                if(cards_inhand[n]>=30 && cards_inhand[n]<=39){
                    result=cards_inhand[n];
                }else{
                    int index=least_second_digit(cards_inhand,
                                                 num_cards_inhand);
                    //If not,in these two else conditions, to avoid 
                    //winning I play the least second digit number.
                    result=cards_inhand[index];   
                }               
            }else{
                int index=least_second_digit(cards_inhand,num_cards_inhand);
                result=cards_inhand[index]; 
            }
        }
        
    //Secondly,If previous cards had been played....    
    }else if(num_cards_played_inround==1||num_cards_played_inround==2||
             num_cards_played_inround==3){
        //If first played card is in the the range 10 to 19   
        if(cards_played_inround[first_array_value]>=10 && 
           cards_played_inround[first_array_value]<=19){
           //If I had no cards In that range,I choose to eliminate
           //Any cards that could make me get penalty points.
           if(cards_inhand[first_array_value]>19){
                i=most_second_digit(cards_inhand,num_cards_inhand);
                result=cards_inhand[i];
            //Or I play the least card.    
           }else{
                result=cards_inhand[first_array_value];
           }
        //If first played card is in the the range 20 to 29
        }else if(cards_played_inround[first_array_value]>=20 && 
                 cards_played_inround[first_array_value]<=29){
            int n=0;  
            //Checking the first array index that contains the first  
            //integer in the 20's and returning that.                                                                             
            while((n<biggest_card)&&(cards_inhand[n]<20)){
                n++;
            }
            //If no twenties,I would try to eliminate the cards with max 
            //second digit to increase my chances of winning.
            if(cards_inhand[n]>=30||cards_inhand[n]<20){
                i=most_second_digit(cards_inhand,num_cards_inhand); 
                result=cards_inhand[i];  
            }else{
                
                result=cards_inhand[n];
            }
        //If first played card is in the the range 30 to 39
        }else if(cards_played_inround[first_array_value]>=30 && 
                 cards_played_inround[first_array_value]<=39){
            int n=0;
            //Checking the first array index that contains the first 
            //integer in the 30's and returning that.
            while((n<biggest_card)&&(cards_inhand[n]<30)){
                n++;
            }
            
            //If no thirties,I would try to eliminate the cards with max 
            //second digit to increase my chances of winning.
            if(cards_inhand[n]>=40||cards_inhand[n]<30){
                i=most_second_digit(cards_inhand,num_cards_inhand); 
                result=cards_inhand[i];   
            }else{
               result=cards_inhand[n];
            }
        //If first played card is in the the range 40 to 49            
        }else if(cards_played_inround[first_array_value]>=40 && 
                 cards_played_inround[first_array_value]<=49){
            
            //Another need for a marker flag.     
            checker=0;
            checker2=0;
            checker3=0;
            for(int k=0;k<num_cards_played_inround;k++){
                //Checker checks if a 47 had been played in the previous 
                //round
                if(cards_played_inround[k]==47){
                    checker=1;
                //Checks if 48 or 49 are played.    
                }else if(cards_played_inround[k]==48||
                         cards_played_inround[k]==49){
                    checker2=1;            
                } 
            }
            for(int k=0;k<num_cards_inhand;k++){
                //Checks if I have 47
                if(cards_inhand[k]==47){
                    checker3=1;
                }
            }        
            //Checking the first array index that contains the first 
            //integer in the 40's and returning that.
            int n=0;
            while((n<biggest_card)&&(cards_inhand[n]<40)){
                n++;
            }
            //If no fourties,I would try to eliminate the cards with max 
            //second digit to increase my chances of winning.
            if(cards_inhand[n]<40){
                i=most_second_digit(cards_inhand,num_cards_inhand);
                result=cards_inhand[i];
            //If I had fourties        
            }else{
                //if 47 had been played,then play the least 40            
                if(checker==1){
                    result=cards_inhand[n];
                }else{

                    if(n+1<num_cards_inhand && ((cards_inhand[n+1]!=47 &&
                       cards_inhand[n+1]!=48 && cards_inhand[n+1]!=49) ||
                      (cards_inhand[n]>=30 && cards_inhand[n]<=39))){
                    //(n+1) refers to the next array value as it is sorted 
                    //in increasing order (this index refers to the second 
                    //least 40 as least 40 is for buffalo round)
                        result=cards_inhand[n+1];
                    }else{
                        result=cards_inhand[n];
                    }
                    
                }
            }
            //If I have 47 and a 48 or 49 had been played,I play 47.
            if(checker3==1 && checker2==1){
                result=47;
            }
        }     
        
    }else{
        result=cards_inhand[first_array_value];
    }
    return result;
}
    
    





