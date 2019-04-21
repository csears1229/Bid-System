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
// Vector Sorting Class Definition
//============================================================================

class VectorSort {
public:
    static int partition(vector<Bid>&, int, int);
    static void quickSort(vector<Bid>&, int, int);
    static void selectionSort(vector<Bid>&);
    static vector<Bid> loadBids(string);
};

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> VectorSort::loadBids(string csvPath) {

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][19];
            bid.datePaid = file[i][10];
            bid.receiptNumber = file[i][15];
            bid.netSales = strToDouble(file[i][18],'$');
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int VectorSort::partition(vector<Bid>& bids, int begin, int end) {
    // Initialize variables
    int midPoint, low, high;
    bool done = false;
    Bid pivot, temp;

    // pick middle variable as pivot
    midPoint = begin + (end - begin) / 2;
    pivot = bids[midPoint];

    low = begin;
    high = end;

    while (!done) {
        while (bids[low].title < pivot.title) {
            ++low;
        }
        while (pivot.title < bids[high].title) {
            --high;
        }
        if (low >= high) {
            done = true;
        }
        else {
            // Swap values and update variables
            temp = bids[low];
            bids[low] = bids[high];
            bids[high] = temp;

            ++low;
            --high;
        }
    }
    return high;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void VectorSort::quickSort(vector<Bid>& bids, int begin, int end) {
    // Initialize variables
    int j = 0;

    // check to see if list is already sorted
    if (begin >= end) {
        return;
    }

    // Partition the array to being sorting
    j = VectorSort::partition(bids, begin, end);

    // Sort low and high partitions
    quickSort(bids, begin, j);
    quickSort(bids, j + 1, end);

    return;

}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void VectorSort::selectionSort(vector<Bid>& bids) {
    // Initialize variables
    Bid temp;
    int smallest;

    // Iterate through the each of the items in the vector
    for (int i = 0; i < bids.size(); ++i) {
        smallest = i;

        // Find index of smallest value and store it as smallest
        for (int j = i + 1; j < bids.size(); ++j) {
            if (bids[j].title < bids[smallest].title)
                smallest = j;
        }
        // swap variables to sort
        temp = bids[i];
        bids[i] = bids[smallest];
        bids[smallest] = temp;
    }
}
