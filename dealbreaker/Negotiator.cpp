#include "stdafx.h"
#include "DumbBot.h"
#include "Negotiator.h"

Negotiator* Negotiator::_instance = NULL;

Negotiator* Negotiator::getInstance(DumbBot* const thisBot)
{
	if(_instance == NULL)
	{
		_instance = new Negotiator(thisBot);
#pragma message("<< Negotiator: Singleton instance created >>")
	}
	return _instance;
}
void Negotiator::Destroy() 
{
	delete _instance;
	_instance = NULL;
}
Negotiator::Negotiator(DumbBot* const thisBot) : thisBot(thisBot) {
	// the thread is not-a-thread until we call start()
}
void Negotiator::start() { 
	m_Thread = boost::thread(&Negotiator::processPress, this);
	// blocking function call for this until child thread terminates
	m_Thread.join();
}
void Negotiator::reset() { 
	// initialise internal arrays
	for (int i=0; i<MapAndUnits::MAX_POWERS + 1; i++)
	{
		allies[i]		= 0;
		m_power_sc[i]	= 0;
		adj_threat[i]	= 0;
	}
}

// invoked every turn
void Negotiator::processPress() {
	OutputDebugString("Negotiator::processPress()\n");

	// re-init all internal arrays
	reset();

	// count SC's for each power
	MapAndUnits* m_map_and_units = thisBot->get_map_and_units();
	calculate_threat(m_map_and_units->power_played);
/*
	for ( int province_counter = 0; province_counter < m_map_and_units->number_of_provinces; province_counter++ )
	{
		if ( m_map_and_units->game_map[ province_counter ].is_supply_centre )
		{
			Token owner = m_map_and_units->game_map[ province_counter ].owner;
			m_power_sc[ MapAndUnits::get_power_index(owner) ]++;
		}
	}

	// display SC tally
	for (int i=0; i<MapAndUnits::MAX_POWERS + 1; i++)
		if (m_power_sc[i] != 0)
			thisBot->_printf("SC count for %d: %d", i, m_power_sc[i]);

	// send peace proposal to all powers
	for ( int power_counter = 0; power_counter < m_map_and_units->number_of_powers; power_counter++ )
	{
		// do not send peace to yourself
		if ( ( power_counter != m_map_and_units->power_played.get_subtoken() ) )
		{
			thisBot->send_press_to_server(Token( CATEGORY_POWER, power_counter ),
				TOKEN_PRESS_PRP & (TOKEN_PRESS_PCE & 
				( Token(CATEGORY_POWER, power_counter) + m_map_and_units->power_played )
				));
		}
	}
*/
}

bool Negotiator::is_allied_with( const Token power ) const
{
	for (int i=0; i<MapAndUnits::MAX_POWERS + 1; i++)
	{
		if (allies[i] == power)
			return true;
	}
	return false;
}
void Negotiator::add_ally( const Token power )
{
	if (!is_allied_with(power))
	{
		for (int i=0; i<MapAndUnits::MAX_POWERS + 1; i++)
		{
			if (allies[i] == 0)
			{
				allies[i] = power;
				return;
			}
		}
	}
}
bool Negotiator::remove_ally( const Token power )
{
	for (int i=0; i<MapAndUnits::MAX_POWERS + 1; i++)
	{
		if (allies[i] == power)
		{
			allies[i] = 0;
			return true;
		}
	}
	return false;
}

void Negotiator::calculate_threat ( const Token power )
{
	MapAndUnits* m_map_and_units = thisBot->get_map_and_units();
	for ( int province_counter = 0; province_counter < m_map_and_units->number_of_provinces; province_counter++ )
		if (  m_map_and_units->game_map[ province_counter ].is_supply_centre )
			if (  m_map_and_units->game_map[ province_counter ].owner == power )
				our_centres.insert(m_map_and_units->game_map[ province_counter ]);

//	for (set<MapAndUnits::PROVINCE_DETAILS>::const_iterator p = our_centres.begin( ); p != our_centres.end( ); ++p)
	//	thisBot->_printf("%s\n", static_cast<MapAndUnits::PROVINCE_DETAILS>(*p).toString());


	set<MapAndUnits::PROVINCE_DETAILS>::const_iterator province_it;
	for(province_it = our_centres.begin(); province_it != our_centres.end(); ++province_it){
		MapAndUnits::PROVINCE_DETAILS province = *province_it;
		// Prints out our provinces
		thisBot->_printf("%s\n", "Province");
		thisBot->_printf("%s\n", province.toString());

		// Records which powers have units in provinces adjacent to us.
		MapAndUnits::PROVINCE_INDEX n = province.province_token.get_subtoken();
		set< MapAndUnits::COAST_ID > *adj_coasts = m_map_and_units->get_adjacent_coasts(n);
		if(adj_coasts != NULL){
			set<MapAndUnits::COAST_ID>::const_iterator coast_it;
			for(coast_it = adj_coasts->begin(); coast_it != adj_coasts->end(); ++coast_it){
				MapAndUnits::COAST_ID coast = *coast_it;
				if(m_map_and_units->game_map[coast.province_index].province_in_use){
					adj_threat[m_map_and_units->game_map[coast.province_index].owner.get_subtoken()]++;
				}

				// Print first level adjacent provinces
				thisBot->_printf("%s\n", "Adj1");
				thisBot->_printf("%s\n", m_map_and_units->game_map[coast.province_index].toString());

				// Does the same for provinces adjacent to our adjacent provinces.
				set< MapAndUnits::COAST_ID > *adj_coasts_two = m_map_and_units->get_adjacent_coasts(coast);
				if(adj_coasts_two != NULL){
					thisBot->_printf("%d\n", adj_coasts_two->size());
					set<MapAndUnits::COAST_ID>::const_iterator coast_it_two;
					for(coast_it_two = adj_coasts_two->begin(); coast_it_two != adj_coasts_two->end(); ++coast_it_two){
						MapAndUnits::COAST_ID coast_two = *coast_it_two;

						if(m_map_and_units->game_map[coast_two.province_index].province_in_use){
							adj_threat[m_map_and_units->game_map[coast_two.province_index].owner.get_subtoken()]++;
						}

						// Print secondary adjacent province + owner
						thisBot->_printf("%s\n", m_map_and_units->game_map[coast_two.province_index].toString());
						TokenTextMap* map = TokenTextMap::instance();
						thisBot->_printf("%s\n", map->m_token_to_text_map[ m_map_and_units->game_map[coast_two.province_index].owner ].c_str());
					}
				}
		/*		for(int i=0; i<MapAndUnits::MAX_POWERS + 1; i++){
					thisBot->_printf("%d\n", adj_threat[i]);
				}*/
			}
		}
	}
}
 