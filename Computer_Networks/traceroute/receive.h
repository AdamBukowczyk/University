
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "component.h"
#include "packet.h"

class Receiver
{
	int* socketfd; /* gniazdo z którego dany Receiver ma odbierać pakiety */

public:
	
	Receiver( int* socketfd );

	Packet receive_packet();

};