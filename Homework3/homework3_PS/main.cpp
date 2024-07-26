#include "datastructure.h"//including the header file
using namespace std;

// This is the constructor of the class Queries_HT which initializes the hash table
Queries_HT::Queries_HT(int size, const std::string& file_path) : hash_size(size), hash_table(new Chain*[size]) {
        // Initialize all buckets with nullptr
                    for (int i = 0; i < hash_size; ++i) {//This loop initializes each bucket in the hash table to nullptr.
                        hash_table[i] = nullptr;
                    }

                    // Open the file
                    ifstream file(file_path);
                    if (!file.is_open()) {//condition to check the file is open or not
                        cerr << "Error opening file " << file_path << std::endl;//show error in opening file
                        return;//return 
                    }

                    // Read the query file and insert elements into the hash table
                    string line;//initialize the string variable line
                    while (getline(file, line)) { // loop to get the each line from the file
                        if(line[0]!='>'){
                        insertion(line);
                        }
                    }

                file.close();//close the file
        }
        void Queries_HT::insertion(string line){//This method inserts a string into the hash table.
            long long int hash_value = (long long int)getRadixValue(line) % hash_size;//get the radix value and store in the hash_value variable
            // Create a new node with the line number as data
            hash_value=(hash_value+hash_size)%hash_size;
            Chain* newNode = new Chain(line); // Assuming you want to store 1 for each collision
            // Insert at the beginning of the list
            newNode->next = hash_table[hash_value];//assign new node next with the hash_table value
            if(hash_table[hash_value]!=nullptr){//condition to check the value is not nullptr
                collisions++;//increment the collisions
            }
            hash_table[hash_value] = newNode;
        }
        bool Queries_HT:: search(string line){//This method searches for a string in the hash table.
            //cout<<"search started"<<endl;
            long long int hash_value=getRadixValue(line)%hash_size; //get the radix value and store in the hash_value variable
            hash_value=(hash_value+hash_size)%hash_size;//calculate the hash_value
            Chain* cur=hash_table[hash_value];
            while(cur!=NULL){// loop until curr is not equal to null
                string data = cur->data;//assign the curr data to the data
                if(data.compare(line)==0){//condition to check the data is equals to zero
                    return true;//return if it is true
                }
                cur=cur->next;//update the curr to curr next
            }
            //cout<<"search ended"<<endl;//print the message to console
            return false;//return false
        }
        long long int Queries_HT::getRadixValue(string line){//This method calculates the hash value of the given string using a radix-based hashing algorithm.
            long long int  prime=31;//initialize the prime variable
            long long int power=1;//initialize the power variable
            long long int hash_value=0;//initialize the hash_value variable
            for(auto x:line){//loop until the line
                if(x=='N'){//condition to check the character equals to N
                    x='A';//then assign the char A to x
                }
                hash_value=(hash_value+(x-'A')*power)%hash_size;//calculate the hash value
                power=(power*prime)%hash_size;//calculate the power
            }
            return hash_value;//return hashvalue
        }
int Queries_HT:: countOfCollisions(){
            return collisions;
        }
Queries_HT:: ~Queries_HT() {//This is the destructor of the class Queries_HT
            // Deallocate memory for each chain in the hash table
            for (int i = 0; i < hash_size; ++i) {//loop until the hash size
                Chain* current = hash_table[i];//update the current with each element in hashtable
                while (current != nullptr) {//loop until current is null ptr
                    Chain* temp = current;//assign current to a temp pointer
                    current = current->next;//move curr to curr next
                    delete temp;//remove temp
                }
            }

            // Deallocate memory for the hash table itself
            delete[] hash_table;
        }

