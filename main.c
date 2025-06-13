//Name: Sojood Nazih Asfour , Id: 1230298, Sec3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_CITIES 100
#define MAX_SIZE 1000

// Structure to represent a node in the linked list of cities
struct Node
{
    char* CityName;
    int index;
    struct Node* Next;
};

// Structure for an edge between cities in the graph
struct Edge
{
    int Src, Dest;
    int Weight;
};

// Structure to store Minimum Spanning Tree (MST) results
struct MST
{
    int totalCost;
    struct Edge* Edges;
    int edgeCount;
    double Time;
};

// Structure for a node in the min-heap (used in Prim's algorithm)
struct MinHeapNode
{
    int vertex;
    int weight;
};

// Structure for the min heap data structure
struct MinHeap
{
    int size;
    int capacity;
    int* pos;
    struct MinHeapNode* array;
};

// Union-Find structure for Kruskal's algorithm (disjoint set data structure)
struct Subset
{
    int parent;
    int rank;
};

typedef struct Node* CityList;
typedef struct Edge Edge;
typedef struct MST* MST;
typedef struct MinHeapNode MinHeapNode;
typedef struct MinHeap MinHeap;
typedef struct Subset Subset;

void displayMenu();
CityList createCityList(CityList);
void DeletList(CityList);
int CheckCityExistent(char*, CityList);
void insertCityToList(char*, CityList);
CityList loadCitiesFromFile(char*);
int getCityCount(CityList);
int getCityIndex(char*, CityList);
void buildCityGraph(char*, CityList, int[][MAX_CITIES]);
int getEdgesCounts(int[][MAX_CITIES], int);
CityList loadCity();
char* getCityName(int, CityList);
MinHeapNode newMinHeapNode(int, int);
MinHeap* createMinHeap(int);
void swapNode(MinHeapNode*, MinHeapNode*);
void minHeapify(MinHeap*, int);
int isEmpty(MinHeap*);
MinHeapNode removeMin(MinHeap*);
void decreaseKey(MinHeap*, int, int);
MST primsMSTAlgo(int[][MAX_CITIES], int, CityList, int);
int find(Subset[], int);
void Union(Subset[], int, int);
MST kruskalsMSTAlgo(int[][MAX_CITIES], int, CityList);
void compareAlgorithms(MST, MST);
void printList(CityList);
void printMST(MST, CityList);
void destroyCityList(CityList);

int cityCount = 0;
int graph[MAX_CITIES][MAX_CITIES];// Adjacency matrix representing the graph

