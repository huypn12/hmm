#include "pchheader.hpp"

#include "hmm.hpp"

#include <fstream>
#include <iterator>

using namespace org::mcss;

std::string trace_to_str(const std::vector<int> &trace) {
  std::stringstream ss;
  if (!trace.empty()) {
    std::copy(trace.begin(), trace.end() - 1,
              std::ostream_iterator<int>(ss, ","));
    ss << trace.back();
  }
  return ss.str();
}

std::vector<int> trace_from_str(const std::string &trace_str) {
  std::vector<int> trace;
  std::stringstream ss(trace_str);
  for (int i; ss >> i;) {
    trace.push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }
  return trace;
}

std::vector<int> load_trace(const std::string &trace_dump) {
  std::stringstream ss;
  std::ifstream ifs(trace_dump);
  if (ifs.is_open()) {
    ss << ifs.rdbuf();
  }
  auto trace = trace_from_str(ss.str());
  return trace;
}


void save_trace(const std::string &trace_dump, const std::vector<int> &trace) {
  std::ofstream ofs(trace_dump);
  if (ofs.is_open()) {
    ofs << trace_to_str(trace);
  }
}


void generate_normal_trace() {
  // Reference model: 5-state DTMC, labelled with 3 alphabet
  org::mcss::mc_random rand;
  // Original model:
  // We don't need HMM here due to 1-1 map from state to labal
  int states_count = 4;
  auto init_p = Eigen::VectorXd(states_count);
  auto transition_p = Eigen::MatrixXd(states_count, states_count);
  init_p << 0.25, 0.25, 0.25, 0.25;
  transition_p <<
    0.1, 0.3, 0.6, 0,
    0.3, 0.1, 0.6, 0,
    0.2, 0.1,   0, 0.7,
    0,  0, 1, 0;

  auto orig_dtmc = std::make_shared<org::mcss::dtmc>(states_count, init_p, transition_p);
  std::cout << "Original DTMC parameters: \n"
            << orig_dtmc->model_info() << std::endl;

  // Generate state trace
  auto trace_len = 10000;
  auto trace = std::vector<int>(trace_len);
  std::generate(trace.begin(), trace.end(),
                [&]() { return orig_dtmc->next_state(); });
  // Map trace of state to label
  std::unordered_map<int, int> label;
  label.insert(std::pair<int, int>(0, 0));
  label.insert(std::pair<int, int>(1, 1));
  label.insert(std::pair<int, int>(2, 2));
  // state 3 and 4 has the same label
  label.insert(std::pair<int, int>(3, 2));
  // Make label trace
  auto label_trace = std::vector<int>();
  std::transform(trace.begin(), trace.end(), std::back_inserter(label_trace),
                 [&](int state) { return label.at(state); });
  // Save trace
  save_trace("label_trace_100_normal.txt", label_trace);
}

void experiment_normal_trace() {
  // Estimate a HMM model which would emit the same trace
  org::mcss::mc_random rand;
  auto state_count = 10;
  auto alphabet_count = 3;
  auto init_p = rand.random_vector(state_count);
  auto transition_p = rand.random_matrix(state_count, state_count);
  auto emission_p = rand.random_matrix(state_count, alphabet_count);
  auto test_model = std::make_unique<org::mcss::hmm>(state_count, alphabet_count,
                                                     init_p, transition_p, emission_p);

  std::cout << "######## Test model with 3 hidden states ########" << std::endl;
  std::cout << ">>>Test model before fitting: \n"
            << test_model->model_info() << std::endl;

  //  auto label_trace = load_trace("label_trace_1000.txt");
  auto label_trace = load_trace("label_trace_100_normal.txt");
  test_model->fit(label_trace, 1000, 1e-4);

  std::cout << ">>>Test model after fitting: \n"
            << test_model->model_info() << std::endl;

  std::cout << "######## Test model with 2 hidden states ########" << std::endl;
  state_count = 2;
  alphabet_count = 3;
  init_p = rand.random_vector(state_count);
  transition_p = rand.random_matrix(state_count, state_count);
  emission_p = rand.random_matrix(state_count, alphabet_count);
  auto test_model_2 = std::make_unique<org::mcss::hmm>(
      state_count, alphabet_count, init_p, transition_p, emission_p);
  std::cout << ">>> Test model before fitting: \n"
            << test_model_2->model_info() << std::endl;

  test_model_2->fit(label_trace, 1000, 1e-6);

  std::cout << ">>>Test model after fitting: \n"
            << test_model_2->model_info() << std::endl;
}

std::shared_ptr<hmm> make_random_hmm(const int &state_count, const int &alphabet_count) {
  // Estimate a HMM model which would emit the same trace
  org::mcss::mc_random rand;
  auto init_p = rand.random_vector(state_count);
  auto transition_p = rand.random_matrix(state_count, state_count);
  auto emission_p = rand.random_matrix(state_count, alphabet_count);
  auto model = std::make_shared<org::mcss::hmm>(
      state_count, alphabet_count, init_p, transition_p, emission_p);
  return model;
}

void save_report() {
  
}

void experiment_abnormal_trace() {
  // Estimate a HMM model which would emit the same trace
  org::mcss::mc_random rand;
  auto state_count = 3;
  auto alphabet_count = 3;
  auto test_model = make_random_hmm(state_count, alphabet_count);

  std::stringstream report_ss;
  std::cout << "######## Test model with 3 hidden states ########" << std::endl;
  std::cout << ">>> Test model before fitting: \n"
            << test_model->model_info() << std::endl;
  auto label_trace = load_trace("label_trace_100_abnormal.txt");
  test_model->fit(label_trace, 1000, 1e-6);

  std::cout << ">>>Test model after fitting: \n"
            << test_model->model_info() << std::endl;

  std::cout << "######## Test model with 2 hidden states ########" << std::endl;
  state_count = 2;
  alphabet_count = 3;

  auto test_model_2 = make_random_hmm(state_count, alphabet_count);
  std::cout << ">>> Test model before fitting: \n"
            << test_model_2->model_info() << std::endl;

  test_model_2->fit(label_trace, 10000, 1e-6);

  std::cout << ">>>Test model after fitting: \n"
            << test_model_2->model_info() << std::endl;
}

int main(int argc, char **argv) {
  //generate_normal_trace();

  std::cout << "NORMAL CASE " << std::endl;
  experiment_normal_trace();

  std::cout << "ABNORMAL CASE " << std::endl;
  experiment_abnormal_trace();

  return EXIT_SUCCESS;
}
