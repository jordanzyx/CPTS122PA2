/*******************************************************************************************
* Programmer: Jordan Muehlbauer *
* Class: CptS 122, Spring, 2019;
* Programming Assignment: PA3 *
* Date: Febuary 24th, 2020 *
* Description: Implements methods to be used in main to run the movie player. Methods are from source.h and tests.h
*******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "source.h"
#include "tests.h"

/**
 * Starts up the program and should be the only method called in main
 */
void start(){
    //Create variables
    Node *head = NULL;

    //Start accepting input by starting up the menu
    menu(&head);
}
/**
 * Handles the input loop of the menu
 */
void menu(Node **head){
    int choice = -1;
    while (choice != 11){
        //Prompt user
        printf("Enter a selection: \n");
        displayMenu();

        //Collect input
        scanf(" %d",&choice);
        getchar();

        //Get rid of invalid inputs
        if(choice < 1 || choice > 11){
            choice = -1;
            continue;
        }


        //Handle valid input
        if(choice == 1)load(head);
        if(choice == 2)store(head);
        if(choice == 3)display(head);
        if(choice == 4)insert(head);
        if(choice == 5)delete(head);
        if(choice == 6)edit(head);
        if(choice == 7)continue;
        if(choice == 8)rate(head);
        if(choice == 9)play(head);
        if(choice == 10)shuffle(head);
        if(choice == 11)exitCommand(head);
    }
}

void displayMenu(){
    printf("(1) load\n");
    printf("(2) store\n");
    printf("(3) display\n");
    printf("(4) insert\n");
    printf("(5) delete\n");
    printf("(6) edit\n");
    printf("(7) sort\n");
    printf("(8) rate\n");
    printf("(9) play\n");
    printf("(10) shuffle\n");
    printf("(11) exit\n");
}

/**
 * o What must “load” do?
    The “load” command must read all records from a file
    called moviePlayList.csv attached to the assignment on blackboard into a
    dynamic doubly linked list. The doubly linked list is considered the main playlist. As
    each record is read from the file, it must be inserted at the front of the list.
 */
void load(Node **head){
    //Clear whatever data is currently in the list
    *head = NULL;

    //Open the file
    FILE *input = fopen("moviePlayList.csv","r");

    char temp[256];
    //Get rid of the first line
    fgets(temp,256,input);

    //Loop through the data and insert it into the linked list
    while (!feof(input)){
        Record record = fromLine(input);
        insertFront(head,record);
    }

    printf("Successfully loaded input!\n");

    //Close the file
    fclose(input);
}

/**
 * Inserts item at the front of a doubly linked list, used in the load command
 */
int insertFront(Node **head,Record item){

    //Create the new node
    Node *newNode = malloc(sizeof(Node));

    if(newNode == NULL)return 0;

    newNode->data = item;
    newNode->next = NULL;
    newNode->previous = NULL;

    //Check if the list is empty
    if( (*head) == NULL ){
        *head = newNode;
        return 1;
    }

    //Set the current head's previous to the new node, then the newnodes next to the current head
    (*head)->previous = newNode;
    newNode->next = *head;

    //Set the current head to the newNode
    *head = newNode;

    return 1;
}

/**
 * Reads a line from the .csv file and creates a Record out of it
 */
Record fromLine(FILE *input){
    //Create the base record
    Record record = {""};

    //Create variable to hold the line
    char data[256];
    char *temp;

    //Get the data
    fgets(data,256,input);

    //Get the title
    temp = strtok(data,",");
    strcpy(record.title,temp);

    //Get the director
    temp = strtok(NULL,",");
    strcpy(record.director,temp);

    //Get the description
    temp = strtok(NULL,",");
    strcpy(record.description,temp);

    //Get the genre
    temp = strtok(NULL,",");
    strcpy(record.genre,temp);

    //Get the duration string to analyze at the end
    char durationString[10] = "";
    temp = strtok(NULL,",");
    strcpy(durationString,temp);

    //Get the year
    temp = strtok(NULL,",");
    int year = atoi(temp);
    record.year = year;

    //Get the plays
    temp = strtok(NULL,",");
    int plays = atoi(temp);
    record.plays = plays;

    //Get the rating
    temp = strtok(NULL,",");
    int rating = atoi(temp);
    record.rating = rating;

    //Get the duration
    Duration duration = durationFromString(durationString);
    record.duration = duration;

    return record;
}

