#include "LinkedList.h"
#include <iostream>

// Function to delete the
// node at given position
void Linkedlist::deleteNode(int nodeOffset)
{
    Node* toDelete = head;
    int ListLen = getLength();

    // Check if the position to be
    // deleted is greater than the length
    // of the linked list.
    if (ListLen <= nodeOffset || nodeOffset < 0) {
        std::cout << "Index out of range"
            << std::endl;
        return;
    }

    // Deleting the head.
    if (nodeOffset == 0) {

        // Update head
        head = head->next;
        
        free(toDelete->value);
        delete toDelete; // free memory of the node
        return;
    }

    Node* previous = getNode(nodeOffset - 1);
    toDelete = previous->next;
    previous->next = toDelete->next;
    delete toDelete;
   
}

Node* Linkedlist::getNode(int offset) 
{
    Node* temp1 = head;
    int ListLen = getLength();

    // Check if the position to be
    // deleted is greater than the length
    // of the linked list.
    if (ListLen <= offset) {
        std::cout << "Index out of range"
            << std::endl;
        return nullptr;
    }

    // Declare temp1
    temp1 = head;

    // Deleting the head.
    if (offset == 0) {

        return head;
    }

    // Traverse the list to
    // find the node to be deleted.
    while (offset-- > 0) {

        // Update temp1
        temp1 = temp1->next;
    }

    return temp1;

    
}

int Linkedlist::getLength()
{
    Node* temp1 = head;
    int ListLen = 0;

    if (head == nullptr) {
        std::cout << "List empty." << std::endl;
        return 0;
    }

    // Find length of the linked-list.
    while (temp1 != nullptr) {
        temp1 = temp1->next;
        ListLen++;
    }

    return ListLen;
}


// Function to insert a new node.
void Linkedlist::insertNode(Node* node)
{
    // Create the new Node.
    //Node* newNode = new Node(monster);

    // Assign to head
    if (head == nullptr) {
        head = node;
        return;
    }

    // Traverse till end of list
    Node* temp = head;
    while (temp->next != nullptr) {

        // Update temp
        temp = temp->next;
    }

    // Insert at the last.
    temp->next = node;
}



// Function to print the
// nodes of the linked list.
void Linkedlist::printList()
{
    Node* temp = head;

    // Check for empty list.
    if (head == nullptr) {
        printf("List empty");
        return;
    }

    //printf("\n\nMonsters:");
    // Traverse the list.
    while (temp != nullptr) {
        //printf("\n%s is at [%d, %d]",
        //    temp->monster.name,
        //    temp->monster.x,
        //    temp->monster.y);
        temp -> value -> printInfo();
        temp = temp->next;
    }
}