int main()
{
    clock_t t1, t2;
    MST prim = NULL;
    MST kruskal = NULL;
    CityList cityList = NULL;
    char sourceCity[MAX_SIZE];
    int sourceIndex = 0;

    printf("Road network connecting Palestinian cities and towns with a minimal cost System.....\n\n");

    int choice = 0;

    printf("Please enter an option from the menu:\n");
    displayMenu();
    scanf("%d", &choice);

    // Enforce loading cities first if an operation other than load or exit is chosen
    if (choice != 1)
    {
        while (choice != 1 && choice != 5)
        {
            printf("\nYou can't do any operation before load the information from the file!!\n\n");
            printf("Please enter an option from the menu:\n\n");
            displayMenu();
            scanf("%d", &choice);

            if (choice == 5) // If the user chooses to exit at this point, terminate
            {
                printf("\nExit from the program...\nThank you for using this program...\nGood bye!\n");
                return 0;
            }
        }
    }

    while (choice != 5)
    {
        switch (choice)
        {
        // Load cities and build graph adjacency matrix
        case 1:
            cityList = loadCity();
            break;
        // Apply Prim's Algorithm
        case 2:

            printf("\nEnter the source city name: ");
            scanf("%s", sourceCity);
            sourceIndex = getCityIndex(sourceCity, cityList);

            // Validate source city input
            while (sourceIndex == -1)
            {
                printf("\nInvalid city name. Please enter a valid city name!\n");
                printf("\nEnter the source city name: ");
                scanf("%s", sourceCity);
                sourceIndex = getCityIndex(sourceCity, cityList);
            }

            // Measure execution time for Prim's algorithm
            t1 = clock();
            prim = primsMSTAlgo(graph, cityCount, cityList, sourceIndex);
            t1 = clock() - t1;

            // Store and display Prim's algorithm results
            if (prim != NULL)
            {
                prim->Time = ((double)t1) / CLOCKS_PER_SEC; // Convert clock cycles to seconds
                printf("\nPrim's Algorithm Results (Source: %s):\n", sourceCity);
                printMST(prim, cityList);
            }
            else
                printf("Failed to compute Prim's MST please try again!\n");
            break;
        // Apply Kruskal's Algorithm
        case 3:
            // Free previous Kruskal's result if it exists to prevent memory leaks
            if (kruskal != NULL)
            {
                if (kruskal->Edges != NULL)
                    free(kruskal->Edges);
                free(kruskal);
                kruskal = NULL;
            }

            // Measure execution time for Kruskal's algorithm
            t2 = clock();
            kruskal = kruskalsMSTAlgo(graph, cityCount, cityList);
            t2 = clock() - t2;

            // Store and display Kruskal's algorithm results
            if (kruskal != NULL)
            {
                kruskal->Time = ((double)t2) / CLOCKS_PER_SEC; // Convert clock cycles to seconds
                printf("\nKruskal's Algorithm Results:\n");
                printMST(kruskal, cityList);
            }
            else
                printf("Failed to compute Kruskal's MST please try again!\n");

            break;
        // Compare the two algorithms
        case 4:
            // Prompt for source city for Prim's
            printf("\nEnter the source city name for prim's algorithm: ");
            scanf("%s", sourceCity);
            sourceIndex = getCityIndex(sourceCity, cityList);

            // Validate source city input
            while (sourceIndex == -1)
            {
                printf("\nInvalid city name. Please enter a valid city name!\n");
                printf("\nEnter the source city name: ");
                scanf("%s", sourceCity);
                sourceIndex = getCityIndex(sourceCity, cityList);
            }

            // Measure and run Prim's algorithm
            t1 = clock();
            prim = primsMSTAlgo(graph, cityCount, cityList, sourceIndex);
            t1 = clock() - t1;

            // Measure and run Kruskal's algorithm
            t2 = clock();
            kruskal = kruskalsMSTAlgo(graph, cityCount, cityList);
            t2 = clock() - t2;

            // Compare results if both algorithms successfully computed an MST
            if (prim && kruskal)
            {
                prim->Time = ((double)t1) / CLOCKS_PER_SEC;
                kruskal->Time = ((double)t2) / CLOCKS_PER_SEC;
                compareAlgorithms(prim, kruskal);
            }
            else
            {
                printf("One or both algorithms failed to compute MST for comparison.\n");
            }
            break;
        // Handle invalid menu options
        default:
            printf("\nInvalid option please enter another one!\n");
            break;
        }
        // Prompt for the next menu choice
        printf("Please enter an option from the menu:\n");
        displayMenu();
        scanf("%d", &choice);
    }

    printf("\nExit from the program...\nThank you for using this program...\nGood bye!\n");

    // Free all dynamically allocated memory before exiting the program
    if (prim != NULL)
    {
        if (prim->Edges != NULL)
            free(prim->Edges);
        free(prim);
    }

    if (kruskal != NULL)
    {
        if (kruskal->Edges != NULL)
            free(kruskal->Edges);
        free(kruskal);
    }
    if (cityList != NULL)
        destroyCityList(cityList);
    return 0;
}

// function to display the menu options to the user
void displayMenu()
{
    printf("1. Load cities\n");
    printf("2. Apply Prim's Algorithm\n");
    printf("3. Apply Kruskal's Algorithm\n");
    printf("4. Compare the two algorithms\n");
    printf("5. Exit\n");
}


// Function to create an empty city list (a sentinel head node)
CityList createCityList(CityList C)
{
    if (C != NULL)
        DeletList(C);

    C = (CityList)malloc(sizeof(struct Node));
    if (C == NULL)
        printf("Out of space!\n");
    else
    {
        C->Next = NULL;
        C->index = -1;
        C->CityName = NULL;
    }

    return C;
}

// Function to delete all city nodes in the list, effectively clearing it.
void DeletList(CityList C)
{
    if (C == NULL)
        return;

    CityList temp, p;
    p = C->Next;
    C->Next = NULL;

    while (p != NULL)
    {
        temp = p->Next;
        free(p->CityName);
        free(p);
        p = temp;
    }
}

// Function to delete and free all nodes in the list
void destroyCityList(CityList C)
{
    if (C == NULL)
        return;

    CityList current = C->Next;
    CityList temp;

    // Traverse and free all city nodes
    while (current != NULL)
    {
        temp = current->Next;
        free(current->CityName);
        free(current);
        current = temp;
    }
    if (C->CityName != NULL)
        free(C->CityName);
    free(C);
}


