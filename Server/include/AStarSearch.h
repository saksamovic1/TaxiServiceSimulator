#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H

#include <vector>
#include <functional>
#include <set>

struct Node {
    int G, H; // ovo su one tezine u a star
    std::pair<int, int> coordinates; // koordinate trenutne tacke
    Node* parent;

    Node(std::pair<int, int> coordinates_, Node *parent_ = nullptr)
    {
        parent = parent_;
        coordinates = coordinates_;
        G = H = 0;
    }

    int getScore() {
        return G + H;
    }
};

class AStarSearch
{

public:
    AStarSearch();
    void setObstacleLocations(std::vector<std::pair<int, int>> obstacleLocations);
    bool detectCollision(std::pair<int, int> currentCoordinates);
    Node* findNodeOnList(std::vector<Node *> nodes, std::pair<int, int> currentCoordinates);
    void releaseNodes(std::vector<Node *> currentPath);
    std::vector<std::pair<int, int>> findPath(
        std::pair<int, int> beginCoordinates, std::pair<int, int> endCoordinates);

private:
    std::vector<std::pair<int, int>> obstacleLocations;
    std::vector<std::pair<int, int>> possibleDirections;
    float heuristic(std::pair<int, int>, std::pair<int, int>);
};

#endif // ASTARSEARCH_H
