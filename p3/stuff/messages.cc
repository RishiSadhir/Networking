#include "messages.h"

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs) {
    *this = rhs;
}

RoutingMessage & RoutingMessage::operator=(const RoutingMessage & rhs) {
    /* For now.  Change if you add data members to the struct */
    return *this;
}

#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
    os << "Generic RoutingMessage()";
    return os;
}
#endif

#if defined(LINKSTATE)

RoutingMessage::RoutingMessage(int new_age, int new_lat, int new_src, int new_dest){

	age = new_age;
	latency = new_lat;
	src = new_src;
	dest = new_dest;

}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "LinkState RoutingMessage()";
    return os;
}
#endif

#if defined(DISTANCEVECTOR)

RoutingMessage::RoutingMessage(int new_src, map<int, TopoLink> new_vex){

    src = new_src;
    vex = new_vex;    

}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "DistanceVector RoutingMessage()";
    return os;
}
#endif
