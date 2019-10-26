#include "hmm.h"
#include "logger.h"

#include <iterator>
#include <memory>
#include <unordered_map>
#include <string>

#define PANIC -1
#define OK 0

using namespace org::mcss;

static Logger logger;

int ReadTraceFile(const std::string &fpath, int &alphabet_count,
                  std::string &trace_str) {
  auto ifs = std::ifstream(fpath);
  if (ifs.is_open()) {
    auto line = std::string("");
    int i = 0;
    while (std::getline(ifs, line)) {
      if (i == 0) {
        alphabet_count = std::stoi(line);
      } else if (i == 1) {
        trace_str = line;
      } else {
        break;
      }
      i++;
    }
    return OK;
  } else {
    return PANIC;
  }
}

std::shared_ptr<Dtmc> SampleModel() { return nullptr; }

void experiment()
{
  
}

int main(int argc, char *argv[])
{
  
}
