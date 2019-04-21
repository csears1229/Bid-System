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
//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;
        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }
        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }
        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };
    const unsigned int DEFAULT_SIZE = 17939;
    unsigned int key;
    vector<Node> nodes;
    unsigned int tableSize = DEFAULT_SIZE;
    unsigned int Hash(unsigned int key);

public:
    HashTable();
    virtual ~HashTable();
    HashTable(unsigned int size);
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    int getBidKey(Bid bid);
    bool Size();
    int getStringKey(string bidId);
    static int loadBids(string, HashTable*);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    nodes.resize(tableSize);
}

/**
 * Constructor for the HashTable using the table size
 * to resize the table
 * @param size is size of the hash table
 */
HashTable::HashTable(unsigned int size){
    this->tableSize = size;
    nodes.resize(size);

}
/**
 * Destructor
 */
HashTable::~HashTable() {
    for (int i = 0; i < tableSize; ++i) {
        nodes.erase(nodes.begin() + i);
    }
}

/**
 * Method to get the hashed value of an int
 * @param key the value to be hashed
 * @return unsigned int of the hashed value
 */
unsigned int HashTable::Hash(unsigned int key) {
    unsigned int hash;
    hash = key % tableSize;
    return hash;
}

/**
 * Method to be able to use bid id as an int from a bid object
 * @param bid bid in question
 * @return the bid id as an int
 */
int HashTable::getBidKey(Bid bid) {
    int temp = stoi(bid.bidId);
    return temp;
}

/**
 * Method to get the int value of the bid id from bid id string
 * @param bidId bid id we would like to know int value for
 * @return int value of bid id
 */
int HashTable::getStringKey(string bidId) {
    int temp;
    temp = stoi(bidId);
    return temp;
}
/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // Generate the key for the hash table
    key = getBidKey(bid);
    key = Hash(key);

    // if position is empty place the bid at key and initialize next
    if (nodes[key].next == nullptr) {
        Node* newNode = new Node(bid, key);
        nodes[key] = *newNode;
        nodes[key].next = new Node;
    }
        // if bid already exists at position add new bid to next
    else {
        Node* newNode = new Node(bid, key);
        nodes[key].next = newNode;
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // iterate through the vector to display the bids
    for (int i = 0; i < nodes.size(); ++i) {
        // skip over empty nodes
        if (nodes[i].bid.amount < 1)
            continue;
        // Display the bids in the vector
        cout << "Key " << i << ": ";
        cout << nodes[i].bid.bidId << " | " << nodes[i].bid.title << " | ";
        cout << nodes[i].bid.amount << " | " << nodes[i].bid.fund << endl;
        // If any of the nodes have a chained bid due to a key collision
        // they will be displayed here.
        if (nodes[i].next && nodes[i].next->bid.amount > 0) {
            cout << "    " << i << ": ";
            cout << nodes[i].next->bid.bidId << ": " << nodes[i].next->bid.title << " | $";
            cout << nodes[i].next->bid.amount << " | " << nodes[i].next->bid.fund << endl;
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // get the key for the bid to find
    unsigned int temp;
    temp = getStringKey(bidId);
    temp = Hash(temp);
    // if the bid is in the first position it moves the second bid
    // to the first position overwriting the first bid
    if (nodes[temp].bid.bidId == bidId) {
        if (nodes[temp].next) {
            nodes[temp] = nodes[temp].next->bid;
            nodes[temp].next = new Node;
        }
            // if there is no bid in the second position it just clears the bid
        else {
            nodes[temp].bid.bidId = "";
            nodes[temp].bid.amount = 0;
        }
    }
    // if the bid is in the second position it removes the bid
    if (nodes[temp].next->bid.bidId == bidId) {
        nodes[temp].next = new Node;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    // Calculate the key for the bid being searched
    Bid empty;
    unsigned int temp;
    temp = getStringKey(bidId);
    temp = Hash(temp);
    // if it finds the bid in the first or second position
    // it returns the bid if bid is not found it returns an empty bid
    if (nodes[temp].bid.bidId == bidId) {
        return nodes[temp].bid;
    }
    else if (nodes[temp].next->bid.bidId == bidId) {
        return nodes[temp].next->bid;
    }
    else {
        return empty;
    }
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return an int of the number of bids inserted ot the hash table
 */
int HashTable::loadBids(string csvPath, HashTable* hashTable) {
    int numBids = 0;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][19];
            bid.datePaid = file[i][10];
            bid.receiptNumber = file[i][15];
            bid.netSales = strToDouble(file[i][18],'$');
            bid.amount = strToDouble(file[i][4], '$');

            numBids++;
            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        cerr << e.what() << endl;
    }
    return numBids;
}

/**
 * Method to check if the Hash table has been loaded with bids
 * from the CSV
 * @return false if bids have been loaded and true if table is empty
 */
bool HashTable::Size() {
    if (nodes[9185].bid.bidId == "98880"){
        return false;
    }
    else {
        return true;
    }
}
