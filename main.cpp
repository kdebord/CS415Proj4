#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<int> createWeightList(char weights);
std::vector<int> createValueList(char values);
int createCapacityValue(char capacity);
int knapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems);

int main() {
    std::cout << "Enter which test set to run on (0 or 1): " << std::endl;
    char numSelection;
    std::cin >> numSelection;
    std::vector<int> weights = createWeightList(numSelection);
    std::vector<int> values = createValueList(numSelection);
    int capacity = createCapacityValue(numSelection);
    int numItems = 0;
    while(numItems <= weights.size()) {
        std::cout << "Knapsack Solution with " << numItems << " items: "
                  << knapsack(capacity, values, weights, numItems) << std::endl;
        numItems++;
    }
        return 0;
}

int knapsack(int capactiy, std::vector<int> values, std::vector<int> weights, int numItems)
{
   int i, w;
   int sackTable[numItems+1][capactiy+1];

   //Build 2d array from bottom up
   for(i = 0; i <= numItems; i++)
   {
       for( w = 0; w <= capactiy; w++)
       {
           if( i == 0 || w == 0)
               sackTable[i][w] = 0;
           else if (weights[i-1] <= w)
               sackTable[i][w] = std::max(values[i-1] + sackTable[i-1][w-weights[i-1]], sackTable[i-1][w]);
           else
               sackTable[i][w] = sackTable[i-1][w];
       }
   }
    //return final entry in 2d array
    return sackTable[numItems][capactiy];
}

std::vector<int> createWeightList(char weights)
{
    std::vector<int> returnWeights;
    std::string inputFile = "p00_w.txt";
    inputFile[2] = weights;
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

std::vector<int> createValueList(char values)
{
    std::vector<int> returnValues;
    std::string inputFile = "p00_v.txt";
    inputFile[2] = values;
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
    std::string inputFile = "p00_c.txt";
    inputFile[2] = capacity;
    std::ifstream file;
    file.open(inputFile);
    int tempNum;
    file >> tempNum;
    file.close();
    return tempNum;
}