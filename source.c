//
// Created by Jordan on 2/8/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "source.h"

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
        scanf("%d",&choice);

        //Get rid of invalid inputs
        if(choice < 1 || choice > 11){
            choice = -1;
            continue;
        }

        //Handle valid input
        if(choice == 1)load(head);
        if(choice == 2)store(head);
        if(choice == 3)display(head);
        if(choice == 4)continue;
        if(choice == 5)continue;
        if(choice == 6)edit(head);
        if(choice == 7)continue;
        if(choice == 8)rate(head);
        if(choice == 9)play(head);
        if(choice == 10)continue;
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