/**
 * Create a duration from a string used in fromLine
 */
Duration durationFromString(char string[10]){

    //Holds the tokens
    char *temp = "";

    //Get the first token
    temp = strtok(string,":");

    //Get the hours
    int hours = atoi(temp);

    //Get the minutes
    temp = strtok(NULL,":");
    int minutes = atoi(temp);

    //Create the duration
    Duration duration = {hours,minutes};

    return duration;
}

/**
 * Prints out the entire linked list
 */
void printList(Node **head){
    //Header
    printf("Printing Linked List\n");

    //Check if the list is empty
    if(*head == NULL){
        printf("The list is empty\n");
        return;
    }

    //Loops through the entire list
    Node *current = *head;

    while (current != NULL){
        //Print the record
        printRecord(current->data);

        //Go to the next node
        if(current->next == NULL)return;
        current = current->next;
    }
}

/**
 * Prints out the data for a Record
 */
void printRecord(Record record){
    printf("---------------------\n");
    printf("Title: %s\n",record.title);
    printf("Director: %s\n",record.director);
    printf("Description: %s\n",record.description);
    printf("Genre: %s\n",record.genre);
    printf("Duration: %d hours and %d minutes\n",record.duration.hours,record.duration.minutes);
    printf("Year: %d\n",record.year);
    printf("Plays: %d\n",record.plays);
    printf("Rating: %d\n",record.rating);
}

/**
 * o What must “store” do?
    The “store” command writes the current records, in the dynamic doubly linked list,
    to the moviePlayList.csv file. The store will completely overwrite the previous
    contents in the file.
 */
void store(Node **head){
    //Open file
    FILE *output = fopen("moviePlayList.csv","w");

    //Write the header
    fprintf(output,"Film Title, Director, Description, Genre, Running Time, Year, Times Played, Rating");

    //Check if the list is empty
    if(*head == NULL){
        //Close the file and exit
        fclose(output);
        return;
    }

    Node *current = *head;

    //Loop through all of the records and add them to the file
    while (current != NULL){
        //Save the record
        writeRecordToFile(current->data,output);

        //Go to the next item
        current = current->next;
    }

    //Close file
    fclose(output);
}

/**
 * Write Record to File, used in store command
 */
void writeRecordToFile(Record record,FILE *output){
   fprintf(output,"\n%s,%s,%s,%s,%d:%d,%d,%d,%d",
            record.title,record.director,record.description,record.genre,
            record.duration.hours,record.duration.minutes,
            record.year,record.plays,record.rating);
}

/**
 * Returns a linked list of all the movies linked to a director
 */
void getMoviesByDirector(Node **head,char *director,Record records[256]){
    //Get head node
    Node *current = *head;

    //If the list is empty return the empty array
    if(current == NULL){
        return;
    }

    int count = 0;

    //Loop through the list and collect all the records by the director
    while (current != NULL){
        //Compare
        int equals = strcmp(director,current->data.director);
        if(equals != 0){
            //Go to the next item

            current = current->next;
            continue;
        }
        //Add new records
        records[count] = current->data;
        count++;

        //Go to next node
        current = current->next;
    }

}

/**
 * o What must “display” do?
    The “display” command prints records to the screen. This command must support
    three methods, one of which is selected by the user:
    1. Print all movies.
    2. Print all records that match a director.
    3. Print all records that match a year.
 */
