
#pragma once
#include "LL.hpp"
#include "Player.hpp"

struct Grid
{
    int rows, cols;
    char **indexes;
};

template <typename T>
class GG
{
public:
    GG();
    GG(int r, int c);

    void PrintAdjacencyList();
    void Print();
    
    void SimulateAutoCarMovement(int source, int dest);
    void SimulatePlayerCarMovement(int source, int dest);
    void StartMenu();

private:
    int vertices;
    Grid grid;
    LL<Vertex> adj_list;
    Player player;
    int level;

    int* Djisktra(int source, int dest);
    void CreateMatrix(int rows, int cols);
    void GenerateRandomGraph();
    void MakeConnections();
    void PrintVertexConnections(int v);

    bool validateNextPos(int x, int y, int curX, int curY);
    bool validateStartPos(int x, int y);
    void ManageScore(char object);
    void SortLeaderboards();
    void UpdateRecords();

    void storeCurrentProgress();
    void restoreCurrentProgress();
};
