#include <stdio.h>
#include <assert.h>

// You must use these #defines - DO NOT CHANGE THEM

#define TOWN_ADELAIDE       0
#define TOWN_BRISBANE       1
#define TOWN_BROKEN_HILL    2
#define TOWN_CANBERRA       3
#define TOWN_DARWIN         4
#define TOWN_EUCLA          5
#define TOWN_HOBART         6
#define TOWN_LORD_HOWE_IS   7
#define TOWN_MELBOURNE      8
#define TOWN_PERTH          9
#define TOWN_SYDNEY         10

// New Zealand

#define TOWN_AUCKLAND       11
#define TOWN_CHRISTCHURCH   12
#define TOWN_WELLINGTON     13

// Australia

#define TIMEZONE_AWST_OFFSET  800 // Australian Western Standard Time

#define TIMEZONE_ACWST_OFFSET 845 // Australian Central Western Standard Time

#define TIMEZONE_ACST_OFFSET  930 // Australian Central Standard Time
#define TIMEZONE_ACDT_OFFSET 1030 // Australian Central Daylight Time

#define TIMEZONE_AEST_OFFSET 1000 // Australian Eastern Standard Time
#define TIMEZONE_AEDT_OFFSET 1100 // Australian Eastern Daylight Time

#define TIMEZONE_LHST_OFFSET 1030 // Lord Howe Standard Time
#define TIMEZONE_LHDT_OFFSET 1100 // Lord Howe Daylight Time

// New Zealand
#define TIMEZONE_NZST_OFFSET 1200 // NZ Standard Time
#define TIMEZONE_NZDT_OFFSET 1300 // NZ Daylight Time

// returned by get_local_time
#define INVALID_INPUT (-1)


// ADD YOUR #defines (if any) here

#define JAN 1
#define FEB 2
#define MAR 3
#define APR 4
#define MAY 5
#define JUN 6
#define JUL 7
#define AUG 8
#define SEP 9
#define OCT 10
#define NOV 11
#define DEC 12


int get_local_time(int town, int utc_month, int utc_day, int utc_time);
void run_unit_tests(void);


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
int max_day_of_month(int month);
int daycheck(int utc_day, int utc_month);
int time_addition(int utc_time,int offset);
int timecheck(int utc_time);
int towncheck(int town,int month,int day,int time);
int monthcheck(int month);



// DO NOT CHANGE THIS FUNCTION

// This main function won't be marked as part of the assignment
// It tests the functions you have to write.
// Do not change this main function.  If it does not work with
// your functions you have misunderstood the assignment.

int main(void) {
    int call_get_local_time = 0;

    printf("Enter 0 to call run_unit_tests()\n");
    printf("Enter 1 to call get_local_time()\n");
    printf("Call which function: ");
    scanf("%d", &call_get_local_time);

    if (!call_get_local_time) {
        printf("calling run_unit_tests()\n");
        run_unit_tests();
        printf("unit tests done\n");
        return 0;
    }

    int town = 0;
    int month = 0;
    int day = 0;
    int time = 0;

    printf("Enter %d for Adelaide\n", TOWN_ADELAIDE);
    printf("Enter %d for Brisbane\n", TOWN_BRISBANE);
    printf("Enter %d for Broken_hill\n", TOWN_BROKEN_HILL);
    printf("Enter %d for Canberra\n", TOWN_CANBERRA);
    printf("Enter %d for Darwin\n", TOWN_DARWIN);
    printf("Enter %d for Eucla\n", TOWN_EUCLA);
    printf("Enter %d for Hobart\n", TOWN_HOBART);
    printf("Enter %d for Lord Howe Island\n", TOWN_LORD_HOWE_IS);
    printf("Enter %d for Melbourne\n", TOWN_MELBOURNE);
    printf("Enter %d for Perth\n", TOWN_PERTH);
    printf("Enter %d for Sydney\n", TOWN_SYDNEY);
    printf("Enter %d for Auckland\n", TOWN_AUCKLAND);
    printf("Enter %d for Christchurch\n", TOWN_CHRISTCHURCH);
    printf("Enter %d for Wellington\n", TOWN_WELLINGTON);

    printf("Which town: ");
    scanf("%d", &town);

    printf("Enter UTC month as integer 1..12: ");
    scanf("%d", &month);

    printf("Enter UTC day as integer 1..31: ");
    scanf("%d", &day);

    printf("Enter UTC time as hour * 100 + minutes\n");
    printf("Enter UTC time as integer [0..2359]: ");
    scanf("%d", &time);

    int local_time = get_local_time(town, month, day, time);

    if (local_time == INVALID_INPUT) {
        printf("invalid input - local time could not be calculated\n");
    } else {
        printf("local_time is %d\n", local_time);
    }
    return 0;
}



