//
// APS105 Lab 9
//
// This is a program written to maintain a personal music library,
// using a linked list to hold the songs in the library.
//
// Author: Randy Ding
// Student Number: 1008011704
//

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "musiclibrary.h"

// A node in the linked list

// Each string in the node is declared as a character pointer variable,
// so they need to be dynamically allocated using the malloc() function,
// and deallocated using the free() function after use.

// Declarations of linked list functions
//
// DECLARE YOUR LINKED-LIST FUNCTIONS HERE
//

typedef struct list{

  Node *head;
}LinkedList;



Node *createNode(char *artist, char *songName, char *genre){

  Node *newNode = (Node*)malloc(sizeof(Node));

  if(newNode != NULL){

    newNode -> artist = artist;
    newNode -> songName = songName;
    newNode -> genre = genre;
    newNode -> next = NULL;
  }

  return newNode;
}


bool insertAtFront(LinkedList* head, char *artist, char *songName, char *genre){

  Node* newNode = createNode(artist, songName, genre);

  if(newNode == NULL) return false;

  newNode -> next = head -> head;
  head -> head = newNode;

  return true;
}


bool insertNode(LinkedList* head, char *artist, char *songName, char *genre){


  if(head -> head == NULL){

    head -> head = createNode(artist, songName, genre);
    return true;
  }

  //the current node that we are looking at
  Node *current = head -> head;

  //insert at front
  if(strcmp(current -> songName, songName) > 0){

    insertAtFront(head, artist, songName, genre);
    return true;
  }

  //loop to check if song name is a greater letter than the current node, if so, moves to next node until its becomes less
  while(current -> next != NULL && strcmp(current -> next -> songName, songName) <= 0){

    //checks if there is a duplicate
    if(strcmp(current -> next -> songName, songName) == 0){

      songNameDuplicate(songName);
      return false;
    }

    current = current -> next;
  }

  Node *newNode = createNode(artist, songName, genre);
  newNode -> next = current -> next;
  current -> next = newNode;

  return true;
}


bool printList(LinkedList* head){

  Node* current = head -> head; 

  if(current == NULL) {printMusicLibraryEmpty(); return false;}

  printMusicLibraryTitle();
  while(current != NULL){

    printf("\n%s\n%s\n%s\n", current -> songName, current -> artist, current -> genre);
    current = current -> next;    
  }

  return true;
}


void deleteFront(LinkedList *head){

  Node *newHead = head -> head -> next;
  free(head -> head -> songName);
  free(head -> head -> artist);
  free(head -> head -> genre);
  free(head -> head);
  head -> head = newHead;
}

void deleteAllNodes(LinkedList *head){

  while(head -> head != NULL){

    songNameDeleted( head -> head -> songName);
    deleteFront(head);
  }
}

bool deleteAtMatch(LinkedList *head, char *songName){

  if(head -> head == NULL){
    
    songNameNotFound(songName);
    return false;
  }

  if(strcmp(head -> head -> songName, songName) == 0){

    songNameDeleted(songName);
    deleteFront(head);
    return true;
  }

  Node *current = head -> head;
  while(current -> next != NULL && strcmp(current -> next -> songName, songName) != 0){

    current = current -> next;
  }

  if(current -> next == NULL){

    songNameNotFound(songName);
    return false;
  }

  songNameDeleted(songName);
  Node *temp = current -> next;
  current -> next = temp -> next;
  free(temp -> songName);
  free(temp -> artist);
  free(temp -> genre);
  free(temp);
  return true;
}


Node* firstNode(LinkedList *head, char *songName){

  Node *current = head -> head;
  while(current != NULL){

    if(strcmp(current -> songName, songName) == 0){
      songNameFound(songName);
      printf("\n%s\n%s\n%s\n", current -> songName, current -> artist, current -> genre);
      return current;
    }
    else{

      current = current -> next;
    }
  }

  songNameNotFound(songName);
  return NULL;
}


int main(void) {
  // Declare the head of the linked list.
  //   ADD YOUR STATEMENT(S) HERE

  LinkedList list;
  list.head = NULL;


  // Announce the start of the program
  printf("Personal Music Library.\n\n");
  printf("%s", "Commands are I (insert), D (delete), S (search by song name),\n"
               "P (print), Q (quit).\n");

  char response;
  char input[MAX_LENGTH + 1];
  do {
    inputStringFromUser("\nCommand", input, MAX_LENGTH);

    // Response is the first character entered by user.
    // Convert to uppercase to simplify later comparisons.
    response = toupper(input[0]);

    if (response == 'I') {
      // Insert a song into the linked list.
      // Maintain the list in alphabetical order by song name.
      //   ADD STATEMENT(S) HERE

      char *promptName = "Song name" ;
      char *promptArtist =  "Artist" ;
      char *promptGenre = "Genre" ;

      char* songName = (char*)malloc(sizeof(char)*(MAX_LENGTH+1));
      char* artist = (char*)malloc(sizeof(char)*(MAX_LENGTH+1));
      char* genre = (char*)malloc(sizeof(char)*(MAX_LENGTH+1));
      //gets the songname from user
      inputStringFromUser(promptName, songName, MAX_LENGTH);

      //gets artist from user
      inputStringFromUser(promptArtist, artist, MAX_LENGTH);

      //gets genre from user
      inputStringFromUser(promptGenre, genre, MAX_LENGTH);

      insertNode(&list, artist, songName, genre);

    } else if (response == 'D') {
      // Delete a song from the list.
      //   ADD STATEMENT(S) HERE

      char *prompt = "\nEnter the name of the song to be deleted";
      char *songName = (char*)malloc(sizeof(char)*(MAX_LENGTH+1));
      inputStringFromUser(prompt, songName, MAX_LENGTH);

      deleteAtMatch(&list, songName);

    } else if (response == 'S') {
      // Search for a song by its name.

      char *prompt = "\nEnter the name of the song to search for";
      char *songName = (char*)malloc(sizeof(char)*(MAX_LENGTH + 1));
      inputStringFromUser(prompt, songName, MAX_LENGTH);

      firstNode(&list, songName);
      //   ADD STATEMENT(S) HERE

    } else if (response == 'P') {
      // Print the music library.

      //   ADD STATEMENT(S) HERE
      printList(&list);

    } else if (response == 'Q') {
      ; // do nothing, we'll catch this below
    } else {
      // do this if no command matched ...
      printf("\nInvalid command.\n");
    }
  } while (response != 'Q');

  // Delete the entire linked list.
  //   ADD STATEMENT(S) HERE

  deleteAllNodes(&list);
  // Print the linked list to confirm deletion.
  //   ADD STATEMENT(S) HERE
  printList(&list);

  return 0;
}