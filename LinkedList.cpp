//
// Created by Carson Sears
//

#include <algorithm>
#include <iostream>
#include <time.h>
#include <climits>
#include <vector>
#include <string>

#include "CSVparser.hpp"

using namespace std;

// Structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    string datePaid;
    string receiptNumber;
    double netSales;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

/**
* Simple C function to convert a string to a double
* after stripping out unwanted char
*
* credit: http://stackoverflow.com/a/24875936
*
* @param ch The character to strip out
*/
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

//============================================================================
// Linked List Class Definition
//============================================================================


class LinkedList {

private:
    // Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        Node() {
            next = nullptr;
        }

        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    int size;
    Node *head;
    Node *tail;



public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
    static void loadBids(string, LinkedList*);
};

/**
* Default constructor
*/
LinkedList::LinkedList() {
    // housekeeping variables
    head = NULL;
    tail = NULL;
    size = 0;

}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
}

/**
 * Append a new bid to the end of the list
 * @param bid bid to be inserted at end of linked list
 */
void LinkedList::Append(Bid bid) {
    // append logic
    // Check to see if list is empty
    if (head == NULL)
        head = new Node(bid);
        // Itterate through the list to find the end
    else {
        Node *nodePointer = new Node;
        nodePointer = head;

        while (nodePointer->next != NULL) {
            nodePointer = nodePointer->next;
        }

        // Add new Node to end of list
        nodePointer->next = new Node(bid);
    }
}

/**
 * Prepend a new bid to the start of the list
 * @param bid the bid to be inserted at front of linked list
 */
void LinkedList::Prepend(Bid bid) {
    Node *nodePointer = new Node;
    nodePointer->bid = bid;
    nodePointer->next = head;
    head = nodePointer;
}

/**
 * Simplified output of all bids in the list
 */
void LinkedList::PrintList() {
    Node *nodePointer = head;

    while (nodePointer) {
        cout << nodePointer->bid.bidId << ": ";
        cout << nodePointer->bid.title << " | " << nodePointer->bid.amount << " | " << nodePointer->bid.fund << endl;
        nodePointer = nodePointer->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    Node *nodePointer, *previousNode;
    if (head->bid.bidId == bidId) {
        nodePointer = head;
        head = head->next;
        delete nodePointer;
    }
    else {
        nodePointer = head;
        while (nodePointer != NULL && nodePointer->bid.bidId != bidId) {
            previousNode = nodePointer;
            nodePointer = nodePointer->next;
        }
        if (nodePointer) {
            previousNode->next = nodePointer->next;
            delete nodePointer;
        }
    }


}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    Bid bid;

    Node *nodePointer, *previousNode;
    if (head->bid.bidId == bidId) {
        cout << head->bid.bidId << ": " << head->bid.title << " | ";
        cout << head->bid.amount << " | " << head->bid.fund<< endl;
        return nodePointer->bid;
    }
    else {
        nodePointer = head;
        while (nodePointer != NULL && nodePointer->bid.bidId != bidId) {
            previousNode = nodePointer;
            nodePointer = nodePointer->next;
        }
        if (nodePointer) {
            return nodePointer->bid;
        }
        else {
            return bid;
        }
    }
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    int size = 0;
    Node *nodePointer = new Node;
    nodePointer = head;
    while (nodePointer != NULL) {
        size++;
        nodePointer = nodePointer->next;
    }
    return size;
}



/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void LinkedList::loadBids(string csvPath, LinkedList *list) {
    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][19];
            bid.datePaid = file[i][10];
            bid.receiptNumber = file[i][15];
            bid.netSales = strToDouble(file[i][18],'$');
            bid.amount = strToDouble(file[i][4], '$');

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        cerr << e.what() << endl;
    }
}
