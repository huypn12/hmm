#ifndef __DISCRETE_TRACE_HPP__
#define __DISCRETE_TRACE_HPP__

#include <trace.hpp>

#include <vector>

class discrete_trace : public trace {
private:
  std::vector<int> container_;

public:
  int &operator[](const int &i) { return container_[i]; }
  const std::vector<int> &container() { return container_; }

  void from(const std::string &);
  std::string str();
  void from_file(const std::string &);
  void to_file(const std::string &);

  void append(const int &e) { container_.push_back(e); }
  void flush() { container_.clear(); }
};

#endif
