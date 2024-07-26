#ifndef QUERIES_HT_H
#define QUERIES_HT_H

#include <iostream> // including the iostream library
#include <fstream>// including the fstream library
#include <string>// including the string library
#include <time.h>// including the time library

struct Chain {//structure chain
    std::string data; // string variable data
    Chain* next; // next pointer which is char type
    Chain(std::string val) : data(val), next(nullptr) {}
};

class Queries_HT { // queries_ht class
private: // initialize the private varibles
    int hash_size;
    Chain** hash_table; 
    int collisions = 0;

public: // it has the public methods 
    Queries_HT(int size, const std::string& file_path);//constructor of type class
    void insertion(std::string line);//insertion method
    bool search(std::string line);//function to search
    long long int getRadixValue(std::string line);//function to getting the radix value
    int countOfCollisions();//function to count the collisions
    ~Queries_HT();//destructor 
};

#endif // QUERIES_HT_H

