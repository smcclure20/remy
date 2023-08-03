#ifndef LINK_HH
#define LINK_HH

#include <deque>
#include <vector>
#include <numeric>
#include <cmath>

#include "packet.hh"
#include "delay.hh"

// TODO: Clean this up

class Link
{
private:
  std::deque< Packet > _buffer;

  Delay _pending_packet;

  unsigned int _limit;

  double _counter_interval;

  unsigned int _packet_counters[10];

  unsigned int _current_bucket;

public:
  Link( const double s_rate,
	const unsigned int s_limit )
    : _buffer(), _pending_packet( 1.0 / s_rate ), _limit( s_limit ), _counter_interval(10),  _packet_counters(), _current_bucket(0) {}

  int get_buffer_size(void) { return _buffer.size(); }

  double get_recent_util() {
    double total_count = 0;
    total_count = std::accumulate(_packet_counters, _packet_counters + 10, total_count);
    return ( total_count / _counter_interval) / ( 1 / _pending_packet.delay()); // This can be off by at most 10% if the current bucket just changed
  }

  void add_packet_to_counter(double tickno) {
    unsigned int tick_bucket = ((int) std::floor(tickno / (_counter_interval / 10))) % 10;
    while (tick_bucket > _current_bucket)
    {
      _current_bucket += 1;
      _packet_counters[_current_bucket] = 0;
    }
    _packet_counters[_current_bucket] += 1;
  }

  void set_int_fields(Packet & p) {p.queue_stat = get_buffer_size(); p.link_stat = get_recent_util();}

  void accept( Packet & p, const double & tickno ) noexcept {
    if ( _pending_packet.empty() ) {
      set_int_fields(p);
      _pending_packet.accept( p, tickno );
      add_packet_to_counter(tickno);
    } else {
      if ( _limit and _buffer.size() < _limit ) {
        
        _buffer.push_back( p );
      }
    }
  }

  template <class NextHop>
  void tick( NextHop & next, const double & tickno );

  double next_event_time( const double & tickno ) const { return _pending_packet.next_event_time( tickno ); }

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
