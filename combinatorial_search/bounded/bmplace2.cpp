#include "bm.h"

bool TailPlacer::augment() {
    //dfs_augment();
    return bfs_augment();
}

TailPlacer::TailPlacer(int n, int m) {    
    verts.reserve(n*m);
    unmatched_red_pointers.reserve(n*m);
    contacts = 0;
        
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

bool TailPlacer::bfs_augment() {
    queue<Node*> bfsq;
    map<Node*, DEdge*> from_edge;

    list<int>::iterator it;
    for(it = unmatched_reds.begin(); it != unmatched_reds.end(); it++) {
        bfsq.push(verts[*it]);
        from_edge[verts[*it]] = NULL;
    }

    Node *cur_node;
    
    while(!bfsq.empty()) {
        cur_node = bfsq.front(); bfsq.pop();

        if(!(cur_node->matched) && !(cur_node->red)) {
            // push a unit of flow along the path back and return
            DEdge *path_edge;
            Node *prev = cur_node;
            for(path_edge = from_edge[prev]; path_edge != NULL;) {
                path_edge->push_unit_to(prev);
                prev = path_edge->neighbor_of(prev);
                path_edge = from_edge[prev];
            }
            unmatched_reds.erase(unmatched_red_pointers[prev->name]);
            prev->matched = true;
            cur_node->matched = true;
#ifdef DEBUG_BFS_AUGMENT
            cout << cur_node->name << " and " << prev->name << " are matched now.\n";
#endif
            return true;
        }
        else {
            set<DEdge*>::iterator it;
            for(it = cur_node->active_edges.begin();
                it != cur_node->active_edges.end(); it++) {
                if((*it)->get_cap_from(cur_node) <= 0)
                    continue;
                Node *next = (*it)->neighbor_of(cur_node);
                if(from_edge.find(next) == from_edge.end()) {
                    bfsq.push(next);
                    from_edge[next] = *it;
                }
            }
        }
    }

    return false;
}

void TailPlacer::dfs_augment() {
    // TODO: fill stub
}

bool TailPlacer::decide() {
    return unmatched_reds.empty();
}

void TailPlacer::add_red(int name) {
    Node *p = verts[name];
    assert(!p->red);

    // 1. deactivate all active edges
    Node *unred = p->deactivate_active(true);

    if(unred) {
        unmatched_reds.push_back(unred->name);
        unmatched_red_pointers[unred->name] = --unmatched_reds.end();
        assert(unred->red);
    }

    // 2. put node in LHS
    p->red = true;

    contacts += p->adj.size();

    // 3. for each neighbor that isn't red...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        if(nbr->red) // if that neighbor is red ...
            continue;

        contacts--;
        // at this point nbr is a non-red neighbor

        // activate this edge and set capacity to 1 outward
        (*it)->activate();
        (*it)->set_cap_from(p, 1);
    }

    // 4. augment
    while(!unmatched_reds.empty() && augment());
}

void TailPlacer::remove_red(int name) {
    Node *p = verts[name];
    assert(p->red);
    // 1. deactivate all active edges
    Node *unred = p->deactivate_active(false);

    if(unred)
    {
        unmatched_reds.push_back(unred->name);
        unmatched_red_pointers[unred->name] = --unmatched_reds.end();
        assert(unred->red);
    }

    // 2. remove node from LHS
    p->red = false;

    // 3. for each red neighbor ...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        if(!nbr->red)
            continue;
        --contacts;
        // at this point nbr is a red neighbor

        // activate this edge and set capacity 1 inward
        (*it)->activate();
        (*it)->set_cap_from(nbr, 1);        
    }

    // 4. augment
    while(!unmatched_reds.empty() && augment());
}

int TailPlacer::contacts_with(int name) {
    Node *p = verts[name];
    assert(!p->red);
    int n_nbrs = p->adj.size(), ncons = contacts;

    set<DEdge*>::iterator it;

    for(it = p->active_edges.begin(); it != p->active_edges.end(); it++) {
        ncons++;
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);
        
        if(nbr->red)
            continue;
        ncons--;
    }

    return ncons;
}
