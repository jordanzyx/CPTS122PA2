/*******************************************************************************************
* Programmer: Jordan Muehlbayer *
* Class: CptS 122, Spring, 2019;
* Programming Assignment: PA2 *
* Date: Febuary 10th, 2020 *
* Description: This file is an outline of methods needed for the movie player. Implemented in source.c
*******************************************************************************************/
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

/**
 * Returns a linked list of all the movies linked to a director
 */
void getMoviesByDirector(Node **head,char *director,Record records[256]);

void setRecordsToDefault(Record records[256]);
int getLengthOfRecords(Record records[256]);
void printListOfRecords(Record records[256]);

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
void editNode(Node *node);
void editNodeTitle(Node *node);
void editNodeDirector(Node *node);
void editNodeDescription(Node *node);
void editNodeGenre(Node *node);
void editNodeHours(Node *node);
void editNodeMinutes(Node *node);
void editNodeYear(Node *node);
void editNodePlays(Node *node);
void editNodeRating(Node *node);

/**
 * o What must “rate” do?
    The “rate” command must allow the user to assign a value of 1 – 5 to a movie; 1 is the
    lowest rating and 5 is the highest rating. The rating will replace the previous rating.
 */
void rate(Node **head);
/**
 * o What must “play” do?
    The “play” command must allow the user to select a movie and must start “playing”
    each movie in order from the current movie. “Playing” the movie for this assignment
    means displaying the contents of the record that represents the movie for a short
    period of time, clearing the screen and showing the next record in the list, etc. This
    continues until all movies have been played.
 */
void play(Node **head);
/**
 * • 17 pts – Correct “insert” command implementation
    o (7 pts - 1pt/attribute) For prompting and getting the details of
    a new record from the user
    o (10 pts) For correctly inserting the record at the front of the list
 * @param head
 */
void insert(Node **head);
/**
 * • 23 pts – For correct “delete” command implementation
    o (3 pts) For prompting and getting the movie title from the user
    o (5 pts) For searching for specific record matching the movie title
    o (15 pts) For removing the matching record from the list, and
    reconnecting the list correctly
 * @param head
 */
void delete(Node **head);
void deleteByName(Node **head,char title[256]);
/**
 * 15 pts – Correct “shuffle” command implementation
    o (5 pts) For generating the random order based on the number of movies
    in the list
    o (10 pts) For moving through the list (forwards and backwards) and
    playing the movies in the order generated
 * @param head
 */
void shuffle(Node **head);

/**
 * Plays through the list starting at a node
 */
void playNode(Node *node);

/**
 * o What must “exit” do?
    The “exit” command saves the most recent list to the moviePlayList.csv file.
    This command will completely overwrite the previous contents in the file.
 */
void exitCommand(Node **head);

void insertRecord(Node **head,Record newRecord);

int testInsert();

int testDelete();

int testShuffle();

int getLengthOfList(Node **head);

#endif //CPTS122PA1_SOURCE_H
