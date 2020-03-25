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
    int current_stop = 0;
    int n = stops.size();

    int refilled_at_distance = 0;

    if (tank >= dist) {
        return 0;
    }

    while (current_stop < n)
    {
        while(current_stop < n && stops[current_stop] - refilled_at_distance <= tank) {
            current_stop = current_stop + 1;
        }

        if(refilled_at_distance == stops[current_stop - 1]) 
        {
            return -1;
        }

        if(current_stop <= n){
            refills = refills + 1;
            refilled_at_distance = stops[current_stop - 1];

            if(refilled_at_distance + tank < dist && current_stop == n)  
            { 
                return -1;
            }
        }
    }
    

    return refills;
}

int minRefill(int dist, int tank, vector<int> & stops) {
    int refills = 0;
    int current_distance = 0;
    stops.insert(stops.begin(), 0);
    int n = stops.size();
    vector<int> legs = {};

    if(tank >= dist) {
        return 0;
    }

    for(int i = 0; i < n; i++)
    {
        if(i == n - 1) {
            legs.push_back(dist - stops[n-1]);
        } else {
            int distance = stops[i + 1] - stops[i];
            legs.push_back(distance);
        }
    }

    int distSinceRefill = 0;
    int index = 0;
    while(index < legs.size()) {

        if(legs[index] > tank) {
            return -1;
        }

        if (distSinceRefill + legs[index] <= tank) {
            distSinceRefill = distSinceRefill + legs[index];
        } else {
            refills = refills + 1;
            distSinceRefill = 0;
            index--;
        }

        index++;
    }

    return refills;
}

// void test_solution() {
//     int dist1 = 950;
//     int tank1 = 400;
//     vector<int> stops1 = { 200, 375, 550, 750 };
//     cout << compute_min_refills(dist1, tank1, stops1) << "\n";
//     cout << minRefill(dist1, tank1, stops1) << "\n";

//     cout << "=================\n";

//     int dist2 = 10;
//     int tank2 = 3;
//     vector<int> stops2 = { 1, 2, 5, 9 };
//     cout << compute_min_refills(dist2, tank2, stops2) << "\n";
//     cout << minRefill(dist2, tank2, stops2) << "\n";
//     cout << "=================\n";

//     int dist3 = 200;
//     int tank3 = 250;
//     vector<int> stops3 = { 100, 150 };
//     cout << compute_min_refills(dist3, tank3, stops3) << "\n"; 
//     cout << minRefill(dist3, tank3, stops3) << "\n"; 
//     cout << "=================\n";

//     int dist4 = 500;
//     int tank4 = 200;
//     vector<int> stops4 = { 100, 200, 300, 400 };
//     cout << compute_min_refills(dist4, tank4, stops4) << "\n";
//     cout << minRefill(dist4, tank4, stops4) << "\n";
//     cout << "=================\n";

//     int dist5 = 100;
//     int tank5 = 30;
//     vector<int> stops5 = { 10, 25, 50, 75 };
//     cout << compute_min_refills(dist5, tank5, stops5) << "\n";
//     cout << minRefill(dist5, tank5, stops5) << "\n";
//     cout << "=================\n";

//     int dist6 = 750;
//     int tank6 = 300;
//     vector<int> stops6 = { 200, 575, 650 };
//     cout << compute_min_refills(dist6, tank6, stops6) << "\n";
//     cout << minRefill(dist6, tank6, stops6) << "\n";
//     cout << "=================\n";

//     int dist7 = 700;
//     int tank7 = 200;
//     vector<int> stops7 = { 100, 200, 300, 400 };
//     cout << compute_min_refills(dist7, tank7, stops7) << "\n";
//     cout << minRefill(dist7, tank7, stops7) << "\n";
//     cout << "=================\n";

//     while(true){
//         int randomDist = rand() % 100;
//         int randomTank = rand() % 100;
//         cout<< "dist: "<< randomDist << "\n";
//         cout<< "tank: " << randomTank << "\n";
//         vector<int> randomStops = {};
//         for (int i = 0; i < rand() % 10 + 2; i++) {
//             int randomStop = rand() % (randomDist - 1);
//             randomStops.push_back(randomStop);
//         }
//         sort(randomStops.begin(), randomStops.end());
        
//         for(int j = 0; j < randomStops.size(); j++) {
//             cout<< randomStops[j] << " ";
//         }

//         int compute_min_refills_res = compute_min_refills(randomDist, randomTank, randomStops);
//         int minRefill_res = minRefill(randomDist, randomTank, randomStops);
//         if (compute_min_refills_res == minRefill_res) {
//             cout << "\nOK!" << "\n";
//             cout << "\n=================\n";
//         } else {
//             cout << "\nWrong result: " << compute_min_refills_res << " vs " << minRefill_res << "\n";;
//             cout << "\n=================\n";
//             break;
//         }
//     }
// }


int main() {
    int d = 0;
    cin >> d;
    int m = 0;
    cin >> m;
    int n = 0;
    cin >> n;

    vector<int> stops(n);
    for (size_t i = 0; i < n; ++i)
        cin >> stops.at(i);

    cout << minRefill(d, m, stops) << "\n";

    return 0;
}
