#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <fstream>
#include <iostream>
#include <sstream>

// Super simple & stupid incremental logger
namespace org::mcss {
class Logger {
private:
  long window_size_;
  std::stringstream log_buf_;
  std::ofstream log_fptr_;

  long LogSize() {
    log_buf_.seekp(0, std::ios::end);
    std::stringstream::pos_type offset = log_buf_.tellp();
    return long(offset);
  }

public:
  Logger(const std::string &log_fpath) { SetLogFile(log_fpath); }

  ~Logger() { Dump(); }

  void SetLogFile(const std::string &log_fpath) {
    log_fptr_ = std::ofstream(log_fpath);
    log_fptr_.open(log_fpath, std::ios::out | std::ios::app);
    if (!log_fptr_.is_open()) {
      std::cerr << "[E] Unable to open log file, logging to stderr instead."
                << std::endl;
      log_fptr_.basic_ios<char>::rdbuf(std::cerr.rdbuf());
    }
  }

  void Log(const std::string &mesg) {
    log_buf_ << mesg << std::endl;
    auto log_size = LogSize();
    if (LogSize() > 4096) {
      log_fptr_ << log_buf_.str();
      log_buf_.clear();
    }
  }

  void LogInfo(const std::string &mesg) { Log("[INFO]" + mesg); }

  void LogError(const std::string &mesg) { Log("[ERROR] " + mesg); }

  void Dump() { log_fptr_ << log_buf_.str(); }
};
} // namespace org::mcss

#endif
