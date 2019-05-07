#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <math.h>
#include <chrono>

//used in greedy apprach
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
std::vector<int> getOptimalSet(std::vector<int> values, std::vector<int> weights, int capacity, std::vector<std::vector<int>> sackTable);
std::vector<int> greedyApproach(int capacity, std::vector<int> values, std::vector<int> weights, int numItems,  double &finalValue, bool useHeap);
int numOfBitsInNum(int num);

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

    capacity_file = "p01_c.txt";
    weights_file = "p01_w.txt";
    value_file = "p01_v.txt";

    //parses files to create needed lists of weight and values as well as capacity
    std::vector<int> weights = createWeightList(weights_file);
    std::vector<int> values = createValueList(value_file);
    int capacity = createCapacityValue(capacity_file);

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
    std::vector<int> greedyOptimal = greedyApproach(capacity, values, weights, numItems, finalValue, false);
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