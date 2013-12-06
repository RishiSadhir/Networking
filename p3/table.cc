#include "table.h"

Table::Table(const Table & rhs) {
    *this = rhs;
}

Table & Table::operator=(const Table & rhs) {
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;

    return *this;
}

#if defined(GENERIC)
Table::Table(){
  topo.clear();
}

ostream & Table::Print(ostream &os) const
{
  os << "Generic Table()";  
  return os;
}
#endif

#if defined(LINKSTATE)
Table::Table() {
    topo.clear();
    update = false;
    routing.clear();
}

ostream & Table::Print(ostream &os) const
{
  os << "LinkState Table()";
  return os;
}

int Table::UpdateLink(int lat, int src, int dest){

  int currentage = topo[src][dest].age;
  currentage++;
  topo[src][dest].age = currentage;
  topo[src][dest].cost = lat;
  topo[src][dest].dist = lat;
  update = true;

  return currentage; 

}

bool Table::UpdateTable(int age, int lat, int src, int dest){

    update = true;

    int oldage = topo[src][dest].age;
    int oldcost = topo[src][dest].cost;

    if(oldcost == -1){
      topo[src][dest].cost = lat;
      topo[src][dest].age = age;
      topo[src][dest].dist = lat;

      return true;
    }
    if(oldage < age){
      topo[src][dest].cost = lat;
      topo[src][dest].age = age;
      topo[src][dest].dist = lat;

      return true;
    }

    return false;

}

int Table::getRoute(int dest){

  return routing[dest];

}

int Table::find(int *src, int *dest){

  map <int, map <int, TopoLink> >::const_iterator iterator;
  map <int, TopoLink>::const_iterator itr2;

  int smallest = -1;

  // Loops through all of topo
  // First check if it's in the set by checking ready == 1
  // If it is in the working set, the check if it has the smallest distance
  // If it is smaller, then set this as the new best node (dest)
  // Set src also because that is the previous node. (parent)
  for(iterator = topo.begin(); iterator != topo.end(); iterator++){
    for(itr2 = (iterator)->second.begin(); itr2 != (iterator)->second.end(); itr2++){

      if(itr2->second.ready == 1){
        if(itr2->second.dist < smallest || smallest == -1){
          smallest = itr2->second.dist;
          *dest = itr2->first;
          *src = iterator->first;
        }
      }

    }
  }

  return 1;

}



#endif

#if defined(DISTANCEVECTOR)
Table::Table() {
    topo.clear();
    dv.clear();
    routing.clear();
    cost.clear();
    dv_all.clear();
    update = false;
}

bool Table::UpdateLink(int lat, int src, int dest){

    // Set yourself to 0. Sure do it every time?
  // Probably set by default.
    dv[src] = 0;
    dv_all[src][src] = 0;
    routing[src] = src;

// Reset current cost of this neighbor with the new latency.
    cost[dest] = lat;

    // If this neighbor hasn't been set
    // Just update it in cost map as existing
    // and put it in local vector.
    if(dv[dest] == 0){
      dv[dest] = lat;
    }
    
    if(Calculate(src)){
      return true;
    }
    

// Check if the new cost to this destination is better 
    // than the current distance stored in our local vector
    // Do all this in calculate
    // If calculate updates our local distance vector
    // then send the message to everyoen that you have been updated.

    return false;

}

bool Table::Calculate(int src){
  update = false;

  map <int, int>::const_iterator i;
  map <int, int>::const_iterator ic;

  for(i = dv.begin(); i != dv.end(); i++){
    int dest = i->first;
    //int curcost = i->second;

    if(dest == src){
      dv[dest] = 0;
      continue;
    }

    int smallest = -1;
    int right_key = -1;

    for(ic = cost.begin(); ic != cost.end(); ic++){
      int key = ic->first;
      int neighbor_cost = ic->second;

      int new_cost = neighbor_cost + dv_all[key][dest];


      if(smallest == -1 || new_cost < smallest){
        smallest = new_cost;
        right_key = key;
      }

    } 

    if(i->second != smallest){
      dv[dest] = smallest;
      update = true;
      routing[dest] = right_key;
    }

  }

  return update;

}

bool Table::UpdateMessage(int me, int src, map <int, int> d){

  // Update the sender's dv with the new one
  dv_all[src] = d;

  if(Calculate(src)){
    return true;
  }

  return false;

}

int Table::getRoute(int dest){

  return routing[dest];

}

ostream & Table::Print(ostream &os) const
{
  os << "DistanceVector Table()";
  return os;
}


#endif