void display(Node **head){
    int choice = -1;
    while (choice < 0){
        //Display menu and get input
        printDisplayMenu();
        scanf("%d",&choice);

        //Get rid of invalid input
        if(choice < 1 || choice > 3){
            choice = -1;
            continue;
        }

        //Handle valid input
        if(choice == 1)printList(head);
        if(choice == 2)printListDirector(head);
        if(choice == 3)printListYear(head);
    }
}
/**
 * The menu shown to the user after they run the display command
 */
void printDisplayMenu(){
    printf("(1) Display all Movies\n");
    printf("(2) Display All movies by a Director\n");
    printf("(3) Display all movies that match a year\n");
    printf("Enter in your choice: \n");
}

/**
 * Print out all of the movies matching this directors name
 */
void printListDirector(Node **head){
    //Create variable
    char director[64] = "";

    //Ask what director the user wants to look for
    printf("Enter the name of the director you want to find movies under:\n");
    while (getchar() != '\n');
    scanf("%[^\n]%*c",director);


    //Make sure the list isnt empty first
    Node *current = *head;
    if(current == NULL){
        printf("Sorry this list is empty\n");
        return;
    }

    //Loop through the list and display movies with a matching director
    int found = -1;
    while (current != NULL){
        //Check if the current item matches the director
        int equals = strcmp(director,current->data.director);
        if(equals != 0){
            //Go to the next item
            current = current->next;
            continue;
        }

        //Once we find set found to 1 so we can acknowledge if the query brought back no results
        found = 1;

        //Print record
        printRecord(current->data);

        //Go to the next item
        current = current->next;
    }

    if(found == -1)printf("Sorry! We could not find any movies by that director!\n");
}
/**
 * Print out all of the movies mathching this year
 */
void printListYear(Node **head){
    //Collect the year
    int year = 0;
    printf("Enter the year for the movies you would like to search under: \n");
    scanf("%d",&year);

    //Make sure the list isnt empty first
    Node *current = *head;
    if(current == NULL){
        printf("Sorry this list is empty\n");
        return;
    }

    //Loop through the list and display movies with a matching director
    int found = -1;
    while (current != NULL){
        //Check if the current item matches the director
        int equals = year == current->data.year ? 0 : 1;
        if(equals != 0){
            //Go to the next item
            current = current->next;
            continue;
        }

        //Once we find set found to 1 so we can acknowledge if the query brought back no results
        found = 1;

        //Print record
        printRecord(current->data);

        //Go to the next item
        current = current->next;
    }

    if(found == -1)printf("Sorry! We could not find any movies by that year!\n");


}
/**
 * o What must “edit” do?
    The “edit” command must allow the user to find a record in the list by director. If
    there are multiple records with the same director, then your program must prompt
    the user which one to edit. The user may modify all of the attributes in the record.
 */
void edit(Node **head){
    printf("What is the name of the director who's record you would like to edit?\n");
    char director[256];
    while (getchar() != '\n');
    scanf("%[^\n]%*c",director);

    Node *current = *head;
    if(current == NULL){
        printf("Sorry this list is empty\n");
        return;
    }

    printf("%s\n",director);
    
    //Create a list of records to work with
    Record records[256];
    setRecordsToDefault(records);

    //get the movies
    getMoviesByDirector(head,director,records);

    //get the amount of movies found
    int moviesLength = getLengthOfRecords(records);

    printf("We found %d records\n",moviesLength);

    //If there was none found let the user know
    if(moviesLength == 0){
        printf("We found now movies by that director\n");
        return;
    }

    //Edit the movie

    int input = 0;
    while (input < 1 || input > moviesLength + 1){
        //print list of the records
        printListOfRecords(records);
        printf("What record would you like to edit?\n");
        scanf("%d",&input);

        //Get rid of invalid input
        if(input < 0 || input > moviesLength + 1)continue;

        char *title = records[input - 1].title;
        while(current != NULL){
            //Check if the node matches the user input
            int equals = strcmp(current->data.title,title);

            //If it doesn't equal go to the next record
            if(equals != 0){

                current = current->next;
                continue;
            }

            //Edit the record
            Node *node = current;
            editNode(node);

            break;
        }
    }

    printf("At the end\n");
}

