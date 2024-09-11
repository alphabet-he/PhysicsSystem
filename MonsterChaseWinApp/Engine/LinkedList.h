#pragma once
#include "Node.h"

// Linked list class to
// implement a linked list.
class Linkedlist {

    public:
        Node* head;

        // Default constructor
        Linkedlist() { head = nullptr; }

        // Function to insert a
        // node at the end of the
        // linked list.
        void insertNode(Node* node);

        // Function to print the
        // linked list.
        void printList();

        // Function to delete the
        // node at given position
        void deleteNode(int offset);

        Node* getNode(int offset);

        int getLength();

};



