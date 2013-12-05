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

    // Change cost between you and your neighbor
    // Ping all neighbors that you have this path

    SendToNeighbors(new RoutingMessage());
}

void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;

    // Check path to destination node.
    // If this path is shorter, replace.
    // If it's not shorter, leave it the same.
    // If it's empty obviously add
    // routing map?
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