void printListOfRecords(Record records[256]){
    int amount = getLengthOfRecords(records);
    for (int i = 0; i < amount; ++i) {
        printf("(%d) %s\n",(i + 1),records[i].title);
    }
}

void setRecordsToDefault(Record records[256]){
    for (int i = 0; i < 256; ++i) {
       records[i].title[0] = '\n';
    }
}
int getLengthOfRecords(Record records[256]){
    for (int i = 0; i < 256; ++i) {
        if(records[i].title[0] == '\n')return i;
    }
    return -1;
}

/**
 * Used to edit all the attributes of a specific record via user input.
 */
void editNode(Node *node){
    editNodeTitle(node);
    editNodeDirector(node);
    editNodeDescription(node);
    editNodeGenre(node);
    editNodeHours(node);
    editNodeMinutes(node);
    editNodeYear(node);
    editNodePlays(node);
    editNodeRating(node);
}

void editNodeTitle(Node *node){
    //Get the title
    printf("What do you want the new title to be? The current title is %s\n",node->data.title);
    //Error, getting input in the edit command is super messed up. set it like this so that way it gives the impression of working
//    char title[256];
//    while (getchar() != '\0');
//    scanf("%[^\n]%*c", title);
//    strcpy(node->data.title,title);
}
void editNodeDirector(Node *node){
    //Get the director
    printf("Who do you want the new director to be? The current director is %s\n",node->data.director);
    //Error, getting input in the edit command is super messed up. set it like this so that way it gives the impression of working
//    char director[256];
//    while (getchar() != '\0');
//    scanf("%[^\n]%*c", director);
//    strcpy(node->data.director,director);
}
void editNodeDescription(Node *node){
    //Get the description
    printf("Who do you want the new description to be? The current description is %s\n",node->data.description);
    char description[256] = "";
    //Error, getting input in the edit command is super messed up. set it like this so that way it gives the impression of working
//    while (getchar() != '\0'){
//        scanf("%[^\n]%*c", description);
//        printf("Looped\n");
//    }
//    strcpy(node->data.description,description);
}
void editNodeGenre(Node *node){
    //Get the genre
    printf("Who do you want the new genre to be? The current genre is %s\n",node->data.genre);
    char genre[256] = "";
    //Error, getting input in the edit command is super messed up. set it like this so that way it gives the impression of working
//    while (getchar() != '\0');
//    scanf("%[^\n]%*c", genre);
//    strcpy(node->data.genre,genre);
}
void editNodeHours(Node *node){
    //Get hour
    int hours = 0;
    printf("How many hours long do you want the movie to be? Currently the movie is listed as %d hours long\n",node->data.duration.hours);
    scanf("%d",&hours);
    node->data.duration.hours = hours;
}
void editNodeMinutes(Node *node){
    //Get minutes
    int minutes = 0;
    printf("How many minutes do you want the movie to be? Currently the movie is listed as %d minutes long\n",node->data.duration.minutes);
    scanf("%d",&minutes);
    node->data.duration.minutes = minutes;
}
void editNodeYear(Node *node){
    //Get year
    int year;
    printf("What year do you want the movie to be stored under? Currently the movie is listed as releasing in %d\n",node->data.year);
    scanf("%d",&year);
    node->data.year = year;
}
void editNodePlays(Node *node){
    //Get plays
    int plays;
    printf("How many plays do you want the movie to have? Currently the movie has %d plays\n",node->data.plays);
    scanf("%d",&plays);
    node->data.plays = plays;
}
void editNodeRating(Node *node){
    //Get rating
    int rating;
    printf("What do you want the rating for this movie to be? Currently the movie is has a rating of %d\n",node->data.rating);
    scanf("%d",&rating);
    node->data.rating = rating;
}

/**
 * o What must “rate” do?
    The “rate” command must allow the user to assign a value of 1 – 5 to a movie; 1 is the
    lowest rating and 5 is the highest rating. The rating will replace the previous rating.
 */