// Function to search for a city in the list
int CheckCityExistent(char* city, CityList C)
{
    CityList currentCity = C->Next;
    while (currentCity != NULL)
    {
        if (strcasecmp(currentCity->CityName, city) == 0)
            return currentCity->index;
        currentCity = currentCity->Next;
    }
    return -1;
}

// Function to insert a new city into the linked list
void insertCityToList(char* city, CityList C)
{
    CityList currentCity = C;

    // Traverse to the end of the list
    while (currentCity->Next != NULL)
        currentCity = currentCity->Next;

    // Allocate memory for the new city node
    CityList newCity = (CityList)malloc(sizeof(struct Node));
    if (newCity == NULL)
    {
        printf("Out of Space!\n");
        return;
    }
    newCity->index = currentCity->index + 1;
    newCity->CityName = strdup(city);
    newCity->Next = NULL;
    currentCity->Next = newCity;
}

// Function to load cities from a file into a CityList
CityList loadCitiesFromFile(char* fileName)
{
    FILE* in = fopen(fileName, "r");
    if (in == NULL)
    {
        printf("Error, Can't open the file '%s'!\n", fileName);
        return NULL;
    }

    CityList cityList = createCityList(NULL);
    if (cityList == NULL)
    {
        fclose(in);
        return NULL;
    }

    char fileData[MAX_SIZE];
    char *str;

    // Read the first line from the file
    str = fgets(fileData, MAX_SIZE, in);

    // Loop through each line of the file
    while (str != NULL)
    {
        char* src = strtok(str, "#");
        char* dest = strtok(NULL, "#");
        char* weightStr = strtok(NULL, "#");

        // Skip invalid lines
        if (src == NULL || dest == NULL || weightStr == NULL)
        {
            str = fgets(fileData, MAX_SIZE, in);
            continue;
        }

        // Check if source city already exists if not, insert it
        if (CheckCityExistent(src, cityList) == -1)
            insertCityToList(src, cityList);

        // Check if destination city already exists f not, insert it
        if (CheckCityExistent(dest, cityList) == -1)
            insertCityToList(dest, cityList);

        str = fgets(fileData, MAX_SIZE, in);
    }
    fclose(in);
    return cityList;
}

// Function to get the number of cities in the linked list
int getCityCount(CityList C)
{
    int count = 0;
    CityList currentCity = C->Next;
    while (currentCity != NULL)
    {
        count++;
        currentCity = currentCity->Next;
    }
    return count;
}

// Function to get the index of a city given its name
int getCityIndex(char* city, CityList C)
{
    CityList currentCity = C->Next;
    while (currentCity != NULL)
    {
        if (strcasecmp(city, currentCity->CityName) == 0)
            return currentCity->index;
        currentCity = currentCity->Next;
    }
    return -1;
}

// Function to build the adjacency matrix representation of the graph from file data
void buildCityGraph(char* fileName, CityList C, int matrix[][MAX_CITIES])
{
    int V = getCityCount(C); // Get the number of cities (vertices)

    // Initialize matrix with INT_MAX to represent no connection (infinity)
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            matrix[i][j] = INT_MAX;
        }
    }

    FILE* in = fopen(fileName, "r");
    if (in == NULL)
    {
        printf("Error, Can't open the file '%s'!\n", fileName);
        return;
    }

    char fileData[MAX_SIZE];
    char *str;

    str = fgets(fileData, MAX_SIZE, in);

    while (str != NULL)
    {
        char* src = strtok(str, "#");
        char* dest = strtok(NULL, "#");
        char* weightStr = strtok(NULL, "#");

        // Skip invalid lines
        if (src == NULL || dest == NULL || weightStr == NULL)
        {
            str = fgets(fileData, MAX_SIZE, in);
            continue;
        }

        int weight = atoi(weightStr);
        int srcIndex = getCityIndex(src, C);
        int destIndex = getCityIndex(dest, C);

        //If both cities are valid, add the edge to the adjacency matrix
        if (srcIndex != -1 && destIndex != -1)
        {
            matrix[srcIndex][destIndex] = weight; // Set weight from source to destination
            matrix[destIndex][srcIndex] = weight; // Set weight from destination to source (undirected graph)
        }

        str = fgets(fileData, MAX_SIZE, in);
    }
    fclose(in);
}

// Function to count the total number of unique edges in the graph
int getEdgesCounts(int graph[][MAX_CITIES], int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (graph[i][j] != INT_MAX) // If there's an edge (not infinity)
            {
                count++; // Increment edge count
            }
        }
    }
    return count;
}

