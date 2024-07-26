#include <stdio.h>
#include <iostream>
#include "headers.h"
#include <fstream>
#include <random>
#include <time.h>
#include <string.h>
#include <string>

#define SUCCESS true
#define FAILED false

using namespace std;

bool GNOME::readData(const char *filepath)
{
    ifstream gnomefile(filepath);
    string currLine;
    int i;
    if (!gnomefile.is_open())
    {
        cout << "err: unable to open the file";
        return FAILED;
    }
    while (getline(gnomefile, currLine))
    {
        i = 0;
        while (i < currLine.size())
        {
            if (currLine[i] == 'N')
                currLine[i] = 'A';
            ++i;
        }
        memcpy(data + data_size, currLine.c_str(), currLine.size());
        data_size += currLine.size();
    }
    // for(int i=0;i<data_size;++i){
    //     cout<<data[i];
    // }

    cout << "\nData read successfully: " << data_size;
    return SUCCESS;
}

void questionHelper(char *ch, int nmers, bool question2)
{
    PrefixTrie trie(36);

    cout << "For " << nmers << ":" << endl;
    int siz = 50000 - 37;
    char chArray[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < nmers; ++i)
    {
        // cout<<"test1 "<<endl;
        int randStart = (rand() % siz);
        int randchar = rand() % 36;
        char prev = ch[randStart + randchar];

        if (question2)
        {
            int errorRate = rand() % 100;
            if (errorRate < 5)
            {
                int randtemp = rand() % 4;
                while (chArray[randtemp] == prev)
                    randtemp = rand() % 4;
                ch[randStart + randchar] = chArray[randtemp];
            }
        }
        // cout<<"test2 "<<endl;
        trie.insert(ch + randStart);
        // cout<<"test3 "<<endl;
        if (question2)
            ch[randStart + randchar] = prev;
    }
    int hitcount = 0;
    // cout<<"test test"<<endl;
    for (int i = 0; i < 50000 - 37; ++i)
    {
        if (trie.fuzzySearch(ch + i))
        {
            hitcount++;
        }
    }
    cout << "total number of nodes in prefixTrie: " << trie.countTrieNodes();
    cout << "total hits: " << hitcount << endl;
}

void question(GNOME &subjectData, bool question2)
{
    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<long long unsigned> dist(0, subjectData.getSize() - 50000 - 1);

    questionHelper(subjectData.getPointer(dist(gen)), 5000, question2);
    questionHelper(subjectData.getPointer(dist(gen)), 50000, question2);
    questionHelper(subjectData.getPointer(dist(gen)), 100000, question2);
    questionHelper(subjectData.getPointer(dist(gen)), 1000000, question2);
}


bool question3readFragmentData(const char *path, PrefixTrie &trie){
    ifstream queryFile(path);
    string currLine;
    // fragment *fr;
    if(!queryFile.is_open()){
        cout<<"err: unable open query File";
        return FAILED;
    }
    bool val=true;
    // cout<<"just started";
    while(getline(queryFile,currLine)){
        if(val){
            val=!val;
            continue;
        }
        trie.insert(currLine.c_str());
        val=!val;
    }
    cout<<"\nfragments read successfully"<<endl;
    return true;
}



void question3Helper(char *ch, int segmentSize, PrefixTrie &trie){
    cout<<"\n For"<<segmentSize <<":"<<endl;
    int hitCounter=0;
    clock_t start=clock();
    for(int i=0;i<segmentSize-32;++i){
        if(trie.trieSearch(ch+i))
        ++hitCounter;
    }
    clock_t end=clock();
    cout<<"total trie nodes:"<<trie.countTrieNodes()<<endl;
    cout<<"total hits: "<<hitCounter<<endl;
    cout<<"time taken: "<<((double)(end-start)/CLOCKS_PER_SEC)<<endl;
}


void question3(GNOME &subjectData, char *queryDatasetPath){
    PrefixTrie trie(32);
    question3readFragmentData(queryDatasetPath,trie);
    static random_device rd;
    static mt19937_64 gen(rd());
    uniform_int_distribution<long long unsigned> dist0(0, subjectData.getSize() - 100000 - 1);
    uniform_int_distribution<long long unsigned> dist1(0, subjectData.getSize() - 1000000 - 1);
    uniform_int_distribution<long long unsigned> dist2(0, subjectData.getSize() - 100000000 - 1);
    unsigned long long int randd=dist0(rd);
    // cout<<"random number 1"<<randd<<endl;
    question3Helper(subjectData.getPointer(randd),100000,trie);
    randd=dist1(rd);
    // cout<<"random number 2"<<randd<<endl;
    question3Helper(subjectData.getPointer(randd),1000000,trie);
    question3Helper(subjectData.getPointer(dist2(gen)),100000000,trie);
}



int main(int argn, char *args[])
{

    if (argn < 4)
    {
        cout << "err: Invalid Arguments";
        return EXIT_FAILURE;
    }

    srand(time(nullptr));
    char *question_no = args[1];
    char *subjectDataPath = args[3];
    char *queryDataPath = args[2];
    unsigned long long size = get_size(subjectDataPath);
    GNOME gnomeObj(size);
    // creating a object to operate on gnome data an array with size of file size is allocated on creation
    gnomeObj.readData(subjectDataPath);

    if(question_no[0]=='A'){
        cout<<"\nQuestion A started"<<endl;
        question(gnomeObj,false);
    }
    else if(question_no[0]=='B'){
        cout<<"\nQuestion B started"<<endl;
        question(gnomeObj,true);
    }
    else if(question_no[0]=='C'){
        cout<<"\nQuestion C started"<<endl;
        question3(gnomeObj,queryDataPath);
    }
    else{
        cout<<"error: wrong option selected"<<endl;
    }
    return 0;
}