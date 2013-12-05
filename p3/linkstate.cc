#include "linkstate.h"
#include <set>
#include "context.h"

LinkState::LinkState(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

LinkState::LinkState(const LinkState & rhs) :
    Node(rhs)
{
    *this = rhs;
}

LinkState & LinkState::operator=(const LinkState & rhs) {
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState() {}


/** Write the following functions.  They currently have dummy implementations **/
void LinkState::LinkHasBeenUpdated(Link* l) {
    cerr << *this << ": Link Update: " << *l << endl;

    // Take these from link
    int lat = l->GetLatency();
    int src = l->GetSrc();
    int dest = l->GetDest();

    // deal with age
    int age = routing_table.UpdateLink(lat, src, dest);

    SendToNeighbors(new RoutingMessage(age, lat, src, dest));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m) {
    

    int age = m->age;
    int lat = m->latency;
    int src = m->src;
    int dest = m->dest;

    if(routing_table.UpdateTable(age, lat, src, dest)){
        SendToNeighbors(m);
    }


}

void LinkState::TimeOut() {
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* LinkState::GetNextHop(Node *destination) { 
    cerr << " (ignored)" << endl;
    int new_dest;

    // Algo goes here. If the table has changed recalculate path
    if(routing_table.update){

        set<int> workers;
        set<int> oldguys;
        map <int, TopoLink> neighbors;
        map <int, TopoLink>::const_iterator iterator;
        map <int, int>::const_iterator iter;
        map <int, int> parents;

        // Put src Node into the visited set.
        oldguys.insert(number);

        neighbors = routing_table.topo[number];

        // Put all neighbors into worker set. List all of neighbors parents. 
        // Set distances. (To costs since these are the src's neighbors)
        // set ready to 1. They are in the working set
        for(iterator = neighbors.begin(); iterator != neighbors.end(); iterator++){
            int new_num = iterator->first;

            parents[new_num] = number;
            routing_table.topo[number][new_num].dist = routing_table.topo[number][new_num].cost;
            routing_table.topo[number][new_num].ready = 1;

            workers.insert(new_num);
            
        }

        while(workers.size() > 0){            
            //cerr << "Loop" << endl;
            int src = -1;
            int dest = -1;

            // Find the vertex with the shortest distance that hasn't been visited
            routing_table.find(&src, &dest);

            // Check if visited. If so set inSet as false and start over.
            if(oldguys.count(dest) > 0){
                routing_table.topo[src][dest].ready = 0;
                continue;
            }

            // Setting up Paths
            parents[dest] = src;
            // Mark as visited
            oldguys.insert(dest);
            // remove from set
            workers.erase(dest);

            neighbors = routing_table.topo[dest];

            //Loop through all the neighbors
            // calculate distances and update them.
            for(iterator = neighbors.begin(); iterator != neighbors.end(); iterator++){

                int key = iterator->first;

                if(oldguys.count(key) <= 0){

                    workers.insert(key);

                    int neighbor_dist = routing_table.topo[dest][key].dist;
                    int new_dist = routing_table.topo[dest][key].cost + routing_table.topo[src][dest].dist;

                    if(neighbor_dist == -1){
                        routing_table.topo[dest][key].dist = new_dist;
                    }
                    else if(new_dist == -1 || neighbor_dist < new_dist){

                    }
                    else{
                        routing_table.topo[dest][key].dist = new_dist;
                    }

                    routing_table.topo[dest][key].ready = 1;

                }


            }

                    //for(iter = parents.begin(); iter != parents.end(); iter++){
                      //  cerr << "I am : " << iter->first << ": " << iter
                    //}
        }


        for(iter = parents.begin(); iter != parents.end(); iter++){

            int parent = iter->second;
            int cur = iter->first;
            int curnode = iter->first;

            while(parent != number){
                cur = parent;
                parent = parents[parent];
            }

            routing_table.routing[curnode] = cur;
        }
       

    }

    new_dest = routing_table.getRoute(destination->GetNumber());
    Node *n = new Node(new_dest, NULL, 0, 0);
    Node *ret = context->FindMatchingNode(const_cast<Node *> (n));

    return ret;
}

Table* LinkState::GetRoutingTable() {
    return &routing_table;
}

ostream & LinkState::Print(ostream &os) const { 
    Node::Print(os);
    return os;
}
