#ifndef HW2_LOG_H
#define HW2_LOG_H

#include <iostream>
#include <string>
#include <fstream>
#include <memory>

namespace log {
    enum class Level{
        DEBUG, //0
        INFO,
        WARN,
        ERROR  //3
    };

    class BaseLogger{
        Level level_;
        std::ostream& out_;
        void log(const std::string&, Level);
    public:
        BaseLogger(std::ostream&, Level);
        virtual ~BaseLogger(){};
        void debug(std::string);
        void info(std::string);
        void warn(std::string);
        void error(std::string);
        void set_level(Level);
        Level level();
        void flush();
    };

    class FileLogger:public BaseLogger{
        std::ofstream file_;
    public:
        explicit FileLogger(const std::string&);
        FileLogger(const std::string&, Level);
        ~FileLogger();
    };

    class StdoutLogger:public BaseLogger{
    public:
        StdoutLogger();
        explicit StdoutLogger(Level);
        ~StdoutLogger();
    };

    class StderrLogger:public BaseLogger{
    public:
        StderrLogger();
        explicit StderrLogger(Level);
        ~StderrLogger();
    };

    class Logger{
        std::shared_ptr<BaseLogger> global_logger_;
        Logger();
        Logger& operator= (const Logger&);
    public:
        static Logger& get_instance();
        std::shared_ptr<BaseLogger> get_global_logger();
        void set_global_logger(std::shared_ptr<BaseLogger>);
    };

    void init_with_stderr_logger(const Level&);
    void init_with_stdout_logger(const Level&);
    void init_with_file_logger(const std::string&, const Level&);

    void debug(const std::string&);
    void info(const std::string&);
    void warn(const std::string&);
    void error(const std::string&);

}
#endif //HW2_LOG_H