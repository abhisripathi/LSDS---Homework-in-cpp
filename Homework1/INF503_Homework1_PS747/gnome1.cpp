#include <iostream>
#include <string.h>
#include <fstream>

#include <unistd.h>
#include <time.h>
#include "gnome1.h"



using namespace std;

// function to read the data from file
bool GNOME::readData(const char *filepath)
{
    ifstream gnomefile(filepath); // opening file
    string currLine;
    if (!gnomefile.is_open()) // check if file is opened successfully or not
    {
        cout << "err: unable to open the file";
        return FAILED;
    }
    while (getline(gnomefile, currLine)) // read each line from file
    {
        if (!currLine.size()) // check to skip the empty lines
            continue;
        if (data_size + currLine.size() < max_size) // if condition to check that the array size exceeds the maximum size
        {
            if (currLine[0] == '>') // condition to check if the line contains the scaffold
            {
                scaff++; // increment the scaffold count
                currLine.push_back('\0'); //to ensure the boundary of the scaffold header
            }
            memcpy(data + data_size, currLine.c_str(), currLine.size()); //copy the line data to the data array
            data_size += currLine.size(); // update the size of data_size
        }
    }
    // for(int i=0;i<data_size;++i){
    //     cout<<data[i];
    // }

    cout << "\nData read successfully: " << data_size;
    return SUCCESS;
}

// function to compute the ACGT frequencise
bool GNOME::computeACGT(unsigned long long buffer[5])
{
    unsigned long long A = 0, C = 0, G = 0, T = 0, O = 0;

    if (data == nullptr) // condition to check if the data array is empty or not
    {
        cout << "err: no gnome data found";
        return FAILED;
    }
    //find the frequency ACGT in the data array
    for (unsigned long long i = 0; i < data_size; i++)
    {
        switch (data[i])
        {
        case 'A':
            ++A;
            break;
        case 'C':
            ++C;
            break;
        case 'G':
            ++G;
            break;
        case 'T':
            ++T;
            break;
        default:
            O++;
        }
    }
    buffer[0] = A;
    buffer[1] = C;
    buffer[2] = G;
    buffer[3] = T;
    buffer[4] = O;
    return SUCCESS;
}

// Function to answer question 1

bool GNOME::question1()
{
    if (data == nullptr) // condition to check if the data array is empty or not
    {
        cout << "err: no gnome data found";
        return FAILED;
    }
    unsigned long long total_scaffolds = 0;
    unsigned long long all_scaffold_size = 0;
    unsigned long long curr_scaffold_size = 0;
    string curr_scaffold_name = "";
    unsigned long long first_large_size = 0, second_large_size = 0;
    string first_large_scaffold = "", second_large_scaffold = "";
    for (unsigned long long i = 0; i < data_size; ++i) // loop through the data array
    {
        if (data[i] == '>') // condition to check if scaffold header is found
        {
            // cout << "scaffold found";
            //for every new scaffold checking if its largest scaffold
            if (curr_scaffold_size > first_large_size) // condition to check if current scaffold is largest
            {
                swap(first_large_scaffold, second_large_scaffold); // swap first large and second large scaffold
                swap(first_large_size, second_large_size); // swap first and second large size
                first_large_scaffold = curr_scaffold_name; // assign current scaffold name to first large scaffold 
                first_large_size = curr_scaffold_size; // assign current scaffold size to first large size
            }
            else if (curr_scaffold_size > second_large_size) // Condition to Check if current scaffold is second largest
            {
                second_large_scaffold = curr_scaffold_name; // assign current scaffold name to second largest scaffold
                second_large_size = curr_scaffold_size; // assign the current scaffold size to the second largest size
            }
            all_scaffold_size += curr_scaffold_size; // add the current scaffold size to the all scaffold size
            total_scaffolds++; // increment the total scaffolds variable
            curr_scaffold_size = 0; // make the current scaffold size to zero
            curr_scaffold_name = ""; // make the current scaffold name to empty string
            ++i; // pre increment the i variable
            // while (i < data_size && (isdigit(data[i]) || data[i] == '-'))
            while (i < data_size && (data[i] != '\0')) // while loop to read the scaffold name
            {
                curr_scaffold_name.push_back(data[i]);
                ++i; // pre increment the i variable
            }
            --i; // pre decrement the i variable
        }
        else 
        {
            //total characters in current scaffold
            curr_scaffold_size++;
        }
    }
    // Condition to Check if the current scaffold is the largest or second largest
    if (curr_scaffold_size > first_large_size)
    {
        swap(first_large_scaffold, second_large_scaffold);
        swap(first_large_size, second_large_size);
        first_large_scaffold = curr_scaffold_name;
        first_large_size = curr_scaffold_size;
    }
    else if (curr_scaffold_size > second_large_size)
    {
        second_large_scaffold = curr_scaffold_name;
        second_large_size = curr_scaffold_size;
    }
    all_scaffold_size += curr_scaffold_size;
    // cout<< "\nall_scaffold_size: " << all_scaffold_size <<endl;
    // cout<< "Data_Size: " << data_size <<endl;
    // cout<< "Scaff value is: " << scaff << endl;
    cout << "\nTotal scaffolds: " << total_scaffolds << endl;
    cout << "First longest scaffold: " << first_large_scaffold << ", size: " << first_large_size << endl;
    cout << "Second longest scaffold: " << second_large_scaffold << ", size: " << second_large_size << endl;
    cout << "Average scaffold length: " << all_scaffold_size / total_scaffolds << endl;
    return SUCCESS;
}

int main(int argn, char *args[])
{
    if (argn < 3) // condition to check if correct number of command-line arguments are provided
    {
        cout << "err: Invalid Arguments"; // print invalid arguments message to the console
        return EXIT_FAILURE; // return the failure
    }
    char *question_no = args[1]; // get the question number from command-line arguments
    char *filepath = args[2]; // // get the file path from command-line arguments
    unsigned long long size = get_size(args[2]); //getting filesize for the given file from filesystem and later used as size of char array
    // cout<<size<<endl;
    GNOME gnome(size); // creating the gnome object
    //creating a object to operate on gnome data an array with size of file size is allocated on creation
    gnome.readData(filepath); // read the data from the file
    if (*question_no == '1') // condition to check if question 1 is requested
    {
        gnome.question1();
    }
    else if (*question_no == '2') // condition to check if question 2 is requested
    {
        unsigned long long buffer[5];
        clock_t start = clock();
        gnome.computeACGT(buffer);
        clock_t end = clock();
        cout << "\nTime taken: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        //buffer[0] : A, buffer[2] : C, buffer[2] : G, buffer[3] : T 
        unsigned long long total = (buffer[0] + buffer[1] + buffer[2] + buffer[3]);
        double GC_content = (buffer[1] + buffer[2]);
        // cout << "Total: " << total << endl;
        // cout<< "GC_content: " << GC_content << endl;
        // cout<< "Buffer 4: " << buffer[4]<<endl;
        // cout<< "The percentage of C's in scaffold is: " << (buffer[1] * 100)/total << endl;
        // cout<< "The percentage of G's in scaffold is: " << (buffer[2] * 100)/total << endl;
        cout << "The percentage(%) of GC content is: " << (GC_content / total) * 100 << "\%" << endl;
    }
    else
        cout << "\nerr: Invalid Question Number";
    return EXIT_SUCCESS;
}
