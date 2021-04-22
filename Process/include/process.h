#ifndef HW1_PROCESS_H
#define HW1_PROCESS_H
#include <unistd.h>
#include <string>
#include <vector>
using std::vector;
using std::string;

class Process{
    int fdwr_,fdr_;
    pid_t ft_;
public:

     Process(const string& p, const vector<string>& argv);
    ~Process();
    size_t write(const void* , size_t );
    void writeExact(const void* , size_t );
    size_t read(void* , size_t );
    void readExact(void* , size_t );

    void closeStdin();
    void close();
};
#endif //HW1_PROCESS_H