#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <time.h>
#include <utility>
#include <unordered_set>
#include "random_string.h"
using namespace std;
class Queries_BL { //class called Queries_BL
private:
    char** query_data; // Pointer to array of C-style strings storing query data
    long long int num_queries=0; // Number of queries
    long long int fragment_size=0; // Size of query fragments initialized to zero
    string subject_data=""; // String to store subject data
    std::unordered_set<std::string> subject_set; // Set to store unique subjects

public:
    long long int max(long long int a,long long int b){ // Function to find maximum of two numbers
        if(a>b) return a; // Return a if it's greater than b
        return b; // Otherwise, return b
    }
    long long int min(long long int a,long long int b){ // Function to find minimum of two numbers
        if(a<b) return a; // Return a if it's greater than b
        return b; // Otherwise, return b
    }
    // Constructor taking query data file and subject data file paths
    Queries_BL(const std::string& query_data_file,const std::string& subject_data_file) {
        read_query_data(query_data_file); // Call function to read query data
        ifstream file(subject_data_file); // Open subject data file
        string line; // Variable to store each line from file
        while(getline(file,line)){ //loop to read each line from file
           if(line.size()>0 && line[0]!='\n' && line[0]!='>') // Check if line is not empty and doesn't start with newline or '>'
           subject_data+=line; // Append line to subject data
        }
    }
    ~Queries_BL(){ // Destructor
      for (long long int i = 0; i < num_queries; ++i) { // Loop through each query
            delete[] query_data[i]; // Delete each C-style string
        }
        delete[] query_data; // Delete array of C-style strings
    }
    void read_query_data(const std::string& file_name) {
        std::ifstream file(file_name);
        if (file.is_open()) {
            std::string line;
            long long int max_line_length = 0;
            long long int num_lines = 0;

            // Calculate the number of lines and the maximum line length
            while (std::getline(file, line)) {
                if (line.size() > 0 && line[0] != '\n' && line[0] != '>') {
                    ++num_lines;
                    ++num_queries;
                    max_line_length = std::max(max_line_length, static_cast<long long int>(line.size()));
                }
            }
            file.close();

            // Allocate memory for query_data
            num_queries = num_lines;
            fragment_size=max_line_length;
            query_data = new char*[num_lines];
            for (long long int i = 0; i < num_lines; ++i) {
                query_data[i] = new char[max_line_length +1]; // +1 for null terminator
            }

            // Read data from file and copy long long into query_data
            file.open(file_name);
            long long int current_line = 0;
            while (std::getline(file, line)) {
                if (line.size() > 0 && line[0] != '\n' && line[0] != '>') {
                    strcpy(query_data[current_line], line.c_str());
                    ++current_line;
                     for (long long int i = 0; i <= line.size() - 16; ++i) {
                        std::string w_mer = line.substr(i, 16);
                        subject_set.insert(w_mer); // Store each w-mer in the unordered_set
                    }
                }
            }
            file.close();
        } else {
            std::cerr << "Unable to open file: " << file_name << std::endl;
        }
    }
    void fuzzy_search_subject_data(long long int num_searches) {
        long long int subject_data_len = subject_data.size();
        long long int range=subject_data_len-fragment_size;
        long long int hit_count=0;
        for (long long int i = 0; i < num_searches; ++i) {
           long long int pos=rand()%range;
           string query = subject_data.substr(pos,fragment_size);
           hit_count+=blast(query);
       }
       cout<<"Hit count for "<<num_searches<<" of subject_data : "<<hit_count<<endl;
    }
    void search_on_entire_subject_data(){
       string query=subject_data.substr(0,31);
       long long int n=subject_data.size();
       long long int hit_count=0;
       for(long long int i=31;i<n;i++){
          query+=subject_data[i];
          long long int score=blast(query);
          hit_count+=score;
          query=query.substr(1,31);
       }
       cout<<"Hit count for entire subject data ("<<n<<" fragments is ) " << hit_count <<endl; 
    }
    void random_fuzzy_search(long long int num_searches){
       char arr[5]={'A','C','G','T','N'};
       long long int hit_count=0;
       for(long long int i=0;i<num_searches;i++){
          string random_query="";
          /*for(long long int j=0;j<fragment_size;j++){
              random_query+=arr[rand()%5];
          }*/
          random_query=random_generated_string(fragment_size);
          long long int score=blast(random_query);
          hit_count+=score;
       }
       cout<<"Hit count for "<<num_searches<<" of random generated data : "<<hit_count<<endl;
    }

private:
    
