#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

vector<string> courses = {"AI", "DBMS", "OS", "CN"};

map<string, vector<int>> domains = {
    {"AI", {1, 2, 3}},
    {"DBMS", {1, 2, 4}},
    {"OS", {2, 3, 4}},
    {"CN", {1, 3, 4}}
};

map<string, vector<string>> neighbors = {
    {"AI", {"DBMS", "OS"}},
    {"DBMS", {"AI", "CN"}},
    {"OS", {"AI", "CN"}},
    {"CN", {"DBMS", "OS"}}
};

map<string, int> assignment;

int assignments = 0;
int backtracks = 0;
int step = 0;

void printDomains(map<string, vector<int>>& currentDomains)
{
    for (string course : courses)
    {
        cout << course << " = {";

        for (int i = 0; i < currentDomains[course].size(); i++)
        {
            cout << currentDomains[course][i];

            if (i != currentDomains[course].size() - 1)
                cout << ", ";
        }

        cout << "} ";
    }

    cout << endl;
}

bool isConsistent(string course, int value)
{
    for (string neighbor : neighbors[course])
    {
        if (assignment.find(neighbor) != assignment.end())
        {
            if (assignment[neighbor] == value)
                return false;
        }
    }

    return true;
}

string selectMRV(map<string, vector<int>>& currentDomains)
{
    string selected = "";
    int minimum = 1000;

    for (string course : courses)
    {
        if (assignment.find(course) == assignment.end())
        {
            int size = currentDomains[course].size();

            if (size < minimum)
            {
                minimum = size;
                selected = course;
            }
        }
    }

    return selected;
}

bool forwardChecking(
    string course,
    int value,
    map<string, vector<int>>& currentDomains)
{
    for (string neighbor : neighbors[course])
    {
        if (assignment.find(neighbor) == assignment.end())
        {
            vector<int> newDomain;

            for (int v : currentDomains[neighbor])
            {
                if (v != value)
                    newDomain.push_back(v);
            }

            currentDomains[neighbor] = newDomain;

            if (newDomain.empty())
                return false;
        }
    }

    return true;
}

bool solve(map<string, vector<int>> currentDomains)
{
    if (assignment.size() == courses.size())
        return true;

    string course = selectMRV(currentDomains);

    cout << "\nStep " << ++step << endl;

    cout << "Variable selected using MRV: "
         << course << endl;

    vector<int> values = currentDomains[course];

    for (int value : values)
    {
        if (!isConsistent(course, value))
            continue;

        cout << "Value assigned: "
             << value << endl;

        assignment[course] = value;
        assignments++;

        map<string, vector<int>> newDomains = currentDomains;

        newDomains[course] = {value};

        bool valid =
            forwardChecking(
                course,
                value,
                newDomains
            );

        cout << "Updated domains after Forward Checking: ";
        printDomains(newDomains);

        if (!valid)
        {
            cout << "Backtracking required: Yes" << endl;

            assignment.erase(course);
            backtracks++;

            continue;
        }

        cout << "Backtracking required: No" << endl;

        if (solve(newDomains))
            return true;

        cout << "Backtracking from "
             << course
             << " = "
             << value
             << endl;

        assignment.erase(course);
        backtracks++;
    }

    return false;
}

int main()
{
    cout << "University Course Scheduling CSP\n\n";

    cout << "Initial Domains:\n";
    printDomains(domains);

    cout << "\nConstraints:\n";
    cout << "AI != DBMS\n";
    cout << "AI != OS\n";
    cout << "DBMS != CN\n";
    cout << "OS != CN\n";

    cout << "\nStarting MRV + Forward Checking...\n";

    bool result = solve(domains);

    if (result)
    {
        cout << "\n================================\n";
        cout << "Valid Final Timetable\n";
        cout << "================================\n";

        for (string course : courses)
        {
            cout << course
                 << " -> Slot "
                 << assignment[course]
                 << endl;
        }
    }
    else
    {
        cout << "\nNo valid timetable exists.\n";
    }

    cout << "\nTotal assignments: "
         << assignments << endl;

    cout << "Total backtracks: "
         << backtracks << endl;

    return 0;
}