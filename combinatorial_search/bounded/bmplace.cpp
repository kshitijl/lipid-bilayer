#include "bm.h"

bool TailPlacer::augment() {
    //dfs_augment();
    return bfs_augment();
}

TailPlacer::~TailPlacer() {
    delete [] from_edge;
}

/*
  Each grid-specific constructor calls common_constructor, then
  1. creates n_verts vertices, giving each one a name; puts them in verts
      (I *think* these names have to be in sequence from 0..n_verts-1)
  2. creates all the edges necessary for the graph; puts them in edges
 */

void TailPlacer::common_constructor(int n_verts) {
    verts.reserve(n_verts);
    from_edge = new DEdge*[n_verts];
    unmatched_reds.reserve(n_verts);
    n_unmatched = 0;
    unmatched_index.resize(n_verts, -1);
    contacts = 0;    
}

void TailPlacer::add_edge(int name1, int name2) {
    Node *a, *b;
    a = verts[name1]; b = verts[name2];
    assert(a->name == name1);
    assert(b->name == name2);
    edges.push_back(new DEdge(a, b));
}

TailPlacer::TailPlacer(int n, int m, float f) {
    common_constructor(n*m);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name = i*m + j;

            verts.push_back(new Node(name));
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name1, name2;
            if(j < m-1) {
                // add an edge from (i, j) to (i, j+1)
                name1 = i*m + j; name2 = i*m + (j+1);
                add_edge(name1, name2);
            }

            if(i < n-1) {
                // add an edge from from (i,j) to (i+1, j)
                name1 = i*m + j; name2 = (i+1)*m + j;
                add_edge(name1, name2);
            }

            if(i > 0 && j < m-1) {
                name1 = i*m + j, name2 = (i-1)*m + (j+1);
                add_edge(name1, name2);
            }
        }
    }    
}

TailPlacer::TailPlacer(int n, int m) {
    common_constructor(n*m);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name = i*m + j;

            verts.push_back(new Node(name));
        }
    }
        
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            int name1, name2;
            if(j < m-1) {
                // add an edge from (i, j) to (i, j+1)
                name1 = i*m + j; name2 = i*m + (j+1);
                add_edge(name1, name2);
            }

            if(i < n-1) {
                // add an edge from from (i,j) to (i+1, j)
                name1 = i*m + j; name2 = (i+1)*m + j;
                add_edge(name1, name2);
            }
        }
    }
}

TailPlacer::TailPlacer(int n, int m, int o) {
    common_constructor(n*m*o);
    int prev_name = -1;
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            for(int k = 0; k < o; k++) {
                int name = k + j*o + i*m*o;
                assert(prev_name + 1 == name);
                prev_name = name;
                
                verts.push_back(new Node(name));
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            for(int k = 0; k < o; k++) {
                int name1, name2;

                if(k < o-1) {
                    name1 = k + j*o + i*m*o;
                    name2 = k+1 + j*o + i*m*o;
                    add_edge(name1, name2);
                }

                if(j < m-1) {
                    name1 = k + j*o + i*m*o;
                    name2 = k + (j+1)*o + i*m*o;
                    add_edge(name1, name2);
                }

                if(i < n-1) {
                    name1 = k + j*o + i*m*o;
                    name2 = k + j*o + (i+1)*m*o;
                    add_edge(name1, name2);
                }
            }
        }
    }
}

bool TailPlacer::bfs_augment() {
    queue<Node*> bfsq;

    memset(from_edge, NULL, verts.size()*sizeof(from_edge[0]));

    for(int it = 0; it < n_unmatched; it++) {
        bfsq.push(verts[unmatched_reds[it]]);
        from_edge[verts[unmatched_reds[it]]->name] = NULL;
    }

    Node *cur_node;
    
    while(!bfsq.empty()) {
        cur_node = bfsq.front(); bfsq.pop();

        if(!(cur_node->matched) && !(cur_node->red)) {
            // push a unit of flow along the path back and return
            DEdge *path_edge;
            Node *prev = cur_node;
            for(path_edge = from_edge[prev->name]; path_edge != NULL;) {
                path_edge->push_unit_to(prev);
                prev = path_edge->neighbor_of(prev);
                path_edge = from_edge[prev->name];
            }

            // TODO: clean unmatch removal code
            remove_from_unmatched(prev->name);

            prev->matched = true;
            cur_node->matched = true;
#ifdef DEBUG_BFS_AUGMENT
            cout << cur_node->name << " and " << prev->name << " are matched now.\n";
#endif
            return true;
        }
        else {
            for(int it = 0; it < cur_node->n_active; it++) {
                DEdge *active_edge = cur_node->active[it];
                if(active_edge->get_cap_from(cur_node) <= 0)
                    continue;
                Node *next = active_edge->neighbor_of(cur_node);
                if(from_edge[next->name] == NULL) {
                    bfsq.push(next);
                    from_edge[next->name] = active_edge;
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
    return n_unmatched == 0;
}

void TailPlacer::add_red(int name) {
    Node *p = verts[name];
    assert(!p->red);

    // 1. deactivate all active edges
    Node *unred = p->deactivate_active(true);

    if(unred) {
        add_to_unmatched(unred->name);
        assert(unred->red);
    }

    add_to_unmatched(name);

    // 2. put node in LHS
    p->red = true;

    contacts += p->adj.size();

    // 3. for each neighbor...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        nbr->red_nbrs++;
        
        if(nbr->red) // if that neighbor is red ...
            continue;
        
        contacts--;
        // at this point nbr is a non-red neighbor

        // activate this edge and set capacity to 1 outward
        (*it)->activate();
        (*it)->set_cap_from(p, 1);
    }

    // 4. augment
    while(n_unmatched > 0 && augment());
}

void TailPlacer::remove_red(int name) {
    Node *p = verts[name];
    assert(p->red);
    // 1. deactivate all active edges
    Node *unred = p->deactivate_active(false);

    if(unred)
    {
        add_to_unmatched(unred->name);
        assert(unred->red);
    }

    remove_from_unmatched(name);

    // 2. remove node from LHS
    p->red = false;

    // 3. for each neighbor ...
    vector<DEdge*>::iterator it;
    for(it = p->adj.begin(); it != p->adj.end(); it++) {
        Node *nbr = (*it)->neighbor_of(p);
        assert(nbr != NULL);

        nbr->red_nbrs--;

        if(!nbr->red) {
            continue;
        }
        --contacts;

        // at this point nbr is a red neighbor

        // activate this edge and set capacity 1 inward
        (*it)->activate();
        (*it)->set_cap_from(nbr, 1);        
    }

    // 4. augment
    while(n_unmatched > 0 && augment());
}

int TailPlacer::contacts_with(int name) {
    Node *p = verts[name];
    assert(!p->red);
    
    return contacts + p->red_nbrs;
}

void TailPlacer::add_to_unmatched(int name) {
    assert(unmatched_index[name] == -1);
    
    unmatched_reds[n_unmatched] = name;
    unmatched_index[name] = n_unmatched;
    n_unmatched++;
}

void TailPlacer::remove_from_unmatched(int name) {
    assert(n_unmatched > 0);

    int index = unmatched_index[name];

    assert(index != -1);
    assert(unmatched_reds[index] == name);
    
    unmatched_reds[index] = unmatched_reds[n_unmatched-1];
    unmatched_index[name] = -1;
    n_unmatched--;
}
