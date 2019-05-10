#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <chrono>
#include <bitset>
#include "gnuplot.h"

//used in greedy approach
struct Item
{
    int weight = 0;
    int value = 0;
    double itemRatio = 0;
    int index = 0;

};
//Comapre function used by the priority queue for heap based setup
struct CompareRatio {
    bool operator()(Item const& i1, Item const& i2)
    {
        return i1.itemRatio < i2.itemRatio;
    }
};

std::vector<int> createWeightList(std::string file_num);
std::vector<int> createValueList(std::string file_num);
int createCapacityValue(std::string capacity);
std::vector<std::vector<int>> knapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems);
std::vector<std::vector<int>> hashedKnapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems);
std::vector<int> getOptimalSet(std::vector<int> values, std::vector<int> weights, int capacity, std::vector<std::vector<int>> sackTable);
std::vector<int> greedyApproach(int capacity, std::vector<int> values, std::vector<int> weights, int numItems,  double &finalValue, bool useHeap);
int numOfBitsInNum(int num);
int findSlot(int i, int j, int n, int W, int array[], int key);
void set(int i, int j, int n, int W, int array[], int key);
int lookup(int i, int j, int n, int W, int array[], int key);
std::string toBinary(int n, int length);
void createGreedyApproachGraphs(int capacity, std::vector<int> values, std::vector<int> weights);

int main() {

    std::string capacity_file, weights_file, value_file;

    /*
    std::cout << "Enter file containing the capacity: ";
    std::cin >> capacity_file;
    std::cout << "Enter file containing the weights: ";
    std::cin >> weights_file;
    std::cout << "Enter file containing the values: ";
    std::cin >> value_file;
     */

    capacity_file = "p08_c.txt";
    weights_file = "p08_w.txt";
    value_file = "p08_v.txt";

    //parses files to create needed lists of weight and values as well as capacity
    std::vector<int> weights = createWeightList(weights_file);
    std::vector<int> values = createValueList(value_file);
    int capacity = createCapacityValue(capacity_file);

    //testing stuff

    int numItems = weights.size();

    std::cout << "Knapsack capacity = "<< capacity << ". Total number of items = " << weights.size() << "\n\n";

    //clock time to measure speed
    auto start = std::chrono::high_resolution_clock::now();
    //table is stored as vector holding vectors to make passing through function args easier
    //can be treated as a 2d array once created
    std::vector<std::vector<int>> sackTable = knapsack(capacity, values, weights, numItems);
    //create list of optimal items to grab
    std::vector<int> optimalSet = getOptimalSet(values, weights, capacity, sackTable);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Traditional Dynamic Programming Optimal Value: ";
    std::cout << sackTable[numItems][capacity];
    std::cout << std::endl;

    std::cout << "Traditional Dynamic Programming Optimal subset: ";
    //some iterator work to not have to deal with extra comma at end of list
    std::vector<int>::reverse_iterator iter = optimalSet.rbegin();
    std::cout << "{ ";
    std::cout << *iter;
    //sets iter to second element before starting loop
    for(iter++; iter != optimalSet.rend(); iter++)
        std::cout << ", " << *iter;
    std::cout << " }" << std::endl;

    std::cout << "Traditional Dynamic Programming Time Taken : " << duration << " nanoseconds\n";




    //final value is passed by reference through greedy solution
    double finalValue = 0.0;
    //results list of optimal items index
    std::cout << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<int> greedyOptimal = greedyApproach(capacity, values, weights, numItems, finalValue, false);
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Greedy Approach Optimal Value: ";
    std::cout << finalValue;
    std::cout << std::endl;
    //sorts indexes in order to print
    std::cout << "Greedy Approach Optimal Subset: ";
    std::sort(greedyOptimal.begin(), greedyOptimal.end());
    std::vector<int>::iterator greedyIter = greedyOptimal.begin();
    std::cout << "{ ";
    std::cout << *greedyIter;
    for(greedyIter++; greedyIter != greedyOptimal.end(); greedyIter++)
        std::cout << ", " << *greedyIter;
    std::cout << " }" << std::endl;

    std::cout << "Greedy Approach Time Taken: " << duration2 << " nanoseconds\n";

    finalValue = 0.0;
    std::cout << std::endl;
    auto start3 = std::chrono::high_resolution_clock::now();
    greedyOptimal = greedyApproach(capacity, values, weights, numItems, finalValue, true);
    auto end3 = std::chrono::high_resolution_clock::now();
    auto duration3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    std::cout << "Heap-Based Greedy Approach Optimal Value: ";
    std::cout << finalValue;
    std::cout << std::endl;
    //sorts indexes in order to print
    std::cout << "Heap-Based Greedy Approach Optimal Subset: ";
    std::sort(greedyOptimal.begin(), greedyOptimal.end());
    greedyIter = greedyOptimal.begin();
    std::cout << "{ ";
    std::cout << *greedyIter;
    for(greedyIter++; greedyIter != greedyOptimal.end(); greedyIter++)
        std::cout << ", " << *greedyIter;
    std::cout << " }" << std::endl;

    std::cout << "Heap-Based Greedy Approach Time Taken: " << duration3 << " nanoseconds\n";

    createGreedyApproachGraphs(capacity, values, weights);

    return 0;
}

