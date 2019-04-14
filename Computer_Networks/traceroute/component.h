
/* Adam Bukowczyk, nr indeksu: 290841 */


#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <errno.h>
#include <cassert>
#include <regex>
#include <array>


int argument_valid ( int argc, const char* argv[] );
u_int16_t compute_icmp_checksum ( const void *buff, int length );
