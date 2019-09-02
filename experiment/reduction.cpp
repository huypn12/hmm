#include "pchheader.hpp"

#include "hmm.hpp"

void experiment() {
  // Reference model: 5-state DTMC, labelled with 3 alphabet
  org::mcss::mc_random rand;
  // Init sample randomly
  int states_count = 5;
  auto init_p = rand.random_vector(states_count);
  auto transition_p = rand.random_matrix(states_count, states_count);
  auto orig_dtmc = std::make_shared<org::mcss::dtmc>(5, init_p, transition_p);
  // Generate state trace
  auto trace = std::vector<int>(1000);
  std::generate(trace.begin(), trace.end(),
                [&] () { return orig_dtmc->next_state(); });
  // Map trace of state to label
  std::unordered_map<int, int> label;
  label.at(0) = 0;
  label.at(1) = 1;
  label.at(2) = 2;
  label.at(3) = 2;
  label.at(4) = 2; // state 3 and 4 is for reduction experiment
  // Make label trace
  auto label_trace = std::vector<int>();
  std::transform(trace.begin(), trace.end(), std::back_inserter(label_trace),
                 [&] (int state) { return label.at(state); });
  // Estimate a HMM model which would emit the same trace
  auto test_model = std::make_shared<org::mcss::hmm>(3, 4);
  test_model->fit(label_trace);
}


int main(int argc, char **argv) {
  experiment();

  return EXIT_SUCCESS;
}
