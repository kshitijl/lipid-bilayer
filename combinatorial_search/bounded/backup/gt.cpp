#include "bm.h"

class GandT {
public:
    TailPlacer tp;
    int n; // size of problem: number of dipoles n
    int howmany;

    int lower_bound, upper_bound;

    vector<int> solution;

    clock_t before, after;

    int n_verts;
    
    GandT(int _n, int _w, int _h) : tp(_w, _h) {
        n = _n;
        solution.resize(n);
        howmany = 0;
        n_verts = _w*_h;
    }

    void begin(int first_vert) {
        upper_bound = (3*n-4)/2;
        lower_bound = 0;

        before = clock();

        solution[0] = first_vert;
        tp.add_red(first_vert);
        search(1);
    }

    bool search(int level) {
        //cout << level << "\n"; cout.flush();
        if(lower_bound == upper_bound)
            return true;
        if(!tp.decide())
            return false;
        if(level == n) {
            if(tp.contacts >= lower_bound) {
                after = clock();
                cout << "Found solution with " << tp.contacts
                     << " at " << (double)(after-before)/CLOCKS_PER_SEC << "\n";
                //tp.print_status();
                //cout << "\n";
                cout.flush();
                lower_bound = tp.contacts;                
            }
            howmany++;

            //cout << tp.contacts << " " << howmany << "\n"; cout.flush();
            return true;
        }

        bool result = false;

        int place;
        if(level < 1)
            place = -1;
        else
            place = solution[level-1];

        while(place < n_verts-1) {
            place++;
            int con_val = tp.contacts;
            if(con_val == 0 || tp.contacts_with(place) > con_val) {
                tp.add_red(place);
                solution[level] = place;
                bool tmp = search(level+1);
                result = result || tmp;
                tp.remove_red(place);
            }
        }

        return result;
    }
};

/*
  0 1 2  3
  4 5 6  7
  8 9 10 11
 */

int main(int argc, char **argv) {
    GandT searcher(20, 7, 40);

    searcher.begin(41);

    return 0;
}
