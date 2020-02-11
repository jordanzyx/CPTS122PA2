//
// Created by Jordan on 2/8/2020.
//

#ifndef CPTS122PA1_SOURCE_H
#define CPTS122PA1_SOURCE_H

/**
 * Duration is defined as follows:
    o Hours – an integer
    o Minutes – an integer
 */
typedef struct duration{
    int hours;
    int minutes;
}Duration;
/**
 * Each record consists of the following attributes:
    o Film Title – a string
    o Director – a string
    o Description – a string
    o Genre – a string
    o Running Time - a struct Duration type consisting of hours and minutes,
    both integers
    o Year – an integer
    o Number times played – an integer
    o Rating – an integer (1 – 5)
 */
typedef struct record{
    char title[256];
    char director[256];
    char description[256];
    char genre[256];
    Duration duration;
    int year;
    int plays;
    int rating;
}Record;

/**
 * Finally, each struct Node in the doubly linked list must be defined as follows:
    o Data – a Record
    o Pointer to the next node
    o Pointer to the previous node
 */
typedef struct node{
    Record data;
    struct node *next;
    struct node *previous;
}Node;

/**
 * Starts up the program and should be the only method called in main
 */
void start();
/**
 * Handles the input loop of the menu
 */
void menu(Node **head);

void displayMenu();

/**
 * o What must “load” do?
    The “load” command must read all records from a file
    called moviePlayList.csv attached to the assignment on blackboard into a
    dynamic doubly linked list. The doubly linked list is considered the main playlist. As
    each record is read from the file, it must be inserted at the front of the list.
 */
void load(Node **head);
/**
 * Inserts item at the front of a doubly linked list, used in the load command
 */
int insertFront(Node **head,Record item);
/**
 * Reads a line from the .csv file and creates a Record out of it
 */
Record fromLine(FILE *input);
/**
 * Create a duration from a string used in fromLine
 */
Duration durationFromString(char string[10]);

/**
 * Prints out the entire linked list
 */
void printList(Node **head);

/**
 * Prints out the data for a Record
 */
void printRecord(Record record);

/**
 * o What must “store” do?
    The “store” command writes the current records, in the dynamic doubly linked list,
    to the moviePlayList.csv file. The store will completely overwrite the previous
    contents in the file.
 */
void store(Node **head);

/**
 * Write Record to File, used in store command
 */
void writeRecordToFile(Record record,FILE *output);

/**
 * o What must “display” do?
    The “display” command prints records to the screen. This command must support
    three methods, one of which is selected by the user:
    1. Print all movies.
    2. Print all records that match a director.
    3. Print all records that match a year.
 */
void display(Node **head);
/**
 * The menu shown to the user after they run the display command
 */
void printDisplayMenu();
/**
 * Print out all of the movies matching this directors name
 */
void printListDirector(Node **head);
/**
 * Print out all of the movies mathching this year
 */
void printListYear(Node **head);

/**
 * o What must “edit” do?
    The “edit” command must allow the user to find a record in the list by director. If
    there are multiple records with the same director, then your program must prompt
    the user which one to edit. The user may modify all of the attributes in the record.
 */
void edit(Node **head);

/**
 * Used to edit all the attributes of a specific record via user input.
 */
Record editNode(Node *node);

/**
 * o What must “rate” do?
    The “rate” command must allow the user to assign a value of 1 – 5 to a movie; 1 is the
    lowest rating and 5 is the highest rating. The rating will replace the previous rating.
 */
void rate();
/**
 * o What must “play” do?
    The “play” command must allow the user to select a movie and must start “playing”
    each movie in order from the current movie. “Playing” the movie for this assignment
    means displaying the contents of the record that represents the movie for a short
    period of time, clearing the screen and showing the next record in the list, etc. This
    continues until all movies have been played.
 */
void play();
/**
 * o What must “exit” do?
    The “exit” command saves the most recent list to the moviePlayList.csv file.
    This command will completely overwrite the previous contents in the file.
 */
void exitCommand(Node **head);

#endif //CPTS122PA1_SOURCE_H
