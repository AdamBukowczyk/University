
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "component.h"
#include "send.h"
#include "packet.h"
#include "receive.h"

using namespace std;

int main( int argc, const char* argv[] )
{
	/* walidacja argumentów programu */
	if(! argument_valid( argc, argv ) )
		return -1;

	const char* destination = argv[ 1 ];
	/* tworzenie gniazda surowego wraz ze sprawdzeniem powodzenia operacji */
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
	
	int pid = getpid(); /* process id - używane w nagłówkach wysyłanych i odbieranych pakietów do identyfikacji pakietów skierowanych do naszego progrmau */
	int seq_num = 0; /* zmienna seq_num jest używana jako numer sekwencyjny w nagłówkach pakietów */

	Sender s( destination ); 	/* przypisujemy adres IP który podaliśmy na wejściu do instancji klasy */
	Receiver r( &sockfd );	/* przekazujemy gniazdo surowe do klasy Receiver i tworzymy instancję klasy */

	array< Packet, 3 > received_packets_list;
	int received_packets_count = 0; /* zmienna wyznaczająca ilość otrzymanych interesujących nas pakietów w każdej iteracji pętli */

	bool destination_reach = false; /* wartość boolowska mówiąca, czy otrzymano odpowiedź od adresu docelowego */

	for( int ttl = 1 ; ttl <= 30 ; ttl ++ )
	{
		for( int i = 0 ; i < 3 ; ++ i )
			s.send_packet( ttl, sockfd, pid, seq_num ++ ); /* wysyłanie pakietu o obecnym ttl ; numer sekwencyjny musi być unikatowy dla każdego pakietu, by uniknąć ewentualnego potraktowania kolejnych pakieów jako dyplikaty */

		chrono::high_resolution_clock::time_point sent_time = chrono::high_resolution_clock::now( ); /* czas wysłania pakietu, czas startowy */
	
		Packet sent_packet ( pid, ttl, destination, sent_time ); /* zapamiętany wzór wysłanego pakietu */
		
		/* przygotowanie wartości dla funkcji select, wzięte z wykładu */
		fd_set descriptors;
		FD_ZERO( &descriptors );
		FD_SET( sockfd, &descriptors );
		struct timeval tv; tv.tv_sec = 1 ; tv.tv_usec = 0;

		/* odbieranie pakietów */
		while( received_packets_count < 3 )
		{
			int ready = select( sockfd + 1, &descriptors, NULL, NULL, &tv );

			if( ready < 0 ) /* Wystąpienie błędu */
				throw runtime_error( " Error: read from socket fail " );

			if( ready == 0 ) /* upłynięcie wyznaczonego czasu */
				break;

			else
			{
				Packet p = r.receive_packet(); /* odebrany pakiet */
				if( packet_equal( p, sent_packet ) ) /* sprawdzam, czy to pakiet który aktualnie wysłałem a nie np. jakiś o niższym ttl który dopiero przyszedł */
				{
					received_packets_list[ received_packets_count ++ ] = p; /* dodawanie pakietu do odebranych */
					if( ( string )( p.packet_ip_addr ) == destination ) /* sprawdzenie, czy osiągnęliśmy adres docelowy */
						destination_reach = true;							
				} 
			}
			/* funkcja select automatycznie dba o to, żeby nie blokować gniazda */
		}

		printf("%d.\t", ttl);
		print_route( received_packets_list, received_packets_count, sent_time );
		received_packets_count = 0;

		if(destination_reach == true)
			break;
	}


	return 0;
}