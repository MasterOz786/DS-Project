
#include <iostream>
#include <fstream>
#include <algorithm>
#include<sstream>
#include <ctime>
#include <conio.h>
#include "GG.hpp"
#include "Queue.hpp"
#include <Windows.h>

// Graph class implementation
template <typename T>
GG<T>::GG()
{
    this->vertices = 0;
}

template <typename T>
GG<T>::GG(int r, int c)
{
    this->grid.rows = r;
    this->grid.cols = c;
    this->vertices = r * c;
    CreateMatrix(r, c);
}

template <typename T>
bool GG<T>::validateStartPos(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

    if (x < 0 || x % grid.cols > grid.cols || y < 0 || y % grid.rows > grid.rows)
    {
        std::cout << x << ' ' << y << '\n';
        std::cout << "Aukaatoo baahr!\n";
        return false;
    }
    else if (this->grid.indexes[x][y] == ' ')
    {
        std::cout << "Obstacle touunn ni start honna, kumm aali jaga toun gaddi nou sulf maaro ni te rehn deo..\n";
        return false;
    }

    return true;
}

template <typename T>
bool GG<T>::validateNextPos(int x, int y, int curX, int curY)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    if (x < 0 || x >= this->grid.rows || y < 0 || y >= this->grid.cols)
    {
        std::cout << "Invalid position\n";
        return false;
    }
    
    else if (x > curX + 1 || x < curX - 1 || y > curY + 1 || y < curY - 1)
    {
        std::cout << "Error: Jitni chaadar dekhain utney paaon phelain!\n";
        return false;
    }

    else if (this->grid.indexes[x][y] == ' ')
    {
        std::cout << "Kandd mei wajney ka ziada shok hai?\n";
        return false;
    }

    // prevent diagonals switching
    else if ((x > curX && y > curY || x > curX && y < curY) || (x < curX && y > curY || x < curX && y < curY))
    {
        std::cout << "Diagonals are not allowed!\n";
        return false;
    }
    return true;
}

template <typename T>
void GG<T>::PrintVertexConnections(int v)
{
    Node<Vertex> *temp = this->adj_list.head;
    for (int i = 0; i < v; i++)
    {
        temp = temp->next;
    }
    temp->data.PrintNeighbours();
}

template <typename T>
void GG<T>::PrintAdjacencyList()
{
    Node<Vertex> *temp = this->adj_list.head;
    for (int i = 0; i < this->grid.rows * this->grid.cols; i++)
    {
        std::cout << temp->data.GetID() << " -> ";
        temp->data.PrintNeighbours();
        temp = temp->next;
    }
}

template <typename T>
void GG<T>::CreateMatrix(int rows, int cols)
{
    // store the adjacency list in a pair matrix
    grid.indexes = new char *[rows];

    // assigning memory
    for (int i = 0; i < rows; i++)
    {
        grid.indexes[i] = new char[cols];
    }

    GenerateRandomGraph();
    MakeConnections();
}

template <typename T>
void GG<T>::GenerateRandomGraph()
{
    srand(time(0));
    // filling the adjacency list
    for (int i = 0; i < this->grid.rows; i++)
        for (int j = 0; j < this->grid.cols; j++)
        {
            // generate obstacles
            if (rand() % 7 == 0)
            {
                int weight = ' ';
                this->grid.indexes[i][j] = weight;
                this->adj_list.insertUnique(Vertex((i * this->grid.cols) + j, weight));
            }
            else
            {
                int weight = '+';
                this->grid.indexes[i][j] = weight;
                this->adj_list.insertUnique(Vertex((i * this->grid.cols) + j, weight));
            }
        }
}

