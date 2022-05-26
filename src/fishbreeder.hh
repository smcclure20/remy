#ifndef FISHBREEDER_HH
#define FISHBREEDER_HH

#include "breeder.hh"

class FinImprover : public ActionImprover< FinTree, Fin >
{
protected:
  std::vector< Fin > get_replacements( Fin & action_to_improve );

public:
  FinImprover( const Evaluator<  FinTree > & evaluator, const FinTree & fish, const double score_to_beat, const bool sample = false )
    : ActionImprover< FinTree, Fin >( evaluator, fish, score_to_beat, sample) {};
};

class FishBreeder : public Breeder< FinTree >
{
public:
  FishBreeder( const BreederOptions & s_options ) : Breeder( s_options ) {};

  Evaluator< FinTree >::Outcome improve( FinTree & fins );
};

#endif
