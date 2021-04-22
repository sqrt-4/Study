#include "log.h"

namespace log {
    StderrLogger::StderrLogger() : BaseLogger(std::cerr, Level::DEBUG) {}
    StderrLogger::StderrLogger(Level lv) : BaseLogger(std::cerr, lv) {}
    StderrLogger::~StderrLogger() {std::flush(std::cerr);}
}