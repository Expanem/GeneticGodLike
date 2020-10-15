/*!
  \file   pathfinding.cc
  \date   June 2020
  \brief  A* implementation
*/

#include <cmath>
#include <vector>

using namespace std;

#include "tile.h"
#include "const.h"
#include "tools.h"

bool operator==(Coordinates c1, Coordinates c2) { // PUT ELSEWHERE
    if (c1.x == c2.x && c1.y == c2.y) {
        return true;
    } else {
        return false;
    }
}

class Node {
  public:
    Node(Coordinates at, Node* from = nullptr);
    Node(Node* const& copy);
    ~Node() { delete parent; };
    Coordinates position;
    Node* parent;
    int totalCost;
    int endCost;
    int beginCost;
};

Node::Node(Coordinates at, Node* from)
    : position(at), 
      totalCost(0), 
      endCost(0), 
      beginCost(0), 
      parent(from) {}

Node::Node(Node* const& copy)
    : parent(copy->parent),
      position(copy->position), 
      totalCost(copy->totalCost),
      endCost(copy->endCost), 
      beginCost(copy->beginCost) {}

void getBestOpenedNode(vector<Node*>& openedList, vector<Node*>& closedList, Node*& current) {
    current = openedList[0];
    long unsigned int currentIndex = 0;
    for (long unsigned int o = 0; o < openedList.size(); o++) {
        if (openedList[o]->totalCost < current->totalCost) {
            current = openedList[o];
            currentIndex = o;
        }
    }
    openedList.erase(openedList.begin() + currentIndex);
    closedList.push_back(current);
}

bool won(vector<Coordinates>& path, Node* current, Node* start, Node* target, bool& foundSolution) {
    if (current->position == target->position) {
        Node* back = current;
        if (start->position == back->position){foundSolution = true; return true;}
        while (not(back->parent->position == start->position)) {
            path.push_back(back->parent->position);
            back = back->parent;
        }
        foundSolution = true;
        return true;
    }
    return false;
}

void getNewChildren(Obstacles_map* obstacles, vector<Node*>& children, Node* current) {
    for (short i = -1; i <= 1; i++) {
        for (short j = -1; j <= 1; j++) {
            if (not(abs(i) == abs(j))) {
                Coordinates newPosition = {current->position.y + i, current->position.x + j};
                if (newPosition.y < 0 || newPosition.y >= world_size) {
                    continue;
                }
                if (newPosition.x < 0 || newPosition.x >= world_size) {
                    continue;
                }
                if (obstacles->is_obstacle(newPosition.y, newPosition.x)) { continue; }
                children.push_back(new Node(newPosition, current));
            }
        }
    }
}

void addChildrenToOpened(vector<Node*>& openedList, vector<Node*> closedList, vector<Node*>& children, Node* current, Node* target) {
    for (long unsigned int childID = 0; childID < children.size(); childID++) {
        bool already = false;
        for (long unsigned int c = 0; c < closedList.size(); c++) {
            if (children[childID]->position == closedList[c]->position) {
                already = true;
            }
        }
        if (not already) {
            double lDistance = children[childID]->position.y - target->position.y;
            double cDistance = children[childID]->position.x - target->position.x;
            children[childID]->beginCost = current->beginCost + 1;
            children[childID]->endCost = pow(lDistance, 2) + pow(cDistance, 2);
            children[childID]->totalCost = children[childID]->beginCost + children[childID]->endCost;

            for (long unsigned int o = 0; o < openedList.size(); o++) {
                if (children[childID]->position == openedList[o]->position &&
                    children[childID]->beginCost > openedList[o]->beginCost) {
                    continue;
                }
            }

            openedList.push_back(children[childID]);
        }
    }
}



Coordinates pathfinding(Coordinates start_coord, Coordinates end_coord, Obstacles_map* obstacles) {
    int direction;
    Node* start = new Node( {start_coord.x, start_coord.y});
    Node* target = new Node( {end_coord.x, end_coord.y});
    Node* current(start);
    vector<Node*> openedList;
    vector<Node*> closedList;
    vector<Coordinates> path;
    openedList.push_back(start);
    bool foundSolution = false;
    Coordinates next_coord = {-1,-1};

    do {
        getBestOpenedNode(openedList, closedList, current);
        if (won(path, current, start, target, foundSolution)) { break; }
        vector<Node*> children;
        getNewChildren(obstacles, children, current);
        addChildrenToOpened(openedList, closedList, children, current, target);
    } while (openedList.size() > 0);

    long int nextStepID = path.size() - 1;
    /*
    if (nextStepID < 0) {
        if (foundSolution) {
            return angle(players->getPlayer(startID)->getX(),
                         players->getPlayer(startID)->getY(),
                         players->getPlayer(targetID)->getX(),
                         players->getPlayer(targetID)->getY());
        } else { cout << "NO SOLUTION FOR PATH"; exit(1); }
    }
    */
    next_coord.x = path[nextStepID].x;
    next_coord.y = path[nextStepID].y;
    return next_coord;
}