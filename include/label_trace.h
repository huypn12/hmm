#ifndef __LABEL_TRACE_H__
#define __LABEL_TRACE_H__

#include "trace.h"

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace org::mcss {
class label_trace : public trace {
private:
  std::vector<int> container_;

public:
  const int &operator[](const int &i) const { return container_[i]; }
  const std::vector<int> &container() { return container_; }
  size_t size() const { return std::move(container_.size()); }

  void FromStr(const std::string &str) override {
    std::stringstream ss(str);
    for (int i; ss >> i;) {
      container_.push_back(i);
      if (ss.peek() == ',')
        ss.ignore();
    }
  }
  std::string ToStr() override {
    std::stringstream ss;
    if (!container_.empty()) {
      std::copy(container_.begin(), container_.end() - 1,
                std::ostream_iterator<int>(ss, ","));
      ss << container_.back();
    }
    return std::move(ss.str());
  }

  void FromFile(std::ifstream &ifs) override {
    std::stringstream ss;
    if (ifs.is_open()) {
      ss << ifs.rdbuf();
    }
    FromStr(ss.str());
  }

  void FromFile(const std::string &fpath) {
    auto ifs = std::ifstream(fpath);
    FromFile(ifs);
  }

  void ToFile(std::ofstream &ofs) override {
    if (ofs.is_open()) {
      ofs << ToStr();
    }
  }

  void ToFile(const std::string &fpath) {
    auto ofs = std::ofstream(fpath);
    ToFile(ofs);
  }

  void Flush() override { container_.clear(); }

  void Append(const int &e) { container_.push_back(e); }
};
} // namespace org::mcss

#endif // __LABEL_TRACE_H__
