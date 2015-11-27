#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

int calc(int n, int k) {
    return 3*n - k - floor(n/k)*ceil(2*sqrt(k)) - ceil(2*sqrt(n%k));
}

int findk(int n) {
    int s = floor(pow(n,1.0/3));
    if(n <= s*s*(s+1))
        return s*s;
    if(n <= s*(s+1)*(s+1))
        return s*(s+1);
    else return (s+1)*(s+1);
}

int main(int argc, char **argv) {
    int size = 50;
    if(argc == 2) {
        sscanf(argv[1], "%d", &size);
    }
    unsigned long s = 1;
    
    for(int i = 3; i <= size; i++) {
        int k = findk(i), ans;
        ans = calc(i, k);
        s += ans;
        //cout << i << " " << calc(i, findk(i)) << "\n";
    }
    cout << s << "\n";
    return 0;
}
