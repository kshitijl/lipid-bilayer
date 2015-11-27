#ifndef BM_H

#define BM_H 0
#include <iostream>
#include <ctime>
#include <cassert>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <list>

using namespace std;

const int kissing_number = 4;

class Node;

class DEdge {
public:
    Node *a, *b;
    bool active;
    int fcap, bcap;

    DEdge(Node *_a, Node *_b);
    void deactivate();
    void activate();
    void set_cap_from(Node *x, int cap);
    int get_cap_from(Node *x);
    void push_unit_to(Node *x);

    void print();

    Node *neighbor_of(Node *x);
};

class Node {
public:
    int name;
    bool matched;
    bool red;
    vector<DEdge*> adj;
    set<DEdge*> active_edges;
    
     Node(int _name) {
        name = _name;
        matched = false;
        red = false;

        // TODO: reconsider this trivial optimization
        //adj.reserve(kissing_number);
    }

     // returns NULL if this node isn't matched
     // otherwise returns the red *node that is
     // getting unmatched
     Node *deactivate_active(bool am_i_becoming_red);
};

class TailPlacer {
private:
    void dfs_augment();
    bool bfs_augment();
public:
    vector<Node*> verts;
    vector<DEdge*> edges;
    set<int> unmatched_reds;
    int contacts;
    /* TODO: get_contacts
     */

    // construct an n*m 2d square lattice
    TailPlacer(int n, int m);

    void add_red(int name);
    void remove_red(int name);
    bool augment();
    bool decide();
    void print_status();
    bool try_point(int name);
    int contacts_with(int name);
};
#endif
