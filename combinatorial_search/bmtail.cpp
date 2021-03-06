class GandT {
public:
    TailPlacer tp;
    int n;
    Point search_box;
    int lower_bound, upper_bound;
    vector<Point>solution;
    clock_t before, after;

    GandT(int _n, Point p) : search_box(p) { n = _n; lower_bound = 0; solution.reserve(n); }

    void begin() {
        Point first = Point(0,0);
        upper_bound = (3*n-4)/2;
        
        before = clock();
        solution[0] = first;
        tp.add_red(first);
        search(1);
    }
    
    bool search(int level) {
        if(lower_bound == upper_bound)
          return true;
        if(!tp.decide())
            return false;
        // TODO: if bounded return false
        if(level == n) {
            if(tp.get_contacts() >= lower_bound) {
                after = clock();
                cout << "Found solution with " << tp.get_contacts()
                     << " at " << (double)(after-before)/CLOCKS_PER_SEC << "\n";
                cout << "Time spent on DLX: " << (double)tp.dlx_time/CLOCKS_PER_SEC << "\n";
                print_solution(tp, 2*n);
                cout << "\n";
                cout.flush();
                lower_bound = tp.get_contacts();
            }
            return true;
        }

        bool result = false;
        
        // get the point after the one placed in the last level
        Point place = solution[level-1];

        while(!(place == search_box)) {
            place = place.get_next(search_box);

            if(tp.contacts_with(place) > tp.get_contacts()) {
                tp.add_red(place);
                solution[level] = place;
                bool tmp  = search(level+1);
                result = result || tmp;
                tp.remove_red(place);
            }
        }

        return result;
    }
};

/*
With 20, (7, 80):
Found solution with 28 at 24.6655
Time spent on DLX: 8.03786
..........                              
..........  
 */
