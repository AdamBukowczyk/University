
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "component.h"

using namespace std;

/* walidacja argumentów funkcji. Zgodnie ze specyfikacją - adres IPv4 */
int argument_valid ( int argc, const char* argv[] )
{
	if( argc != 2 )
		throw runtime_error( " Arguments amount error " );

	unsigned int valid1, valid2, valid3, valid4;

	regex rgx( "[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}\\.[[:digit:]]{1,3}" );
	if( regex_match( argv[ 1 ], rgx ) == false )
		throw runtime_error( " IP address format error " );

	if( sscanf( argv[ 1 ], "%d.%d.%d.%d ", &valid1, &valid2, &valid3, &valid4 ) == EOF )
		throw runtime_error( " Argv reading error " );

	if( valid1 > 255 || valid2 > 255 || valid3 > 255 || valid4 > 255  )
		throw runtime_error( " Wrong IP bytes values " );

	return 1;
}
/* funkcja obliczająca sumę kontrolną dla nagłówka */
u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = (const u_int16_t*) buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}