   long long int blast(const std::string& query, long long int max_mismatches = 2) {
        long long int hits = 0;
        long long int w_mer_size = 16; // Size of each w-mer
        for (long long int i = 0; i <= query.size() - w_mer_size; ++i) {
            std::string w_mer = query.substr(i, w_mer_size);
            std::vector<std::string> neighborhood; 
            neighborhood.push_back(w_mer);
            /*char arr[4] = {'A','C','G','T'};
            for(long long int i=0;i<16;i++){
              for(long long int j=i+1;j<16;j++){
                 for(long long int k=0;k<4;k++){
                    for(long long int l=0;l<4;l++){
                       string s1=w_mer.substr(0,i)+arr[k]+w_mer.substr(i+1,j-(i+1))+arr[l]+w_mer.substr(j+1);
                       neighborhood.push_back(s1);
                    }
                 }
              }
            }*/
            for (const std::string& word : neighborhood) {
                if (subject_set.find(word) != subject_set.end()) {
                    // Perform Smith-Waterman seed extension
                   long long int score = smithWaterman(query, word, 2, -1, -1);
                   if (score >= 26) { // Threshold for extension
                        ++hits;
                    }
                   
                }
            }
        }
        return hits;
    }
    //smithWaterman to compare two n-mer sequences
    long long int smithWaterman(const std::string& sequence1, const std::string& sequence2, long long int match_score, long long int mismatch_penalty, long long int gap_penalty) {
    long long int n = sequence1.size();
    long long int m = sequence2.size();

    // Initialize the score matrix and the traceback matrix
    std::vector<std::vector<long long int>> score(n + 1, std::vector<long long int>(m + 1, 0));
    std::vector<std::vector<long long int>> traceback(n + 1, std::vector<long long int>(m + 1, 0));

    long long int max_score = 0;
    // Loop through each cell in the matrices
    for (long long int i = 1; i <= n; ++i) {
        for (long long int j = 1; j <= m; ++j) {
        // Calculate scores for match, gap deletion, and gap insertion
            long long int match = score[i - 1][j - 1] + (sequence1[i - 1] == sequence2[j - 1] ? match_score : mismatch_penalty);
            long long int delete_gap = score[i - 1][j] + gap_penalty;
            long long int insert_gap = score[i][j - 1] + gap_penalty;
            
            // Choose the maximum score among match, delete_gap, and insert_gap
            score[i][j] = std::max(match, std::max(delete_gap, insert_gap));

            // Update traceback matrix
            if (score[i][j] == 0) {
                traceback[i][j] = 0;
            } else if (score[i][j] == match) {
                traceback[i][j] = 1;
            } else if (score[i][j] == delete_gap) {
                traceback[i][j] = 2;
            } else {
                traceback[i][j] = 3;
            }

            // Update max score
            max_score = std::max(max_score, score[i][j]);
        }
    }
    return max_score; // Return the maximum score
}

};

