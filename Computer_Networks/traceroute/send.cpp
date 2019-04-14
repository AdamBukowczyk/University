
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "send.h"

using namespace std;
/* poniższe funkcje zostały stworzone na podstawie funkcji z wykładu */

Sender::Sender( const char* ip_addr )
{ 
	/* wpisywanie adresu odbiorcy do struktury adresowej */
	bzero( &recipient, sizeof( recipient ) );
	recipient.sin_family = AF_INET;
  inet_pton( AF_INET, ip_addr, &recipient.sin_addr );
}

void Sender::send_packet( int ttl, const int& socket, int id, int seq )
{
	/* wysyłanie pakietu o określonych wartościach */

	/* konstruowanie komunikatu ICMP do wysłania. */
	struct icmphdr icmp_header;
	icmp_header.type = ICMP_ECHO;
	icmp_header.code = 0;
	icmp_header.un.echo.id = id;
	icmp_header.un.echo.sequence = seq;
	icmp_header.checksum = 0;
	icmp_header.checksum = compute_icmp_checksum ( (u_int16_t*)&icmp_header, sizeof( icmp_header ) );


	/* ustawianie opcji gniazda */
	setsockopt ( socket, IPPROTO_IP, IP_TTL, &ttl, sizeof( int ) );
	/* wysyłanie pakietu przez gniazdo */
	ssize_t bytes_sent = sendto ( 
		socket, 
		&icmp_header, 
		sizeof( icmp_header ), 
		0, 
		( struct sockaddr* ) &recipient, 
		sizeof( recipient ) 
	);
	if( bytes_sent <= 0 )
		throw runtime_error( " Error - could not sent a packet " );
}
