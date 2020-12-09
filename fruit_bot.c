// fruit_bot.c
// Assignment 3, COMP1511 18s1: Fruit Bot
//
// This program by Aravind Venkateswaran (z5208102) on 21/05/18
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: minor bug in main fixed

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "fruit_bot.h"

#define MAXIMUM 250000


void print_player_name(void);
void print_move(struct bot *b);
void run_unit_tests(void);

// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
//This is the struct that I created which will help me along the way
struct profit {
    int profit;
    struct location *buy;
    struct location *sell;
    char *fruit;
    int quantity_buy;
    int quantity_sell;
    int total_profit;
};

//This function returns the number of locations
int number_of_locations(struct bot *head);
//This function calculates the best profit in the world to buy by taking a profit
//array.
struct profit max_profit_buy(struct profit array[],int profit_array_index,
                             struct bot *head);
//This function returns the least distance from current location to 
//specified location.
int east_or_west(struct location *location1, struct location *head);
//This function creates the struct profit array to buy fruits.
struct profit buy(struct bot *b);
//This function creates the struct profit array to sell fruits.
struct profit sell(struct bot *b);
//This function calculates the best profit in the world to sell by taking a profit
//array.
struct profit max_profit_sell(struct profit array[],int profit_array_index,
                              struct bot *head);
//A function to dump fruits if I have excess fruits.                              
struct profit dump_fruit(struct bot *b);
//Checks if the bot needs electricity 
int electricity_checker(int move,struct bot *b,int quantity_to_buy,
                        struct profit best_move);
//Searches for nearest electricity location
struct location *nearest_electricity(struct bot *b,int quantity_to_buy,
                                     int move,struct profit best_move);
//Checks if the location has electricity for sale.                                     
int has_electricity_for_sale(struct location *l,int quantity_to_buy);
//checks for the greatest quantity in a location array.
struct location *quantity_max(struct location *array[],
                              int index,struct bot *head);
                              


// YOU SHOULD NOT NEED TO CHANGE THIS MAIN FUNCTION

int main(int argc, char *argv[]) {
    if (argc > 1) {
        // supply any command-line argument to run unit tests
        run_unit_tests();
        return 0;
    }
    struct bot *me = fruit_bot_input(stdin);
    if (me == NULL) {
        print_player_name();
    } else {
        print_move(me);
    }
    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("Ryuu");

}

// print_move - should print a single line indicating
//              the move your bot wishes to make
//
// This line should contain only the word Move, Sell or Buy
// followed by a single integer

