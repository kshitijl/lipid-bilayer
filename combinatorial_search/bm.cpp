#include <iostream>
#include <ctime>
#include <cassert>
#include <map>
#include <queue>
#include <vector>

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
    vector<DEdge*> adj;
    
    Node(int _name) {
        name = _name;
        matched = false;

        // TODO: reconsider this trivial optimization
        //adj.reserve(kissing_number);
    }

    void deactivate_active(bool am_i_becoming_red);
};

class TailPlacer {
public:
    vector<Node*> verts;
    vector<DEdge*> edges;
    map<int, Node*> reds;
    int matched_reds;
    /* TODO: get_contacts
       Keep track of matched-unmatched better
     */

    // construct an n*m 2d square lattice
    TailPlacer(int n, int m);

    void add_red(int name);
    void remove_red(int name);
    bool augment();
    void dfs_augment();
    bool bfs_augment();
    bool decide();
    void print_status();
};
//01206611411

/*
  0 1 2  3
  4 5 6  7
  8 9 10 11
 */

int main() {
    TailPlacer tp(3,4);
    int pts[] = {1, 5, 9, 8, 6, -1};
//    int pts[] = {1, 2, 5, 6, 9, -1};
    for(int i = 0; pts[i] != -1; i++) {
        tp.add_red(pts[i]);
        cout << "Decision: " << tp.decide() << "\n";
    }
    tp.print_status();
    
    cout << "\n\n";
    tp.remove_red(6);
    cout << "Decision: " << tp.decide() << "\n";
    tp.add_red(2); 
    cout << "Decision: " << tp.decide() << "\n";
    tp.remove_red(8);
    cout << "Decision: " << tp.decide() << "\n";
    tp.print_status();
    tp.add_red(6);
    cout << "Decision: " << tp.decide() << "\n";
    tp.print_status();
    tp.remove_red(1);
    tp.remove_red(5);
    tp.remove_red(9);
    tp.remove_red(6);
    tp.remove_red(2);
    for(int i = 0; pts[i] != -1; i++) {
        tp.add_red(pts[i]);
        cout << "Decision: " << tp.decide() << "\n";
    }
    tp.remove_red(6);
    cout << "Decision: " << tp.decide() << "\n";
    tp.print_status();
    /*Node *k = tp.verts[4];
    vector<DEdge*>::iterator it;
    for(it = k->adj.begin(); it != k->adj.end(); it++) {
        (*it)->print();
        }*/
    return 0;
}

DEdge::DEdge(Node *_a, Node *_b) {
    a = _a; b = _b;
    active = false;
    fcap = bcap = 0;

    //cout << a->name << ", " << b->name << "\n";

    (a->adj).push_back(this);
    (b->adj).push_back(this);
}

void DEdge::deactivate() {
    active = false;
    //a->matched = false;
    //b->matched = false;
    fcap = bcap = 0;
}

TailPlacer::TailPlacer(int n, int m) {    
    verts.reserve(n*m);
    matched_reds = 0;
        
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name = i*m + j;

            verts.push_back(new Node(name));
        }
    }
        
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name1, name2;
            Node *a, *b;
            if(j < m-1) {
                // add an edge from (i, j) to (i, j+1)
                name1 = i*m + j; name2 = i*m + (j+1);
                a = verts[name1]; b = verts[name2];
                assert(a->name == name1);
                assert(b->name == name2);
                edges.push_back(new DEdge(a, b));
            }

            if(i < n-1) {
                // add an edge from from (i,j) to (i+1, j)
                name1 = i*m + j; name2 = (i+1)*m + j;
                a = verts[name1]; b = verts[name2];
                assert(a->name == name1);
                assert(b->name == name2);
                edges.push_back(new DEdge(a, b));
            }
        }
    }
}

void Node::deactivate_active(bool x) {
    vector<DEdge*>::iterator it;

    for(it = adj.begin(); it != adj.end(); it++) {
        if((**it).active) {
            Node *nbr = (*it)->neighbor_of(this);
            if(matched) {
                if((x && (*it)->get_cap_from(this) != 0) ||
                   (!x && (*it)->get_cap_from(nbr) != 0)) {
                    matched = false;
                    nbr->matched = false;
                }
            }
            (*it)->deactivate();
        }
    }
}

void DEdge::activate() {
    active = true;
}

Node *DEdge::neighbor_of(Node *x) {
    if(x == a)
        return b;
    if(x == b)
        return a;
    cout << a->name << ", " << b->name << ", " << x->name << "\n";
    return NULL;
}
 

