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
    all_dvs.clear();
    routing.clear();
    update = false;
}

bool Table::UpdateLink(int lat, int src, int dest){

    if()

    topo[src][dest].cost = lat;

    dv[dest].cost = lat;

}

bool Table::Calculate(){



}

bool Table::UpdateMessage(int src, map <int, int> d){



}

ostream & Table::Print(ostream &os) const
{
  os << "DistanceVector Table()";
  return os;
}


#endif
