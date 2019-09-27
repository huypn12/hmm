#ifndef __MARKOV_HPP__
#define __MARKOV_HPP__

namespace org::mcss {
class Markov {
protected:
  static const int kBeginState = -1;
public:
  virtual int Next() = 0;
  virtual const int &current_state() = 0;
  virtual const int &previous_state() = 0;
};
} // namespace mcss

#endif
