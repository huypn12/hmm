#ifndef __MARKOV_HPP__
#define __MARKOV_HPP__

namespace mcss {
  class markov {
    public:
      virtual int next_state();
      virtual int get_current_state();
      virtual int get_previous_state();
  };
} // org::mcss

#endif
