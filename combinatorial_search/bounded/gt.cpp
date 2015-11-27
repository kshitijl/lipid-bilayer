#include "bm.h"

int kissing_number;

class GandT {
public:
    TailPlacer tp;
    int n; // size of problem: number of dipoles n
    int howmany;

    int lower_bound, upper_bound;

    vector<int> solution;

    clock_t before, after;

    int n_verts;

    /* 2d square
    GandT(int _n, int _w, int _h) : tp(_w, _h) {
        n = _n;
        solution.resize(n);
        howmany = 0;
        n_verts = _w*_h;
        kissing_number = 4;
    } */

    // 2d hexagonal
    GandT(int _n, int _w, int _h) : tp(_w, _h, 1.0f) {
        n = _n;
        solution.resize(n);
        howmany = 0;
        n_verts = _w*_h;
    }

    // 3d cubic
    GandT(int _n, int _w, int _h, int _d) : tp(_w, _h, _d) {
        n = _n;
        solution.resize(n);
        howmany = 0;
        n_verts = _w * _h * _d;
    }

    void begin(int first_vert, int first_bound) {
        upper_bound = ((kissing_number-1)*n)/2;
        lower_bound = first_bound;

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
            //cout << tp.contacts << " ";
            if(tp.contacts >= lower_bound) {
                after = clock();
                cout << "Found solution with " << tp.contacts
                     << " at " << (double)(after-before)/CLOCKS_PER_SEC
                     << " after " << howmany << " full combinations.\n";
                tp.print_status();
                cout << "\n";
                cout.flush();
                lower_bound = tp.contacts;                
            }
            
            howmany++;

            //cout << tp.contacts << " " << howmany << "\n"; cout.flush();
            return true;
        }

        int remaining = n-level;
        int bound = tp.contacts + remaining*(kissing_number - 1);
        int bound2 = bound-1;
        
        if(bound2 <= lower_bound || bound <= lower_bound)
            return false;

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

    int k,l,m,n,start_bound=0;

    if(argc == 2) {
        sscanf(argv[1], "%d", &k);
        GandT searcher(k, k/2, 2*k);
        searcher.begin(2*k+1,0);
    }
    else if (argc == 4) {
        sscanf(argv[1], "%d", &k);
        sscanf(argv[2], "%d", &l);
        sscanf(argv[3], "%d", &m);
        kissing_number = 6;
        GandT searcher(k,l,m);
        searcher.begin(m+1,0);
    }
    else if (argc == 6) {
        sscanf(argv[1], "%d", &k);
        sscanf(argv[2], "%d", &l);
        sscanf(argv[3], "%d", &m);
        sscanf(argv[4], "%d", &n);
        sscanf(argv[5], "%d", &start_bound);
        kissing_number = 6;
        GandT searcher(k,l,m,n);
        searcher.begin((l*m*n)/2,start_bound);
    }
    
    return 0;
}
