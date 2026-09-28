#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <iomanip>
#include <numeric>

using namespace std;




struct Location
{
    char name;
    double x;
    double y;
};



double distanceBetween(Location a, Location b)
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return sqrt(dx * dx + dy * dy);
}




double routeCost(
    const vector<int>& route,
    const vector<Location>& locations)
{
    double total = 0.0;

    // Warehouse index = 0

    // W -> first location
    total += distanceBetween(
        locations[0],
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

    // Last location -> W
    total += distanceBetween(
        locations[route.back()],
        locations[0]
    );

    return total;
}



void printRoute(
    const vector<int>& route,
    const vector<Location>& locations)
{
    cout << "W";

    for (int i : route)
    {
        cout << " -> " << locations[i].name;
    }

    cout << " -> W";
}




vector<int> generateRandomRoute(mt19937& gen)
{
    vector<int> route =
    {
        1, 2, 3, 4, 5, 6, 7
    };

    shuffle(route.begin(), route.end(), gen);

    return route;
}




struct HCResult
{
    vector<int> bestRoute;
    double bestCost;
    int iterations;
};


HCResult hillClimbing(
    vector<int> currentRoute,
    const vector<Location>& locations)
{
    double currentCost =
        routeCost(currentRoute, locations);

    int iterations = 0;


    while (true)
    {
        vector<int> bestNeighbor = currentRoute;

        double bestNeighborCost = currentCost;


        

        for (int i = 0; i < currentRoute.size(); i++)
        {
            for (int j = i + 1; j < currentRoute.size(); j++)
            {
                vector<int> neighbor = currentRoute;

                swap(neighbor[i], neighbor[j]);

                double cost =
                    routeCost(neighbor, locations);


                // Select best neighbour
                if (cost < bestNeighborCost)
                {
                    bestNeighbor = neighbor;
                    bestNeighborCost = cost;
                }
            }
        }


      

        if (bestNeighborCost < currentCost)
        {
            currentRoute = bestNeighbor;

            currentCost = bestNeighborCost;

            iterations++;
        }
        else
        {
            // No improvement
            break;
        }
    }


    HCResult result;

    result.bestRoute = currentRoute;
    result.bestCost = currentCost;
    result.iterations = iterations;

    return result;
}



struct SAResult
{
    vector<int> bestRoute;
    double bestCost;
    int iterations;
};


SAResult simulatedAnnealing(
    vector<int> currentRoute,
    const vector<Location>& locations,
    mt19937& gen,
    bool showDetails = true)
{


    double temperature = 100.0;

    double alpha = 0.95;

    double minimumTemperature = 0.1;



    double currentCost =
        routeCost(currentRoute, locations);


    // Maintain best solution found
    vector<int> bestRoute = currentRoute;

    double bestCost = currentCost;


    int iterations = 0;


    if (showDetails)
    {
        cout << "\n========== SIMULATED ANNEALING ==========\n\n";

        cout << "Initial Route: ";
        printRoute(currentRoute, locations);

        cout << "\nInitial Cost: "
             << currentCost << "\n\n";
    }


    // Random number generator for choosing positions
    uniform_int_distribution<int> positionDist(0, 6);

    // Random number between 0 and 1
    uniform_real_distribution<double> probabilityDist(0.0, 1.0);


 

    while (temperature >= minimumTemperature)
    {
        iterations++;


      

        vector<int> neighbor = currentRoute;


        // Select two different locations
        int i = positionDist(gen);
        int j = positionDist(gen);

        while (i == j)
        {
            j = positionDist(gen);
        }


        // Swap them
        swap(neighbor[i], neighbor[j]);


       

        double newCost =
            routeCost(neighbor, locations);


        // Delta E
        double deltaE = newCost - currentCost;


        bool accepted = false;

        double probability = 0.0;


       

        if (deltaE < 0)
        {
            accepted = true;
        }


      

        else
        {
            probability =
                exp(-deltaE / temperature);

            double r = probabilityDist(gen);

            if (r < probability)
            {
                accepted = true;
            }
        }



        if (accepted)
        {
            currentRoute = neighbor;

            currentCost = newCost;
        }


      

        if (currentCost < bestCost)
        {
            bestRoute = currentRoute;

            bestCost = currentCost;
        }


        
       

        if (showDetails)
        {
            cout << "Iteration " << iterations
                 << " | ";

            cout << "Temperature = "
                 << fixed << setprecision(4)
                 << temperature;

            cout << " | Current Cost = "
                 << setprecision(2)
                 << currentCost;

            cout << " | Delta E = "
                 << deltaE;

            cout << " | ";

            if (accepted)
                cout << "ACCEPTED";
            else
                cout << "REJECTED";

            cout << endl;
        }



        temperature =
            alpha * temperature;
    }


    if (showDetails)
    {
        cout << "\nBest Route Found: ";

        printRoute(bestRoute, locations);

        cout << "\nBest Cost: "
             << bestCost;

        cout << "\nTotal Iterations: "
             << iterations << "\n";
    }


    SAResult result;

    result.bestRoute = bestRoute;
    result.bestCost = bestCost;
    result.iterations = iterations;

    return result;
}




int main()
{
   

    vector<Location> locations =
    {
        {'W', 0, 0},
        {'A', 2, 6},
        {'B', 5, 2},
        {'C', 6, 7},
        {'D', 8, 3},
        {'E', 1, 4},
        {'F', 7, 6},
        {'G', 3, 1}
    };


   

    random_device rd;

    mt19937 gen(rd());


    vector<int> initialRoute =
        generateRandomRoute(gen);


    SAResult sa =
        simulatedAnnealing(
            initialRoute,
            locations,
            gen,
            true
        );


   

    cout << "\n\n";
    cout << "====================================================\n";
    cout << "       QUESTION 3: HC vs SA (5 RUNS)\n";
    cout << "====================================================\n\n";


    // Arrays to store results
    vector<double> hcCosts;
    vector<double> saCosts;

    vector<int> hcIterations;
    vector<int> saIterations;


    cout << left
         << setw(8) << "Run"
         << setw(15) << "HC Cost"
         << setw(15) << "SA Cost"
         << setw(15) << "HC Iter."
         << setw(15) << "SA Iter."
         << endl;


    cout << "------------------------------------------------------------\n";


  

    for (int run = 1; run <= 5; run++)
    {
        // Different random initial route
        vector<int> routeHC =
            generateRandomRoute(gen);

        vector<int> routeSA =
            generateRandomRoute(gen);



        HCResult hc =
            hillClimbing(
                routeHC,
                locations
            );


      

        SAResult sa =
            simulatedAnnealing(
                routeSA,
                locations,
                gen,
                false
            );


        // Store results
        hcCosts.push_back(hc.bestCost);
        saCosts.push_back(sa.bestCost);

        hcIterations.push_back(hc.iterations);
        saIterations.push_back(sa.iterations);


        // Print table row
        cout << left
             << setw(8) << run
             << setw(15) << fixed << setprecision(2)
             << hc.bestCost
             << setw(15)
             << sa.bestCost
             << setw(15)
             << hc.iterations
             << setw(15)
             << sa.iterations
             << endl;
    }


   

    double hcBest =
        *min_element(
            hcCosts.begin(),
            hcCosts.end()
        );

    double hcWorst =
        *max_element(
            hcCosts.begin(),
            hcCosts.end()
        );

    double hcAverage =
        accumulate(
            hcCosts.begin(),
            hcCosts.end(),
            0.0
        ) / hcCosts.size();


    double saBest =
        *min_element(
            saCosts.begin(),
            saCosts.end()
        );

    double saWorst =
        *max_element(
            saCosts.begin(),
            saCosts.end()
        );

    double saAverage =
        accumulate(
            saCosts.begin(),
            saCosts.end(),
            0.0
        ) / saCosts.size();


    double hcAvgIterations =
        accumulate(
            hcIterations.begin(),
            hcIterations.end(),
            0.0
        ) / hcIterations.size();


    double saAvgIterations =
        accumulate(
            saIterations.begin(),
            saIterations.end(),
            0.0
        ) / saIterations.size();


  

    cout << "SUMMARY\n";
   


    cout << left
         << setw(25) << "Measure"
         << setw(20) << "Hill Climbing"
         << setw(20) << "Simulated Annealing"
         << endl;


    cout << "------------------------------------------------------------\n";


    cout << left
         << setw(25) << "Best Cost"
         << setw(20) << hcBest
         << setw(20) << saBest
         << endl;


    cout << left
         << setw(25) << "Worst Cost"
         << setw(20) << hcWorst
         << setw(20) << saWorst
         << endl;


    cout << left
         << setw(25) << "Average Cost"
         << setw(20) << hcAverage
         << setw(20) << saAverage
         << endl;


    cout << left
         << setw(25) << "Average Iterations"
         << setw(20) << hcAvgIterations
         << setw(20) << saAvgIterations
         << endl;


    return 0;
}