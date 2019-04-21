//============================================================================
// Name        : Data Structures and Algorithms
// Author      : Carson Sears
// Version     : 1.0
// Description : Application showing popular data structures
//                and algorithms using C++
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>
#include <climits>
#include <vector>
#include <string>
#include <iomanip>

#include "CSVparser.hpp"
#include "LinkedList.cpp"
#include "HashTable.cpp"
#include "VectorSort.cpp"
#include "BinarySearchTree.cpp"

using namespace std;

//=================================================
//  GLOBAL VARIABLES
//=================================================
string csvPath = "eBid_Monthly_Sales.csv";
clock_t ticks;
string bidKey;


 /**
 * Display the bid information for a single bid
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << left;
    cout << "\n   Id:  " << setw(30) << "Bid Title  " << "| Winning Bid | Receipt #  "
            "| Date Paid  | Net Profit | Fund\n";
    cout << bid.bidId << ": " << setw(30) << bid.title << " | $" << setw(10) << bid.amount << " | "
         << bid.receiptNumber << " | " << bid.datePaid << " | $" << setw(9) << bid.netSales
         << " | " << bid.fund << endl;
}

/**
 * Prompt user for bid information to insert a new bid
 * to the data structure
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter receipt number: ";
    cin >> bid.receiptNumber;

    cout << "Enter date paid: ";
    cin >> bid.datePaid;

    cout << "Enter net sales: ";
    cin >> bid.netSales;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, bid.title);

    cout << "Enter fund: ";
    getline(cin, bid.fund);

    cout << "Enter amount: ";
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Method used to format the printing of the time to show performance
 * for each data structure used
 * @param ticks number of clock ticks it took to perform an action
 */
void printTime(clock_t ticks) {
    cout << "time: " << ticks << " clock ticks" << endl;
    cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds\n" << endl;
}

/**
 * Method used to show menu options and messages for the sort operations
 * in this application
 */
void sortMenu() {
    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Message explaining the different opperations
    cout << "\nThese operations will sort the bids by title\n";
    cout << "\nSelection Sort: Average performance: O(n^2))\n"
            "                Worst case performance O(n^2))\n"
            "Quick Sort:     Average performance: O(n log(n))\n"
            "                Worst case performance O(n^2))\n\n";
    cout << "Please select an option from the menu\n"
            "Performance will be displayed in clock ticks and seconds\n";

    // Menu and switch for handing menu options
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Selection Sort All Bids" << endl;
        cout << "  2. Quick Sort All Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  9. Return to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // if input is not int cin will fail and loop will runoff.
        // if we have bad input it will clear cin to allow for input again
        if (cin.fail()) {
            // get rid of failure state
            cin.clear();

            // discard 'bad' character(s)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {

            // Selection sort all bids
            case 1:
                // Clear the bids vector to begin the selection sort
                bids.clear();

                // Method call to load the bids to the vector to be sorted
                bids = VectorSort::loadBids(csvPath);

                // Print the number of bids loaded to the vector
                cout << "\n" << bids.size() << " bids ready to be sorted" << endl;

                // Message to show user that the bids are sorting
                cout << "Sorting Bids\n" << endl;

                // Initialize clock to get starting number of clock ticks
                ticks = clock();

                // Call selectionSort function to sort the bids
                VectorSort::selectionSort(bids);

                // Output to show number of bids sorted, improves readability
                cout << bids.size() << " Bids sorted" << endl;

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                printTime(ticks); // Method formats the time output
                break;

            // Quick sort all bids
            case 2:
                // Clear the bids vector to begine the quick sort
                bids.clear();

                // Method call to load the bids to the vector to be sorted
                bids = VectorSort::loadBids(csvPath);

                // Print the number of bids loaded to the vector
                cout << "\n" << bids.size() << " bids ready to be sorted" << endl;

                // Message to show user that the bids are sorting
                cout << "Sorting Bids\n" << endl;

                // Initialize clock to get starting number of clock ticks
                ticks = clock();

                // call quickSort function to sort the bids
                VectorSort::quickSort(bids, 0, bids.size() - 1);

                // Output to show number of bids sorted, improves readability
                cout << bids.size() << " Bids sorted" << endl;

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                printTime(ticks); // Method formats the time output
                break;

            // Display all the bids
            case 3:
                // Loop and display the bids read
                for (int i = 0; i < bids.size(); ++i) {
                    displayBid(bids[i]);
                }
                cout << endl;
                break;

            // Return to Main Menu found in main()
            case 9:
                bids.clear();
                break;

            // Default case to catch bad input
            default:
                cout << "!! Invalid Input Please Try Again !!" << endl;
                break;
        }
    }
}
/**
 * Method used for the menu of search and insert methods for the different
 * data structures used in the application
 */