template <typename T>
void GG<T>::Print()
{
    // Setting color control variable
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout << '\n';
    for (int i = 0; i < grid.rows; i++)
    {
        for (int j = 0; j < grid.cols; j++)
        {
            if (adj_list.searchNode((i * grid.cols) + j)->data.getNeighbours().head)
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
            std::cout << grid.indexes[i][j];
            // printing horizontal edges
            if (j < grid.cols - 1)
            {
                Node<Vertex> *temp = adj_list.searchNode((i * grid.cols) + j);
                Node<Edge> *temp2 = temp->data.getNeighbours().head;
                bool inserted = false;
                while (temp2 != nullptr)
                {
                    if (temp2->data.dest == (i * grid.cols) + j + 1)
                    {
                        // Changing text color
                        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                        std::cout << "----";
                        inserted = true;
                        break;
                    }
                    temp2 = temp2->next;
                }
                if (!inserted)
                {
                    std::cout << "    ";
                }
            }
        }
        std::cout << '\n';

        // printing vertical edges
        if (i < grid.rows - 1)
        {
            for (int j = 0; j < grid.cols; j++)
            {
                Node<Vertex> *temp = adj_list.searchNode((i * grid.cols) + j);
                Node<Edge> *temp2 = temp->data.getNeighbours().head;
                bool inserted = false;
                while (temp2 != nullptr)
                {
                    if (temp2->data.dest == ((i + 1) * grid.cols) + j)
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                        std::cout << "|    ";
                        inserted = true;
                        break;
                    }
                    temp2 = temp2->next;
                }
                if (!inserted)
                {
                    std::cout << "     ";
                }
            }
        }
        std::cout << '\n';
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

template <typename T>
void GG<T>::MakeConnections()
{
    // making connections
    Node<Vertex> *temp = this->adj_list.head;
    for (int i = 0; temp != nullptr; i++)
    {
        int source = temp->data.GetID();
        int weight = '+';

        if (temp->data.GetType() != ' ')
        {
            // make connections on the right
            if (source % grid.cols < grid.cols - 1 && source + 1 < this->vertices)
            {
                Node<Vertex> *vNode = adj_list.searchNode(source + 1);
                if (vNode->data.GetType() != ' ')
                {
                    weight = vNode->data.GetType();
                    temp->data.AddEdge(Edge(source, source + 1, weight));
                }
            }

            // make connections on the left
            if (source % grid.cols > 0 && source - 1 >= 0)
            {
                Node<Vertex> *vNode = adj_list.searchNode(source - 1);
                if (vNode->data.GetType() != ' ')
                {
                    weight = vNode->data.GetType();
                    temp->data.AddEdge(Edge(source, source - 1, weight));
                }
            }

            // make connections on the top
            if (source >= grid.cols)
            {
                Node<Vertex> *vNode = adj_list.searchNode(source - grid.cols);
                if (vNode->data.GetType() != ' ')
                {
                    weight = vNode->data.GetType();
                    temp->data.AddEdge(Edge(source, source - grid.cols, weight));
                }
            }

            // make connections on the bottom
            if (source < this->vertices - grid.cols && source + grid.cols < this->vertices)
            {
                Node<Vertex> *vNode = adj_list.searchNode(source + grid.cols);
                if (vNode->data.GetType() != ' ')
                {
                    weight = vNode->data.GetType();
                    temp->data.AddEdge(Edge(source, source + grid.cols, weight));
                }
            }
        }
        temp = temp->next;
    }
}

// Dijkstra Algorithim for finding shortest path from source to dest
template <typename T>
int *GG<T>::dijkstra(int source, int dest)
{
    // Setting color control variable
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // initializing the distance array
    int *dist = new int[this->vertices];
    for (int i = 0; i < this->vertices; i++)
    {
        dist[i] = 100000;
    }

    // initializing the parent array
    int *parent = new int[this->vertices];
    for (int i = 0; i < this->vertices; i++)
    {
        parent[i] = -1;
    }

    // initializing the visited array
    bool *visited = new bool[this->vertices];
    for (int i = 0; i < this->vertices; i++)
    {
        visited[i] = false;
    }

    // initializing the priority queue
    PriorityQueue<Vertex> pq;

    // setting the source distance to 0
    dist[source] = 0;
    pq.enqueue(adj_list.searchNode(source)->data);

    while (!pq.isEmpty())
    {
        Vertex u = pq.showFront();
        pq.dequeue();
        visited[u.GetID()] = true;

        Node<Edge> *temp = u.getNeighbours().head;
        while (temp != nullptr)
        {
            if (!visited[temp->data.dest])
            {
                int alt = dist[u.GetID()] + temp->data.weight;
                if (alt < dist[temp->data.dest])
                {
                    dist[temp->data.dest] = alt;
                    parent[temp->data.dest] = u.GetID();
                    pq.enqueue(adj_list.searchNode(temp->data.dest)->data);
                }
            }
            temp = temp->next;
        }
    }

    // printing the path
    int cur = dest;
    while (cur != -1)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        std::cout << cur;
        // remove the ending/trailing arrow
        if (cur != source)
            std::cout << " <- ";
        cur = parent[cur];
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // store the shortest path
    return parent;
}

// Function for showing car traveling from one point to the other on the grid and car is represented  by C and Destination is represented by D
template <typename T>
void GG<T>::SimulateAutoCarMovement(int source, int dest)
{
    int *parent = new int[this->vertices];
    parent = dijkstra(source, dest);

    // reversing the path array
    int *path = new int[this->vertices];
    int i = 0;
    int cur = dest;
    while (cur != -1)
    {
        path[i] = cur;
        cur = parent[cur];
        i++;
    }
    std::reverse(path, path + i);

    // resize the path array and delete the parent
    path = (int *)realloc(path, i * sizeof(int));
    delete[] parent;

    // setting the start point
    int x = source / this->grid.cols;
    int y = source % this->grid.cols;
    this->grid.indexes[x][y] = 'C';
    /// setting the end point
    x = dest / this->grid.cols;
    y = dest % this->grid.cols;
    this->grid.indexes[x][y] = 'D';

    x = -1, y = -1;
    cur = 0;
    while (i >= 0)
    {
        // removing the car from the previous position
        if (x != -1 && y != -1)
            this->grid.indexes[x][y] = '+';
        int index = path[cur];
        x = index / this->grid.cols;
        y = index % this->grid.cols;

        // printing the grid with delays and updating the car positon
        this->grid.indexes[x][y] = 'C';
        i == 0 ? Sleep(5000) : Sleep(500);
        system("cls");
        Print();

        // updating counters
        i--;
        cur++;
    }
}

template <typename T>
void GG<T>::SimulatePlayerCarMovement(int source, int dest)
{
    int sourceX, sourceY;
    int destX, destY;

    int curX, curY;
    sourceX = source / this->grid.cols;
    sourceY = source % this->grid.cols;
    this->grid.indexes[sourceX][sourceY] = 'C';
    curX = sourceX, curY = sourceY;

    destX = dest / this->grid.cols;
    destY = dest % this->grid.cols;
    this->grid.indexes[destX][destY] = 'D';

    while (sourceX != destX || sourceY != destY)
    {
        Print();
 
        char ch;

        std::cout << "Press arrow keys to move your car on path\n";
        std::cout << "Save and Quit? (y)\n";
        // std::cin >> x >> y;
            ch = _getch(); // Read the actual key code
            switch(ch) {
                case 72:   // For up arrow key

                        sourceX -= 1;
                    
                    break;
                case 80:   // For down key

                        sourceX += 1;
                    
                    break;
                case 75:   // For left arrow key
                        sourceY -= 1;
                    
                    break;
                case 77:  // For right arrow key
    
                        sourceY += 1;
                    
                    break;
                    case 'y':
                        storeCurrentProgress();
                        return;
                        break;
                default:
                    break;
            }
    

        

        if (validateNextPos(sourceX, sourceY, curX, curY) == false)
        {
            sourceX = curX;
            sourceY = curY ;
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

            //continue;
        }

            this->grid.indexes[sourceX][sourceY] = 'C';

        if(sourceX != curX || sourceY != curY)
            this->grid.indexes[curX][curY] = '+';
        curX = sourceX, curY = sourceY;


        

        system("cls");
        Print();
        // CHeck9ng the carv has reach the detination or not
        if(sourceX == destX && sourceY == destY){
            std::cout << "HURRAH, You have won the game!" << std::endl;
         }
    }
}

template <typename T>
void GG<T>::storeCurrentProgress()
{
    std::ofstream file;
    file.open("progress.txt");
    file << this->grid.rows << " " << this->grid.cols << '\n';
    // store the matrix
    for (int i = 0; i < this->grid.rows; i++)
    {
        for (int j = 0; j < this->grid.cols; j++)
        {
            file << this->grid.indexes[i][j];
        }
        file << '\n';
    }
    // delimiter
    file << "|\n";
    // store the adjacency list
    Node<Vertex> *temp = this->adj_list.head;
    for (int i = 0; i < this->grid.rows * this->grid.cols; i++)
    {
        file << temp->data.GetID();
        if(temp->data.getNeighbours().head != nullptr){
            file << " ";
        }
        Node<Edge> *temp2 = temp->data.getNeighbours().head;
        while (temp2 != nullptr)
        {
            file << temp2->data.dest;
            if(temp2->next != nullptr){
                file << " ";
            }
            temp2 = temp2->next;
        }
        file << '\n';
        temp = temp->next;
    }

    file.close();
}

template <typename T>
void GG<T>::restoreCurrentProgress()
{

    delete this->adj_list.head;
    this->adj_list.head = nullptr;
    this->adj_list.printList();

    std::ifstream file;
    file.open("progress.txt");
    int rows, cols;
    file >> rows >> cols >> std::ws;
    CreateMatrix(rows, cols);

    int source, dest;
    std::string line;
    for (int i = 0; i < rows; i++)
    {
        std::getline(file, line);
        for (int j = 0; j < cols; j++)
        {
            // update the grid
            this->grid.indexes[i][j] = line[j];
            // set the location of the car
            if (line[j] == 'C')
                source = (i * this->grid.cols) + j;

            // set the location of the destination
            else if (line[j] == 'D')
                dest = (i * this->grid.cols) + j;
        }
    }

    
    std::getline(file, line);  // Skipping a line

    std::getline(file, line);

    // Deleting randomly generatrd list
    delete adj_list.head;
    delete adj_list.tail;
    adj_list.head = nullptr;
    adj_list.tail = nullptr;

    // first integer is the vertex, rest are the edges
    while (file)
    {
    
        std::stringstream ss(line);
        int vertex;
        ss >> vertex;
        this->adj_list.insertAtEnd(vertex);  // Inserting vertix in adjacency list
        int edge;
        bool inserted = false;
        while (ss >> edge)
        {
            bool inserted = true;
            this->adj_list.searchNode(vertex)->data.AddEdge(Edge(vertex, edge, '+')); // Inserting edge


        }
        if(!inserted){
            this->adj_list.searchNode(vertex)->data.setType(' ');
        }
        std::getline(file, line);
        
    }

    file.close();

    SimulatePlayerCarMovement(source, dest) ;
}

template <typename T>
void GG<T>::StartMenu()
{
    std::string choice;
    std::cout << "Welcome!\n"
              << "1. Simulate Auto Car Movement\n"
              << "2. Simulate Player Car Movement\n"
              << "3. Resume previous game\n"
              << "4. Exit\n";
    std::getline(std::cin, choice);

    if (choice == "1" || choice == "2")
    {
        int sx, sy;
        int ex, ey;

        std::cout << "Enter the start index: ";
        std::cin >> sx >> sy;
        std::cout << "Enter the end index: ";
        std::cin >> ex >> ey;

        int source = (sx * this->grid.cols) + sy;
        int dest = (ex * this->grid.cols) + ey;

        if (validateStartPos(source, dest) == true)
        {
            if (choice == "1")
            {
                SimulateAutoCarMovement(source, dest);
            }
            else if (choice == "2")
            {
                SimulatePlayerCarMovement(source, dest);
            }

            else
            {
                std::cout << "Invalid choice!\n";
                StartMenu();
            }
        }
    }
    else if (choice == "3")
    {
        restoreCurrentProgress();
    }

    else if (choice == "4")
    {
        exit(0);
    }
    else
    {
        std::cout << "Invalid choice!\n";
        StartMenu();
    }
}