void print_move(struct bot *b) {
    
   struct profit best_move;
   int electricity_return_val;
   int numoflocations = number_of_locations(b);
   int quantity_to_buy = b->battery_capacity-b->battery_level;
   struct location *electricity = NULL;
   int mark=0;
   int mark2 =0;
   int turns_left_condition =0;
   int double_checker=0;
   //To check number of locations.
   //DEBUG->printf("THE LOC ARE %d",numoflocations);
   
   //This just triggers the right struct profit array maker.
   //If no fruit->buy,else->sell.
   if(b->fruit_kg == 0){    
    best_move = buy(b);
   }else{
    best_move = sell(b);
   }
  
   
   
   //If no fruit and buying something.
   if(b->fruit_kg == 0){
        if(best_move.buy != NULL && best_move.profit>0){
        //This allowed me to test the whether the program returned the 
        //best buying location
        /*DEBUG->printf("The best buying location is %s because the 
                        profit is %d\n",best_move.buy->name,
                        best_move.profit);*/
            //Distance to buying location            
            int move = east_or_west(best_move.buy,b->location);
            //Distance to Selling location
            int sell_move = east_or_west(best_move.sell,b->location);
            electricity = nearest_electricity(b,quantity_to_buy,
                                              move,best_move);
            //This applies to turns in the beginning where you dont have 
            //Much cash. When buying something this checks if u have the 
            //electricity to do this and if not,it checks whether the 
            //battery level is sufficient.If both fails,a marker is triggered.                                
            if(electricity!=NULL){
                if((b->cash-(-best_move.buy->price*best_move.quantity_buy) 
                             <=(-electricity->price)*quantity_to_buy) && 
                              b->battery_level<(abs(sell_move)+abs(move))){
                    mark=1;
                }
                if(b->battery_level>= abs(east_or_west(electricity,b->location))){
                    double_checker=1;
                }      
            }
            
            //checks if electricity is required         
            electricity_return_val = electricity_checker(move,b,
                                                         quantity_to_buy,
                                                         best_move);
            if(electricity!=NULL){                                             
               turns_left_condition = 
               (abs(east_or_west(best_move.buy,electricity))/b->maximum_move)
               +(abs(east_or_west(electricity,b->location))/b->maximum_move)+
               (abs(east_or_west(best_move.sell,best_move.buy))/b->maximum_move)+3;
            }else{
                turns_left_condition = 0;
            }                                                    
            //Electricity buying conditions                                             
            if((electricity_return_val == 1 || mark==1) && 
               (b->turns_left>turns_left_condition)&& double_checker==1){
                    
                    move = east_or_west(electricity,b->location);
                    if(b->cash<=200){
                        quantity_to_buy = 0.5*b->cash/abs(electricity->price);
                    }  
                    //The debug below triggeres when electricity needed. This
                    //allowed me to test this logic.   
                    //DEBUG->printf("MOVE IS :%d\n",move);
                    if(move==0){
                        printf("Buy %d",quantity_to_buy);
                    }else{
                        printf("Move %d",move);
                    }
                                
            //If electricity is not needed.    
            }else{
                electricity_return_val = electricity_checker(move,b,
                                                             quantity_to_buy,
                                                             best_move);
                turns_left_condition = abs(move)/b->maximum_move+
                                       abs(sell_move)/b->maximum_move+2;
                if(move!=0){
                    printf("Move %d\n",move);
                //Conditions to buy anything.    
                }else if(move==0 && (b->turns_left>turns_left_condition && 
                        (b->battery_level>=abs(sell_move)||electricity!=NULL))){
                    printf("Buy %d\n",best_move.quantity_buy);
                }else{
                    printf("Move %d\n",move);
                }    
            }
        //If not suitable to buy anything.        
        }else{
            printf("Move 0");
        }
    //If selling something         
    }else{
        //I tested if it really is the best selling location.
        //DEBUG->/*printf("The best selling location is %s because the 
        //profit is %d\n",best_move.sell->name,best_move.profit);*/
          int move = east_or_west(best_move.sell,b->location);
          electricity_return_val = electricity_checker(move,b,quantity_to_buy,
                                                       best_move);
          electricity = nearest_electricity(b,quantity_to_buy,move,best_move);
          if(electricity!=NULL){                                             
               turns_left_condition = 
               (abs(east_or_west(electricity,b->location))/b->maximum_move)+
               (abs(east_or_west(best_move.sell,electricity))/b->maximum_move)+2;
          if(b->battery_level>= abs(east_or_west(electricity,b->location))){
               double_checker=1;
          }     
          }else{
                turns_left_condition = 0;
                double_checker=0;
          }
                                                             
          //Checks if electricity is needed                                             
          if((electricity_return_val == 1) && double_checker==1 && 
             b->turns_left > turns_left_condition){
             
                move = east_or_west(electricity,b->location);
                //DEBUG->printf("MOVE IS :%d\n",move);
                if(b->cash<=200){
                        quantity_to_buy = 0.5*b->cash/abs(electricity->price);
                }        
                if(move==0){
                    printf("Buy %d",quantity_to_buy);
                }else{
                    printf("Move %d",move);
                }
          //If electricity is not needed      
          }else{
            if(move!=0){
                printf("Move %d\n",move);
            }else if(move==0){
                printf("Sell %d\n",best_move.quantity_sell);
            }       
          }
   }           
     
//This prints out all the data that is being tested to make sure everthing
//is right.
/*DEBUG->printf("The best move is with profit %d at buying location %s 
and selling location %s, fruit is %s, buying quantity is %d and selling 
quantity is %\n",best_move.profit,best_move.buy->name,best_move.sell->name,
best_move.fruit,best_move.quantity_buy,best_move.quantity_sell);*/
}


