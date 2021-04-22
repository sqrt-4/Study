#include <memory>
#include "log.h"

using std::shared_ptr;

namespace log{

    Logger::Logger() {
        global_logger_=shared_ptr<StdoutLogger>(new StdoutLogger);
    }

    Logger& Logger::get_instance() {
        static Logger instance;
        return instance;
    }

    shared_ptr<BaseLogger> Logger:: get_global_logger(){
        return global_logger_;
    }

    void Logger::set_global_logger(std::shared_ptr<BaseLogger> logger) {
        if (logger) {
            global_logger_ = logger;
            return;
        }
        throw std::runtime_error("Invalid logger ptr");
    }

    void init_with_stderr_logger(const Level& lv=Level::DEBUG){
        auto ErrLogger=shared_ptr<StderrLogger>(new StderrLogger(lv));
        Logger::get_instance().set_global_logger(ErrLogger);
    }

    void init_with_stdout_logger(const Level& lv=Level::DEBUG){
        auto OutLogger=shared_ptr<StdoutLogger>(new StdoutLogger(lv));
        Logger::get_instance().set_global_logger(OutLogger);
    }

    void init_with_file_logger(const std::string& path, const Level& lv=Level::DEBUG) {
        auto FLogger=shared_ptr<FileLogger>(new FileLogger(path,lv));
        Logger::get_instance().set_global_logger(FLogger);
    }

    void debug(const std::string& msg) {
        Logger::get_instance().get_global_logger()->debug(msg);
    }

    void info(const std::string& msg) {
        Logger::get_instance().get_global_logger()->info(msg);
    }

    void warn(const std::string& msg) {
        Logger::get_instance().get_global_logger()->warn(msg);
    }

    void error(const std::string& msg) {
        Logger::get_instance().get_global_logger()->error(msg);
    }

}
