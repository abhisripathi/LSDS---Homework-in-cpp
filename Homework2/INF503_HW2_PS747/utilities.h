#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <array>
#define SUCCESS true // Defining the macros for success and failure indicators
#define FAILED false

using namespace std;

// given the path of the file returns the size(in bytes) of the file from the file properties
unsigned long long get_size(const char *filepath)
{
    struct stat fstatus; //declaring the stat struct to store the file status
    stat(filepath, &fstatus); //storing the file status and store it in fstatus
    return fstatus.st_size; //returning the size of the file from file status
}
void merge(long long int start, long long int mid, long long int end, vector<array<char, 32>> &queries)
{
    int temp = -1;
    long long int i, j, indexer;
    if (end - start < 2)
    {
        while ((++temp) < 32 && queries[start][temp] == queries[end][temp])
            ;
        if (temp == 32)
            return;
        else if (queries[start][temp] > queries[end][temp])
        {
            temp = -1;
            while ((++temp) < 32)
                swap(queries[start][temp], queries[end][temp]);
            return;
        }
    }
    vector<array<char, 32>> left, right;
    left.reserve(mid - start + 1);
    right.reserve(end - mid);
    for (i = start; i <= mid; ++i)
        left.emplace_back(queries[i]);
    for (i = mid + 1; i <= end; ++i)
        right.emplace_back(queries[i]);

    i = 0;
    j = 0;
    indexer = start;
    while (i < left.size() && j < right.size())
    {
        temp = -1;
        while ((++temp) < 32 && left[i][temp] == right[j][temp])
            ;
        if (temp == 32 || left[i][temp] < right[j][temp])
        {
            temp = -1;
            while ((++temp) < 32)
                queries[indexer][temp] = left[i][temp];
            ++i;
        }
        else
        {
            temp = -1;
            while ((++temp) < 32)
                queries[indexer][temp] = right[j][temp];
            ++j;
        }
        ++indexer;
    }
    while (i < left.size())
    {
        temp = -1;
        while ((++temp) < 32)
            queries[indexer][temp] = left[i][temp];
        ++i;
        ++indexer;
    }
    while (j < right.size())
    {
        temp = -1;
        while ((++temp) < 32)
            queries[indexer][temp] = right[j][temp];
        ++j;
        ++indexer;
    }
}

void merge_sort(long long int start, long long int end, vector<array<char, 32>> &queries)
{
    if (start < end)
    {
        long long int mid = (end - start) / 2 + start;
        merge_sort(start, mid, queries);
        merge_sort(mid + 1, end, queries);
        merge(start, mid, end, queries);
    }
}
class Queries_AR
{
public:
    vector<array<char,32>> queries; // Vector for query storage, with each query being a 32-character array
    long long int size; //Declaring the variable to store the size

    Queries_AR() // Default Constructor
    {
        
    }
    Queries_AR(char *path) //Custom Constructor that takes file path as input and retrieves queries from a text document

    {
        long long int fileSize = get_size(path); // getting the size of the file
        cout<<"queries filesize:"<<fileSize<<endl; // printing the size of the file
        // fflush(stdout);
        queries.reserve(fileSize/45); //Reserving the space in the vector by file size & expected entry length
        // cout<<"check1 \n";
        readDataSet(path);
    }
    void readDataSet(char* path){
        ifstream queryFile(path); // opening the file with given path
        int fileSize;
        string currLine; //Declaring the string currLine to store each line read from the file
        if (!queryFile.is_open()) 
        cout << "err: unable to open the file"; //print the error if the file cannot be opened
        bool val=false; //Declaring the boolean variable to alternate reading lines
        // cout<<"check2 \n";
        while (getline(queryFile, currLine)){ // while loop to read the lines from the file
            val=!val; 
            if(val)
            continue;
            array<char,32> arr; // array to store the current query
            fileSize=33;
            while(--fileSize){
                arr[32-fileSize]=currLine[32-fileSize]; //Copying the chars from currLine to array
            }
            queries.emplace_back(arr); //Adding query to vector of queries
        }
        queries.shrink_to_fit(); //Shrink the vector to fit the actual number of elements
        // cout<<"check3 \n";
        fflush(stdout);
        cout << "\nqueries read successfully: " << queries.size();// Printing the no:of queries read
        queryFile.close();// Closing the file
    }
    bool linear_search(char *c){ // Linear search for a query
        int temp;
        for(int i=0;i<queries.size();++i){ // Iterating through the queries
            temp=-1;
            while((++temp)<32&&queries[i][temp]==c[temp]); // Comparing the each character of the query
            if(temp==32)
            return true; // Returning true if all characters match
        }
        return false; // Returning false if no match is found
    }
    bool binary_search(char *c){ // Binary search for a query in the vector
        long long int start=0,end=queries.size();
        int temp; //declaring the temp variable
        while(start<=end){
            long long int mid= (end-start)/2+start; // calculating the middle index
            temp=-1; // initializing the temp to -1
            while((++temp)<32&&queries[mid][temp]==c[temp]); // while loop to compare each character of the query
            if(temp==32) // if statement to return true if all characters match
            return true;
            else if(queries[mid][temp]<c[temp]) // Move to the right half
            start=mid+1;
            else // Move to the left half
            end=mid-1;
        }
        return false; //return false if no match is found
    }
    void sortQueries(){
        // cout<<"before sort"<<endl;
        // for(int i=0;i<10;++i){
        //     cout<<string(queries[i].begin(),queries[i].end())<<endl;
        // }
        //sort(queries.begin(),queries.end());
        // cout<<"after sort"<<endl;
        // for(int i=0;i<10;++i){
        //     cout<<string(queries[i].begin(),queries[i].end())<<endl;
        // }
        merge_sort(0, queries.size() - 1, queries);
    }
    ~Queries_AR(){}
};

class GNOME
{
    public:
    char *data = nullptr; // data pointer to store the gnome data
    long long int size = 0; // size variable to store the total size of the data
    GNOME(char *path) // Constructor that reads gnome data from a file
    {
        unsigned long long fileSize = get_size(path); // Getting size of the file and storing it in filesize
        cout<<"\ngnome size"<< fileSize<<endl; // Printing the size of the file
        fflush(stdout); // Flush the output buffer
        data = (char *)malloc(fileSize); // Allocating memory for the gnome data
        ifstream queryFile(path); // Open the file with the given path
        string currLine; // declaring string to store the each line read from the file
        currLine.reserve(100); // reversing the space for 100 characters 
        if (!queryFile.is_open())  // condition to check if the file failed to open 
            cout << "err: unable to open the file"; // print error message
        while (getline(queryFile, currLine)) //reading the file line by line until end of the file
        {
            if (currLine[0] != '>'){ // condition to check is the line doesnot start with >
                memcpy(data + size, currLine.c_str(), currLine.size()); //copying the current line content
                size+=currLine.size(); // incrementing the size 
            }
        }
        cout<<"total chars read from gnome file:"<<size<<endl; // printing the total chars read from the gnome file
        queryFile.close(); // closing the file
    } 
    ~GNOME() // destructor to free the allocated memory
    {
        free(data); // free the allocated memory
    }
};