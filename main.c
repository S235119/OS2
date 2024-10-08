/* You are not allowed to use <stdio.h> */
#include "io.h"
#include "mm.h"
#include <unistd.h>
#include <stdlib.h>




/**
 * @name  main
 * @brief This function is the entry point to your program
 * @return 0 for success, anything else for failure
 *
 *
 * Then it has a place for you to implementation the command
 * interpreter as  specified in the handout.
 */
typedef struct Collection {
    int element;
    struct Collection* next;
} Collection;

int
main()
{
    /*-----------------------------------------------------------------
*TODO:  You need to implement the command line driver here as
*       specified in the assignment handout.
*
* The following pseudo code describes what you need to do
*
*  Declare the counter and the collection structure variables
*
*
*  In a loop
*    1) Read a command from standard in using read_char function
*    2) If the command is not 'a', 'b', 'c': then break the loop
*    3) Process the command as specified in the handout
*  End loop
*
*  Print your collection of elements as specified in the handout
*    as a comma delimited series of integers
*-----------------------------------------------------------------*/

    char character;
    Collection *head = NULL;
    Collection *tail = NULL;


    int count = 0;

    do{

        character = read_char();
        if(character == 'a') {
            Collection *newElement = (Collection*)simple_malloc(sizeof(Collection));
            newElement->element = count;
            newElement->next = NULL;

            if(head == NULL){
                head = newElement;
                tail = newElement;
            } else{
                tail -> next = newElement;
                tail = newElement;
            }
            count++;
        }

        if(character == 'b'){
            count++;
        }

        if(character == 'c'){
            if(head != NULL){
                if(head == tail){
                    simple_free(head);
                    head = NULL;
                    tail = NULL;
                } else{
                    Collection *current = head;
                    while (current->next != tail) {
                        current = current->next;
                    }
                    simple_free(tail);
                    tail = current;
                    tail ->next = NULL;
                }
            }
            count++;
        }
    }
    while (character == 'a' || character == 'b' || character == 'c');

    Collection* point;
    while (head != NULL) {
        write_int(head -> element);
        if (head -> next != NULL) {
            write_char(',');
        }
        point = head -> next;       //point holds the address of the next element before head is freed
        simple_free(head);        //freeing memory allocated for each element to avoid memory leaks

        /*head is assigned the address of the next element to ensure that head always points to
        a valid element and no to a freed memory location*/
        head = point;
    }

    write_char(';');
    write_char('\n');

    /*
    write_string("count = ");
    write_int(count);
    write_char('\n');
     */

    return 0;

}

