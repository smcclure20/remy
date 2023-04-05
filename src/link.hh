#ifndef LINK_HH
#define LINK_HH

#include <deque>
#include <vector>

#include "packet.hh"
#include "delay.hh"

// TODO: Clean this up

class Link
{
private:
  std::deque< Packet > _buffer;

  Delay _pending_packet;

  unsigned int _limit;

  int _packet_counter;

  double _counter_interval;

  double _last_reset;

public:
  Link( const double s_rate,
	const unsigned int s_limit )
    : _buffer(), _pending_packet( 1.0 / s_rate ), _limit( s_limit ), _packet_counter(0), _counter_interval(10), _last_reset(0) {}

  int get_buffer_size(void) { return _buffer.size(); }

  void clear_packet_counter(double tickno) {_packet_counter = 0; _last_reset = tickno;}
  double get_recent_util(void) {return (_packet_counter / _counter_interval) / ( 1 / _pending_packet.delay());}

  void set_int_fields(Packet & p) {p.queue_stat = get_buffer_size(); p.link_stat = get_recent_util();}

  void accept( Packet & p, const double & tickno ) noexcept {
    if ( _pending_packet.empty() ) {
      set_int_fields(p);
      _pending_packet.accept( p, tickno );
      _packet_counter++;
    } else {
      if ( _limit and _buffer.size() < _limit ) {
        
        _buffer.push_back( p );
      }
    }
  }

  template <class NextHop>
  void tick( NextHop & next, const double & tickno );

  double next_event_time( const double & tickno ) const { return std::min(_pending_packet.next_event_time( tickno ), _last_reset + _counter_interval); }

  std::vector<unsigned int> packets_in_flight( const unsigned int num_senders ) const
  {
    std::vector<unsigned int> ret( num_senders );
    for ( const auto & x : _buffer ) {
      ret.at( x.src )++;
    }
    std::vector<unsigned int> propagating = _pending_packet.packets_in_flight( num_senders );
    for ( unsigned int i = 0; i < num_senders; i++ ) {
      ret.at( i ) += propagating.at( i );
    }
    return ret;
  }

  void set_rate( const double rate ) { _pending_packet.set_delay( 1.0 / rate ); }
  double rate( void ) const { return 1.0 / _pending_packet.delay(); }
  void set_limit( const unsigned int limit )
  {
    _limit = limit;
    while ( _buffer.size() > _limit ) {
      _buffer.pop_back();
    }
  }
};

#endif
