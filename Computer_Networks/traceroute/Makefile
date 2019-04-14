CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

traceroute: traceroute.o component.o receive.o send.o packet.o
	$(CXX) $(CXXFLAGS) -o traceroute traceroute.o component.o receive.o send.o packet.o

traceroute.o: traceroute.cpp

component.o: component.cpp component.h

receive.o: receive.cpp receive.h

send.o: send.cpp send.h

packet.o: packet.cpp packet.h

clean:
	$(RM) *.o

distclean:
	$(RM) traceroute *.0

.PHONY: clean