#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
// #include <malloc.h>




using namespace std;

// Default constructor
PrefixTrie() : root(nullptr), wordLength(36) {}

// Custom constructor to build a trie from a set of queries (of size n)
PrefixTrie(const char **queries, int n) : wordLength(36) {
    root = new TrieNode();
    for (int i = 0; i < n; ++i) {
        insert(queries[i]);
    }
}

unsigned long long get_size(const char *filepath)
{
    struct stat fstatus;
    stat(filepath, &fstatus);
    return fstatus.st_size;
}


inline int getIndex(char ch){
    if(ch=='A')
    return 0;
    else if(ch=='C')
    return 1;
    else if(ch == 'G')
    return 2;
    else
    return 3;
}


struct TrieNode{
    TrieNode *children[4]{};
};

//A function to traverse (search) the trie using a genome of size G.
bool fuzzySearchHelper(TrieNode *root, char *ch, int index, bool flag){
    if(index==36)//condition to check index equals to 32
    return true; // returns true
    if(root==nullptr) // condition to check root is equals to null or not
    return false; // returns false

    int i=getIndex(ch[index]); // getting the index 
    if(root->children[i]!=nullptr){ //condition to check root children equlas to null or not
        return fuzzySearchHelper(root->children[i],ch,index+1,false); // returns the output from fuzzyhelper method
    }
    else //if not it executes this else block
    {
        if(flag) //consition to check the flag true or not
        return false;//returns false
        int j=0;//initialize the variable j=0
        while(j<4){//loop to iterate until j becomes leass than 4
            if(fuzzySearchHelper(root->children[i],ch,index+1,true))
            return true;//return true value
            ++j;//increment the looping variable
        }
    }
    return false;//at the end return the false nothing validates
}


long long int countTriesNodesHelper(TrieNode *root,int index,int wordLength){
    if(index==wordLength)
    return 0;
    long long int counter=0;
    for(int i=0;i<4;++i){
        if(root->children[i]!=nullptr)
        counter+=1+countTriesNodesHelper(root->children[i],index+1,wordLength);
    }
    return counter;
}


class PrefixTrie{

    TrieNode *root;
    int wordLength;
public:
    PrefixTrie(int wl){
        root=new TrieNode();
        wordLength=wl;
    }

    void insert(const char *ch){
        TrieNode *indexer=root;
        int i=0;
        // cout<<"test4"<<endl;
        while(i < wordLength){
            int i2=getIndex(ch[i]);
            // cout<<ch[i]<<i2<<endl;
            if(indexer->children[i2]==nullptr)
            indexer->children[i2]=new TrieNode();
            indexer=indexer->children[i2];
            ++i;
        }
        // cout<<"test5"<<endl;
    }

    bool fuzzySearch(char *ch){
        return fuzzySearchHelper(root, ch, 0,false);
    }
    
    bool trieSearch(char *ch){
        TrieNode *indexer=root;
        int i=0;
        while(i<wordLength){
            int i2=getIndex(ch[i]);
            if(indexer->children[i2]==nullptr)
            return false;
            indexer=indexer->children[i2];
            ++i;
        }
        return true;
    }


    long long int countTrieNodes(){
        return countTriesNodesHelper(root,0,wordLength);
    }


};

// Destructor
~PrefixTrie() {
    deleteTrie(root);
}

class GNOME
{
    char *data = nullptr;
    unsigned long long max_size = 0;//total allocated memory size for data
    unsigned long long data_size = 0;//actual size of data in the data object
    unsigned long long scaff = 0;

public:
    GNOME(unsigned long long size)
    {
        data = (char *)malloc(size);
        max_size = size;
    }
    char * getPointer(unsigned long long index){
        return data+index;
    }
    unsigned long long getSize(){
        return data_size;
    }
    bool readData(const char *filepath);
    ~GNOME()
    {
        free(data);
    }
};