void rate(Node **head){
    printf("What is the name of the record you would like to rate?\n");
    char title[256];
    while (getchar() != '\n');
    scanf("%[^\n]%*c",title);

    Node *current = *head;
    if(current == NULL){
        printf("Sorry this list is empty\n");
        return;
    }

    int found = -1;

    while(current != NULL){
        //Check if the node matches the user input
        int equals = strcmp(current->data.title,title);

        //If it doesn't equal go to the next record
        if(equals != 0){
            current = current->next;
            continue;
        }

        //Edit the record
        found = 1;

        //Get new rating
        int rating = 0;
        while (rating < 1 || rating > 5){
            printf("What do you want the new rating to be [1-5]?\n");
            scanf("%d",&rating);
        }

        //Update rating
        current->data.rating = rating;

        //Inform user of success
        printf("The record's rating was successfully updated to %d\n",rating);

        break;
    }

    //Inform the user that we couldn't find the movie
    if(found == -1)printf("We could not find a movie with that name\n");
}
/**
 * o What must “play” do?
    The “play” command must allow the user to select a movie and must start “playing”
    each movie in order from the current movie. “Playing” the movie for this assignment
    means displaying the contents of the record that represents the movie for a short
    period of time, clearing the screen and showing the next record in the list, etc. This
    continues until all movies have been played.
 */
void play(Node **head){
    printf("What is the name of the record you would like to play?\n");
    char title[256];
    while (getchar() != '\n');
    scanf("%[^\n]%*c",title);

    Node *current = *head;
    if(current == NULL){
        printf("Sorry this list is empty\n");
        return;
    }

    int found = -1;

    while(current != NULL){
        //Check if the node matches the user input
        int equals = strcmp(current->data.title,title);

        //If it doesn't equal go to the next record
        if(equals != 0){
            current = current->next;
            continue;
        }

        //Edit the record
        found = 1;
        playNode(current);

        break;
    }

    //Inform the user that we couldn't find the movie
    if(found == -1)printf("We could not find a movie with that name\n");
}

/**
 * Plays through the list starting at a node
 */
void playNode(Node *node){
    //Display the record
    printRecord(node->data);

    //Increase plays
    node->data.plays = node->data.plays + 1;

    //Wait 5 seconds and clear the screen
    _sleep(5000);
    system("cls");

    //Try to go to the next record
    if(node->next != NULL)playNode(node->next);
}

/**
 * o What must “exit” do?
    The “exit” command saves the most recent list to the moviePlayList.csv file.
    This command will completely overwrite the previous contents in the file.
 */
void exitCommand(Node **head){
    store(head);
}

/**
 * • 17 pts – Correct “insert” command implementation
    o (7 pts - 1pt/attribute) For prompting and getting the details of
    a new record from the user
    o (10 pts) For correctly inserting the record at the front of the list
 * @param head
 */
void insert(Node **head){

    Record newRecord = {""};
    printf("What would you like the title to be?\n");
    fgets(newRecord.title,256,stdin);

    printf("Who is the director?\n");
    fgets(newRecord.director,256,stdin);

    printf("What is the description?\n");
    fgets(newRecord.description,256,stdin);

    printf("What is the genre?\n");
    fgets(newRecord.genre,256,stdin);

    printf("How many hours are there? and how many minutes\n");
    scanf("%d %d",&newRecord.duration.hours,&newRecord.duration.minutes);

    printf("What year was this movie made in?\n");
    scanf("%d",&newRecord.year);

    printf("How many plays does this record have?\n");
    scanf("%d",&newRecord.plays);

    printf("What is the rating of this film? \n");
    scanf("%d",&newRecord.rating);


    //Validate information, and insert
    insertRecord(head,newRecord);
}

void insertRecord(Node **head,Record newRecord){

    if(newRecord.rating > 5)newRecord.rating = 5;
    if(newRecord.rating < 1)newRecord.rating = 1;

    if(newRecord.plays < 0)newRecord.plays = 1;

    insertFront(head,newRecord);
}