// Function to load cities from file, get their count and build the graph
CityList loadCity()
{
    CityList C = loadCitiesFromFile("cities.txt");
    if (C != NULL)
    {
        cityCount = getCityCount(C);
        buildCityGraph("cities.txt", C, graph); // Build the adjacency matrix
        printf("\nGraph loaded successfully!\n");
        printf("Graph order (number of cities): %d\n", cityCount);
        printf("Graph size (number of edges): %d\n\n", getEdgesCounts(graph, cityCount));
    }
    return C;
}

// Function to get the city name corresponding to a given index
char* getCityName(int index, CityList C)
{
    CityList currentCity = C->Next;
    while (currentCity != NULL)
    {
        if (currentCity->index == index)
            return currentCity->CityName;
        currentCity = currentCity->Next;
    }
    return NULL;
}

// Function to create and return a new MinHeapNode
MinHeapNode newMinHeapNode(int vertex, int weight)
{
    MinHeapNode node;
    node.vertex = vertex;
    node.weight = weight;
    return node;
}

// Function to create and initialize a MinHeap
MinHeap* createMinHeap(int capacity)
{
    MinHeap *H = (MinHeap*)malloc(sizeof(struct MinHeap));
    if (H == NULL)
        printf("Out of space!\n");

    H->pos = (int*)malloc(capacity * sizeof(int));
    if (H->pos == NULL)
    {
        printf("Out of space!\n");
        free(H); // Free the heap structure if pos allocation fails
    }
    // Allocate memory for the array of MinHeapNodes
    H->array = (MinHeapNode*)malloc(capacity * sizeof(MinHeapNode));
    if (H->array == NULL)
    {
        printf("Out of space for MinHeap array!\n");
        free(H->pos);
        free(H);
    }
    H->size = 0;
    H->capacity = capacity;
    return H;
}

//Function to swap two MinHeapNode pointers
void swapNode(MinHeapNode *H1, MinHeapNode *H2)
{
    struct MinHeapNode temp = *H1;
    *H1 = *H2;
    *H2 = temp;
}

// Function to heapify the min heap
void minHeapify(MinHeap *H, int index)
{
    int small, left, right;
    small = index;
    left = 2 * index + 1; //Calculate left child index
    right = 2 * index + 2; //Calculate right child index

    //If left child exists and is smaller than current smallest update smallest to left child
    if (left < H->size && H->array[left].weight < H->array[small].weight)
        small = left;

    //If right child exists and is smaller than current smallest update smallest to right child
    if (right < H->size && H->array[right].weight < H->array[small].weight)
        small = right;

    //If the smallest is not the current node
    if (small != index)
    {
        //Update positions in the pos array after swap
        H->pos[H->array[small].vertex] = index;
        H->pos[H->array[index].vertex] = small;

        //Swap the nodes in the heap array
        swapNode(&H->array[small], &H->array[index]);

        //Recursively heapify the affected subtree
        minHeapify(H, small);
    }
}

//Function to check if the min heap is empty
int isEmpty(MinHeap *H)
{
    return H->size == 0;
}

//Function to remove and return the node with the minimum key (weight) from the min heap
MinHeapNode removeMin(MinHeap* H)
{
    if (isEmpty(H))
        printf("Empty heap!\n");

    MinHeapNode firstNode = H->array[0]; //Store the root (minimum element)
    MinHeapNode lastNode = H->array[H->size - 1]; //Store the last element
    H->array[0] = lastNode;//Move the last element to the root

    // Update positions in the pos array
    H->pos[firstNode.vertex] = H->size - 1; //Old root's vertex now at last position
    H->pos[lastNode.vertex] = 0; //Last element's vertex now at root position

    --H->size;
    //Heapify from the root
    minHeapify(H, 0);
    return firstNode;
}

// Function to decrease the key (weight) of a specific vertex in the min heap
void decreaseKey(MinHeap *H, int vertix, int weight)
{
    // Get the current index of the vertex in the heap array using the pos array
    int temp = H->pos[vertix];
    // Update the weight of that node
    H->array[temp].weight = weight;

    while (temp > 0 && H->array[temp].weight < H->array[(temp - 1) / 2].weight)
    {
        //update positions in the pos array for both current and parent nodes
        H->pos[H->array[temp].vertex] = (temp - 1) / 2;
        H->pos[H->array[(temp - 1) / 2].vertex] = temp;

        //Swap the current node with its parent
        swapNode(&H->array[temp], &H->array[(temp - 1) / 2]);

        //Move to the parent's index
        temp = (temp - 1) / 2;
    }
}

