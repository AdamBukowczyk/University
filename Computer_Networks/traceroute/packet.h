
/* Adam Bukowczyk, nr indeksu: 290841 */

#ifndef packet_h
#define packet_h 

#include "component.h"
#include <set>

using namespace std;

class Packet
{
public:

	int packet_id;
	int packet_ttl;
	const char* packet_ip_addr;
	chrono::high_resolution_clock::time_point packet_time; /* czas utowrzenia pakietu */

	Packet( );

	Packet( int id, int ttl, const char* ip_addr, chrono::high_resolution_clock::time_point time );

	Packet& operator = ( const Packet& p );

};

bool packet_equal(Packet Packet1, Packet Packet2);

void print_route( array< Packet, 3 > received_packets, int received_packets_amount, chrono::high_resolution_clock::time_point& sent_time );

#endif /* packet_h */