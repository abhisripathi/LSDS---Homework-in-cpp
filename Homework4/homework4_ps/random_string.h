#include <string>

inline std::string random_generated_string(long long int fragment_size) {
    char arr[5] = {'A', 'C', 'G', 'T', 'N'};
    std::string str = "";
    for (int i = 0; i < fragment_size; i++) {
        str += arr[rand() % 5];
    }
    return str;
}