void searchMenu() {
    // Variables used for the different data structures
    LinkedList bidList;
    BinarySearchTree* bst = new BinarySearchTree();
    HashTable* bidTable = new HashTable();
    Bid bid;

    // Message explaining different options and data structures in the application
    cout << "\nThese operations will allow you to search the bids by Bid Id\n"
            "or add a bid to the data structure to measure performance of the operations.\n\n";
    cout << "Linked List (Unsorted): Insert O(1)\n"
            "                        Search O(N)\n";
    cout << "Binary Search Tree:     Insert O(Log N)\n"
            "                        Search O(Log N)\n";
    cout << "Hash Table:             Insert O(1)\n"
            "                        Search O(1)\n";
    cout << "\nPlease select an option from the menu\n"
            "Performance will be displayed in clock ticks and seconds\n";

    // Menu and switch used for the search data structures
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Linked List - Search" << endl;
        cout << "  2. Linked List - Append Bid" << endl;
        cout << "  3. Linked List - Prepend Bid" << endl;
        cout << "  4. Binary Search Tree - Search" << endl;
        cout << "  5. Binary Search Tree - Insert" << endl;
        cout << "  6. Hash Table - Search" << endl;
        cout << "  7. Hash Table - Insert" << endl;
        cout << "  8. Display All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // if input is not int cin will fail and loop will runoff.
        // if we have bad input it will clear cin to allow for input again
        if (cin.fail()) {
            // get rid of failure state
            cin.clear();

            // discard 'bad' character(s)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }


        switch (choice) {

            // Search Linked List data structure
            case 1:
                // if the bids have not been loaded from the csv file we will first load bids
                if (bidList.Size() <= 1) {
                    // Message for user explaining whats going on
                    cout << "\nCreating Linked List" << endl;

                    // Start point for clock ticks to count time
                    ticks = clock();

                    // Method for loading bids to the linked list from the CSV
                    LinkedList::loadBids(csvPath, &bidList);

                    // Show how many bids were loaded to the linked list
                    cout << bidList.Size() << " Bids Loaded to Linked List" << endl;

                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    printTime(ticks); // Method formats the time output
                }

                // User enters Bid Id to search for
                cout << "Enter a Bid Id for search (79519 - 98912): ";
                cin >> bidKey;

                // Start point for clock ticks to count time
                ticks = clock();

                // Search for the bid and return the bid found
                bid = bidList.Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                // If the bid is not found it will give message otherwise it will print the bid
                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                printTime(ticks); // Method formats the time output

                break;

            // Append bid to the end of Linked List
            case 2:
                // Create a new bid from user input to add to the end of Linked List
                bid = getBid();

                // Start point for clock ticks to count time
                ticks = clock();

                // Add bid to end of the List
                bidList.Append(bid);

                // Show the bid that was created
                displayBid(bid);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "Bid Appended to Linked List" << endl;
                printTime(ticks); // Method formats the time output

                break;

            // Prepend bid to front of Linked List
            case 3:
                // Create a new bid from user input to add to the front of Linked List
                bid = getBid();

                // Start point for clock ticks to count time
                ticks = clock();

                // Add bid to the front of the linked list
                bidList.Prepend(bid);

                // Show the bid that was created
                displayBid(bid);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "Bid Prepended to Linked List" << endl;
                printTime(ticks); // Method formats the time output

                break;

            // Search Binary Search Tree
            case 4:
                // If bids have not been loaded from CSV, first load bids
                if (bst->Size()){

                    cout << "\nCreating Binary Search Tree" << endl;

                    // Initialize a timer variable before loading bids
                    ticks = clock();

                    // Complete the method call to load the bids
                    // Method will return the number of bids loaded from the CSV
                    int numBids = BinarySearchTree::loadBids(csvPath, bst);

                    // Calculate elapsed time and display result
                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << numBids << " Bids in Binary Search Tree" << endl;
                    printTime(ticks); // Method formats the time output
                }

                // Allow user to enter bid id they would like to search for
                cout << "Enter a Bid Id for search (79519 - 98912): ";
                cin >> bidKey;

                // Start point for clock ticks to count time
                ticks = clock();

                // Search the Binary Search Tree for the bid
                bid = bst->Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                // If bid is not found message will display otherwise bid will be displayed
                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                printTime(ticks); // Method formats the time output
                break;


            // Insert bid to Binary Search Tree
            case 5:
                // Create a new bid from user input to add to the Binary Search Tree
                bid = getBid();

                // Start point for clock ticks to count time
                ticks = clock();

                // Insert the bid to the Binary Search Tree
                bst->Insert(bid);

                ticks = clock() - ticks;// current clock ticks minus starting clock ticks

                // Print the bid that was created and inserted
                displayBid(bid);
                printTime(ticks); // Method formats the time output

                break;

            // Search the Hash Table
            case 6:
                // If the bids have not been loaded from the CSV we will first load bids
                if (bidTable->Size()){
                    cout << "\nCreating Hash Table" << endl;

                    // Start point for clock ticks to count time
                    ticks = clock();

                    // Load bids from CSV method will return number of bids loaded
                    int numBids = HashTable::loadBids(csvPath, bidTable);

                    ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                    cout << numBids << " Bids in Hash Table" << endl;
                    printTime(ticks); // Method formats the time output
                }

                // Allow user to enter bid id they would like to search for
                cout << "Enter a Bid Id for search (79519 - 98912): ";
                cin >> bidKey;

                // Start point for clock ticks to count time
                ticks = clock();

                // method to search the Hash Table, method will return
                // empty bid if bid is not found
                bid = bidTable->Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                // If bid was not found print message otherwise print the bid
                if (!bid.bidId.empty()) {
                    displayBid(bid);
                } else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                printTime(ticks); // Method formats the time output
                break;

            // Insert bid in Hash Table
            case 7:
                // Create a new bid from user input to add to the Hash Table
                bid = getBid();

                // Start point for clock ticks to count time
                ticks = clock();
                bidTable->Insert(bid);
                ticks = clock() - ticks;
                cout << "Bid Added to Hash Table" << endl;
                printTime(ticks); // Method formats the time output

                break;

            // Print data structures
            case 8:

                /**
                 * Each data structure has their own print function because
                 * each structure is different so we have an extra menu to
                 * pick which print function to call
                 */
                int printOption;
                cout << "How Would You Like to Print the Bids?" << endl;
                cout << "1. Linked List (Unordered)\n2. Binary Search Tree "
                        "(In Order Traversal)\n3. Hash Table" << endl;
                cout << "Selection: ";
                cin >> printOption;

                switch (printOption) {
                    // Print Linked List
                    case 1:
                        bidList.PrintList();
                        break;

                    // Print Binary Search tree
                    case 2:
                        bst->InOrder();
                        break;

                    // Print Hash Table
                    case 3:
                        bidTable->PrintAll();
                        break;

                    default:
                        cout << "!! Invalid, Please Try again" << endl;
                        break;
                }
                break;

            // Return to Main Menu found in main()
            case 9:
                break;

            default:
                cout << "!! Invalid Input Please Try Again !!" << endl;
                break;
        }
    }

}

/**
 * Main Menu for running the application
 *
 */
int main(int argc, char* argv[]) {
    // Welcome message explaining application to others
    cout << "This application takes a CSV holding information for\n "
            "almost 18,000 bids submitted to a municipal government\n"
            "data feed for auctions of property.\n"
            "\nI have created sort algorithms as well as methods for \n"
            "common data structures to perform search and insert functions\n"
            "on the data provided in the CSV file.\n" << endl;


    int choice = 0;
    // Main Menu
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Sort Methods" << endl;
        cout << "  2. Search and Insert Methods" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        // if input is not int cin will fail and loop will runoff.
        // if we have bad input it will clear cin to allow for input again
        if (cin.fail()) {
            // get rid of failure state
            cin.clear();

            // discard 'bad' character(s)
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }


        switch (choice) {
            // Menu for the different sort functions available
            case 1:
                sortMenu();
                break;

            // Menu for the different search and insert functions
            case 2:
                searchMenu();
                break;

            // Exit the application
            case 9:
                break;

            // Default case to catch bad input
            default:
                cout << "!! Invalid Input Please Try Again !!" << endl;
                break;
        }
    }

    cout << "Good bye." << endl;
    return 0;
}