std::vector<std::vector<int>> knapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems)
{
    //creating of "2d array" using vectors
   std::vector<std::vector<int>> sackTable;
   for(int i = 0; i < numItems + 1; i++) {
       sackTable.push_back(std::vector<int>());
       for(int j = 0; j < capactiy + 1; j++) {
           sackTable[i].push_back(0);
       }
   }

   //Build 2d array from bottom up
   for(int i = 0; i <= numItems; i++)
   {
       for(int w = 0; w <= capactiy; w++)
       {
           if( i == 0 || w == 0)
               sackTable[i][w] = 0;
           else if (weights[i-1] <= w)
               sackTable[i][w] = std::max(values[i-1] + sackTable[i-1][w-weights[i-1]], sackTable[i-1][w]);
           else
               sackTable[i][w] = sackTable[i-1][w];
       }
   }
    //return final vector array
    return sackTable;
}

std::vector<std::vector<int>> hashedKnapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems)
{
    /*
    //creating the hash table
    int hashtable[2*weights.size()];
    //Build 2d array from bottom up
    for(int i = 0; i <= numItems; i++)
    {
        for(int w = 0; w <= capactiy; w++)
        {
            if( i == 0 || w == 0)
                set(i,w,numItems,capactiy,hashtable,0);

            else if (weights[i-1] <= w)
                sackTable[i][w] = std::max(values[i-1] + sackTable[i-1][w-weights[i-1]], sackTable[i-1][w]);
            else
                sackTable[i][w] = sackTable[i-1][w];
        }
    }
    //return final vector array
    return sackTable;
     */
}

std::vector<int> createWeightList(std::string file_num)
{
    //opens file based off of given which file asked for
    //parses file line by line and inputs to vector
    //PS: I really like vectors
    std::vector<int> returnWeights;
    std::ifstream file;
    file.open(file_num);
    if( ! file.is_open())
    {
        std::cout << "Error: input fine not found.\n";
        exit(1);
    }
    int tempNum;
    while( file >> tempNum )
    {
        returnWeights.push_back(tempNum);
    }
    file.close();

    return returnWeights;

}

std::vector<int> createValueList(std::string file_num)
{
    //opens file based off of given which file asked for
    //parses file line by line and inputs to vector
    std::vector<int> returnValues;
    std::ifstream file;
    file.open(file_num);
    if( ! file.is_open())
    {
        std::cout << "Error: input fine not found.\n";
        exit(1);
    }
    int tempNum;
    while( file >> tempNum )
    {
        returnValues.push_back(tempNum);
    }
    file.close();

    return returnValues;
}

int createCapacityValue(std::string capacity)
{
    //Same as other create functions but only needs the single line in file
    std::ifstream file;
    file.open(capacity);
    if( ! file.is_open())
    {
        std::cout << "Error: input fine not found.\n";
        exit(1);
    }
    int tempNum;
    file >> tempNum;
    file.close();

    return tempNum;
}

std::vector<int> getOptimalSet(std::vector<int> values, std::vector<int> weights, int capacity, std::vector<std::vector<int>> sackTable)
{

    //finds the optimal set of items to choose
    //returns a vector of those items number in list
    std::vector<int> solutionSet;
    int i = weights.size();
    while (i > 0 && capacity > 0)
    {
        if(sackTable[i][capacity] != sackTable[i-1][capacity])
        {
            //this can be changed to solutionSet.push_back(values[i - 1])
            // if you want the items values instead of # in list
            solutionSet.push_back(i);
            capacity -= weights[i-1];
        }
        i--;
    }
    return solutionSet;
}

bool compareByRatio(Item a, Item b)
{
    //simple compare function to be used to sort the struct in greedy approach
    return a.itemRatio > b.itemRatio;
}

