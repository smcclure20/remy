#ifndef ROUTER_HH_
#define ROUTER_HH_

#include <vector>
#include <queue>
#include "random.hh"
#include "link.hh"
#include "packet.hh"
#include "receiver.hh"

class Router
{
private:
  Link & next_;
  Receiver & rec_;
  std::vector<int> _terminate_flow;
  PRNG & _prng;

public:
  Router(Link & next, Receiver & rec, int num_sources, PRNG & s_prng) : next_( next ), rec_( rec ), _terminate_flow(num_sources, -1), _prng(s_prng) {}

  void accept( Packet & p, const double & tickno ) noexcept;
};

#endif // ROUTER_HH_