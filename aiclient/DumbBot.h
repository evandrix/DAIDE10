/**
 * DumbBot - A Bot which runs under the DAIDE project.
 *
 * This file contains the header for the Bot Class
 *
 * (C) David Norman 2003 david@ellought.demon.co.uk
 *
 * This software may be reused for non-commercial purposes without charge, and
 * without notifying the author. Use of any part of this software for commercial 
 * purposes without permission from the Author is prohibited.
 **/

#include "BaseBot.h"
#include "map_and_units.h"

class DumbBot : public BaseBot
{
public:
	DumbBot( void );
	~DumbBot();

	virtual void send_nme_or_obs( void );
	virtual void process_mdf_message( TokenMessage &incoming_message );
	virtual void process_sco_message( TokenMessage &incoming_message );
	virtual void process_now_message( TokenMessage &incoming_message );

private:
	typedef int WEIGHTING;
	typedef set<MapAndUnits::PROVINCE_INDEX> ADJACENT_PROVINCE_SET;
	typedef map<MapAndUnits::COAST_ID, WEIGHTING> PROXIMITY_MAP;
	typedef multimap<int, MapAndUnits::PROVINCE_INDEX, greater<int> > RANDOM_UNIT_MAP;
	typedef map<WEIGHTING, MapAndUnits::COAST_ID, greater<int> > DESTINATION_MAP;
	typedef map<MapAndUnits::PROVINCE_INDEX, MapAndUnits::PROVINCE_INDEX> MOVING_UNIT_MAP;

	enum { PROXIMITY_DEPTH = 10 };

	int get_power_index( Token power_token );
	WEIGHTING calculate_defence_value( int province_index );
	void calculate_factors( WEIGHTING proximity_attack_weight, WEIGHTING proximity_defence_weight );
	void calculate_destination_value( WEIGHTING *proximity_weight, WEIGHTING strength_weight, WEIGHTING competition_weight );
	void calculate_winter_destination_value( WEIGHTING *proximity_weight, WEIGHTING defence_weight );
	void generate_debug( void );
	void generate_movement_orders( void );
	void check_for_wasted_holds( MOVING_UNIT_MAP &moving_unit_map );
	void generate_retreat_orders( void );
	void generate_remove_orders( int disband_count );
	void generate_build_orders( int build_count );
	void generate_random_unit_list( MapAndUnits::UNIT_SET &units );
	int rand_no( int max_value );
	void log_debug( char *error, ... );

	// The attack value for each province.
	WEIGHTING m_attack_value[ MapAndUnits::MAX_PROVINCES ];

	// The defence value for each province
	WEIGHTING m_defence_value[ MapAndUnits::MAX_PROVINCES ];

	// The size of each power. This is a function of the power's SC count
	int m_power_size[ MapAndUnits::MAX_POWERS + 1 ];

	// The set of provinces adjacent to a given province
	ADJACENT_PROVINCE_SET m_adjacent_provinces[ MapAndUnits::MAX_PROVINCES ];

	// The proximity map. This contains the value of each coast at each depth.
	// It has been pointed out that "proximity" may be a misleading term to use.
	PROXIMITY_MAP m_proximity_map[ PROXIMITY_DEPTH ];

	// The strength of attack we have on the province
	WEIGHTING m_strength_value[ MapAndUnits::MAX_PROVINCES ];

	// The strength of the competition for this province from enemy units
	WEIGHTING m_competition_value[ MapAndUnits::MAX_PROVINCES ];

	// The value of each province
	PROXIMITY_MAP m_destination_value;

	// All our units which need orders, in a random order
	RANDOM_UNIT_MAP m_random_unit_map;

	// The weightings for each consideration. These are effectively constants, set
	// in the constructor. They could be loaded from file to allow DumbBot to be
	// tweaked
	WEIGHTING m_proximity_spring_attack_weight;
	WEIGHTING m_proximity_spring_defence_weight;
	WEIGHTING m_proximity_fall_attack_weight;
	WEIGHTING m_proximity_fall_defence_weight;
	WEIGHTING m_spring_proximity_weight[ PROXIMITY_DEPTH ];
	WEIGHTING m_spring_strength_weight;
	WEIGHTING m_spring_competition_weight;
	WEIGHTING m_fall_proximity_weight[ PROXIMITY_DEPTH ];
	WEIGHTING m_fall_strength_weight;
	WEIGHTING m_fall_competition_weight;
	WEIGHTING m_build_defence_weight;
	WEIGHTING m_build_proximity_weight[ PROXIMITY_DEPTH ];
	WEIGHTING m_remove_defence_weight;
	WEIGHTING m_remove_proximity_weight[ PROXIMITY_DEPTH ];
	WEIGHTING m_play_alternative;
	WEIGHTING m_alternative_difference_modifier;
	WEIGHTING m_size_square_coefficient;
	WEIGHTING m_size_coefficient;
	WEIGHTING m_size_constant;
};