//main function where program execution starts
int main(int argc, char *argv[]) {
    // Check if filename is provided as a command-line argument
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    clock_t start1,end1,start4,end4;
    start4=clock();
    Queries_HT Query_class4(200e6,argv[2]);
    end4=clock();
    string part=argv[1];
    if(part.compare("A")==0){
    // PART 1 : PART A
    // hash size 1e6
    cout<<"######################################### SUMMARY OF COLLISIONS AND TIME TAKEN FOR DIFFERENT HASH SIZES ##################################################\n"<<endl;
    start1 = clock();
    Queries_HT Query_class1(1e6,argv[2]);
    end1 = clock();
    cout<<"number of collisions for size 1,000,000 : "<<Query_class1.countOfCollisions()<<endl;
    cout << "Time taken: " << (double)(end1 - start1) / CLOCKS_PER_SEC << "s\n" << endl;
    // hash size 50e6
    start1 = clock();
    Queries_HT Query_class2(50e6,argv[2]);
    end1 = clock();
    cout<<"number of collisions for size 50,000,000 : "<<Query_class2.countOfCollisions()<<endl;
    cout << "Time taken: " << (double)(end1 - start1) / CLOCKS_PER_SEC << "s\n" << endl;
    
    // hash size 100e6
    start1 = clock();
    Queries_HT Query_class3(100e6,argv[2]);
    end1 = clock();
    cout<<"number of collisions for size 100,000,000 : "<<Query_class3.countOfCollisions()<<endl;
    cout << "Time taken: " << (double)(end1 - start1) / CLOCKS_PER_SEC << "s\n" << endl;
    
    // hash size 200e6
    cout<<"number of collisions for size 200,000,000 : "<<Query_class4.countOfCollisions()<<endl;
    cout << "Time taken: " << (double)(end4 - start4) / CLOCKS_PER_SEC << "s\n" << endl;
    }

        if(part.compare("B") == 0){
        ifstream file(argv[3]);
        if (!file.is_open()) {
            cerr << "Error opening file " << argv[3] << endl;
            return -1; // Exit the program if file cannot be opened
        }
        
        // Read the query file and insert elements into the hash table
        string line;
        string subject_data_set;
        long long int file_size = 0;
        while (getline(file, line)) {
            if (!line.empty() && line[0] != '>') {
                file_size += line.size() + 1; // Add 1 for the newline character
                subject_data_set.append(line + '\n'); // Append the line along with newline character
            }
        }
        
        long long int char_arr_size = subject_data_set.size() + 1;
        char subject_data_arr[char_arr_size];
        long long int i = 0;
        for (char x : subject_data_set) {
            if (x == 'N' || x == 'A' || x == 'C' || x == 'G' || x == 'T') {
                subject_data_arr[i++] = x;
                if (x == 'N') {
                    subject_data_arr[i - 1] = 'A';
                }
            }
        }

        string cur_fragment;
        char_arr_size = i;
        string top10[10];
        long long int j = 0, fragments_count = 0;
        clock_t start1, end1;
        start1 = clock();
        for (i = 0; i < 15 && i < char_arr_size; i++) {
            cur_fragment += subject_data_arr[i];
        }
        for (; i < char_arr_size; i++) {
            cur_fragment += subject_data_arr[i];
            if (Query_class4.search(cur_fragment)) {
                if (j < 10) {
                    top10[j++] = cur_fragment;
                }
                fragments_count++;
            }
            cur_fragment = cur_fragment.substr(1, 15);
        }
        end1 = clock();
        
        cout << "######################################### Searching Summary for hash size of 200e6 ##################################################\n" << endl;
        cout << "Time taken : " << (double)(end1 - start1) / CLOCKS_PER_SEC << "s\n" << endl;
        cout << "Fragments hit count : " << fragments_count << endl;
        cout << "Top 10 fragments :\n";
        for (long long int i = 0; i < j; i++) {
            cout << top10[i] << endl;
        }
    }

    return 0;
}

// Reasoning for PART A time complexity 
// Running time observations for different hash sizes
// number of collisions for size 1e6 : 38
// Time taken: 0.010448s

// number of collisions for size 50e6 : 30
// Time taken: 0.338644s

// number of collisions for size 100e6 : 30
// Time taken: 0.635147s

// number of collisions for size 200e6 : 30
// Time taken: 1.26103s

// here we have seen that the time is linearly dependent on the hash size , as the has size is linearly increased the time is also linearly increased 
// when we doubled the hash size , the running time also doubled 
// So, the time complexity is O(N) 