// Function to Implements Prim's algorithm to find the Minimum Spanning Tree
MST primsMSTAlgo(int graph[][MAX_CITIES], int V, CityList cityList, int source)
{
    MST result = (MST)malloc(sizeof(struct MST));
    if (result == NULL)
    {
        printf("Out of memory for Prim's MST result!\n");
        return NULL;
    }

    result->Edges = (Edge*)malloc((V - 1) * sizeof(Edge));
    if (result->Edges == NULL)
    {
        printf("Out of memory for Prim's MST edges!\n");
        // Free the MST structure itself if edge allocation fails
        free(result);
        return NULL;
    }
    result->totalCost = 0;
    result->edgeCount = 0;
    result->Time = 0;

    // Arrays to keep track of MST state
    int parent[V];//Array to stores the parent of each vertex in the MST
    int key[V]; //Array to stores the minimum weight edge connecting a vertex to the MST
    int inMST[V];// Boolean array 1 if vertex is in MST

    //Create a min heap with capacity V
    MinHeap *heap = createMinHeap(V);
    if (heap == NULL)
    {
        free(result->Edges);
        free(result);
        return NULL;
    }

    // Initialize key values, parent array, and inMST array
    for (int v = 0; v < V; v++)
    {
        key[v] = INT_MAX;// All key values initialized to infinity
        parent[v] = -1; // No parent initially
        inMST[v] = 0;// No vertex is in MST yet

        // Add all vertices to the min-heap with their initial infinite key values
        heap->array[v] = newMinHeapNode(v, key[v]);
        heap->pos[v] = v; // Initialize positions in the pos array
    }
    //Set key of the source vertex to 0
    key[source] = 0;

    // Decrease the key of the source vertex in the min heap to 0
    decreaseKey(heap, source, key[source]);
    heap->size = V;//Set heap size to total number of vertices

    // Loop until the min heap is empty (all vertices processed)
    while (heap->size > 0)
    {
        MinHeapNode minNode = removeMin(heap); // Extract the vertex with the minimum key
        int u = minNode.vertex;// Get the extracted vertex
        inMST[u] = 1;// Mark it as included in MST

        // Iterate through all adjacent vertices of u
        for (int v = 0; v < V; v++)
        {
            // Check if there's an edge from u to v
            if (graph[u][v] != INT_MAX && !inMST[v] && graph[u][v] < key[v])
            {
                key[v] = graph[u][v];// Update key[v] to this smaller weight
                parent[v] = u;// Set u as the parent of v in the MST
                decreaseKey(heap, v, key[v]); // Update v's key in the min heap
            }
        }
    }

    // Construct the MST result from the parent array
    for (int i = 0; i < V; i++)
    {
        if (parent[i] != -1) // If a vertex has a parent
        {
            //Add the edge to the MST result
            result->Edges[result->edgeCount++] = (Edge){parent[i], i, graph[i][parent[i]]};
            //Add edge weight to total cost
            result->totalCost += graph[i][parent[i]];
        }
    }

    //Free min heap memory
    free(heap->array);
    free(heap->pos);
    free(heap);

    return result;
}

