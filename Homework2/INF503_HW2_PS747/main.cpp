#include <time.h> // time library for time code execution
#include<queue>
#include "utilities.h" // including the utilities header file
using namespace std; 

#define TEN_K 1e4
// #define TEN_K 100
#define HUNDRED_K (1e5 - 1e4)
// #define HUNDRED_K 1e3
#define MILLION (1e6 - 1e5)
// #define MILLION 3000

void queue_push(queue<int> &que,int maxi, int data){
    if(que.size()==maxi)
    que.pop();
    que.push(data);
}

void queue_print(queue<int> &que, int maxi, GNOME &subject_data){
    while(maxi--){
        int i=que.front();
        que.pop();
        cout << string(subject_data.data+i,subject_data.data+i+32)<<endl;
        que.push(i);
    }
}

//function to run the search algorithm based on the user choice
void run(char option, Queries_AR &queries, GNOME &subject_data)
{
    bool (Queries_AR::*funcptr)(char *); // pointer to member funciton
    if (option == 'A') // condition for checking the option 'A'
        funcptr = &Queries_AR::linear_search; // setting to linear search if it is option 'A'
    else if (option == 'B')  // condition for checking the option 'B'
        funcptr = &Queries_AR::binary_search; // setting to binary search if it is option 'B'
    else
    {
        cout << "error: wrong option, choose A/B" << endl; // print the error message for the wrong option
        return; // return
    } 
    int indexer = 0; // declare and initializing the indexer variable to zero
    long long int counter = TEN_K, mainCounter = 0; // counter for iterations
    double tenk, hundredk, million, total; // declaring the variables to store the timing results
    queue<int> que;
    clock_t start, end; // variables for the timing

    // if ((queries.*funcptr)(subject_data.data + 48)){
    //     cout<<"--------------------------- found it ---------------------------"<<endl;
    // }
    cout << endl << "Question: " << option << endl;
    start = clock(); // start timing
    while (counter--)
    {
        if ((queries.*funcptr)(subject_data.data + mainCounter)) // condition to check if the query is found
        {
            queue_push(que,20,mainCounter);
        }
        ++mainCounter; //increment the main counter
    } 
    end = clock(); // end the timing
    tenk = (double)(end - start) / CLOCKS_PER_SEC; // calculating the elapsed time
    cout << "time taken for first 10k: " << tenk << " sec"<<endl; //printing the time for the first 10k characters in seconds
    cout << "fragments found in first 10k" << endl; // printing the fragments found for 10k 
    queue_print(que,20,subject_data);
    fflush(stdout); // flush the stdout
    counter = HUNDRED_K; //set the counter for the next 90k chars
    start = clock(); //start timing
    while (counter--) //loop to search queries
    {
        if ((queries.*funcptr)(subject_data.data + mainCounter)) // condition to check if the query is found
        {
            queue_push(que,20,mainCounter);
        }
        ++mainCounter; // increment the main counter
    } 
    end = clock(); // end the timing
    hundredk = (double)(end - start) / CLOCKS_PER_SEC; // calculating the elapsed time
    cout << "time taken for first 100k: " << tenk + hundredk << " sec" <<endl; // printing the time for first 100k chars in seconds
    cout << "fragments found in first 100k" << endl; // printing the fragments found for 100k  
    queue_print(que,20,subject_data);
    fflush(stdout); // flush the stdout
    counter = MILLION; //set the counter for the next 890k chars
    start = clock(); //start timing
    while (counter--) // loop to search for queries in next 890k chars
    {
        if ((queries.*funcptr)(subject_data.data + mainCounter)) // condition to check if the query is found
        {
            queue_push(que,20,mainCounter);
        }
        ++mainCounter; // increment the main counter
    }
    end = clock(); // end the timing
    million = (double)(end - start) / CLOCKS_PER_SEC; // calculating the elapsed time
    cout << "time taken for first 1 million: " << tenk + hundredk + million << " sec"<<endl; // printing the time for first 1M chars in seconds
    cout << "fragments found in first million" << endl; //loop to print the 20 fragments
    queue_print(que,20,subject_data);
    start = clock(); //start timing
    while (mainCounter < subject_data.size - 31) //Loop to search for queries in the remaining of the data
    {
        (queries.*funcptr)(subject_data.data + mainCounter); // search for the query
        ++mainCounter; // increment the maincounter
    }
    end = clock(); // end the timing
    total = (double)(end - start) / CLOCKS_PER_SEC; // Calculate total elapsed time
    cout << "total time taken: " << tenk + hundredk + million + total << " sec" << endl; // Print the total time taken in seconds
}

// main function
int main(int nargs, char *args[])
{
    if (nargs < 4) // condition to check if the correct no:of arguments are provided
    {
        cout << "error: incorrect arguments"; // print the error message
        return 1; // return the error message
    }
    Queries_AR queries(args[2]);
    // cout<<"check5\n";
    fflush(stdout);
    GNOME subject_data(args[3]); //Creating an instance of GNOME with given path
    if(args[1][0]=='B'){ // condition to check if the binary search is selected
        cout<<"started sorting"<<endl; // printing the message
        queries.sortQueries(); // sorting the queries for binary search
        cout<<"Queries Sorted"<<endl; // print the sorted queries
        fflush(stdout); // fflsuh out the stdout
    }
    run(args[1][0],queries,subject_data); // run the search based on the selected option 
    return EXIT_SUCCESS; // returnt the message successfully
}