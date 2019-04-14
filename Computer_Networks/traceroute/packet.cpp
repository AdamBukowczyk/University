
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "packet.h"

using namespace std;


Packet& Packet::operator = ( const Packet& p )
{
	this->packet_id = p.packet_id;
	this->packet_ttl = p.packet_ttl;
	this->packet_ip_addr = p.packet_ip_addr;
	this->packet_time = p.packet_time;

	return *this;
}

/* dwa pakiety są takie same, jeżeli mają ten sam id oraz zostały wysłane z tym samym ttl */
bool packet_equal(Packet Packet1, Packet Packet2)
{
	if( Packet1.packet_id == Packet2.packet_id && Packet1.packet_ttl == Packet2.packet_ttl )
		return true;
	return false;
}

Packet::Packet( int id, int ttl, const char* ip_addr, chrono::high_resolution_clock::time_point time )
{  
	packet_id = id;
	packet_ttl = ttl;
	packet_ip_addr = ip_addr;
	packet_time = time;
}

Packet::Packet( ) /* to jak coś się nie uda */
{ 
	packet_id = -42;
	packet_ttl = -42;
	packet_ip_addr = nullptr;
	packet_time = chrono::high_resolution_clock::now( ); 
}

void print_route( array< Packet, 3 > received_packets, int received_packets_amount, chrono::high_resolution_clock::time_point& sent_time )
{
	if( received_packets_amount == 0 ) /* brak otrzymanych pakietów */
	{
		printf("*\n");
		return;
	}
	/* uzyskiwanie zbioru adresów IP od których otrzymano odpowiedź */
	set< const char* > ip_addresses;
	for( auto i = received_packets.begin() ; i != received_packets.begin() + received_packets_amount ; i ++ )
		ip_addresses.insert( i->packet_ip_addr );
	/* wypisywanie adresów IP */
	for( const char* ip : ip_addresses )
		printf("%*s ", 5, ip );

	if( received_packets_amount == 3 ) /* otrzymano wszystkie odpowiedzi */
	{
		unsigned int avg_time = 0;
		for( Packet p : received_packets )
		{
			avg_time += chrono::duration_cast< chrono::milliseconds >( p.packet_time - sent_time ).count( );
		}
			
		avg_time /= 3;
		printf("     %ums\n", avg_time );
	}
	else
		printf("???\n");

}