// Given an Australian town and a 2018 UTC time
// return the local time in the Australian town
//
// time is returned as hours*100 + minutes
// INVALID_INPUT is return for invalid inputs
//
// utc_month should be 1..12
// utc_day should be 1..31
// utc_time should be 0..2359
// utc_time is hours*100 + minutes
//
// note UTC year is assumed to be 2018
// note UTC seconds is assumed to be zero
//

int get_local_time(int town, int utc_month, int utc_day, int utc_time) {
    // CHANGE THIS FUNCTION
    // YOU ARE NOT PERMITTED TO USE ARRAYS, LOOPS, PRINTF OR SCANF
    // SEE THE ASSIGNMENT SPECIFICATION FOR MORE RESTRICTIONS
    
    int to_return;
    if ( max_day_of_month(utc_month) == INVALID_INPUT || 
         daycheck(utc_day,utc_month) == INVALID_INPUT || 
         timecheck(utc_time) == INVALID_INPUT ||
         towncheck(town,utc_month,utc_day,utc_time)== INVALID_INPUT ||
         monthcheck(utc_month) == INVALID_INPUT) {
            to_return = INVALID_INPUT;
    } else {
    
    
        to_return = towncheck(town,utc_month,utc_day,utc_time); }
        
    return to_return;
}


// ADD YOUR FUNCTIONS HERE

int max_day_of_month(int month) {

    int to_return;
    if (month < JAN || month > DEC) {
        return INVALID_INPUT;
    } else if (month == JAN || month == MAR || month == MAY || month == JUL || month == AUG || month == OCT || month == DEC) {
        to_return = 31;//Max day is 31 in these months
    } else if (month == FEB) {
        to_return = 28;//Max day is 28 in these months
    } else {
        to_return = 30;//Max day is 30 in these months
    }
    return to_return;
    
}

int time_addition(int utc_time, int offset) {

    
    int hours = (utc_time/100);//100 is in the given formula!
    int minutes = (utc_time-((utc_time/100)*100));
    int hours_offset= (offset/100);
    int minutes_offset=(offset-((offset/100)*100));
    int revised_hours;
    int revised_minutes;
    
    if (minutes+minutes_offset>59) {
        revised_minutes= (minutes+minutes_offset) - 60;//minutes is turned back to zero by subtracting 60 when an hour is addded
        hours=1+hours;//hours added
        
    } else {
        revised_minutes= (minutes+minutes_offset);
    }
    
    if (hours+hours_offset>23 ) {
       revised_hours= (hours+hours_offset) - 24;//at midnight hours turn to 0 so 24 is subtracted.
    
    } else {
       revised_hours= (hours+hours_offset);
    }
    
    
    return revised_hours*100+revised_minutes;//Exactly the formula
        
    
}

int daycheck(int utc_day, int utc_month) {

    int to_return;
    if (utc_day > 0 && utc_day <= max_day_of_month(utc_month)) {//day is greater than 0.
        to_return = utc_day;
    } else {
        to_return = INVALID_INPUT;
    }
    return to_return;
    
}


int timecheck(int utc_time) {
    
    int to_return;

    if (((utc_time/100)>-1) && ((utc_time/100)<24) && (utc_time-((utc_time/100)*100)<60) && (utc_time-((utc_time/100)*100)>-1)) { //checking for all invalid inputs hours>-1 and <24 minutes<60 and >-1
        to_return = utc_time;
    } else {
        to_return = INVALID_INPUT;
    }
    return to_return;

} 

