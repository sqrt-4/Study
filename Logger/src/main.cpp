#include <iostream>
#include "log.h"

using namespace log;

int main() {

    StdoutLogger logout;
    logout.set_level(Level::INFO);
    logout.warn("Warn_out");
    logout.info("Info_out");

    std::cout<<"-------"<<std::endl;
    StderrLogger logerr(Level::ERROR);
    logerr.debug("Debug_Err");
    logerr.info("Info_Err");
    logerr.warn("Warn_Err");
    logerr.error("Error_Err");
    std::cout<<"";

    std::string path;
    FileLogger logf("log1");
    logf.debug("Debug_file");
    logf.info("Info_file");
    logf.warn("Warn_file");
    logf.error("Error_file");

    init_with_file_logger("log2",Level::ERROR);
    info("INFO");
    error("ERROR");
    init_with_stdout_logger(Level::WARN);
    debug("INFO");
    warn("WARN");
    std::cout<<"-------"<<std::endl;

    return 0;
}