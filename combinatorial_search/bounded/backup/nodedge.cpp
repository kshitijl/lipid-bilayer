#include "bm.h"

void DEdge::activate() {
    active = true;
}

void DEdge::deactivate() {
    active = false;
    fcap = bcap = 0;
}

Node *Node::deactivate_active(bool x) {
    vector<DEdge*>::iterator it;
    Node *retval = NULL;
#ifdef DEBUG_DEACTIVATE_ACTIVE
    cout << "My name is " << name << "\n";
#endif
    for(it = adj.begin(); it != adj.end(); it++) {
#ifdef DEBUG_DEACTIVATE_ACTIVE
        cout << "k" << *it << " ";cout.flush();
#endif
        if((*it)->active) {
            Node *nbr = (*it)->neighbor_of(this);
            if(matched) {
                if((x && (*it)->get_cap_from(this) != 0) ||
                   (!x && (*it)->get_cap_from(nbr) != 0)) {
                    matched = false;
                    nbr->matched = false;
                    if(x)
                        retval = nbr;
                    else
                        retval = this;
                }
            }
            (*it)->deactivate();
        }
    }

    return retval;
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

DEdge::DEdge(Node *_a, Node *_b) {
    a = _a; b = _b;
    active = false;
    fcap = bcap = 0;

    (a->adj).push_back(this);
    (b->adj).push_back(this);

    //cout << "Edge from " << a->name << " to " << b->name << " at " << this << "\n";
}

void DEdge::print() {
    cout << "(" << a->name << ", " << b->name << ") : ["
         << fcap << ", " << bcap << "] ";
}

void TailPlacer::print_status() {
    /*TODOmap<int,Node*>::iterator it;
    cout << "Reds are: ";
    for(it = reds.begin(); it != reds.end(); it++) {
        cout << it->first << ", ";
    }
    cout << "\n\n";*/
}

bool TailPlacer::try_point(int name) {
    add_red(name);
    bool doable = decide();

    if(!doable)
        remove_red(name);

    return doable;
}
