#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

// --------------------------------------------------
// Location structure
// --------------------------------------------------

struct Location {
    char name;
    double x;
    double y;
};


// --------------------------------------------------
// Calculate Euclidean distance between two locations
// --------------------------------------------------

double distanceBetween(Location a, Location b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return sqrt(dx * dx + dy * dy);
}


// --------------------------------------------------
// Calculate total route cost
// W -> all locations -> W
// --------------------------------------------------

double routeCost(vector<int> route, vector<Location> locations)
{
    double total = 0.0;

    // Start from Warehouse W
    int warehouse = 0;

    // W -> first delivery location
    total += distanceBetween(
        locations[warehouse],
        locations[route[0]]
    );

    // Between delivery locations
    for (int i = 0; i < route.size() - 1; i++)
    {
        total += distanceBetween(
            locations[route[i]],
            locations[route[i + 1]]
        );
    }

    // Last delivery location -> W
    total += distanceBetween(
        locations[route.back()],
        locations[warehouse]
    );

    return total;
}


// --------------------------------------------------
// Print route
// --------------------------------------------------

void printRoute(vector<int> route, vector<Location> locations)
{
    cout << "W";

    for (int i : route)
    {
        cout << " -> " << locations[i].name;
    }

    cout << " -> W";
}




int main()
{
  

    vector<Location> locations = {
        {'W', 0, 0},
        {'A', 2, 6},
        {'B', 5, 2},
        {'C', 6, 7},
        {'D', 8, 3},
        {'E', 1, 4},
        {'F', 7, 6},
        {'G', 3, 1}
    };


   

    vector<int> currentRoute = {
        1, 2, 3, 4, 5, 6, 7
    };


    // Randomly shuffle the initial route

    random_device rd;
    mt19937 gen(rd());

    shuffle(
        currentRoute.begin(),
        currentRoute.end(),
        gen
    );


 
    double currentCost =
        routeCost(currentRoute, locations);


    cout << fixed << setprecision(2);

    cout << "========== HILL CLIMBING ==========\n\n";

    cout << "Initial Route: ";
    printRoute(currentRoute, locations);

    cout << "\nInitial Cost: "
         << currentCost << "\n\n";



    int iteration = 0;

    while (true)
    {
        vector<int> bestNeighbor = currentRoute;

        double bestNeighborCost = currentCost;


    

        for (int i = 0; i < currentRoute.size(); i++)
        {
            for (int j = i + 1; j < currentRoute.size(); j++)
            {
                // Create neighbour
                vector<int> neighbor = currentRoute;

                // Swap two locations
                swap(neighbor[i], neighbor[j]);


                // Calculate neighbour cost
                double neighborCost =
                    routeCost(neighbor, locations);


                // Keep the best neighbour
                if (neighborCost < bestNeighborCost)
                {
                    bestNeighbor = neighbor;
                    bestNeighborCost = neighborCost;
                }
            }
        }


      

        if (bestNeighborCost < currentCost)
        {
            // Move to better neighbour
            currentRoute = bestNeighbor;
            currentCost = bestNeighborCost;

            iteration++;

            cout << "Iteration " << iteration << ":\n";

            cout << "Best Route: ";
            printRoute(currentRoute, locations);

            cout << "\nCost: "
                 << currentCost << "\n\n";
        }
        else
        {
            // No neighbour is better
            break;
        }
    }


   

    cout << "========== FINAL RESULT ==========\n\n";

    cout << "Final Route: ";
    printRoute(currentRoute, locations);

    cout << "\nFinal Cost: "
         << currentCost << "\n";

    cout << "Number of Iterations: "
         << iteration << "\n";


    return 0;
}