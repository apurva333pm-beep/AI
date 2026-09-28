#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

// ============================================================
// COLORS
// ============================================================

vector<string> colors = {
    "Red",
    "Green",
    "Blue"
};


// ============================================================
// VARIABLES
// ============================================================

vector<char> variables = {
    'A', 'B', 'C', 'D', 'E'
};


// ============================================================
// ADJACENCY LIST
// ============================================================

map<char, vector<char>> neighbors = {

    {'A', {'B', 'C'}},

    {'B', {'A', 'C', 'D', 'E'}},

    {'C', {'A', 'B', 'E'}},

    {'D', {'B', 'E'}},

    {'E', {'B', 'C', 'D'}}
};


// ============================================================
// CHECK WHETHER A COLOR CAN BE ASSIGNED
// ============================================================

bool isSafe(
    char variable,
    string color,
    map<char, string>& assignment)
{
    for (char neighbor : neighbors[variable])
    {
        // If neighbor is already assigned
        if (assignment.find(neighbor) != assignment.end())
        {
            // Same color is not allowed
            if (assignment[neighbor] == color)
            {
                return false;
            }
        }
    }

    return true;
}


// ============================================================
// ORDINARY BACKTRACKING
// ============================================================

int normalAssignments = 0;
int normalBacktracks = 0;


// ------------------------------------------------------------
// Get next unassigned variable
// ------------------------------------------------------------

char getNextVariable(
    map<char, string>& assignment)
{
    for (char variable : variables)
    {
        if (assignment.find(variable) == assignment.end())
        {
            return variable;
        }
    }

    return '\0';
}


// ------------------------------------------------------------
// Backtracking function
// ------------------------------------------------------------

bool backtracking(
    map<char, string>& assignment)
{
    // If all variables are assigned
    if (assignment.size() == variables.size())
    {
        return true;
    }


    // Select next variable
    char variable =
        getNextVariable(assignment);


    cout << "\nSelecting variable: "
         << variable << endl;


    // Try every color
    for (string color : colors)
    {
        cout << "Trying "
             << variable
             << " = "
             << color;


        // Check constraint
        if (isSafe(variable, color, assignment))
        {
            cout << " -> Accepted\n";

            // Make assignment
            assignment[variable] = color;

            normalAssignments++;


            // Recursively solve
            if (backtracking(assignment))
            {
                return true;
            }


            // Backtrack
            cout << "Backtracking: remove "
                 << variable
                 << " = "
                 << color
                 << endl;

            assignment.erase(variable);

            normalBacktracks++;
        }
        else
        {
            cout << " -> Rejected\n";
        }
    }


    return false;
}


// ============================================================
// MRV HEURISTIC
// ============================================================

// Calculate remaining legal colors for a variable

vector<string> getRemainingValues(
    char variable,
    map<char, string>& assignment)
{
    vector<string> remaining;

    for (string color : colors)
    {
        if (isSafe(variable, color, assignment))
        {
            remaining.push_back(color);
        }
    }

    return remaining;
}


// ------------------------------------------------------------
// Select variable using MRV
// ------------------------------------------------------------

char selectMRVVariable(
    map<char, string>& assignment)
{
    char bestVariable = '\0';

    int minimumValues = 1000;


    for (char variable : variables)
    {
        // Skip already assigned variables
        if (assignment.find(variable) != assignment.end())
        {
            continue;
        }


        // Count legal remaining colors
        int count =
            getRemainingValues(
                variable,
                assignment
            ).size();


        // Choose variable with minimum
        // remaining values
        if (count < minimumValues)
        {
            minimumValues = count;

            bestVariable = variable;
        }
    }


    return bestVariable;
}


// ============================================================
// MRV BACKTRACKING
// ============================================================

int mrvAssignments = 0;
int mrvBacktracks = 0;


bool mrvBacktracking(
    map<char, string>& assignment)
{
    // All variables assigned
    if (assignment.size() == variables.size())
    {
        return true;
    }


    // Select variable using MRV
    char variable =
        selectMRVVariable(assignment);


    vector<string> remainingValues =
        getRemainingValues(
            variable,
            assignment
        );


    cout << "\nMRV selected variable: "
         << variable
         << " ("
         << remainingValues.size()
         << " values remaining)"
         << endl;


    // Try available colors
    for (string color : remainingValues)
    {
        cout << "Trying "
             << variable
             << " = "
             << color
             << " -> Accepted\n";


        // Assign
        assignment[variable] = color;

        mrvAssignments++;


        // Continue recursively
        if (mrvBacktracking(assignment))
        {
            return true;
        }


        // Backtrack
        cout << "Backtracking: remove "
             << variable
             << " = "
             << color
             << endl;

        assignment.erase(variable);

        mrvBacktracks++;
    }


    return false;
}


// ============================================================
// PRINT FINAL ASSIGNMENT
// ============================================================

void printAssignment(
    map<char, string>& assignment)
{
    cout << "\nFinal Color Assignment:\n";

    for (char variable : variables)
    {
        cout << variable
             << " = "
             << assignment[variable]
             << endl;
    }
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // PART 1 + 2 + 3 + 4
    // ORDINARY BACKTRACKING
    // ========================================================

    cout << "=============================================\n";
    cout << "     ORDINARY BACKTRACKING SEARCH\n";
    cout << "=============================================\n";


    map<char, string> assignment;


    bool result =
        backtracking(assignment);


    if (result)
    {
        printAssignment(assignment);
    }
    else
    {
        cout << "No solution exists.\n";
    }


    cout << "\nNumber of assignments: "
         << normalAssignments
         << endl;

    cout << "Number of backtracks: "
         << normalBacktracks
         << endl;


    // ========================================================
    // PART 5
    // MRV BACKTRACKING
    // ========================================================

    cout << "\n\n";
    cout << "=============================================\n";
    cout << "       MRV BACKTRACKING SEARCH\n";
    cout << "=============================================\n";


    assignment.clear();


    bool mrvResult =
        mrvBacktracking(assignment);


    if (mrvResult)
    {
        printAssignment(assignment);
    }
    else
    {
        cout << "No solution exists.\n";
    }


    cout << "\nNumber of assignments: "
         << mrvAssignments
         << endl;

    cout << "Number of backtracks: "
         << mrvBacktracks
         << endl;


    // ========================================================
    // FINAL COMPARISON
    // ========================================================

    cout << "\n\n";
    cout << "=============================================\n";
    cout << "              COMPARISON\n";
    cout << "=============================================\n";


    cout << "\nOrdinary Backtracking:\n";

    cout << "Assignments = "
         << normalAssignments
         << endl;

    cout << "Backtracks = "
         << normalBacktracks
         << endl;


    cout << "\nMRV Backtracking:\n";

    cout << "Assignments = "
         << mrvAssignments
         << endl;

    cout << "Backtracks = "
         << mrvBacktracks
         << endl;


    return 0;
}