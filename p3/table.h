#ifndef _table
#define _table

#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct TopoLink {
    TopoLink(): cost(-1), age(0), dist(-1), ready(0) {}

    TopoLink(const TopoLink & rhs) {
        *this = rhs;
    }

    TopoLink & operator=(const TopoLink & rhs) {
        this->cost = rhs.cost;
        this->age = rhs.age;
        this->dist = rhs.dist;
        this->ready = rhs.ready;

        return *this;
    }

    int cost;
    int age;
    int dist;
    int ready;
};

// Students should write this class table[a][b]
class Table {
    private:
        // Input node. receive another map. input another node. receive a topolink. do .age or something
        // topo[a] --> map  topo[a] [x] --> topolink  topo[a] [x] . age --> int
    public:
        Table();
        Table(const Table &);
        Table & operator=(const Table &);

        ostream & Print(ostream &os) const;
        map < int, map < int, TopoLink > > topo;

        // Anything else you need

        #if defined(LINKSTATE)
            map <int, int> routing;
            bool update;

            bool UpdateTable(int age, int lat, int src, int dest);
            int UpdateLink(int lat, int src, int dest);
            int getRoute(int dest);
            int find(int *src, int *dest);
            // routing[a] -- > next node

        #endif

        #if defined(DISTANCEVECTOR)

            map <int, map<int, int> > dv_all;
            map<int, int> dv;
            map<int, int> cost;
            map<int, int> routing;
            bool update;

            bool UpdateLink(int lat, int src, int dest);
            bool UpdateMessage(int me, int src, map<int, int> d);
            bool Calculate();

        #endif
};

inline ostream & operator<<(ostream &os, const Table & t) { return t.Print(os);}


#endif
