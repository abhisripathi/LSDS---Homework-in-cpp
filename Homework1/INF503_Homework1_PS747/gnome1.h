#include <sys/types.h>
#include <sys/stat.h>
#define SUCCESS true
#define FAILED false

//given the path getting the size(in bytes) of the file from the file properties
unsigned long long get_size(const char *filepath)
{
    struct stat fstatus;
    stat(filepath, &fstatus);
    return fstatus.st_size;
}

//Declaration of Gnome class
class GNOME
{
    char *data = nullptr; //pointer to store the data 
    unsigned long long max_size = 0;//total allocated memory size for data
    unsigned long long data_size = 0;//actual size of data in the data object
    unsigned long long scaff = 0; // Number of scaffolds read

public:
    GNOME(unsigned long long size) // Constructor for Gnome class
    {
        data = (char *)malloc(size); // allocate memory for data
        max_size = size;
    }
    bool readData(const char *filepath); // function to read data from file
    bool question1(); // function to question 1
    bool computeACGT(unsigned long long buffer[4]); // function to compute the ACGT frequencies
    ~GNOME() // destructor
    {
        free(data);
    }
};
