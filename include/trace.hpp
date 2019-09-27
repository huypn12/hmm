#ifndef __TRACE_HPP__
#define __TRACE_HPP__

#include <string>

class trace {
public:
  virtual int &operator[](const int &i);

  virtual void from(const std::string &);
  virtual std::string str();
  virtual void from_file(const std::string &);
  virtual void to_file(const std::string &);

  virtual void append();
  virtual void flush();
};

#endif
