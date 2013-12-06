#include "distancevector.h"

DistanceVector::DistanceVector(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

DistanceVector::DistanceVector(const DistanceVector & rhs) :
    Node(rhs)
{
    *this = rhs;
}

DistanceVector & DistanceVector::operator=(const DistanceVector & rhs) {
    Node::operator=(rhs);
    return *this;
}

DistanceVector::~DistanceVector() {}


/** Write the following functions.  They currently have dummy implementations **/
void DistanceVector::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;

    int lat = l->GetLatency();
    int src = l->GetSrc();
    int dest = l->GetDest();

    if(routing_table.UpdateLink(lat, src, dest)){
        SendToNeighbors(new RoutingMessage(number, routing_table.dv));
    }
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;

    int src = m->src;
    map<int, int> d = m->vex;

    if(routing_table.UpdateMessage(number, src, d)){
        SendToNeighbors(new RoutingMessage(number, routing_table.dv));
    }
}

void DistanceVector::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* DistanceVector::GetNextHop(Node *destination) { 

    // routing map?

    return NULL;
}

Table* DistanceVector::GetRoutingTable() {
    return NULL;
}

ostream & DistanceVector::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
