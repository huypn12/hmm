#ifndef __MARKOV_HPP__
#define __MARKOV_HPP__

namespace org::mcss {
class markov {
protected:
  static const int begin_state_ = -1;
public:
  virtual int next() = 0;
  virtual const int &current_state() = 0;
  virtual const int &previous_state() = 0;
};
} // namespace mcss

#endif