void DEdge::set_cap_from(Node *x, int cap) {
    assert(x == a || x == b);

    if(x == a)
        fcap = cap;
    else
        bcap = cap; 
}

int DEdge::get_cap_from(Node *x) {
    assert(x == a || x == b);

    if(x == a)
        return fcap;
    return bcap;
}

void DEdge::push_unit_to(Node *x) {
    assert(x == a || x == b);
    cout << "Pushed to " << x->name << "\n";
    cout.flush();

    if(x == b) {
        bcap++;
        fcap--;
    }
    else {
        fcap++;
        bcap--;
    }

    assert(bcap <= 1 && bcap >= 0);
    assert(fcap <= 1 && fcap >= 0);
}

bool TailPlacer::augment() {
    //dfs_augment();
    return bfs_augment();
}

bool TailPlacer::bfs_augment() {
    queue<Node*> bfsq;
    map<Node*, DEdge*> from_edge;

    cout << "Unmatched reds are: ";

    // 1. Put all unmatched reds in the queue
    map<int, Node*>::iterator it;
    for(it = reds.begin(); it != reds.end(); it++) {
        Node *k = it->second;
        if(k->matched)
            continue;
        bfsq.push(k);
        cout << k->name << ", ";
        from_edge[k] = NULL;
    }

    cout << "\n";

    Node *cur_node;
    
    while(!bfsq.empty()) {
        cout << "k"; cout.flush();
        cur_node = bfsq.front(); bfsq.pop();

        if(!(cur_node->matched) && reds.find(cur_node->name) == reds.end()) {
            // push a unit of flow along the path back and return
            matched_reds++;
            DEdge *path_edge;
            Node *prev = cur_node;
            for(path_edge = from_edge[prev]; path_edge != NULL;) {
                path_edge->push_unit_to(prev);
                prev = path_edge->neighbor_of(prev);
                path_edge = from_edge[prev];
            }
            prev->matched = true;
            cur_node->matched = true;
            cout << cur_node->name << " and " << prev->name << " are matched now.\n";
            return true;
        }
        else {
            vector<DEdge*>::iterator it;
            for(it = cur_node->adj.begin(); it != cur_node->adj.end(); it++) {
                if(!(*it)->active)
                    continue;
                if((*it)->get_cap_from(cur_node) <= 0)
                    continue;
                Node *next = (*it)->neighbor_of(cur_node);
                bfsq.push(next);
                from_edge[next] = *it;                
            }
        }
    }

    return false;
}

void TailPlacer::dfs_augment() {
    
}

bool TailPlacer::decide() {
    if(augment())
        cout << "Found an augmenting path during decision.\n";
    
    map<int,Node*>::iterator it;
    for(it = reds.begin(); it != reds.end(); it++) {
        if(!(it->second)->matched)
            return false;
    }

    return true;
}

void TailPlacer::add_red(int name) {
    assert(reds.find(name) == reds.end());

    Node *p = verts[name];
    
    // 1. deactivate all active edges
    p->deactivate_active(true);

    // 2. put node in LHS
    reds[name] = p;

    // 3. for each neighbor that isn't red...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        if(reds.find(nbr->name) != reds.end()) // if that neighbor is red ...
            continue;

        // at this point nbr is a non-red neighbor

        // activate this edge and set capacity to 1 outward
        (*it)->activate();
        (*it)->set_cap_from(p, 1);
    }

    // 4. augment
    augment();
}

void TailPlacer::remove_red(int name) {
    assert(reds.find(name) != reds.end());

    Node *p = verts[name];

    // 1. deactivate all active edges
    p->deactivate_active(false);

    // 2. remove node from LHS
    reds.erase(name);

    // 3. for each red neighbor ...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        if(reds.find(nbr->name) == reds.end())
            continue;

        // at this point nbr is a red neighbor

        // activate this edge and set capacity 1 inward
        (*it)->activate();
        (*it)->set_cap_from(nbr, 1);        
    }

    // 4. augment
    augment();
}

void DEdge::print() {
    cout << "(" << a->name << ", " << b->name << ") ";
}

void TailPlacer::print_status() {
    map<int,Node*>::iterator it;
    cout << "Reds are: ";
    for(it = reds.begin(); it != reds.end(); it++) {
        cout << it->first << ", ";
    }
    cout << "\n\n";
}
