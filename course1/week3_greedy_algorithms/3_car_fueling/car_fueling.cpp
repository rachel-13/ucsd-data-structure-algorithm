#include <iostream>
#include <vector>
#include <cassert>
#define NDEBUG

using std::cin;
using std::cout;
using std::vector;
using std::max;

int compute_min_refills(int dist, int tank, vector<int> & stops) {
    
    int refills = 0;
    int current_refill = 0;
    int n = stops.size();

    int refilled_at_distance = 0;

    if (tank >= dist) {
        return 0;
    }

    while (current_refill < n)
    {
        while(current_refill < n && stops[current_refill] - refilled_at_distance <= tank) {
            current_refill = current_refill + 1;
        }

        if(refilled_at_distance == stops[current_refill - 1]) 
        {
            return -1;
        }

        if(current_refill <= n){
            refills = refills + 1;
            refilled_at_distance = stops[current_refill - 1];

            if(refilled_at_distance + tank < dist && current_refill == n)  
            { 
                return -1;
            }
        }
    }
    

    return refills;
}

void test_solution() {
    int dist1 = 950;
    int tank1 = 400;
    vector<int> stops1 = { 200, 375, 550, 750 };
    cout << compute_min_refills(dist1, tank1, stops1) << "\n";

    int dist2 = 10;
    int tank2 = 3;
    vector<int> stops2 = { 1, 2, 5, 9 };
    cout << compute_min_refills(dist2, tank2, stops2) << "\n";

    int dist3 = 200;
    int tank3 = 250;
    vector<int> stops3 = { 100, 150 };
    cout << compute_min_refills(dist3, tank3, stops3) << "\n"; 

    int dist4 = 500;
    int tank4 = 200;
    vector<int> stops4 = { 100, 200, 300, 400 };
    cout << compute_min_refills(dist4, tank4, stops4) << "\n";

    int dist5 = 100;
    int tank5 = 30;
    vector<int> stops5 = { 10, 25, 50, 75 };
    cout << compute_min_refills(dist5, tank5, stops5) << "\n";

    int dist6 = 750;
    int tank6 = 300;
    vector<int> stops6 = { 200, 575, 650 };
    cout << compute_min_refills(dist6, tank6, stops6) << "\n";

    int dist7 = 700;
    int tank7 = 200;
    vector<int> stops7 = { 100, 200, 300, 400 };
    cout << compute_min_refills(dist7, tank7, stops7) << "\n";
}


int main() {
    test_solution();
    // int d = 0;
    // cin >> d;
    // int m = 0;
    // cin >> m;
    // int n = 0;
    // cin >> n;

    // vector<int> stops(n);
    // for (size_t i = 0; i < n; ++i)
    //     cin >> stops.at(i);

    // cout << compute_min_refills(d, m, stops) << "\n";

    return 0;
}
