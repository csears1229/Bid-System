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
// Binary Search Tree Class Definition
//============================================================================

/**
 * Class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    struct Node {
        Bid bid;
        Node* left;
        Node* right;
        Node() {
            left = nullptr;
            right = nullptr;
        }
        Node(Bid abid) {
            this->bid = abid;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    Node* root;
    Node* tree;
    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
    bool Size();
    static int loadBids(string, BinarySearchTree*);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    cout << root << endl;
    inOrder(root);
}
/**
 * Insert a bid to the Binary Search Tree
 * @param bid the bid to be inserted
 */
void BinarySearchTree::Insert(Bid bid) {
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        addNode(root, bid);
    }
}

/**
 * Remove a bid from the Binary Search Tree
 * @param bidId the id of the bid to be removed
 */
void BinarySearchTree::Remove(string bidId) {
    Node *par, *cur, *suc;
    par = root;
    while (par != nullptr) {
        // if current bid is bid we are searching for
        if (par->bid.bidId == bidId) {
            // if the bid has no child to the left or right delete the bid
            if (!par->left && !par->right){
                delete par;
            }
            // if the bid has a left child but no right child delete the bid
            // and make the parent bid point to the left bid
            else if (par->left && !par->right) {
                cur = par->left;
                delete par;
            }
            // if the bid has a right child but no left child delete the bid
            // and make the parent bid point to the right child
            else if (!par->left && par->right) {
                cur = par->right;
                delete par;
            }
            // if the bid has a right and left child restructure the branches so
            // that they can still properly be searched
            else {
                suc = par->left;
                while (suc->left) {
                    suc = suc->left;
                }
                par->bid = suc->bid;
                delete suc;
                return;
            }
        }
        // if bid id we are seaching for is greater than current go to right branch
        else if (par->bid.bidId < bidId) {
            par = par->right;
        }
        // if id we are searching for is less than the current bid id go to left branch
        else {
            par = par->left;
        }
    }
}


/**
 * Search for a bid and return the bid
 * @param bidId the id of the bid we want to find
 */
Bid BinarySearchTree::Search(string bidId) {
    Bid bid;
    Node *temp;
    temp = root;
    while (temp->bid.bidId != bid.bidId) {
        if (temp->bid.bidId == bidId) {
            bid = temp->bid;
            return bid;
        }
        if (temp->bid.bidId > bidId) {
            if (temp->left == nullptr) {
                return bid;
            }
            else {
                temp = temp->left;
                continue;
            }
        }
        else {
            if (temp->right == nullptr) {
                return bid;
            }
            else {
                temp = temp->right;
                continue;
            }
        }
    }
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    if (bid.bidId >= node->bid.bidId) {
        if (node->right == nullptr) {
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid);
        }
    }
    else {
        if (node->left == nullptr) {
            node->left = new Node(bid);
        }
        else {
            addNode(node->left, bid);
        }
    }
}

/**
 * (Recursive) Method to print a simplified version of all bids in the
 * binary search tree by doing an in order traversal
 * @param node current node in the tree
 */
void BinarySearchTree::inOrder(Node* node) {
    if (!node) return;
    inOrder(node->left);
    cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | " << node->bid.fund << endl;
    inOrder(node->right);
}

/**
 * Load the bids from the CSV to the binary search tree
 * @param csvPath path to the CSV file
 * @param bst the binary search tree that the bids will be put in
 * @return the number of bids added to the tree
 */
int BinarySearchTree::loadBids(string csvPath, BinarySearchTree* bst) {
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

            //cout << "Item: " << bid.bidId << ":" << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            numBids++;
            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        cerr << e.what() << endl;
    }
    return numBids;
}

/**
 * Method to check if the Binary Search Tree has been filled with bids yet
 * @return true if the tree is empty false if the bids have been loaded
 */
bool BinarySearchTree::Size(){
    if (root == nullptr){
        return true;
    }
    else {
        return false;
    }
}

