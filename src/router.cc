#include <limits>
#include "router.hh"

void Router::accept( Packet & p, const double & tickno ) noexcept
{
  if (_terminate_flow.at(p.src) == -1)
  {
    _terminate_flow.at(p.src) = (_prng() % 100 > 50) ? 0 : 1;
  }

  if (_terminate_flow.at(p.src) == 1)
  {
    rec_.accept( p, tickno );
  }
  else 
  {
    next_.accept( p, tickno );
  }
}