int monthcheck(int month) {

    int to_return;
    if (month<=DEC && month>=JAN) {
        to_return = month;
    } else {
        to_return = INVALID_INPUT;
    }
    return to_return;

}
        
    
int towncheck(int town,int month,int day,int time) { 

    int to_return; 
    
    if (town == TOWN_AUCKLAND || town == TOWN_CHRISTCHURCH || town == TOWN_WELLINGTON){
       
        if (month > APR && month < SEP ){
        
            to_return = time_addition(time,TIMEZONE_NZST_OFFSET);
            
        } else if ((month == APR && day > 1) || (month == SEP && day < 30)) {
            
            to_return = time_addition(time,TIMEZONE_NZST_OFFSET);
            
        } else if ((month == APR && day == 1 && time >= 300) || (month == SEP && day == 30 && time <= 200)) {                           //Every day is written as a digit in functions
                                                                                                                                        //Every time is written as hour*100+minutes
            to_return = time_addition(time,TIMEZONE_NZST_OFFSET);
            
        } else { 
        
            to_return = time_addition(time,TIMEZONE_NZDT_OFFSET); }
     
    } else if (town == TOWN_EUCLA) {
        
        to_return = time_addition(time,TIMEZONE_ACWST_OFFSET); 
        
    } else if (town == TOWN_LORD_HOWE_IS) {
    
        if (month > APR && month < OCT ){
        
            to_return = time_addition(time,TIMEZONE_LHST_OFFSET);
            
        } else if ((month == APR && day > 1) || (month == OCT && day < 7)) {
            
            to_return = time_addition(time,TIMEZONE_LHST_OFFSET);
            
        } else if ((month == APR && day == 1 && time >= 200) || (month == OCT && day == 7 && time <= 200)) { 
        
            to_return = time_addition(time,TIMEZONE_LHST_OFFSET);
            
        } else { 
        
            to_return = time_addition(time,TIMEZONE_LHDT_OFFSET); }
     
    } else if ( town == TOWN_PERTH ) {
        
        to_return = time_addition(time,TIMEZONE_AWST_OFFSET);
        
    } else if ( town == TOWN_DARWIN ) {
    
        to_return = time_addition(time,TIMEZONE_ACST_OFFSET);
     
    } else if ( town == TOWN_BRISBANE ) {
    
        to_return = time_addition(time,TIMEZONE_AEST_OFFSET);
    
    } else if ( town == TOWN_ADELAIDE || town == TOWN_BROKEN_HILL ) {
    
        if (month > APR && month < OCT ){
        
            to_return = time_addition(time,TIMEZONE_ACST_OFFSET);
            
        } else if ((month == APR && day > 1) || (month == OCT && day < 7)) {
            
            to_return = time_addition(time,TIMEZONE_ACST_OFFSET);
            
        } else if ((month == APR && day == 1 && time >= 300) || (month == OCT && day == 7 && time <= 200)) {
        
            to_return = time_addition(time,TIMEZONE_ACST_OFFSET);
            
        } else { 
        
            to_return = time_addition(time,TIMEZONE_ACDT_OFFSET); } 
            
    } else if ( town == TOWN_CANBERRA || town == TOWN_HOBART || town == TOWN_SYDNEY || town == TOWN_MELBOURNE ) {
        
        if (month > APR && month < OCT ){
        
            to_return = time_addition(time,TIMEZONE_AEST_OFFSET);
            
        } else if ((month == APR && day > 1) || (month == OCT && day < 7)) {
            
            to_return = time_addition(time,TIMEZONE_AEST_OFFSET);
            
        } else if ((month == APR && day == 1 && time >= 300) || (month == OCT && day == 7 && time <= 200)) {
        
            to_return = time_addition(time,TIMEZONE_AEST_OFFSET);
            
        } else { 
        
            to_return = time_addition(time,TIMEZONE_AEDT_OFFSET); } 
    
    } else {
    
        to_return = INVALID_INPUT;  
    }
    return to_return;

}       
        
    
    
     
         



