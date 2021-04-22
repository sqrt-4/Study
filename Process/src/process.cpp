#include <unistd.h>
#include <string>
#include <signal.h>
#include <sys/wait.h>
#include <stdexcept>
#include <vector>
#include "process.h"

using namespace std;

Process::Process(const string& p, const vector<string>& argv) {
    int fdwr[] {-1,-1},fdr[] {-1,-1};
    if (::pipe(fdwr)<0 || ::pipe(fdr)<0){
        ::close(fdwr[0]);
        ::close(fdwr[1]);
        ::close(fdr[0]);
        ::close(fdr[1]);
        throw runtime_error("Can't pipe");
    }
    ft_=fork();
    if (ft_==0){
        //сын
        ::dup2(fdwr[0],STDIN_FILENO);   //отождествление stdin c чтением из канала
        ::close(fdwr[0]);
        ::close(fdwr[1]);

        ::dup2(fdr[1],STDOUT_FILENO);   //отождествление stdout с записью в канал
        ::close(fdr[0]);
        ::close(fdr[1]);

        vector<string> tmpv=argv;
        string tmps=p;
        vector<char *> tmp;
        tmp.push_back(tmps.data());
        for (string& it:tmpv) {
            tmp.push_back(it.data());
        }

        ::execvp(tmp[0],tmp.data());
        string err;
        err+="Can't Execute:"+argv[0];
        throw runtime_error(err);  //если exec не отработает

    } else if (ft_>0) {
        //отец
        ::close(fdwr[0]);    //закрытие канала fdwr на чтение (тут отец только пишет)
        ::close(fdr[1]);    //закрытие канала fdr на запись (тут отец только читает)
        fdwr_=fdwr[1];
        fdr_=fdr[0];
    }
    else{
        ::close(fdwr[0]);
        ::close(fdwr[1]);
        ::close(fdr[0]);
        ::close(fdr[1]);
        throw runtime_error("Can't fork");
    }
}

size_t Process::write(const void *data, size_t len) {
    ssize_t wr_bytes;
    wr_bytes=::write(fdwr_, data, len);
    if (wr_bytes<0) throw runtime_error("write returned -1");
    return static_cast<size_t>(wr_bytes);
}

size_t Process::read(void *data, size_t len) {
    ssize_t r_bytes;
    r_bytes= ::read(fdr_,static_cast<char *>(data),len);
    if (r_bytes<0) throw runtime_error("read returned -1");
    return static_cast<size_t>(r_bytes);
}

void Process::closeStdin(){
    if (fdwr_!=-1) {
        ::close(fdwr_);  //закрываем запись в канал для отца => перекрываем STDIN сыну
        fdwr_=-1;
    }
}

void Process::close(){
    if (ft_) {
        ::kill(ft_, SIGINT);
        if (::waitpid(ft_, 0, 0)<0)
            throw runtime_error("Process::close(): waitpid error");
        ft_=-1;
    }
}

void Process::writeExact(const void *data, size_t len) {
    size_t wr=0;
    while (wr!=len){
        wr+=write(static_cast<const char*>(data)+wr,len-wr);
        if (wr==0)
            throw runtime_error("Process::writeExact(): 0bytes written");
    }
}

void Process::readExact(void *data, size_t len) {
    size_t r=0;
    while (r != len) {
        r += read(static_cast<char *>(data) + r, len - r);
        if (r == 0)
            throw runtime_error("Process::readExact(): 0bytes read");
    }
}

Process::~Process() {
    if (ft_) {
        ::kill(ft_, SIGKILL);
        ::close(fdwr_);
        ::close(fdr_);
        if (::waitpid(ft_, 0, 0) < 0)
            throw runtime_error("~Process(): waitpid error");
    }
};