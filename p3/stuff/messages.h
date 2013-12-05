#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;

    // Anything else you need

    #if defined(LINKSTATE)

    	int age;
    	int latency;
    	int src;
    	int dest;

    	RoutingMessage(int new_age, int new_lat, int new_src, int new_dest);

    #endif
    #if defined(DISTANCEVECTOR)

        int src;
        map<int, int> vex;

        RoutingMessage(int new_src, map<int, int> new_vex);

    #endif
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif
