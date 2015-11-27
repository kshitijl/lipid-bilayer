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

extern int kissing_number;

class Node;
class DEdge;

class DEdge {
public:
    Node *a, *b;
    int aplace, bplace;
    bool active;
    int fcap, bcap;

    DEdge(Node *_a, Node *_b);
    void deactivate();
    void activate();
    void set_place(Node *x, int place);
    void set_cap_from(Node *x, int cap);
    int get_cap_from(Node *x);
    void push_unit_to(Node *x);

    void print();

    Node *neighbor_of(Node *x);
};

class Node {
private:
    vector<DEdge*> to_deactivate;
public:
    int name;
    bool matched;
    bool red;

    vector<DEdge*> adj, active;
    int n_active;

    int red_nbrs;
    
    Node(int _name) {
        name = _name;
        matched = false;
        red = false;
        n_active = 0;
        red_nbrs = 0;

        adj.reserve(kissing_number);
        active.reserve(kissing_number);
        to_deactivate.reserve(kissing_number);
    }

    int add_active(DEdge *edge);
    void remove_active_at(int index);

    // returns NULL if this node isn't matched
    // otherwise returns the red *node that is
    // getting unmatched
    Node *deactivate_active(bool am_i_becoming_red);
};

class TailPlacer {
private:
    void dfs_augment();
    bool bfs_augment();
    DEdge **from_edge;

    void common_constructor(int n_verts);
    void add_edge(int name1, int name2);
public:
    vector<Node*> verts;
    vector<DEdge*> edges;

    // unmatched_reds: indices (into verts) of unmatched red guys
    // unmatched_index: indices (into unmatched_reds) of each vertex (-1 if not an unmatched red)
    vector<int> unmatched_reds, unmatched_index;
    int n_unmatched;

    int size_n, size_m;

    int contacts;

    // construct an n*m 2d square lattice
    TailPlacer(int n, int m);
    //construct an n*m 2d hexagonal lattice; f is ignored for now
    TailPlacer(int n, int m, float f);
    // construct n*m*o 3d cubic lattice
    TailPlacer(int n, int m, int o);
    ~TailPlacer();

    void add_red(int name);
    void remove_red(int name);
    bool augment();
    bool decide();
    void print_status();
    bool try_point(int name);
    int contacts_with(int name);
    int sum_highest_red_nbrs(int k);

    void add_to_unmatched(int name);
    void remove_from_unmatched(int name);
};
#endif