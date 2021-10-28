/* linkedlist.c            <<-- A template to update and
 * Aleric Stell
 * alerics
 * ECE 2230 Fall 2021
 * MP2
 *
 * Propose: A template for linkedlist.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.
 *
 * Bugs: Many details have not been implemented.
 *
 */
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "linkedlist.h"        // defines public functions for list ADT

// definitions for private constants used in linkedlist.c only

#define SORTED_LLIST    999999
#define UNSORTED_LLIST -888888

// prototypes for private functions used in linkedlist.c only

void my_debug_validate(linked_t *L);
void printlist(linked_t *list_ptr);
/* ----- below are the functions  ----- */

/* Allocates a new, empty list
 *
 * If the comparison function is NULL, then the list is unsorted.
 *
 * Otherwise, the list is initially assumed to be sorted.  Note that if
 * list_insert is used the list is changed to unsorted.
 *
 * The field sorted can only take values SORTED_LLIST or UNSORTED_LLIST
 *
 * Use linked_destruct to remove and deallocate all elements on a list
 * and the header block.
 */
linked_t * linked_construct(int (*compare_function)(const data_t *, const data_t *))
{
        linked_t *L;
        L = (linked_t *) malloc(sizeof(linked_t));
        L->list_head = NULL;
        L->list_tail = NULL;
        L->current_list_size = 0;
        L->comp_proc = compare_function;
        if (compare_function == NULL)
                L->list_sorted_state = UNSORTED_LLIST;
        else
                L->list_sorted_state = SORTED_LLIST;

        return L;
}

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 *
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from linked_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.
 */
