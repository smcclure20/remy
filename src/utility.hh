#ifndef UTILITY_HH
#define UTILITY_HH

#include <cmath>
#include <cassert>
#include <climits>
#include "simulationresults.pb.h"

class Utility
{
private:
  double _tick_share_sending;
  unsigned int _packets_received;
  unsigned int _packets_sent;
  double _total_delay;
  double _first_send_tick;

public:
  Utility( void ) : _tick_share_sending( 0 ), _packets_received( 0 ), _packets_sent( 0 ), _total_delay( 0 ), _first_send_tick( 0 ) {}

  Utility( double start_tick ) : _tick_share_sending( 0 ), _packets_received( 0 ), _packets_sent( 0 ), _total_delay( 0 ), _first_send_tick( start_tick ) {}

  void sending_duration( const double & duration, const unsigned int num_sending __attribute((unused))) { _tick_share_sending += duration;}
  void packets_sent(const unsigned int num_pkts) {_packets_sent += num_pkts;}
  void packets_received( const std::vector< Packet > & packets ) {
    _packets_received += packets.size();

    for ( auto &x : packets ) {
      assert( x.tick_received >= x.tick_sent );
      _total_delay += x.tick_received - x.tick_sent;
    }
  }

  /* returns throughput normalized to equal share of link */
  double average_throughput_normalized_to_equal_share( void ) const
  {
    if ( _tick_share_sending == 0 ) {
      return 0.0;
    }
    return double( _packets_received ) / _tick_share_sending;
  }

  double average_delay( void ) const
  {
    if ( _packets_received == 0 ) {
      return 0.0;
    }
    return double( _total_delay ) / double( _packets_received );
  }

  double utility( double last_sendable_tick = std::numeric_limits<double>::max() ) const
  {
    if ( _tick_share_sending == 0 ) {
      return 0.0;
    }

    if ( _packets_received == 0 ) {
      
      if ( _first_send_tick > last_sendable_tick ) {
        return 0.0;
      }
      // printf("First send tick: %f\n", _first_send_tick);
      // printf("Last sendable tick: %f\n", last_sendable_tick);
      return -INT_MAX;
    }

    return -1 * _tick_share_sending - (_packets_sent - _packets_received);
  }

  SimulationResultBuffers::UtilityData DNA() const {
    SimulationResultBuffers::UtilityData ret;
    ret.set_sending_duration( _tick_share_sending );
    ret.set_packets_received( _packets_received );
    ret.set_total_delay( _total_delay );
    return ret;
  }

};

#endif
