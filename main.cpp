#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<int> createWeightList(char file_num);
std::vector<int> createValueList(char file_num);
int createCapacityValue(char capacity);
std::vector<std::vector<int>> knapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems);
std::vector<int> getOptimalSet(std::vector<int> values, std::vector<int> weights, int capacity, std::vector<std::vector<int>> sackTable);

int main() {
    std::cout << "Enter which test set to run on (0 or 1): " << std::endl;
    char numSelection;
    std::cin >> numSelection;

    //parses files to create needed lists of weight and values as well as capacity
    std::vector<int> weights = createWeightList(numSelection);
    std::vector<int> values = createValueList(numSelection);
    int capacity = createCapacityValue(numSelection);

    int numItems = weights.size();
    //table is stored as vector holding vectors to make passing through function args easier
    //can be treated as a 2d array once created
    std::vector<std::vector<int>> sackTable = knapsack(capacity, values, weights, numItems);
    std::cout << "Knapsack Solution with " << numItems << " items: ";
    std::cout << sackTable[numItems][capacity];
    std::cout << std::endl;

    //create list of optimal items to grab
    std::vector<int> optimalSet = getOptimalSet(values, weights, capacity, sackTable);

    //some iterator work to not have to deal with extra comma at end of list
    std::vector<int>::reverse_iterator iter = optimalSet.rbegin();
    std::cout << "{ ";
    std::cout << *iter;
    //sets iter to second element before starting loop
    for(iter++; iter != optimalSet.rend(); iter++)
        std::cout << ", " << *iter;
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

std::vector<int> createWeightList(char file_num)
{
    //opens file based off of given which file asked for
    //parses file line by line and inputs to vector
    //PS: I really like vectors
    std::vector<int> returnWeights;
    std::string inputFile = "p00_w.txt";
    inputFile[2] = file_num;
    std::ifstream file;
    file.open(inputFile);
    int tempNum;
    while( ! file.eof() )
    {
        file >> tempNum;
        returnWeights.push_back(tempNum);
    }
    file.close();

    return returnWeights;

}

std::vector<int> createValueList(char file_num)
{
    //opens file based off of given which file asked for
    //parses file line by line and inputs to vector
    std::vector<int> returnValues;
    std::string inputFile = "p00_v.txt";
    inputFile[2] = file_num;
    std::ifstream file;
    file.open(inputFile);
    int tempNum;
    while( ! file.eof() )
    {
        file >> tempNum;
        returnValues.push_back(tempNum);
    }
    file.close();

    return returnValues;
}

int createCapacityValue(char capacity)
{
    //Same as other create functions but only needs the single line in file
    std::string inputFile = "p00_c.txt";
    inputFile[2] = capacity;
    std::ifstream file;
    file.open(inputFile);
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