data_t * linked_access(linked_t *list_ptr, int pos_index)
{
        int count=0;
        linked_elem_t *L;

        // debugging function to verify that the structure of the list is valid

        /* handle special cases.
         *   1.  The list is empty
         *   2.  Asking for the head
         *   3.  Asking for the tail
         *   4.  specifying a position that is out of range.  This is not defined
         *       to be an error in this function, but instead it is assumed the
         *       calling function correctly specifies the position index
         */
        if (linked_entries(list_ptr) == 0) {
                return NULL;  // list is empty
        }
        else if (pos_index == LLPOSITION_FRONT || pos_index == 0) {
                return list_ptr->list_head->data_ptr;
        }
        else if (pos_index == LLPOSITION_BACK || pos_index == linked_entries(list_ptr) - 1) {
                return list_ptr->list_tail->data_ptr;
        }
        else if (pos_index < 0 || pos_index >= linked_entries(list_ptr))
                return NULL;   // does not correspond to position in list

        // loop through the list until find correct position index
        L=list_ptr->list_head;
        for (count = 0; count < pos_index; count++) {
                L = L->fwd_link;
        }
        // already verified that pos_index should be valid so L better not be null
        assert(L != NULL);
        assert(L->data_ptr != NULL);
        return L->data_ptr;
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 */
void linked_destruct(linked_t *list_ptr)
{
        // the first line must validate the list

        // Your code starts here
        linked_elem_t * currentNode = list_ptr->list_head;


        for(int i = 0; i < list_ptr->current_list_size; ++i) {
                free(currentNode->data_ptr);    //free data pointer
                if(currentNode->fwd_link == NULL)
                        free(currentNode);          //checks for next node, if exists it'll continue,
                //  if not it'll free the node
                else {
                        currentNode = currentNode->fwd_link;    //goes to next node
                        free(currentNode->back_link);           //frees past node
                }
        }
        free(list_ptr);                      //frees whole list
}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * linked_ptr: pointer to list-of-interest.
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the
 *            list.  Index starts at 0 at head of the list, and incremented by
 *            one until the tail is reached.  The index can also be equal
 *            to LLPOSITION_FRONT or LLPOSITION_BACK (these are special negative
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list,
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the linked_ptr->list_sorted_state must be equal
 * to UNSORTED_LLIST.
 */
void linked_insert(linked_t *list_ptr, data_t *elem_ptr, int pos_index)
{
        assert(list_ptr != NULL);

        linked_elem_t * newNode = (linked_elem_t*)malloc(sizeof(linked_elem_t)); //creates and allocates memory for   newNode
	newNode->data_ptr = elem_ptr;

	if(list_ptr->list_head == NULL)
	{
		list_ptr->list_head = newNode; list_ptr->list_tail = newNode;
		list_ptr->list_head->back_link = NULL; list_ptr->list_tail->fwd_link = NULL;

	}
	else if(pos_index == LLPOSITION_FRONT) {
                newNode->fwd_link = list_ptr->list_head;
                list_ptr->list_head->back_link = newNode;
                newNode->back_link = NULL;
                list_ptr->list_head = newNode;
        }
        else if(pos_index < list_ptr->current_list_size && pos_index != LLPOSITION_BACK) {

                linked_elem_t * currentNode = list_ptr->list_head;

                for(int i = 0; i < pos_index; i++)
                        currentNode = currentNode->fwd_link;

                newNode->fwd_link = currentNode->fwd_link;
                newNode->back_link = currentNode;
                newNode->fwd_link->back_link = newNode;
                currentNode->fwd_link = newNode;
        }
        else {
		list_ptr->list_tail->fwd_link = newNode;
                newNode->back_link = list_ptr->list_tail;
                newNode->fwd_link = NULL;
		list_ptr->list_tail = newNode;
        }

        list_ptr->current_list_size++;

        // the last three lines of this function must be the following
        if (list_ptr->list_sorted_state == SORTED_LLIST)
                list_ptr->list_sorted_state = UNSORTED_LLIST;

}

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use linked_insert_sorted, the list preserves its sorted nature.
 *
 * If you use linked_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call linked_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type data_t.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 *
 * Note: if the element to be inserted is equal in rank to an element already
 *       in the list, the newly inserted element will be placed after all the
 *       elements of equal rank that are already in the list.
 */
void linked_insert_sorted(linked_t *list_ptr, data_t *elem_ptr)
{
        assert(list_ptr != NULL);
        assert(list_ptr->list_sorted_state == SORTED_LLIST);

        linked_elem_t * newNode = (linked_elem_t*)malloc(sizeof(linked_elem_t));
        newNode->data_ptr = elem_ptr;

	if(list_ptr->current_list_size == 0)
	{
		list_ptr->list_head = newNode;
		list_ptr->list_tail = newNode;
	}
	else
	{
		linked_elem_t * currentNode = list_ptr->list_head;

		while(currentNode != NULL) {

			int pos_index = list_ptr->comp_proc(newNode->data_ptr, currentNode->data_ptr);

			if(pos_index == 1) {
				newNode->fwd_link = currentNode;
				newNode->back_link = currentNode->back_link;
				if(currentNode->back_link != NULL)
					currentNode->back_link->fwd_link = newNode;
				currentNode->back_link = newNode;
				if(newNode->back_link == NULL)
					list_ptr->list_head = newNode;
				if(newNode->fwd_link == NULL)
					list_ptr->list_tail = newNode;
				break;
			}
			else if(currentNode->fwd_link == NULL)
			{
				newNode->back_link = currentNode;
				currentNode->fwd_link = newNode;
				list_ptr->list_tail = newNode;
				newNode->fwd_link = NULL;
				break;
			}
			else {
				currentNode = currentNode->fwd_link;
			}
		}
	}
	list_ptr->list_tail->fwd_link = NULL;
        list_ptr->current_list_size++;

        // the last line checks if the new list is correct
	
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LLPOSITION_FRONT and LLPOSITION_BACK
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
data_t * linked_remove(linked_t *list_ptr, int pos_index)
{
        // the first line must validate the list
        data_t * elem_t = (data_t *)malloc(sizeof(data_t));

	pos_index = pos_index == 0 ? LLPOSITION_FRONT : 
		pos_index == linked_entries(list_ptr) - 1 ? LLPOSITION_BACK : pos_index;

	// single element in list matching pos_index
        if(list_ptr->current_list_size == 1 && pos_index == 0)
	{
		elem_t = list_ptr->list_head->data_ptr;
		list_ptr->list_head = NULL;
		list_ptr->list_tail = NULL;
		list_ptr->current_list_size--;
		return elem_t;
	}
	// if front was specified
	else if(pos_index == LLPOSITION_FRONT)
        {
                elem_t = list_ptr->list_head->data_ptr;
		linked_elem_t * tempNode = list_ptr->list_head; 
                list_ptr->list_head = list_ptr->list_head->fwd_link;
		if(list_ptr->list_head != NULL)
	                list_ptr->list_head->back_link = NULL;
		else
			list_ptr->list_tail = NULL;
		free(tempNode);
        }
	// anywhere in the list except head and tail
        else if(pos_index < list_ptr->current_list_size && pos_index != LLPOSITION_BACK)
        {
                linked_elem_t * currentNode = list_ptr->list_head;
		
		//go to the node to remove
                for(int i = 0; i < pos_index; i++)
                        currentNode = currentNode->fwd_link;
		
		// store data to return
                elem_t = currentNode->data_ptr;

		// sets the foward link of the previous node to the node after
		if(currentNode != list_ptr->list_head)
			currentNode->back_link->fwd_link = currentNode->fwd_link;
		// updates head
		else
		{
			list_ptr->list_head = currentNode->fwd_link;
			list_ptr->list_head->back_link = NULL;
		}
		// sets the back link of the next node to the previous node
		if(currentNode != list_ptr->list_tail)
			currentNode->fwd_link->back_link = currentNode->back_link;
		// updates tail
		else
		{
			list_ptr->list_tail = currentNode->back_link;
			list_ptr->list_tail->fwd_link = NULL;
		}
				

		free(currentNode);

        }
	// if tail was specified
        else if(pos_index == LLPOSITION_BACK)
        {
                elem_t = list_ptr->list_tail->data_ptr;
		linked_elem_t * tempNode = list_ptr->list_tail; 
                list_ptr->list_tail = list_ptr->list_tail->back_link;
	        list_ptr->list_tail->fwd_link = NULL;
		free(tempNode);
        }
	// OOB
        else
                return NULL;


        list_ptr->current_list_size--;
        return elem_t;
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * elem_ptr: element against which other elements in the list are compared.
 *
 * compare_function: function to test if elem_ptr is a match to an item in the list.
 *        Returns 0 if they match and any value not equal to 0 if they do not match.
 *
 * NOTICE: pos_index is returned and is not an input value!
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1.
 *
 * pos_index: used as a return value for the position index of matching element
 *
 */
data_t * linked_elem_find(linked_t *list_ptr, data_t *elem_ptr, int *pos_index,
                          int (*compare_function)(const data_t *, const data_t *))
{
        // the first line must validate the list
	
	*pos_index = -1;

	if(list_ptr->current_list_size == 0)
		return NULL;

        linked_elem_t * currentNode = list_ptr->list_head;
        int count = 1;
        while(compare_function(elem_ptr,currentNode->data_ptr) != 0 && count < list_ptr->current_list_size)
        {
                currentNode = currentNode->fwd_link;
                count++;
        }

        
        if(compare_function(elem_ptr,currentNode->data_ptr) == 0)
                *pos_index = count - 1;

        return (*pos_index == -1 ? NULL : currentNode->data_ptr);
}

/* Removes an extremum element from the list and returns a pointer to it.
 * If multiple elements are deemed to have equal extremum values then the element
 * closer to the head of the linked list should be the one removed
 *
 * list_ptr: pointer to list-of-interest.
 *
 * compare_function: function to test if an element has the extremum value.
 *        Returns 1 if the first argumnet is more extreme than the second.
 *
 * The function returns a pointer to the first extremum element if the list
 * contains one or more items.  If the list is empty the return value is NULL.
 *
 * Note if the list is sorted and the objective is to move the first or last item
 * then this function should not be used.  Instead remove the head or tail!
 */
data_t * linked_elem_remove_extremum(linked_t *list_ptr,
                                     int (*compare_function)(const data_t *, const data_t *))
{
        // the first line must validate the list
        if(list_ptr->current_list_size == 0)
                return NULL;

        linked_elem_t * extremum = list_ptr->list_head, *currentNode  = list_ptr->list_head->fwd_link;
        for(int i = 1; i < list_ptr->current_list_size; i++)
        {
                if(compare_function(currentNode->data_ptr, extremum->data_ptr) == 1)
                        extremum = currentNode;
                currentNode = currentNode->fwd_link;
        }
	int pos;
        linked_elem_find(list_ptr, extremum->data_ptr, &pos, list_ptr->comp_proc);

        return linked_remove(list_ptr, pos);
}
/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int linked_entries(linked_t *list_ptr)
{
        assert(list_ptr != NULL);
        assert(list_ptr->current_list_size >= 0);
        return list_ptr->current_list_size;
}

/* This function takes a list pointer and sorts it based on the sort type
 *
 * list_ptr: reference to the list
 *
 * sort_type: determines which of the 5 sorting algorithms to use
 *
 * fcomp: funtion pointer to compare two data_ptr
 *
 * While this function has no return, the list_ptr will be passed by pointer.
 * The list_ptr will have an updated comp_proc, and be sorted, and it's
 * state will be set to sorted
 */
void linked_sort(linked_t **list_ptr, int sort_type, int (*fcomp)(const data_t*, const data_t*))
{
	// check if list exist
	if((*list_ptr) == NULL)
		return;
	else
		(*list_ptr)->comp_proc = fcomp;

	if(sort_type == 1)
		insertionSort(list_ptr);
	if(sort_type == 2)
		extremumSort(list_ptr);
	if(sort_type == 3)
		recursiveSort((*list_ptr)->list_head, (*list_ptr)->list_tail, fcomp);
	if(sort_type == 4)
		iterativeSort((*list_ptr)->list_head, (*list_ptr)->list_tail, fcomp);
	if(sort_type == 5)
	{
		mergeSort(&((*list_ptr)->list_head), fcomp);

		// mergeSort is the only function that requires the tail to be updated
		linked_elem_t * currentNode = (*list_ptr)->list_head;
		while(currentNode->fwd_link != NULL)
			currentNode = currentNode->fwd_link;
		(*list_ptr)->list_tail = currentNode;
	}

	//printlist(*list_ptr);

	(*list_ptr)->list_sorted_state = SORTED_LLIST;
	my_debug_validate(*list_ptr);
}

/* This function sort the list using an insertion sort
 *
 * list_ptr: pointer to the list to-be sorted
 *
 * This function will update list_ptr to point to a newly sorted list
 */
void insertionSort(linked_t ** list_ptr)
{
	linked_t * new_list = linked_construct((*list_ptr)->comp_proc);
	while((*list_ptr)->current_list_size != 0)
	{
		data_t * removedNode = linked_remove(*list_ptr, LLPOSITION_FRONT);
		linked_insert_sorted(new_list, removedNode);
	}
	
	*list_ptr = new_list;
	//linked_destruct(new_list);
}
/* This function sorts the list using an extremum sort
 *
 * list_ptr: pointer to the list to be sorted
 *
 * This function will update list_ptr to point to a newly sorted list
 */
void extremumSort(linked_t ** list_ptr)
{
	linked_t * new_list = linked_construct((*list_ptr)->comp_proc);
	while((*list_ptr)->current_list_size != 0)
	{
		data_t * extremum = linked_elem_remove_extremum(*list_ptr, (*list_ptr)->comp_proc);
		linked_insert(new_list, extremum, LLPOSITION_BACK);
	}

	*list_ptr = new_list;
	//linked_destruct(new_list);
}

/* This function sorts the list using a recursive selction sort
 *
 * m: pointer to the front of the list. After recursion, m will
 *    be the fwd_link of m
 *
 * n: pointer to the tail of the list.
 *
 * This function will update the data in each node to sort it
 * without changing the order of the nodes.
 */
void recursiveSort(linked_elem_t *m, linked_elem_t *n, int (*fcomp)(const data_t*, const data_t*))
{
	data_t * temp = (data_t*)malloc(sizeof(data_t));

	if(m != n)
	{
		// find max
		linked_elem_t * maxNode = findMax(m, n, fcomp);

		// swap max to current position
		temp = m->data_ptr;
		m->data_ptr = maxNode->data_ptr;
		maxNode->data_ptr = temp;

		// recursive with m forward link
		recursiveSort(m->fwd_link, n, fcomp);
	}
}


/* This function sorts the list using a iterative selection sort
 *
 * m: pointer to the front of the list. Through iteration, m
 *    will be update to m->fwd_link
 *
 * n: pointer to the tail of the list
 *
 * This function will update the data of each node to sort it
 * without changing the order of the nodes
 */
void iterativeSort(linked_elem_t *m, linked_elem_t *n, int (*fcomp)(const data_t*, const data_t*))
{
	data_t * temp = (data_t*)malloc(sizeof(data_t));

	while(m != NULL)
	{
		// find max
		linked_elem_t * maxNode = findMax(m, n, fcomp);
		
		// swap max to current position
		temp = m->data_ptr;
		m->data_ptr = maxNode->data_ptr;
		maxNode->data_ptr = temp;

		// iterate foward
		m = m->fwd_link;
	}
}

linked_elem_t * findMax(linked_elem_t * m, linked_elem_t * n, int (*fcomp)(const data_t*, const data_t*))
{
	linked_elem_t * maxNode = m;
	while(m != NULL)
	{
		if((*fcomp)(maxNode->data_ptr, m->data_ptr) != 1)
			maxNode = m;
		m = m->fwd_link;
	}

	return maxNode;
}
/* Uses a merge sort to sort the list
 *
 * head: double pointer representing the front of the list/sublist
 *
 * fcomp: function pointer for comparing data_ptr
 *
 * While there is no return, this function uses pass by pointer to
 * return the head of the list after it is sorted
 */
void mergeSort(linked_elem_t ** head, int (*fcomp)(const data_t *, const data_t *))
{
	// empty list or list of size 1
	if(*head == NULL || (*head)->fwd_link == NULL)
		return;
	linked_elem_t * m = *head, *n = split(*head);

	mergeSort(&m, fcomp);
	mergeSort(&n, fcomp);

	*head = merge(m, n, fcomp);	
}

/* Splits the linked list into two sub list
 * 
 * head: First node of the list/sublist
 *
 * Return: this function returns the first node in the sublist 
 * that is the node after the middle of the list/sublist
 */
linked_elem_t * split(linked_elem_t* head)
{
	linked_elem_t * slow = head;
	linked_elem_t * fast = head->fwd_link;

	// find middle of list using the fast slow pointer method
	while(fast != NULL)
	{
		// this works by advaning fast twice and slow once
		// so that when fast reaches the end of the list,
		// slow will only be at the middle of the list;
		fast = fast->fwd_link;
		if(fast != NULL)
		{
			slow = slow->fwd_link;
			fast = fast->fwd_link;
		}
	}
	
	linked_elem_t * middleNode = slow->fwd_link;
	slow->fwd_link = NULL;
	return middleNode;
}

/* This function take two sublist and merges them into a single list
 *
 * m: the head of the first sublist
 *
 * n: the head of the second sublist
 *
 * fcomp: function pointer to compare the two data_ptr
 *
 * Return: this function returns a pointer to the head of the new list,
 * where the new list is the result of merging m and n
 */
linked_elem_t * merge(linked_elem_t * m, linked_elem_t * n, int (*fcomp)(const data_t*, const data_t*))
{
	if(m == NULL)
		return n;
	if(n == NULL)
		return m;

	// if m should be closer to the head than n
	if((*fcomp)(m->data_ptr, n->data_ptr) != 1)
	{
		n->fwd_link = merge(m, n->fwd_link, fcomp);
		n->fwd_link->back_link = n;
		n->back_link = NULL;
		return n;
	}
	else
	{
		m->fwd_link = merge(m->fwd_link, n, fcomp);
		m->fwd_link->back_link = m;
		m->back_link = NULL;
		return m;
	}
}

void printlist(linked_t * list_ptr)
{
	linked_elem_t *currentNode = list_ptr->list_head;
	while(currentNode != NULL)
	{
		// printf("%d\n", currentNode->data_ptr->ep_id);
		currentNode = currentNode->fwd_link;
	}
}
/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void my_debug_validate(linked_t *L)
{
        assert(NULL != L);
        assert(SORTED_LLIST == L->list_sorted_state || UNSORTED_LLIST == L->list_sorted_state);
        if (0 == L->current_list_size) assert(NULL == L->list_head && L->list_tail == NULL);
        if (NULL == L->list_tail)
                assert(NULL == L->list_head && 0 == L->current_list_size);
        else
                assert(NULL == L->list_tail->fwd_link);
        if (NULL == L->list_head)
                assert(NULL == L->list_tail && 0 == L->current_list_size);
        else
                assert(NULL == L->list_head->back_link);
        if (NULL != L->list_tail && L->list_tail == L->list_head) assert(1 == L->current_list_size);
        if (1 == L->current_list_size) {
                assert(L->list_head == L->list_tail && NULL != L->list_tail);
                assert(NULL != L->list_tail->data_ptr);
                assert(NULL == L->list_tail->fwd_link && NULL == L->list_head->back_link);
        }
        if (1 < L->current_list_size) {
                assert(L->list_head != L->list_tail && NULL != L->list_tail && NULL != L->list_head);
                linked_elem_t *R = L->list_head;
                int tally = 0;
                while (NULL != R) {
                        if (NULL != R->fwd_link) assert(R->fwd_link->back_link == R);
                        else assert(R == L->list_tail);
                        assert(NULL != R->data_ptr);
                        ++tally;
                        R = R->fwd_link;
                }
                assert(tally == L->current_list_size);
        }
        if (NULL != L->list_head && SORTED_LLIST == L->list_sorted_state) {
                linked_elem_t *R = L->list_head;
                while (NULL != R->fwd_link) {
                        assert(-1 != L->comp_proc(R->data_ptr, R->fwd_link->data_ptr));
                        R = R->fwd_link;
                }
        }
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