class queries_nw {
private:
    char** query_data; // Dynamic 2D array to store query data
    long long int num_queries=0; // Number of queries
    long long int query_fragment_size=0; // Size of each query fragment
    string subject_data="";// Data of the subject
    long long int HitCheck(vector<vector<long long int>>&dp, long long int i,long long int j,long long int mismatch,string seq1,string seq2){
       if(i==0 || j==0) return mismatch>2?0:1;
       if(mismatch>2) return 0;
       long long int mcheck=seq1[i-1]==seq2[j-1]?2:-1; // Check if there is a match between characters in seq1 and seq2
       long long int a=max(dp[i-1][j-1]+mcheck,max(dp[i-1][j]-1,dp[i][j-1]-1)); // Calculate the maximum score from three possible moves
       long long int r1=0,r2=0,r3=0;// Initialize variables to store results of recursive calls
       long long int ans=0;// Initialize variable to store the final result
       if(a==dp[i-1][j-1]+mcheck){ // Check if the current score is obtained from a match/mismatch
         if(seq1[i-1]==seq2[j-1]){ // If there's a match between characters
         if(HitCheck(dp,i-1,j-1,mismatch,seq1,seq2)) // Recursively check if previous characters also match
         ans=1;// Set ans to 1 if all previous characters match
         }
         else{
         if(HitCheck(dp,i-1,j-1,mismatch+1,seq1,seq2)) // Recursively check if previous characters also mismatch
         ans=1; s to 1 if all previous characters mismatch
         }
       }
       if(a==dp[i][j-1]-1)
       if(HitCheck(dp,i,j-1,mismatch,seq1,seq2))
       ans=1;
       if(a==dp[i-1][j]-1)
       if(HitCheck(dp,i-1,j,mismatch,seq1,seq2))
       ans=1;
       return ans;
    }
    //Needleman Wunsch function to compare two n-mer sequences
    long long int needleman_wunsch(const std::string& sequence1, const std::string& sequence2) {
        long long int m = sequence1.size(); // Length of sequence1
        long long int n = sequence2.size(); // Length of sequence2
       
       //// Initialize dynamic programming table with dimensions (m+1) x (n+1) and fill with -1
        std::vector<vector<long long int>> dp(m + 1, std::vector<long long int>(n + 1, -1));
        for (long long int i = 0; i <= m; ++i) {
            dp[i][0] = -i; // Initialize first column with gap penalties
        }
        for (long long int j = 0; j <= n; ++j) {
            dp[0][j] = -j; // Initialize first row with gap penalties
        }
        
        for (long long int i = 1; i <= m; ++i) {
            for (long long int j = 1; j <= n; ++j) {
                long long int match = dp[i - 1][j - 1] + (sequence1[i - 1] == sequence2[j - 1] ? 2 : -1); // Match or mismatch
                long long int gap1 = dp[i - 1][j] -1; // Calculate score for gap in sequence1
                long long int gap2 = dp[i][j - 1]-1;  // Calculate score for gap in sequence2
                long long int ma = max(match,max(gap1,gap2)); // Choose maximum score among match, gap1, and gap2
                dp[i][j]=ma; // Assign maximum score to dp[i][j]
            }
        }
        return HitCheck(dp,m,n,0,sequence1,sequence2); // Return result of traceback using HitCheck function
        //if(dp[m][n].second<=2) return 1;
        //return 0;
    }

public:
    queries_nw(const std::string& query_data_file,const std::string& subject_data_file) { //constructor of the queries_nw class
        read_query_data(query_data_file);// Call a function to read query data from a file
        ifstream file(subject_data_file);// Open the subject data fil
        string line;// Declare a string variable to store each line of the file
        while(getline(file,line)){// Loop through each line of the file
           if(line.size()>0 && line[0]!='\n' && line[0]!='>') // Check if the line is not empty and doesn't start with newline or '>'
           subject_data+=line;// Append the line to the subject data string
        }
    }

    // Destructor
    ~queries_nw() {
        // Deallocate memory for query_data
        for (long long int i = 0; i < num_queries; ++i) { //loop to iterate through num_queries
            delete[] query_data[i]; // Deallocate memory for each row of query_data
        }
        delete[] query_data;// Deallocate memory for the array of pointers
    }

