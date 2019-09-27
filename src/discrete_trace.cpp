#include <discrete_trace.hpp>

int &discrete_trace::operator[](const int &i) {
  return container_[i];
}

void discrete_trace::from(const std::string &trace_str) {
  std::stringstream ss(trace_str);
  for (int i; ss >> i;) {
    container_.push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }
}

std::string discrete_trace::str() {
  std::stringstream ss;
  if (!container_.empty()) {
    std::copy(container_.begin(), container_.end() - 1,
              std::ostream_iterator<int>(ss, ","));
    ss << container_.back();
  }
  return ss.str();
}

void discrete_trace::from_file(const std::string &trace_dump) {
  std::stringstream ss;
  std::ifstream ifs(trace_dump);
  if (ifs.is_open()) {
    ss << ifs.rdbuf();
  }
  from_str(ss.str());
}

void discrete_trace::to_file(const std::string &trace_dump) {
  std::ofstream ofs(trace_dump);
  if (ofs.is_open()) {
    ofs << str();
  }
}
