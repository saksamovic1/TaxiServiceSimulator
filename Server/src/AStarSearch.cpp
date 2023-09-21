#include "AStarSearch.h"
#include "math.h"
#include <iostream>
#include <algorithm>

AStarSearch::AStarSearch() {
    possibleDirections = std::vector<std::pair<int, int>>({std::pair<int, int>(0, -1),
                                                           std::pair<int, int>(0, 1),
                                                           std::pair<int, int>(-1, 0),
                                                           std::pair<int, int>(1, 0) });
}

void AStarSearch::setObstacleLocations(std::vector<std::pair<int, int>> obstacleLocations) {
    this->obstacleLocations = obstacleLocations;
}

std::vector<std::pair<int, int>> AStarSearch::findPath(std::pair<int, int> source,
                                                       std::pair<int, int> target) {

    Node* currentNode = nullptr;
    std::vector<Node*> openPath, closedPath;
    openPath.reserve(100);
    closedPath.reserve(100);
    openPath.push_back(new Node(source));


    while (!openPath.empty()) { //ako imamo tu elemenata
        auto currentIterator = openPath.begin(); // uzecemo
        currentNode = *currentIterator;

        for (auto it = openPath.begin(); it != openPath.end(); it++) {
            auto tempNode = *it;
            if (tempNode->getScore() <= currentNode->getScore()) {
                currentNode = tempNode;
                currentIterator = it;
            }
        } 

        if (currentNode->coordinates == target) {
            break;
        }

        closedPath.push_back(currentNode);
        openPath.erase(currentIterator);

        for (int i = 0; i < 4; i++)
        {
            std::pair<int, int> newCoordinates(currentNode->coordinates.first + possibleDirections[i].first,
                                               currentNode->coordinates.second + possibleDirections[i].second);
            if (detectCollision(newCoordinates) ||
                findNodeOnList(closedPath, newCoordinates)) {
                continue;
            }

            int totalCost = currentNode->G + ((i < 4) ? 10 : 14);

            Node *successor = findNodeOnList(openPath, newCoordinates);
            if (successor == nullptr) {
                successor = new Node(newCoordinates, currentNode);
                successor->G = totalCost;
                successor->H = heuristic(successor->coordinates, target);
                openPath.push_back(successor);
            }
            else if (totalCost < successor->G) {
                successor->parent = currentNode;
                successor->G = totalCost;
            }
        }

    }

    std::vector<std::pair<int, int>> path;
    while (currentNode != nullptr) {
        path.insert(path.begin(), currentNode->coordinates);
        currentNode = currentNode->parent;
    }

    path.erase(path.begin());


    releaseNodes(openPath);
    releaseNodes(closedPath);

    return path;
}

bool AStarSearch::detectCollision(std::pair<int, int> currentCoordinates)
{
    if (currentCoordinates.first < 0 || currentCoordinates.first >= 10 ||
        currentCoordinates.second < 0 || currentCoordinates.second >= 10 ||
        std::find(obstacleLocations.begin(), obstacleLocations.end(), currentCoordinates) != obstacleLocations.end()) {
        return true;
    }
    return false;
}

void AStarSearch::releaseNodes(std::vector<Node*> currentPath) {
    for (auto it = currentPath.begin(); it != currentPath.end(); it++) {
        delete *it; // ovo pobrise podatke
    }
    currentPath.clear(); // ovo pobrise objekte u vektoru
}

float AStarSearch::heuristic(std::pair<int, int> firstPoint, std::pair<int, int> secondPoint) //euclidean
{
    return sqrt(pow((firstPoint.first - secondPoint.first), 2) + pow((firstPoint.second - secondPoint.second), 2));
}

Node* AStarSearch::findNodeOnList(std::vector<Node*> nodes, std::pair<int, int> coordinates) {
    for (auto node: nodes) {
        if(node->coordinates == coordinates) {
            return node;
        }
    }
    return nullptr;
}