    // Function to read query data from file
    void read_query_data(const std::string& file_name) {
        std::ifstream file(file_name);
        if (file.is_open()) {
            std::string line;
            long long int max_line_length = 0;
            long long int num_lines = 0;

            // Calculate the number of lines and the maximum line length
            while (std::getline(file, line)) {
                if (line.size() > 0 && line[0] != '\n' && line[0] != '>') {
                    ++num_lines;
                    ++num_queries;
                    max_line_length = std::max(max_line_length, static_cast<long long int>(line.size()));
                }
            }
            file.close();

            // Allocate memory for query_data
            num_queries = num_lines;
            query_fragment_size=max_line_length;
            query_data = new char*[num_lines];
            for (long long int i = 0; i < num_lines; ++i) {
                query_data[i] = new char[max_line_length +1]; // +1 for null terminator
            }

            // Read data from file and copy long long into query_data
            file.open(file_name);
            long long int current_line = 0;
            while (std::getline(file, line)) {
                if (line.size() > 0 && line[0] != '\n' && line[0] != '>') {
                    strcpy(query_data[current_line], line.c_str());
                    ++current_line;
                }
            }
            file.close();
        } else {
            std::cerr << "Unable to open file: " << file_name << std::endl;
        }
    }
    long long int search_on_query_data(string query){
     for(long long int i=0;i<num_queries;i++){
        if(needleman_wunsch(query_data[i],query)){
          return 1;
        }
     }
    
     return 0;
    }
    void fuzzy_search_subject_data(long long int num_searches) {
        long long int subject_data_len = subject_data.size();
        long long int range=subject_data_len-query_fragment_size;
        long long int hit_count=0;
        for (long long int i = 0; i < num_searches; ++i) {
           long long int pos=rand()%range;
           string query = subject_data.substr(pos,query_fragment_size);
           long long int score = search_on_query_data(query);
           hit_count+=score;
       }
       cout<<"Hit count for "<<num_searches<<" of subject_data : "<<hit_count<<endl;
    }
    void random_fuzzy_search(long long int num_searches){
       char arr[5]={'A','C','G','T','N'};
       long long int hit_count=0;
       for(long long int i=0;i<num_searches;i++){
          string random_query="";
          random_query=random_generated_string(query_fragment_size);
          long long int score=search_on_query_data(random_query);
          hit_count+=score;
       }
       cout<<"Hit count for "<<num_searches<<" of random generated data : "<<hit_count<<endl;
    }
    void search_on_entire_subject_data(){
       string query=subject_data.substr(0,31);
       long long int n=subject_data.size();
       long long int hit_count=0;
       for(long long int i=31;i<n;i++){
          query+=subject_data[i];
          long long int score=search_on_query_data(query);
          hit_count+=score;
          query=query.substr(1,31);
       }
       cout<<"Hit count for entire subject data ("<<n<<" fragments is ) " << hit_count <<endl; 
    }
};

int main(int argc,char* argv[]) {
    if(argc!=4){
        cout<<"please provide query_data_file and subject_data_file "<<endl;
        return 0;
    }
    clock_t start,end;
    string query_data_file=argv[2];
    string subject_data_file=argv[3];
    string part=argv[1];
    if(part.compare("1A")==0){
     queries_nw *queries = new queries_nw(query_data_file,subject_data_file);
    start=clock();
    queries->fuzzy_search_subject_data(1000);
    end=clock();
    cout<<"Time taken for 1000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries->fuzzy_search_subject_data(10000);
    end=clock();
    cout<<"Time taken for 10000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries->fuzzy_search_subject_data(100000);
    end=clock();
    cout<<"Time taken for 100000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries->search_on_entire_subject_data();
    end=clock();
    cout<<"Time taken to search entire subject data  : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    delete queries;
    }
    else if(part.compare("1B")==0){
    queries_nw *queries = new queries_nw(query_data_file,subject_data_file);
    start=clock();
    queries->random_fuzzy_search(1000);
    end=clock();
    cout<<"Time taken for 1000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries->random_fuzzy_search(10000);
    end=clock();
    cout<<"Time taken for 10000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries->random_fuzzy_search(100000);
    end=clock();
    cout<<"Time taken for 100000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    delete queries;
    }
    else if(part.compare("2A")==0){
       Queries_BL *queries_bl = new Queries_BL(query_data_file,subject_data_file);
      start=clock();
      queries_bl->fuzzy_search_subject_data(1000);
      end=clock();
      cout<<"Time taken for 1000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
      start=clock();
      queries_bl->fuzzy_search_subject_data(10000);
      end=clock();
      cout<<"Time taken for 1000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
      start=clock();
      queries_bl->fuzzy_search_subject_data(100000);
      end=clock();
      cout<<"Time taken for 1000 subject data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
      start=clock();
    	queries_bl->search_on_entire_subject_data();
    	end=clock();
    	cout<<"Time taken to search entire subject data  : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    	delete queries_bl;
    }
    else if(part.compare("2B")==0){
     Queries_BL *queries_bl = new Queries_BL(query_data_file,subject_data_file);
       start=clock();
    queries_bl->random_fuzzy_search(1000);
    end=clock();
    cout<<"Time taken for 1000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries_bl->random_fuzzy_search(10000);
    end=clock();
    cout<<"Time taken for 10000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    start=clock();
    queries_bl->random_fuzzy_search(100000);
    end=clock();
    cout<<"Time taken for 100000 random data searches : "<< (long double)(end - start) / CLOCKS_PER_SEC << "s\n\n" << endl;
    delete queries_bl;
    }
    else{
    cout<<" Incorrect arguments " <<endl;
    }
    return 0;
}

