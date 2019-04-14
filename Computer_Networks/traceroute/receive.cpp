
/* Adam Bukowczyk, nr indeksu: 290841 */

#include "receive.h"

using namespace std;

/* poniższe funkcje zostały stworzone na podstawie funkcji z wykładu */

Receiver::Receiver( int* socketfd_tmp )
{
	socketfd = socketfd_tmp;
}

Packet Receiver::receive_packet()
{
	/* odbieranie pakietu z gniazda */
	struct sockaddr_in 	sender;	
	socklen_t 			sender_len = sizeof(sender);
	u_int8_t 			buffer[IP_MAXPACKET];

  /* recvfrom odbiera kolejny pakiet z kolejki związanej z gniazdem */
	ssize_t packet_len = recvfrom (
    *socketfd, 
    buffer, /* pakiet jako ciąg bitów */
    IP_MAXPACKET,
    MSG_DONTWAIT, 
    (struct sockaddr*)&sender, /* informacje o nadawcy */
    &sender_len
  );
	if (packet_len < 0) {
		fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
		return Packet( );
	}

  /* zamienianie struktury adresowej w sender na napis z adresem IP */
	char sender_ip_str[20]; 
	inet_ntop(
    AF_INET, 
    &(sender.sin_addr), 
    sender_ip_str, 
    sizeof(sender_ip_str)
  );

  /* odczytywanie nagłówka IP */
	struct iphdr* 		ip_header = ( struct iphdr* ) buffer;
	u_int8_t*			icmp_packet = buffer + 4 * ip_header->ihl;

  /* odczytywanie nagłówka ICMP */
	struct icmphdr*		icmp_header = ( struct icmphdr* ) icmp_packet;
  
	//------------------------------------------//
	
	/* przygotowanie obiektu odebranego pakietu */

	/* jeśli dotarliśmy do adresu docelowego */
	if( icmp_header->type == ICMP_ECHOREPLY )
	{
		chrono::high_resolution_clock::time_point packet_time = chrono::high_resolution_clock::now( ); /* czas odebrania pakietu */
		/* każdy wysłany przez nas pakiet ma unikatowy numer sekwencyjny (nadawane po kolei). Żeby 
		uzyskać ttl z jakim został pakiet wysłany poprzez dzielenie całkowite przez ilość 
		pakietów i dodanie 1 (seq od 0 do 2 to ttl 1 itd.) */
		return Packet( 
			icmp_header->un.echo.id, 
			icmp_header->un.echo.sequence / 3 + 1, 
			sender_ip_str, 
			packet_time 
		);
	}

	/* jeśli czas życia pakietu się skończył (ttl = 0) */
	if( icmp_header->type == ICMP_TIME_EXCEEDED )
	{
		/* komunikaty ICMP time exceeded zawierają oryginalny nagłowek IP, trzeba więc
		przesunąć dane, aby dokopać się do interesujących nas informacji */
		icmp_packet += 8;
		icmp_packet += 4 * ((struct ip*) icmp_packet)->ip_hl;
		struct icmp* icmp_time_excedded = (struct icmp*) icmp_packet;

		chrono::high_resolution_clock::time_point packet_time = chrono::high_resolution_clock::now( );

		return Packet( 
			icmp_time_excedded->icmp_id, 
			icmp_time_excedded->icmp_seq / 3 + 1, 
			sender_ip_str, 
			packet_time );
	}

	return Packet();

}