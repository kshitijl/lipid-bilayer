#include "bm.h"
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