// ADD A COMMENT HERE EXPLAINING YOUR OVERALL TESTING STRATEGY
/*
int number_of_locations:
I tested this manually by using the refree and counting the locations.
I debugged it.<DEBUGGING IN THE PROGRAM>
struct profit max_profit_buy:
I tested this manually by using the refree and calculating the profit.
I debugged it.<DEBUGGING IN THE PROGRAM>
int east_or_west:
I tested this manually by using the refree and counting the locations.
I debugged it.<DEBUGGING IN THE PROGRAM>
struct profit buy:
I printed out the whole array and made sure it was created properly.
I debugged it.<DEBUGGING IN THE PROGRAM>
struct profit sell:
I printed out the whole array and made sure it was created properly.
I debugged it.<DEBUGGING IN THE PROGRAM>
struct profit max_profit_sell:
struct profit dump_fruit:
Used debug referee to test out.
int electricity_checker:
Used debug referee to test out.
struct location *nearest_electricity:
Used debug referee to test out and printed out details.
int has_electricity_for_sale:
Used debug referee to test out.
struct location *quantity_max:
Calculated and checked if the move is done correctly

All these testing strategies were double checked through lab11 worlds and
the debug refree.I also created my own world file with specific scenarious 
to see what my bot does.This is the world 0 file in my lab11 folder. 
It was too hard to create worlds,all my functions required a world and 
most of them a bot,this is all i could do...Sorry!. 
*/
void run_unit_tests(void) {
    // PUT YOUR UNIT TESTS HERE
    // This is a difficult assignment to write unit tests for,
    // but make sure you describe your testing strategy above.
}
// ADD YOUR FUNCTIONS HERE

//If the bot is going to buy,This function produces a struct profit array
//with every buying and selling locations and their profit.
struct profit buy(struct bot *b){
   int numberoflocations = 1;
   numberoflocations = number_of_locations(b);
   struct profit array[MAXIMUM];
   //This tracks the struct profit array index.
   int profit_array_index=0;
   struct location *iloop = b->location;
   struct location *kloop = b->location;
   //This struct has the best solution in the world with the most profit.
   struct profit best_move;
   int total_profit =0;
   //This is a double loop which scans for buying and selling locations
   //and puts them in struct profit array calculating their profits and
   //everything in the struct.
   for(int k=0;k<numberoflocations;k++){
        for(int i=0;i<numberoflocations;i++){
            if(strcmp(iloop->fruit,kloop->fruit)==0){
                if(kloop->quantity !=0 && iloop->quantity!=0){
                    if(iloop->price >0 && kloop->price<0){
                        if(kloop->quantity < iloop->quantity){
                            array[profit_array_index].profit = 
                            iloop->price*(b->cash/abs(kloop->price)) + 
                            kloop->price*(b->cash/abs(kloop->price));
                            array[profit_array_index].quantity_buy = 
                            kloop->quantity;
                            array[profit_array_index].quantity_sell = 
                            kloop->quantity;
                        }else if(kloop->quantity > iloop->quantity){
                            array[profit_array_index].profit = 
                            iloop->price*(b->cash/abs(iloop->price)) + 
                            kloop->price*(b->cash/abs(iloop->price));
                            array[profit_array_index].quantity_buy = 
                            iloop->quantity;
                            array[profit_array_index].quantity_sell = 
                            iloop->quantity;
                        }else{
                            array[profit_array_index].profit = iloop->
                            price*(b->cash/abs(iloop->price)) + 
                            kloop->price*(b->cash/abs(kloop->price));
                            array[profit_array_index].quantity_buy =
                            iloop->quantity;
                            array[profit_array_index].quantity_sell = 
                            iloop->quantity;
                        }               
                        array[profit_array_index].buy = kloop;
                        array[profit_array_index].sell = iloop;
                        array[profit_array_index].fruit = iloop->fruit;
                        profit_array_index++;
                        iloop = iloop->east;
                        
                    }else if(iloop->price <0 && kloop->price>0){
                        if(iloop->quantity < kloop->quantity){
                            array[profit_array_index].profit = 
                            kloop->price*(b->cash/abs(iloop->price)) + 
                            iloop->price*(b->cash/abs(iloop->price));
                            array[profit_array_index].quantity_buy = 
                           iloop->quantity;
                            array[profit_array_index].quantity_sell = 
                           iloop->quantity;
                        }else if(iloop->quantity > kloop->quantity){
                            array[profit_array_index].profit = 
                            iloop->price*(b->cash/abs(kloop->price)) + 
                            kloop->price*(b->cash/abs(kloop->price));
                            array[profit_array_index].quantity_buy = 
                            kloop->quantity;
                            array[profit_array_index].quantity_sell = 
                            kloop->quantity;
                        }else{
                            array[profit_array_index].profit = 
                            iloop->price*(b->cash/abs(iloop->price)) + 
                            kloop->price*(b->cash/abs(kloop->price));
                            array[profit_array_index].quantity_buy = 
                            iloop->quantity;
                            array[profit_array_index].quantity_sell = 
                            iloop->quantity;
                        }                                   
                        array[profit_array_index].buy = iloop;
                        array[profit_array_index].sell = kloop;
                        array[profit_array_index].fruit = iloop->fruit;
                        profit_array_index++;
                        iloop = iloop->east;
                            
                    }
               }     
            }
            iloop = iloop->east;
        }
        kloop = kloop->east;
        iloop = b->location;
   }
   
