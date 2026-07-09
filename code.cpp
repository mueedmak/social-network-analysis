#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <chrono>
#include <windows.h>
#include <conio.h>

using namespace std;

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define CYAN "\033[36m"

struct edge
{
    int target;
    int weight;
};

unordered_map<int, vector<edge>> graph;
unordered_map<int, int> influences;
unordered_map<int, string> labels;

void loadGraph(const string &filename)
{
    cout << "==============================" << endl;
    cout << endl;
    cout << CYAN << "        Loading graph" << RESET << endl;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << RED << " Unable to open file: " << filename << RESET << endl;
        return;
    }

    int u, v, w;
    while (file >> u >> v >> w)
    {
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // Since the graph is undirected so pushing the edge in reverse direction
    }
    file.close();
    cout << GREEN << "  Graph loaded successfully" << RESET << endl;
    cout << endl;
    cout << "==============================" << endl;
}

void loadInfluences(const string &filename)
{
    cout << endl;
    cout << CYAN << "      Loading Influences" << RESET << endl;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << RED << "Unable to open file: " << filename << RESET << endl;
        return;
    }

    int node, influence;
    int count = 0;
    while (file >> node >> influence)
    {
        influences[node] = influence;
        count++;
    }
    file.close();
    cout << CYAN << "      Total Nodes: " << RESET << count << endl;
    cout << GREEN << " Influences Loading Completed" << RESET << endl;
    cout << endl;
    cout << "==============================" << endl;
}
void loadNodeLabels(const string &fileName)
{
    ifstream inputfile(fileName);

    if (!inputfile.is_open())
    {
        cerr << RED << "Error: Unable to open file " << fileName << RESET << endl;
        return;
    }
    cout << endl;
    cout << CYAN << "        Loading Labels" << RESET << endl;
    string line;
    while (getline(inputfile, line))
    {
        stringstream ss(line);
        int nodeNumber;
        string name;

        ss >> nodeNumber;
        getline(ss, name);

        // Trim leading space in name
        if (!name.empty() && name[0] == ' ')
            name = name.substr(1);

        labels[nodeNumber] = name;
    }
    cout << GREEN << "   Labels Loading Completed" << RESET << endl;
    cout << endl;
    cout << "==============================" << endl;
    inputfile.close();
}

void printPathWithLabels(const vector<int> &path)
{
    for (size_t i = 0; i < path.size(); i++)
    {
        int node = path[i];
        cout << CYAN << node << YELLOW << " (" << labels[node] << ")" << RESET;
        if (i != path.size() - 1)
        {
            cout << " ->  ";
        }
    }
    cout << endl;
}
vector<int> aStar(unordered_map<int, vector<edge>> &graph, int start, int end, int &totalWeight)
{
    unordered_map<int, int> distances;                                    // Tracks the shortest distance to each node
    unordered_map<int, int> parent;                                       // Tracks the parent of each node for path reconstruction
    unordered_map<int, int> heuristic;                                    // Stores heuristic values (number of neighbors)
                                                                          // in this queue first element is the value(heuristic+distance) and second element is the node
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq; // Min-heap for A* processing

    // Initialize distances to infinity and its heuristics score(number of neighbors)
    for (auto &node : graph)
    {
        distances[node.first] = INT_MAX;
        heuristic[node.first] = graph[node.first].size();
    }
    distances[start] = 0;               // Distance to the start node is 0
    pq.push({heuristic[start], start}); // Push the start node into the queue with its heuristic

    while (!pq.empty())
    {
        int currnode = pq.top().second;
        pq.pop();

        if (currnode == end)
            break;

        for (int i = 0; i < graph[currnode].size(); i++)
        {
            int nextnode = graph[currnode][i].target;
            int edgeweight = graph[currnode][i].weight;
            int newdist = distances[currnode] + edgeweight;
            if (newdist < distances[nextnode])
            {
                distances[nextnode] = newdist;
                parent[nextnode] = currnode;
                int fScore = newdist + heuristic[nextnode];
                pq.push({fScore, nextnode});
            }
        }
    }

    // path reconstruction
    vector<int> path;
    totalWeight = distances[end]; // Retrieve the total weight (distance to the end node)
    if (distances[end] == INT_MAX)
    { // If end is unreachable
        return {};
    }
    for (int node = end; node != start; node = parent[node])
    {
        path.push_back(node);
    }
    path.push_back(start);
    reverse(path.begin(), path.end()); // Reverse the path to get it in the correct order

    return path;
}

