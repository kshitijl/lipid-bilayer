#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

vector<int> table;
int twodb(int n) {
    if(table[n] != -1)
        return table[n];
    table[n] = 2*n - ceil(2*sqrt(n)); //floor(2*n - 2*sqrt(n));
    return table[n];
}

vector<vector<int> > dp;

void traceback(int n) {
    int m, j,k;

    int best = 0;
    for(j = 1; j <= n; j++) {
        if(best < dp[n][j]) {
            best = dp[n][j];
            k = j;
        }
    }

    m = n;
    while(m>0) {
        cout << k << " ";
        m = m - k;
        best = 0;
        int newk;
        for(j = m; j > 0; j--) {
            if(best < dp[m][j] + min(j,k)) {
                best = dp[m][j] + min(j,k);
                newk = j;
            }
        }
        k = newk;
    }
}

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
    table.resize(1024*1024*10,-1);
    int size = 50;
    
    if(argc == 2) {
        sscanf(argv[1], "%d", &size);
    }
    dp.push_back(vector<int>());
    for(int n = 1; n <= size; n++) {
        dp.push_back(vector<int>());
        dp[n].push_back(0);
        for(int k = 1; k <= n; k++) {
            int quantity = 0;
            for(int i = 1; i <= n-k; i++)
                quantity = max(quantity, dp[n-k][i] + min(i,k));
            
            dp[n].push_back(twodb(k) + quantity);
        }
        int answer = 0;
        for(int j = 1; j <= n; j++)
            answer = max(answer, dp[n][j]);
        //cout << n << " " << answer << " " << pow(n,1.0/3) <<  "\n";
        int foundk, calcans;
        foundk = findk(n);
        calcans = calc(n,foundk);

        if(answer != calcans && n != 1) {
            cout << "WHOOPS!\n";
            traceback(n);
            cout << "\n" << findk(n) << " " << calc(n,findk(n)) << "\n\n";
        }
    }
    return 0;
}