   //This prints for debugging if array details were correct and calculates
   //Total profit.
   for(int i=0;i<profit_array_index;i++){
        /*DEBUG->printf("BUY:Profit array index is %d and my array profit contains 
        %d with sell location %s and buy location %s\n", 
        profit_array_index,array[i].profit,array[i].sell->name,
        array[i].buy->name);*/
        if(array[i].profit>0){
            total_profit = array[i].profit +total_profit;
        }
        
        }
        //Since this is a double loop,the profit is doubled,so we gotta 
        //half it.
        total_profit = total_profit/2;
    if(profit_array_index!=0){
        best_move = max_profit_buy(array,profit_array_index,b);
        best_move.total_profit = total_profit;
    //If there is nothing to buy.    
    }else{
        best_move.profit = 0;
        best_move.buy = NULL;
        best_move.sell = NULL;
        best_move.fruit = "Nothing";
        best_move.quantity_buy = -1;
        best_move.quantity_sell = -1;
        best_move.total_profit = 0;
   } 
            
   return best_move;
}

//This is trigerred when a bot has fruit and a struct profit array is 
//created.
struct profit sell(struct bot *b){

    struct profit array[MAXIMUM];
    int numberoflocations=1;
    numberoflocations=number_of_locations(b);
    int profit_array_index=0;
    struct location *curr=b->location; 
    struct profit best_move;
    int total_profit =0;
    //This is loop which scans for selling locations for the specific 
    //fruit the bot has and puts them in struct profit array 
    //calculating their profits and everything in the struct.
    for(int i=0;i<numberoflocations;i++){
        if(strcmp(b->fruit,curr->fruit)==0){
            if(curr->price>0){
                if(curr->quantity!=0){
                    if(b->fruit_kg < curr->quantity){
                        array[profit_array_index].profit =
                         b->fruit_kg*curr->price;
                        array[profit_array_index].quantity_sell =
                         b->fruit_kg;    
                        array[profit_array_index].quantity_buy = -1;
                        
                        
                    }else if(b->fruit_kg > curr->quantity){
                        array[profit_array_index].profit = 
                        curr->quantity*curr->price;
                        array[profit_array_index].quantity_sell = 
                        curr->quantity;    
                        array[profit_array_index].quantity_buy = -1;
                    }else{
                        array[profit_array_index].profit = 
                        b->fruit_kg*curr->price;
                        array[profit_array_index].quantity_sell = 
                        b->fruit_kg;    
                        array[profit_array_index].quantity_buy = -1;
                    }
                    array[profit_array_index].buy=NULL;
                    array[profit_array_index].sell = curr;
                    array[profit_array_index].fruit = curr->fruit;
                    profit_array_index++;
                }
            }
        }
        curr = curr->east;
    }
    //This is to check whether the array was properly created.               
    /*DEBUG->for(int i=0;i<profit_array_index;i++){
        printf("SELL:Profit array index is %d and my array profit 
        contains %d in location %s,The number of locations is %d\n",
        profit_array_index,array[i].profit,array[i].sell->name,
        numberoflocations);
        }*/
        //This is to calculate the total profit in the world 
        struct profit best_move2 = buy(b); 
    if(profit_array_index!=0){
        best_move = max_profit_sell(array,profit_array_index,b);
        if(best_move.profit>0){
           best_move.total_profit = best_move.profit + 
           best_move2.total_profit;
        }else{
            best_move.total_profit = best_move2.total_profit; 
        }
    //If no selling locations,dump the fruit!           
    }else{
        best_move = dump_fruit(b);
        best_move.total_profit = best_move.profit + best_move2.total_profit;
    }
    return best_move; 
             
}

