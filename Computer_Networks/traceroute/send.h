
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "component.h"

class Sender
{
	/* instancja klasy Sender to adres przywiązany do konkretnego adresu IP */
	struct sockaddr_in recipient;
	
public:

	Sender( const char* ip_addr );

	void send_packet( int ttl, const int& socket, int id, int seq );
	
};