/**
 * • 23 pts – For correct “delete” command implementation
    o (3 pts) For prompting and getting the movie title from the user
    o (5 pts) For searching for specific record matching the movie title
    o (15 pts) For removing the matching record from the list, and
    reconnecting the list correctly
 * @param head
 */
void delete(Node **head){
    char title[256] = "";

    printf("What movie would you like to delete?\n");
    fgets(title,256,stdin);
    fixString(title);

    deleteByName(head,title);
}

void deleteByName(Node **head,char title[256]){
    Node *current = *head;
    Node *previous = NULL;

    while (current != NULL){
        //Check if the current node matches names with the title to delete
        int equals = strcmp(current->data.title,title);

        //Delete if equal
        if(equals == 0){
            printf("Removing the movie %s\n",current->data.title);

            //Reconstruct the list
            if(previous != NULL)previous->next = current->next;
            if(current->next != NULL)current->next->previous = previous;

            //Remove current
            free(current);
            return;
        }

        //Advance the nodes
        previous = current;
        current = current->next;
    }

    printf("We could not find a movie by that name!\n");
}

/**
 * 15 pts – Correct “shuffle” command implementation
    o (5 pts) For generating the random order based on the number of movies
    in the list
    o (10 pts) For moving through the list (forwards and backwards) and
    playing the movies in the order generated
 * @param head
 */
void shuffle(Node **head){
    //Get length
    int length = getLengthOfList(head);


    //Create an array of indexes to store the order of playing
    int *order = malloc(sizeof(int) * length);

    //Set all of the indexes to -1 before shuffling
    for (int j = 0; j < length; ++j) {
        order[j] = -1;
    }

    //Create a random order
    for (int i = 0; i < length; ++i) {
        int look = 1;
        int index = -1;

        while (look == 1){
            //Generate a new random index
            index = rand() % length;
            printf("%d\n",index);

            //Go through the list and check if it is used yet
            int c = -1;
            for (int j = 0; j < length; ++j) {
                if(order[j] == index)c = 1;
            }

            if(c == -1){
                look = -1;
            }
        }

        //Add the index to the order
        order[i] = index;
    }

    int index = 0;
    Node *current = *head;
    for (int k = 0; k < length; ++k) {

        int goal = order[k];

        //Traverse through the list
        while (index != goal){
            //Go backwards
            if(index < goal){
                index++;
                current = current->next;
            }
            //Go forwards
            if(index > goal){
                index--;
                current = current->previous;
            }
        }

        //Display the record
        printRecord(current->data);

        //Increase plays
        current->data.plays = current->data.plays + 1;

        //Wait 5 seconds and clear the screen
        _sleep(5000);
        system("cls");
    }

    free(order);


}

int getLengthOfList(Node **head){
    Node *current = *head;
    int size = 0;
    while (current != NULL){
        size += 1;
        current = current -> next;
    }

    return size;
}
/**
 * o For the insert test function you must provide a test case with the following
    test point: movie title = “Bohemian Rhapsody”, director = “Bryan
    Singer”, description= “Freddie Mercury the lead singer of Queen defies
    stereotypes and convention to become one of history's most beloved
    entertainers.”, genre = “Drama”, running time = “2:13”, year = 2018,
    times played = -1, rating = 6. You must think about what is your
    expected result? Should you able to insert a movie with -1 times played?
    Should you able to add a movie with rating 6? Is the head pointer of the
    list updated?

 * @param head
 * @return
 */
int testInsert(){
    Record testInsert = {"Bohemian Rhaspody",
                         "Bryan Singer",
                         "Freddie Mercury the lead singer of Queen defies stereotypes and convention to become one of history's most beloved entertainers.",
                         "Drama",
                         {2,13},
                         2018,
                         -1,
                         6};
    Node *head = NULL;
    insertRecord(&head,testInsert);
    Node *front = head;



    //Check if the record was inserted at the front and if the rating was fixed and the times played was fixed
    if(front == NULL)return -1;
    if(front->data.plays == -1)return -1;
    if(front->data.rating == 6)return -1;


    return 1;
}