//returns the best struct profit profit to sell
struct profit max_profit_sell(struct profit array[],int profit_array_index,
                              struct bot *head){
    struct profit max;
    max.profit = 0;    
    for(int i =0; i<profit_array_index;i++){
        if(array[i].profit>max.profit){
            //printf("The profits in my array are %d\n",array[i].profit);
            max=array[i];
        //if profit equal,returns struct profit with least distace from bot.    
        }else if(array[i].profit == max.profit){
            int array1 = east_or_west(array[i].sell,head->location);
            int max1 = east_or_west(max.sell,head->location); 
            
            if(array1<0){
                array1=-(array1);
            }
            if(max1<0){
                max1=-(max1);
            }
            if(max1>array1){
                max=array[i];
            }            
        }            
    }
    return max;
}

//if no selling locations available, a struct profit with location 
//that has fruit, "Anything" is returned.
struct profit dump_fruit(struct bot *b){
    struct location *curr = b->location;
    while(strcmp(curr->fruit,"Anything")!=0){
        curr = curr->east;
    }
    
    struct profit result;
    result.profit = b->fruit_kg * curr->price;
    result.buy = NULL;
    result.sell = curr;
    result.fruit = b->fruit;
    result.quantity_buy = -1;
    result.quantity_sell = b->fruit_kg;
    result.total_profit = b->fruit_kg * curr->price;
    
    return result;
        

}

//This function just returns the number of locations.
int number_of_locations(struct bot *head){
    int location_counter=1;
    struct location *curr = head->location->east;
    while(strcmp(head->location->name,curr->name) !=0){
        location_counter++;
        curr = curr->east;
    }
    return location_counter;
}

//Returns maximum profit in an array when buying.
struct profit max_profit_buy(struct profit array[],int profit_array_index, 
                             struct bot *head){
    struct profit max=array[0];    
    for(int i =1; i<profit_array_index;i++){
        
        if(array[i].profit>max.profit){
            max=array[i];
        //if profits are the same,then scans out the location with the least
        //distnace and returns struct profit which has this.    
        }else if(array[i].profit == max.profit){
            int array1 = east_or_west(array[i].buy,head->location);
            int max1 = east_or_west(max.buy,head->location); 
            
            if(array1<0){
                array1=-(array1);
            }
            if(max1<0){
                max1=-(max1);
            }
            
            if(max1>array1){
                max=array[i];
            }            
        }            
    }
    return max;
}

//If given two locations,the destination location and the head location,
//This returns the least distance,east..or..west?.
int east_or_west(struct location *location1, struct location *head){
    
    int least_distance=0;
    int west = 1;
    int east = 1;
    struct location *location_west = location1->west;
    while(strcmp(location_west->name,head->name)!=0){
        location_west = location_west->west;
        west++;
    }
    
    struct location *location_east = location1->east;
    while(strcmp(location_east->name,head->name)!=0){
        location_east = location_east->east;
        east++;
    }
    
    if(east>west){
        least_distance = west;
    }else if(west>east){
        least_distance = -east;
    }else{
        least_distance = -east;
    }
    
    if(strcmp(head->name,location1->name)==0){
        least_distance = 0;
    }    
                
    
    return least_distance;        
   
}

int electricity_checker(int move,struct bot *b,int quantity_to_buy,
                        struct profit best_move){

    struct location *electricity = NULL;
    int near_elec=0;
    int ABS_MOVE;
    int ABS_ELEC;
    int result =0;
    //ABS move is the number of moves(positive)
    ABS_MOVE = abs(move);
    if(ABS_MOVE>b->maximum_move){
        ABS_MOVE = b->maximum_move;
    }       
    electricity = nearest_electricity(b,quantity_to_buy,move,best_move);
    if(electricity!=NULL){
        near_elec = east_or_west(electricity,b->location);
        //This condition assigns ABS ELEC as the most number of moves that
        //will take to the nearest electricity.
        if(near_elec<0){
            ABS_ELEC = -(near_elec-ABS_MOVE);   
        }else{
            ABS_ELEC = near_elec+ABS_MOVE;
        }    
        //if the batter is lesser than most number of moves,must charge 
        //so returns 1,else 0.                              
        if((b->battery_level-ABS_MOVE)<=ABS_ELEC){
            result=1;
        }else{
            result=0;
        }
    }else{
        result=0;
    }
            
    
    return result;   
   
            
}