std::vector<int> greedyApproach(int capacity, std::vector<int> values, std::vector<int> weights, int numItems, double &finalValue, bool useHeap)
{
    //this could probably be done more efficiently but  ¯\_(ツ)_/¯
    std::vector<Item> itemsList;
    //stores struct Item in a vector holding the index and value/weight ratio
    for(int i = 0; i < values.size(); i++)
    {
        Item item;
        item.index = i +1;
        item.itemRatio = (double)values[i]/weights[i];
        item.weight = weights[i];
        item.value = values[i];
        itemsList.push_back(item);
    }
    //sort the list of item.
    //if useHeap is set to true, a heap will be used
    //otherwise a built in sort function is used
    if( ! useHeap ) {
        std::sort(itemsList.begin(), itemsList.end(), compareByRatio);

        //adds the items that can fit into knapsack up until the capacity is reached
        //also keeps a running total of the values available
        std::vector<int> selectedList;
        //selectedList is given the index of optimal item set
        int curWeight = 0;
        for (int i = 0; i < numItems; i++) {
            if (curWeight + itemsList[i].weight <= capacity) {
                curWeight += itemsList[i].weight;
                finalValue += itemsList[i].value;
                selectedList.push_back(itemsList[i].index);
            } else
                break;
        }

        return selectedList;
    }

    else
    {
        //A priority queue is used, which has the runtime of a heap, as far as what i found online says
        std::priority_queue<Item, std::vector<Item>, CompareRatio> heap;
        while( ! itemsList.empty())
        {
            heap.push(itemsList.back());
            itemsList.pop_back();
        }
        //once the queue is created, the comparison is run very similar to the previous one
        std::vector<int>selectedList;
        int curWeight = 0;
        while( ! heap.empty())
        {
            if(curWeight + heap.top().weight <= capacity)
            {
                curWeight += heap.top().weight;
                finalValue += heap.top().value;
                selectedList.push_back(heap.top().index);
                heap.pop();
            }
            else
                break;
        }
        return selectedList;
    }

}

int numOfBitsInNum(int num)
{
    return ceil( log2 ( num + 1 ) );
}

std::string toBinary(int n, int length)
{
    std::string r;
    while(n != 0)
    {
        r = (n%2 == 0 ? "0":"1") + r;
        n/=2;
    }
    while(r.length() != length)
    {
        r = "0" + r;
    }
    return r;
}


int findSlot(int i, int j, int n, int W, int array[], int key)
{
    int const numBits1 = numOfBitsInNum(n);
    int const numBits2 = numOfBitsInNum(W);
    std::string binary1 = toBinary(i, numBits1);
    std::string binary2 = toBinary(j, numBits2);
    std::string r = "1" + binary1 + binary2;
    int slotNum = ( std::stoi(r) % 2*W);
    while(array[slotNum] != -1 && array[slotNum] != key)
        slotNum = (slotNum + 1) % 2*W;
    return slotNum;
}

int lookup(int i, int j, int n, int W, int array[], int key)
{
    int slotNum = findSlot(i, j, n, W, array, key);
    if(array[slotNum] != -1)
        return array[slotNum];
    else
        return -1;
}

void set(int i, int j, int n, int W, int array[], int key)
{
    int slotNum = findSlot(i, j, n, W, array, key);
    if(array[slotNum] != -1)
    {
        array[slotNum] = key;
        return;
    }
    else
        std::cout << "hash table is full\n" << std::endl;
        return;
}

void createGreedyApproachGraphs(int capacity, std::vector<int> values, std::vector<int> weights)
{
    std::ofstream outputFile;
    outputFile.open("greedy_nums.R");
    outputFile << "number_of_items built_in_time heap_time\n";
    double finalVal;
    for(int i = 0; i < values.size(); i++)
    {
        finalVal = 0;
        outputFile << i << " ";
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        greedyApproach(capacity, values,weights,i,finalVal, false);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        outputFile << duration << " ";
        start = std::chrono::high_resolution_clock::now();
        greedyApproach(capacity, values,weights,i,finalVal, true);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        outputFile << duration << std::endl;
    }
    outputFile.close();

    gnuplot p;
    p("set term png");
    p("set output 'Greedy_Approach_Graph.png' ");
    p("set title \"Greedy Approach\" ");
    p("set xlabel \"Number of Items\" ");
    p("set ylabel \"Runtime\" ");
    p("set style line 1 lt 1 lw 3 pt 3 lc rgb \"red\" pi -1 ps 1.5");
    p("set style line 2 lt 1 lw 3 pt 3 lc rgb \"blue\" pi -1 ps 1.5");
    p("set pointintervalbox 3");
    p("plot \'./greedy_nums.R\' u 1:2 with linespoints title \'No Heap\', \'./greedy_nums.R\' u 1:3 with linespoints title \'With Heap\'");

}