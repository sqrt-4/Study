#include "log.h"

namespace log {
    FileLogger::FileLogger(const std::string &path) : file_(path), BaseLogger(file_, Level::INFO){}
    FileLogger::FileLogger(const std::string &path, Level lv) : file_(path), BaseLogger(file_, lv) {}
    FileLogger::~FileLogger() {std::flush(file_);}
}