vector<int> dijkstra(int start, int end, int &totalDistance)
{
    unordered_map<int, int> dist;
    unordered_map<int, int> parent;

    for (auto &node : graph)
    {
        dist[node.first] = INT_MAX;
    }
    dist[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        int current = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();

        if (current == end)
            break;

        for (auto &edge : graph[current])
        {
            int neighbor = edge.target;
            int weight = edge.weight;
            if (currentDist + weight < dist[neighbor])
            {
                dist[neighbor] = currentDist + weight;
                parent[neighbor] = current;
                pq.push({dist[neighbor], neighbor});
            }
        }
    }
    vector<int> path;
    totalDistance = dist[end];
    if (dist[end] == INT_MAX)
    { // If end is unreachable
        return {};
    }
    for (int node = end; node != start; node = parent[node])
    {
        path.push_back(node);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

// Dynamic Programming to find longest increasing path
void longestInfluencePath(int startNode = -1, int endNode = -1)
{

    vector<int> nodes;
    for (auto &node : influences)
    {
        nodes.push_back(node.first);
    }

    // Sort nodes by influence in ascending order
    sort(nodes.begin(), nodes.end(), [](int a, int b)
         { return influences[a] < influences[b]; });

    unordered_map<int, int> MemoryArr; /*An map used to store Longest path length ending
at each node to avoid repeated checks and make use of previous processing to reduce complexity.*/
    unordered_map<int, int> parent;    // Parent map for path reconstruction

    for (int node : nodes)
    {
        MemoryArr[node] = 1; // Each node can at least be a path of length 1

        for (int i = 0; i < graph[node].size(); i++)
        {
            int neighbor = graph[node][i].target;
            if (influences[node] > influences[neighbor] && MemoryArr[node] < MemoryArr[neighbor] + 1)
            {
                MemoryArr[node] = MemoryArr[neighbor] + 1;
                parent[node] = neighbor;
            }
        }
    }

    if (startNode != -1 && endNode != -1)
    {
        // Check for longest chain between two specific nodes
        if (MemoryArr.find(startNode) == MemoryArr.end() || MemoryArr.find(endNode) == MemoryArr.end())
        {
            cout << RED << "One or both nodes are not in the graph." << RESET << endl;
            return;
        }

        // Trace the path from startNode to endNode using the parent map
        vector<int> path;
        for (int node = endNode; node != startNode; node = parent[node])
        {
            path.push_back(node);
            if (parent.find(node) == parent.end())
            {
                cout << RED << "No valid path exists between the nodes." << RESET << endl;
                return;
            }
        }
        path.push_back(startNode);
        reverse(path.begin(), path.end());

        cout << BLUE
             << "Length: " << RESET << path.size() << endl
             << endl
             << BLUE << "Chain:  ";
        for (size_t i = 0; i < path.size(); i++)
        {
            int node = path[i];
            cout << CYAN << node << YELLOW << " (" << labels[node] << ")" << RESET;
            if (i != path.size() - 1)
            {
                cout << " ->  ";
            }
        }
        cout << endl;
    }
    else
    {
        // Find the node with the maximum path length
        int maxLength = 0, maxStartNode = 0;
        for (auto &entry : MemoryArr)
        {
            if (entry.second > maxLength)
            {
                maxLength = entry.second;
                maxStartNode = entry.first;
            }
        }

        // Reconstruct the longest influence chain in the graph
        vector<int> path;
        for (int node = maxStartNode; MemoryArr.find(node) != MemoryArr.end(); node = parent[node])
        {
            path.push_back(node);
            if (parent.find(node) == parent.end())
                break;
        }
        reverse(path.begin(), path.end());
        cout << endl
             << BLUE
             << "Length: " << RESET << maxLength << endl
             << endl
             << BLUE << "Chain:  ";
        for (size_t i = 0; i < path.size(); i++)
        {
            int node = path[i];
            cout << CYAN << node << YELLOW << " (" << labels[node] << ")" << RESET;
            if (i != path.size() - 1)
            {
                cout << " ->  ";
            }
        }
        cout << endl;
        cout << endl;
    }
}

int main()
{
    // Load data
    loadGraph("social-network-proj-graph.txt");
    loadInfluences("social-network-proj-Influences.txt");
    loadNodeLabels("social-network-proj-LABELS.txt");
    int start = 523, end = 5229;
    cout << endl;
    cout << CYAN << "Enter The Start Node: " << RESET;
    cin >> start;
    cout << CYAN << "Enter The End Node: " << RESET;
    cin >> end;
    cout << endl;
    cout << "==============================" << endl
         << endl;

    // Dijkstra Algorithm
    cout << CYAN << "   Shortest Path (Dijkstra)" << RESET << endl;
    cout << BLUE << "From : " << RESET << start << endl
         << BLUE << "To   : " << RESET << end << endl
         << endl;

    auto dStart = chrono::high_resolution_clock::now();
    int dijkstraDistance = 0;
    vector<int> dijkstraPath = dijkstra(start, end, dijkstraDistance);
    auto dEnd = chrono::high_resolution_clock::now();
    auto dDuration = chrono::duration_cast<chrono::microseconds>(dEnd - dStart).count();

    if (dijkstraPath.empty())
    {
        cout << RED << "No path exists between the nodes (Dijkstra)." << RESET << endl;
    }
    else
    {
        cout << BLUE << "Shortest Path: " << RESET;
        printPathWithLabels(dijkstraPath);
        cout << CYAN << "Distance: " << RESET << dijkstraDistance << endl;
    }
    cout << CYAN << "Execution Time (Dijkstra): " << RESET << dDuration << " microseconds" << endl
         << endl;

    // A* Algorithm
    cout << CYAN << "   Shortest Path (A*)" << RESET << endl;
    cout << BLUE << "From : " << RESET << start << endl
         << BLUE << "To   : " << RESET << end << endl
         << endl;

    auto aStart = chrono::high_resolution_clock::now();
    int aStarTotalWeight = 0;
    vector<int> aStarPathResult = aStar(graph, start, end, aStarTotalWeight);
    auto aEnd = chrono::high_resolution_clock::now();
    auto aDuration = chrono::duration_cast<chrono::microseconds>(aEnd - aStart).count();

    if (aStarPathResult.empty())
    {
        cout << RED << "No path exists between the nodes (A*)." << RESET << endl;
    }
    else
    {
        cout << BLUE << "A* Path: " << RESET;
        printPathWithLabels(aStarPathResult);
        cout << CYAN << "Total Weight: " << RESET << aStarTotalWeight << endl;
    }
    cout << CYAN << "Execution Time (A*): " << RESET << aDuration << " microseconds" << endl;

    long long timeDifference = dDuration - aDuration;
    cout << CYAN << "Time Difference (Dijkstra - A*): " << RESET << timeDifference << " microseconds" << endl;
    cout << endl;
    cout << "==============================" << endl;
    cout << endl;
    cout << CYAN << "            PART 2" << RESET;
    cout << endl
         << CYAN << "Select Longest Influence Chain:" << endl;
    cout << YELLOW << "1  . Between Two Nodes" << RESET << endl;
    cout << YELLOW << "2. Whole Graph" << RESET << endl
         << endl;
    cin.ignore();
    char choice = _getch();
    if (choice == '1')
    {
        cout << BLUE << "Longest Increasing Influence Path:" << RESET << endl
             << endl;
        cout << BLUE << "From : " << RESET << start << endl
             << BLUE << "To   : " << RESET << end << endl
             << endl;
        longestInfluencePath(start, end);
    }
    else if (choice == '2')
    {
        cout << BLUE << "Longest Increasing Influence Path:" << RESET << endl;
        longestInfluencePath();
    }
    else
    {
        cout << RED << "Invalid choice." << RESET << endl;
        return 0;
    }

    return 0;
}

