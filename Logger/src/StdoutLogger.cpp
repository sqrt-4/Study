#include "log.h"

namespace log {
    StdoutLogger::StdoutLogger() : BaseLogger(std::cout, Level::INFO) {}
    StdoutLogger::StdoutLogger(Level lv) : BaseLogger(std::cout, lv) {}
    StdoutLogger::~StdoutLogger() {std::flush(std::cout);}
}