#include <utility>

#include "link.hh"

using namespace std;

template <class NextHop>
void Link::tick( NextHop & next, const double & tickno )
{
  if (tickno >= _last_reset + _counter_interval)
  {
    clear_packet_counter(tickno);
  }

  _pending_packet.tick( next, tickno );

  if ( _pending_packet.empty() ) {

    if ( not _buffer.empty() ) {
      Packet p = _buffer.front();
      set_int_fields(p);
      _pending_packet.accept( p, tickno );
      _buffer.pop_front();
      _packet_counter++;
    }
  }
}
