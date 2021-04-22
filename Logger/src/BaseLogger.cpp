#include "log.h"

namespace log {
    BaseLogger::BaseLogger(std::ostream& out, Level lv=Level::DEBUG) : out_(out), level_(lv){}

    void BaseLogger::log(const std::string &msg, Level lv) {
        if (lv >= level()) {
            out_ << msg << std::endl;
        }
    }

    void BaseLogger::set_level(Level lv) {
        level_ = lv;
    }

    Level BaseLogger::level() {
        return level_;
    }

    void BaseLogger::debug(std::string msg) {
        log(msg, Level::DEBUG);
    }

    void BaseLogger::info(std::string msg) {
        log(msg, Level::INFO);
    }

    void BaseLogger::warn(std::string msg) {
        log(msg, Level::WARN);
    }

    void BaseLogger::error(std::string msg) {
        log(msg, Level::ERROR);
    }

    void BaseLogger::flush() {
        std::flush(out_);
    }
}