struct location *nearest_electricity(struct bot *b,int quantity_to_buy,
                                     int move,struct profit best_move) {
    
    struct location *start = b->location;
    struct location *eastwards = b->location;
    struct location *westwards = b->location;
    struct location *array[MAXIMUM];
    int index=0;
    struct location *result=NULL;
    int distance = 0;
    while (distance == 0 || eastwards != start) {
        if (has_electricity_for_sale(eastwards,quantity_to_buy)) {
            //This lets us know what it is comparing.
            /*DEBUG->printf("%d AND CMP %d", -(eastwards->price)*
            quantity_to_buy,best_move.total_profit);*/
            //If conditions has been met mut there is little profit in 
            //continuing, it returns a null pointer 
            if( -(eastwards->price)*quantity_to_buy > 
                best_move.total_profit){
                return NULL;
            }else{
                return eastwards;
            }
        //If conditions are not ment,it stores information in a location
        //array which can be compared after.    
        }else{                
            if(strcmp(eastwards->fruit, "Electricity") == 0 &&
               eastwards->price < 0 &&
               eastwards->quantity > 0){
                array[index] = eastwards;
                index++;
            }         
        }        
        
        //same for eastwards,whichever gets triggered first is returned.    
        if (has_electricity_for_sale(westwards,quantity_to_buy)) {
        /*DEBUG->printf("%d AND CMP %d", 
        -(westwards->price)*quantity_to_buy,best_move.total_profit);*/
            if( -(westwards->price)*quantity_to_buy > 
                best_move.total_profit){
                return NULL;
            }else{
                return westwards;
            }
        }else{
            if(strcmp(westwards->fruit, "Electricity") == 0 &&
               westwards->price < 0 &&
               westwards->quantity > 0){
                array[index] = westwards;
                index++;
            }        
        }
        eastwards = eastwards->east;
        westwards = westwards->west;
        distance = distance + 1;
    }
    if(move<0){
        move = -move;
    }
    //If conditions are not met,all elec dealers are is stored in an array.    
    /*DEBUG->for(int i=0;i<index;i++){
        printf("MY ARRAY CONTAINS LOCATION NAME %s WITH QUANTITY %d,
        index %d\n",array[i]->name,array[i]->quantity,index);
        }*/
    if(index!=0){
        //The best location wowld be the most quantity.
        struct location *best_location =  quantity_max(array,index,b);
       /* DEBUG->printf("%d AND CMP %d", -(best_location->price*
       best_location->quantity),best_move.total_profit);*/
       //THis also shouldn't be true,its like wasting money.
        if((best_location->quantity<=move) || 
           (-(best_location->price*best_location->quantity))>
              best_move.total_profit){
            result= NULL;
        }else{    
            result= best_location; 
        }
    //If no eletricity dealers--Should never end up here.        
    }else{
      //DEBUG->printf("CHECK UR PROGRAM");
      result=NULL;
    } 
         
    return result;

    assert(0);
}

//A condition which checks if there is electricity to buy.
int has_electricity_for_sale(struct location *l,int quantity_to_buy){
    return strcmp(l->fruit, "Electricity") == 0 &&
           l->price < 0 &&
           l->quantity >= quantity_to_buy;
}

//checks the max quantity in a location
struct location *quantity_max(struct location *array[],int index,
                              struct bot *head){

    struct location *max = array[0];    
        for(int i =1;i<index;i++){
            if(array[i]->quantity>max->quantity){
                max=array[i];
            //If the max quantity is equal,it returns the location nearest
            //to the bot.    
            }else if(array[i]->quantity == max->quantity){
                int array1 = east_or_west(array[i],head->location);
                int max1 = east_or_west(max,head->location); 
                
                if(array1<0){
                    array1=-(array1);
                }
                if(max1<0){
                    max1=-(max1);
                }
                
                if(max1>array1){
                    max=array[i];
                }            
            }            
        }
    return max;
}










