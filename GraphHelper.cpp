class Graph {
private:
    enum Color { WHITE, GRAY, BLACK };
    
    vector<vector<int>> adjList;
    int numberOfNodes;
    
    bool isIndexValid(int index) {
        return index>= 0 && index < adjList.size();
    }
    
    bool hasCycleHelper(int current_node, vector<Color> visited) {
        visited[current_node] = GRAY;
        for(int adjacentNode: adjList[current_node]) {
            if (visited[adjacentNode] == GRAY) {
                return true;
            }
            if (visited[adjacentNode] != BLACK) {
                bool cycle = hasCycleHelper(adjacentNode, visited);
                if (cycle)
                    return true;
            } 
        }
        visited[current_node] = BLACK;
        return false;
    }
    
    void topologicalHelper(const int current_node, vector<Color> &visited, stack<int> &st) {
        visited[current_node] = BLACK;
        for(int adjacentNode: adjList[current_node]) {
            if (visited[adjacentNode] == WHITE) {
                topologicalHelper(adjacentNode, visited, st);
            }
        }
        st.push(current_node);
    }
    
    void connectedComponentsHelper(const int current_node, vector<Color> &visited, vector<int> &componentNodes) {
        componentNodes.push_back(current_node);
        visited[current_node] = GRAY;
        for(int adjacentNode: adjList[current_node]) {
            if (visited[adjacentNode] == WHITE)
                connectedComponentsHelper(adjacentNode, visited, componentNodes);
        }
        visited[current_node] = BLACK;
    }
    
public:
    Graph(int n) {
        adjList.resize(n);
        numberOfNodes = n;
    }
    
    void addEdge(int from, int to) {
        if (!isIndexValid(to) || !isIndexValid(from)) {
            throw "Invalid entry";
        }
        adjList[from].push_back(to);
    }
    
    bool hasCycle() {
        bool cycle = false;
        vector<Color> visited(numberOfNodes, WHITE);
        for(int i = 0; i < numberOfNodes && !cycle; i++) {
            if (visited[i] == WHITE) {
                cycle |= hasCycleHelper(i, visited);
            }
        }
        return cycle;
    }
    
    vector<vector<int>> connectedComponents() {
        vector<vector<int>> components;
        vector<Color> visited(numberOfNodes, WHITE);
        stack<int> traversalOrder = topological();
        while (!traversalOrder.empty()) {
            int node = traversalOrder.top();
            traversalOrder.pop();
            if (visited[node] == WHITE)  {
                vector<int> componentNodes;
                connectedComponentsHelper(node, visited, componentNodes);
                components.push_back(componentNodes);
            }
        }
        return components;
    }
    
    stack<int> topological() {
        if (hasCycle()) {
            throw "Graph has a cycle, hence topological sorting is not possible";
        }
        stack<int> st;
        vector<Color> visited(numberOfNodes, WHITE);
        for(int i = 0; i < numberOfNodes; i++) {
            if (visited[i] == WHITE) {
                topologicalHelper(i, visited, st);
            }
        }
        return st;
    }
};