// ADD A COMMENT HERE EXPLAINING YOUR OVERALL TESTING STRATEGY
/*I check if the months inputted is valid or invalid
The same goes for days,time and town.
Next I check whether the actual output is valid and correct for each town.
Lastly checking if the time and minutes are being added right*/

// run unit tests for get_local_time

void run_unit_tests(void) {
    // 2 example unit tests

    // UTC midnight on 20th March -> time in Sydney: 11am AEDT
    assert(get_local_time(TOWN_SYDNEY, 3, 20, 0) == 1100);

    // 42 is not a valid month
    assert(get_local_time(TOWN_SYDNEY, 42, 0, 0) == INVALID_INPUT);

    // ADD YOUR ASSERT STATEMENTS HERE
    
            //TESTING FOR INVALID INPUTS AND A VALID INPUT
    
    //13 is not a valid month
    assert(get_local_time(TOWN_SYDNEY, 13, 12, 0) == INVALID_INPUT);
    
    //0 is not a valid month
    assert(get_local_time(TOWN_SYDNEY, 0, 12, 0) == INVALID_INPUT);
    
    //32 is not a valid day
    assert(get_local_time(TOWN_SYDNEY, 12, 32, 0) == INVALID_INPUT);
    
    //31 is not a valid day for november
    assert(get_local_time(TOWN_SYDNEY, 11, 31, 0) == INVALID_INPUT);
    
    //30 is not a valid day in february
    assert(get_local_time(TOWN_SYDNEY, 2, 30, 0) == INVALID_INPUT);
    
    //60 is not a valid minute
    assert(get_local_time(TOWN_SYDNEY, 2, 22, 1160) == INVALID_INPUT);
    
    //-241 is not a valid time
    assert(get_local_time(TOWN_SYDNEY, 3, 30, -241) == INVALID_INPUT);
     
     //-1 is not a valid month
    assert(get_local_time(TOWN_SYDNEY, -1, 30, 2312) == INVALID_INPUT);
    
    //-1 is not a valid day
    assert(get_local_time(TOWN_SYDNEY, 3, -1, 2312) == INVALID_INPUT);
    
    //24 is not a valid hour
    assert(get_local_time(TOWN_SYDNEY, 3, 30, 2412) == INVALID_INPUT);
    
    //Testing for valid input
    assert(get_local_time(TOWN_SYDNEY, 3, 1, 0) == 1100);
    
            //TESTING EACH TIME ZONE WITH THEIR TOWNS         
    
    //UTC midnight on 1st january -> time in Eucla: 8:45am ACWST
    assert(get_local_time(TOWN_EUCLA, 1, 1, 0) == 845);
    
    //UTC midnight on 1st may -> time in Eucla: 8:45am ACWST
    assert(get_local_time(TOWN_EUCLA, 5, 1, 0) == 845);
    
    //UTC midnight on 1st january -> time in Sydney: 11:00am AEDT
    assert(get_local_time(TOWN_SYDNEY, 1, 1, 0) == 1100);
    
    //UTC midnight on 1st may -> time in Sydney: 10:00am AEST
    assert(get_local_time(TOWN_SYDNEY, 5, 1, 0) == 1000);
    
    //UTC midnight on 1st january -> time in Canberra: 11:00am AEDT
    assert(get_local_time(TOWN_CANBERRA, 1, 1, 0) == 1100);
    
    //UTC midnight on 1st may -> time in Canberra: 10:00am AEST
    assert(get_local_time(TOWN_CANBERRA, 5, 1, 0) == 1000);
    
    //UTC midnight on 1st january -> time in Hobart: 11:00am AEDT
    assert(get_local_time(TOWN_HOBART, 1, 1, 0) == 1100);
    
    //UTC midnight on 1st may -> time in Hobart: 10:00am AEST
    assert(get_local_time(TOWN_HOBART, 5, 1, 0) == 1000);
    
    //UTC midnight on 1st january -> time in Melbourne: 11:00am AEDT
    assert(get_local_time(TOWN_MELBOURNE, 1, 1, 0) == 1100);
    
    //UTC midnight on 1st may -> time in Melbourne: 10:00am AEST
    assert(get_local_time(TOWN_MELBOURNE, 5, 1, 0) == 1000);
    
    //UTC midnight on 1st january -> time in Auckland: 1:00pm NZDT
    assert(get_local_time(TOWN_AUCKLAND, 1, 1, 0) == 1300);
    
    //UTC midnight on 1st may -> time in Auckland: 12:00pm NZST
    assert(get_local_time(TOWN_AUCKLAND, 5, 1, 0) == 1200);
    
    //UTC midnight on 1st january -> time in Christchurch: 1:00pm NZDT
    assert(get_local_time(TOWN_CHRISTCHURCH, 1, 1, 0) == 1300);
    
    //UTC midnight on 1st may -> time in Christchurch: 12:00pm NZST
    assert(get_local_time(TOWN_CHRISTCHURCH, 5, 1, 0) == 1200);
    
    //UTC midnight on 1st january -> time in Wellington: 1:00pm NZDT
    assert(get_local_time(TOWN_WELLINGTON, 1, 1, 0) == 1300);
    
    //UTC midnight on 1st may -> time in Wellington: 12:00pm NZST
    assert(get_local_time(TOWN_WELLINGTON, 5, 1, 0) == 1200);
    
    //UTC midnight on 1st january -> time in Lord Howe Island: 11:00am LHDT
    assert(get_local_time(TOWN_LORD_HOWE_IS, 1, 1, 0) == 1100);
    
    //UTC midnight on 1st may -> time in Lord Howe Island: 10:30am LHST
    assert(get_local_time(TOWN_LORD_HOWE_IS, 5, 1, 0) == 1030);
    
    //UTC midnight on 1st january -> time in Perth: 8:00am AWST
    assert(get_local_time(TOWN_PERTH, 1, 1, 0) == 800);
    
    //UTC midnight on 1st may -> time in Perth: 8:00am AWST
    assert(get_local_time(TOWN_PERTH, 5, 1, 0) == 800);
    
    //UTC midnight on 1st january -> time in Darwin: 9:30am ACST
    assert(get_local_time(TOWN_DARWIN, 1, 1, 0) == 930);
    
    //UTC midnight on 1st may -> time in Darwin: 9:30am ACST
    assert(get_local_time(TOWN_DARWIN, 5, 1, 0) == 930);
    
    //UTC midnight on 1st january -> time in Brisbane: 10:00am AEST
    assert(get_local_time(TOWN_BRISBANE, 1, 1, 0) == 1000);
    
    //UTC midnight on 1st may -> time in Brisbane: 10:00am AEST
    assert(get_local_time(TOWN_BRISBANE, 5, 1, 0) == 1000);
    
    //UTC midnight on 1st january -> time in Adelaide: 10:30am ACDT
    assert(get_local_time(TOWN_ADELAIDE, 1, 1, 0) == 1030);
    
    //UTC midnight on 1st may -> time in Adelaide: 9:30am ACST
    assert(get_local_time(TOWN_ADELAIDE, 5, 1, 0) == 930);
    
    //UTC midnight on 1st january -> time in Broken hill: 10:30am ACDT
    assert(get_local_time(TOWN_BROKEN_HILL, 1, 1, 0) == 1030);
    
    //UTC midnight on 1st may -> time in Broken hill: 9:30am ACST
    assert(get_local_time(TOWN_BROKEN_HILL, 5, 1, 0) == 930);
    
            //CHECKING IF TIME ADDITION IS RIGHT
    
    //UTC 2355 on 1st january -> time in Eucla: 8:40am ACWST
    assert(get_local_time(TOWN_EUCLA, 1, 1, 2355) == 840);
    
            //ALL UNIT TESTS ENDED
    
    
    
    
    
    
    

    // you should add at least 40 more assert statements to this function
    // with a comment for each test explaining it

    // there should be comment before this function
    // explaining your overall testing strategy
}


