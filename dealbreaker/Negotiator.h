#pragma once
#ifndef __NEGOTIATOR_H__
	#define __NEGOTIATOR_H__

#include <boost/thread/thread.hpp>
#include "error_log.h"
#include "tokens.h"
#include "map_and_units.h"
#include "types.h"

class DumbBot;	// forward declaration

class Negotiator
{
public:
	static Negotiator* getInstance(DumbBot* const thisBot);
	void Destroy();
	~Negotiator() { };
	void start();
	void reset();
	void processPress();
	bool is_allied_with(const Token power) const;
	void add_ally(const Token power);
	bool remove_ally(const Token power);

private:
	Negotiator(DumbBot* const thisBot);	// private constructor
	static Negotiator* _instance;
	DumbBot* const thisBot;
	boost::thread m_Thread;
	set< MapAndUnits::PROVINCE_DETAILS > our_centres;
	int adj_threat[MapAndUnits::MAX_POWERS + 1];
	int m_power_sc[MapAndUnits::MAX_POWERS + 1];
	Token allies[MapAndUnits::MAX_POWERS + 1];
	void Negotiator::calculate_threat ( const Token power ); // Calculates adjacent provinces and threats
};

#endif
