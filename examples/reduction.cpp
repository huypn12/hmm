#include "hmm.h"
#include "logger.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <unordered_map>

using namespace org::mcss;

static Logger logger("reduction.log");

void FitKStateModel(const int &state_count, const int &alphabet_count,
                    const std::string &trace_file) {
  auto test_model = std::make_unique<Hmm>(state_count, alphabet_count);
  test_model->InitRandom();

  logger.LogInfo("Test model before fitting \n" + test_model->Str());

  auto trace = label_trace();
  test_model->Fit(trace, 1000, 1e-4);

  logger.LogInfo("Test model after fitting \n" + test_model->Str());
}

void experiment() {
  logger.LogInfo("Experiment: fitting with normal trace, jumps almost uniformly among states");
  FitKStateModel(10, 3, "label_trace_1000_normal.txt");
  FitKStateModel(5, 3, "label_trace_1000_normal.txt");

  logger.LogInfo("Experiment: fitting with abnormal trace, falls into spyder trap after few jumps");
  FitKStateModel(10, 3, "label_trace_1000_abnormal.txt");
  FitKStateModel(5, 3, "label_trace_1000_abnormal.txt");
}

int main(int argc, char **argv) {
  experiment();

  return EXIT_SUCCESS;
}