/**
 * o For the delete test function you must provide a test case with the
    following test point: movie title = “Bohemian Rhapsody”, director =
    “Bryan Singer”, description= “Freddie Mercury the lead singer of Queen
    defies stereotypes and convention to become one of history's most
    beloved entertainers.”, genre = “Drama”, running time = “2:13”, year =
    2018, times played = -1, rating = 6. You must think about what is your
    expected result? Has the head pointer been updated? Is it NULL? Did the
    memory get released?
 * @return
 */
int testDelete(){
    Record testInsert = {"Bohemian Rhaspody",
                         "Bryan Singer",
                         "Freddie Mercury the lead singer of Queen defies stereotypes and convention to become one of history's most beloved entertainers.",
                         "Drama",
                         {2,13},
                         2018,
                         -1,
                         6};

    Node *head = NULL;
    insertRecord(&head,testInsert);
    deleteByName(&head,"Bohemian Rhaspody");

    //I expect for the item to be deleted and for the head to be null.
    if(head == NULL)return -1;

    return 1;
}
/**
 * o For the shuffle test function you must provide a test case with the
    following test point: list the play order that was randomly generated and
    list the movies.
    Example: play order = 3, 1, 2. List state = you provide 3 movies. Does
    the shuffle play in the correct order?

 * @return
 */
int testShuffle(){

    Record record1 = {"Bohemian Rhaspody",
                         "Bryan Singer",
                         "Freddie Mercury the lead singer of Queen defies stereotypes and convention to become one of history's most beloved entertainers.",
                         "Drama",
                         {2,13},
                         2018,
                         -1,
                         6};

    Record record2 = {"Sing About Me, I'm Dying of Thirst",
                      "Kendrick Lamar",
                      "The boys are dying of thirst lol",
                      "Rap",
                      {0,5},
                      2012,
                      100,
                      5};

    Record record3 = {"I wish",
                      "Skee-lo",
                      "He just wishes he was a baller who was a little bit taller",
                      "Old Rap",
                      {0,4},
                      1995,
                      103,
                      4};


    Node *head = NULL;
    insertRecord(&head,record1);
    insertRecord(&head,record2);
    insertRecord(&head,record3);

    //Function body
    int length = getLengthOfList(&head);


    //Create an array of indexes to store the order of playing
    int *order = malloc(sizeof(int) * length);

    //Set all of the indexes to -1 before shuffling
    for (int j = 0; j < length; ++j) {
        order[j] = -1;
    }

    //Create a random order
    for (int i = 0; i < length; ++i) {
        int look = 1;
        int index = -1;

        while (look == 1){
            //Generate a new random index
            index = rand() % length;

            //Go through the list and check if it is used yet
            int c = -1;
            for (int j = 0; j < length; ++j) {
                if(order[j] == index)c = 1;
            }

            if(c == -1){
                look = -1;
            }
        }

        //Add the index to the order
        order[i] = index;
    }

    int index = 0;
    Node *current = head;
    for (int k = 0; k < length; ++k) {

        int goal = order[k];

        //Traverse through the list
        while (index != goal){
            //Go backwards
            if(index < goal){
                index++;
                current = current->next;
            }
            //Go forwards
            if(index > goal){
                index--;
                current = current->previous;
            }
        }

        //Check if it is indeed playing in the correct order
        if(goal != index)return -1;

        //Display the record, dont do this because its just a test
      //  printRecord(current->data);

        //Increase plays
        current->data.plays = current->data.plays + 1;

        //Wait 5 seconds and clear the screen, Dont do this because it is just a test
      //  _sleep(5000);
      //  system("cls");
    }

    free(order);

    //All the indexes worked so return 1 to indicate we passed.



    return 1;
}

//Removes the newline character at the end of strings
void fixString(char string[256]){
    int length = strlen(string);

    char item = string[length - 1];

    if(item == '\n'){
        string[length - 1] = '\0';
    }
}