#ifndef __MC_RANDOM_HPP__
#define __MC_RANDOM_HPP__

#include <random>
#include <optional>

namespace org::mcss {

class mc_random {
private:
  
protected:
  void init_seed();

public:
  int choose_dirichlet();
  int choice_uniform();
};

}

#endif //HMM_INCLUDE_MC_RANDOM_HPP