//Function  to find the root of the set
int find(Subset subsets[], int i)
{
    // If i is not the parent of itself, then it's not the root
    if (subsets[i].parent != i)
        //set parent[i] to the root of its set
        subsets[i].parent = find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// Function to union two sets containing elements n1 and n2
void Union(Subset subsets[], int n1, int n2)
{
    //Find the roots of the sets containing n1 and n2
    int n1root = find(subsets, n1);
    int n2root = find(subsets, n2);

    //Attach smaller rank tree under root of high rank tree (Union by Rank)
    if (subsets[n1root].rank < subsets[n2root].rank)
        subsets[n1root].parent = n2root;
    else if (subsets[n1root].rank > subsets[n2root].rank)
        subsets[n2root].parent = n1root;
    // If ranks are the same, make one as root and increment its rank
    else
    {
        subsets[n2root].parent = n1root;
        subsets[n1root].rank++;
    }
}

// Implements Kruskal's algorithm to find the Minimum Spanning Tree
MST kruskalsMSTAlgo(int graph[][MAX_CITIES], int V, CityList cityList)
{
    MST M = (MST)malloc(sizeof(struct MST));
    if (M == NULL)
    {
        printf("Out of memory for MST.\n");
        return NULL;
    }

    M->Edges = (Edge*)malloc((V - 1) * sizeof(Edge));
    if (M->Edges == NULL)
    {
        printf("Out of memory for MST edges!\n");
        free(M);
        return NULL;
    }
    M->edgeCount = 0;
    M->totalCost = 0;

    // Calculate the exact number of edges in the graph
    int num_graph_edges = getEdgesCounts(graph, V);

    struct Edge* E = (struct Edge*)malloc(num_graph_edges * sizeof(struct Edge));
    if (E == NULL)
    {
        printf("Out of memory for all graph edges!\n");
        free(M->Edges);
        free(M);
        return NULL;
    }

    int edgeIndex = 0;
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if (graph[i][j] != INT_MAX) // If an edge exists
            {
                E[edgeIndex].Src = i;
                E[edgeIndex].Dest = j;
                E[edgeIndex++].Weight = graph[i][j];
            }
        }
    }

    // Sort edges by weight in ascending order
    for (int i = 0; i < num_graph_edges - 1; i++)
    {
        for (int j = 0; j < num_graph_edges - i - 1; j++)
        {
            if (E[j].Weight > E[j + 1].Weight)
            {
                Edge temp = E[j]; // Swap
                E[j] = E[j + 1];
                E[j + 1] = temp;
            }
        }
    }

    Subset *s = (struct Subset*)malloc(V * sizeof(struct Subset));
    if (s == NULL)
    {
        printf("Out of memory for subsets!\n");
        free(E);
        free(M->Edges);
        free(M);
        return NULL;
    }

    for (int v = 0; v < V; v++)
    {
        s[v].parent = v; // Each element is its own parent
        s[v].rank = 0;  // Initial rank is 0
    }

    int e = 0, i = 0;
    while (e < V - 1 && i < num_graph_edges)
    {
        Edge nextEdge = E[i++]; // Get the next smallest edge

        // Find the roots of the source and destination vertices
        int in1 = find(s, nextEdge.Src);
        int in2 = find(s, nextEdge.Dest);

        // If the source and destination are in different sets
        if (in1 != in2)
        {
            M->Edges[e++] = nextEdge; // Add the edge to the MST result
            M->totalCost += nextEdge.Weight; // Add its weight to the total cost
            Union(s, in1, in2);// Union the two sets
        }
    }
    M->edgeCount = e; // Set the final count of edges in the MST
    free(E);
    free(s);
    return M;
}

// Function to compare the total cost and execution time of Prim's and Kruskal's algorithms.
void compareAlgorithms(MST prim, MST kruskal)
{
    printf("\nComparison between Prim's and Kruskal's algorithms:\n");
    printf("\nPrim's cost = %d, Prim's execution time (sec) %lf\n\n", prim->totalCost, prim->Time);
    printf("Kruskal's cost = %d, Kruskal's execution time (sec) %lf\n\n", kruskal->totalCost, kruskal->Time);

    // Compare total costs
    if (prim->totalCost == kruskal->totalCost)
    {
        printf("Both algorithms produced the same total cost which is equal %d\n\n", kruskal->totalCost);
    }
    else if (prim->totalCost > kruskal->totalCost)
    {
        printf("Kruskal's algorithm produced less cost\n\n");
    }
    else
    {
        printf("Prim's algorithm produced less cost\n\n");
    }

    // Compare execution times
    if (prim->Time > kruskal->Time)
    {
        printf("Kruskal's algorithm was faster by %lf seconds.\n\n", prim->Time - kruskal->Time);
    }
    else if (prim->Time < kruskal->Time)
    {
        printf("Prim's algorithm was faster by %lf seconds.\n\n", kruskal->Time - prim->Time);
    }
    else
    {
        printf("Both algorithms took the same amount of time.\n\n");
    }
}

// Function to print the details of an MST
void printMST(MST M, CityList C)
{
    if (M == NULL)
    {
        printf("MST is NULL, cannot print.\n");
        return;
    }
    printf("\nTotal Cost = %d\n", M->totalCost);
    printf("Execution time = %lf seconds\n", M->Time);
    printf("\nMinimum Spanning Tree edges:\n\n");

    printf("Source - Distination -> Cost\n\n");
    for (int i = 0; i < M->edgeCount; i++)
    {
        printf("%s - %s -> %d\n", getCityName(M->Edges[i].Src, C), getCityName(M->Edges[i].Dest, C), M->Edges[i].Weight);
    }
    printf("\n");
}
