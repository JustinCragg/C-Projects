#include "NavMap.h"
#include "Creature.h"
#include <Functions.h>
#include <Renderer2D.h>
#include <list>
#include <algorithm>
#include <iostream>

/****************************************************************************************************
****************************************************************************************************/
NavMap::NavMap(SceneObject* parent, int mapLength, int mapHeight, int density, float hWeight) : Component(parent) {
    init(mapLength, mapHeight, density, hWeight);
}

/****************************************************************************************************
****************************************************************************************************/
void NavMap::init(int mapLength, int mapHeight, int density, float hWeight) {
    m_componentType = comp::NavMap;

    m_mapLength = mapLength;
    m_mapHeight = mapHeight;

    m_density = density * NODE_BUFFER;
    m_hWeight = hWeight;

    createNodes();
}

/****************************************************************************************************
****************************************************************************************************/
void NavMap::createNodes() {
    m_nodes.clear();
    // The distance between each node
    int offset = (NODE_BUFFER * 11) - m_density;

    // Fills out the area
    for (float y = (m_mapHeight / 2.0f) - offset; y > (-m_mapHeight / 2.0f) + offset; y -= offset) {
        m_height++;
        for (float x = (-m_mapLength / 2.0f) + offset; x < (m_mapLength / 2.0f) - offset; x += offset) {
            m_nodes.push_back(new Node{ x, y });
            m_length++;
        }
    }
    m_length /= m_height;

    for (int i = 0; i < (int)m_nodes.size(); i++) {
        m_nodes[i]->id = i + 1;
        int x = i % m_length;
        int y = (i / m_length) % m_height;

        // Add all of the possible paths
        if (x != m_length - 1) {
            float cost = math::distanceBetweenSq(m_nodes[i]->pos, m_nodes[i + 1]->pos);
            m_nodes[i + 1]->paths.push_back(new Path{ m_nodes[i], cost });
            m_nodes[i]->paths.push_back(new Path{ m_nodes[i + 1], cost });
        }
        if (y != m_height - 1) {
            float cost = math::distanceBetweenSq(m_nodes[i]->pos, m_nodes[i + m_length]->pos);
            m_nodes[i + m_length]->paths.push_back(new Path{ m_nodes[i], cost });
            m_nodes[i]->paths.push_back(new Path{ m_nodes[i + m_length], cost });
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
void NavMap::drawNodes(aie::Renderer2D* renderer) {
    for (Node* node : m_nodes) {
            renderer->setRenderColour(node->colour.x, node->colour.y, node->colour.z);
            renderer->drawCircle(node->pos.x, node->pos.y, 5, 1);
        for (Path* path : node->paths) {
            if (path->cost == INFINITY) {
                renderer->setRenderColour(1,0,1);
                renderer->drawLine(node->pos.x, node->pos.y, path->target->pos.x, path->target->pos.y, 1, 2);
            }
            else {
                renderer->setRenderColour(WHITE.x, WHITE.y, WHITE.z);
                //renderer->drawLine(node->pos.x, node->pos.y, path->target->pos.x, path->target->pos.y, 1, 2); // Uncomment this to draw the paths between nodes
            }
        }
    }
}

/****************************************************************************************************
****************************************************************************************************/
Node* NavMap::getClosestNode(math::Vector3 origin) {
    float outputDist = -1;
    Node* outputNode = nullptr;
    float distance = 0;

    for (Node* node : m_nodes) {
        distance = math::distanceBetweenSq(node->pos, origin);
        if (outputDist == -1 || distance < outputDist) {
            outputDist = distance;
            outputNode = node;
        }
    }

    return outputNode;
}

/****************************************************************************************************
****************************************************************************************************/
float NavMap::calcHScore(Node* currentNode, Node* endNode, Heuristic hType, std::vector<Creature*> enemies) {
    float hScore = 0;

    switch (hType) {
    case Custom:
        // Euclidean
        hScore = math::distanceBetweenSq(currentNode->pos, endNode->pos);
        // Larger score is less distance to enemies
        for (Creature* enemy : enemies) {
            float inverseDistance = (SHORT_DISTANCE*SHORT_DISTANCE) - (enemy->m_distance);
            hScore += inverseDistance * 2.0f;
        }
        break;
    case Euclidean:
        hScore = math::distanceBetweenSq(currentNode->pos, endNode->pos);
        break;
    case Manhattan:
        hScore = abs(endNode->pos.x - currentNode->pos.x) + abs(endNode->pos.y - currentNode->pos.y);
        break;
    case Dijkstra:
        hScore = 0;
        break;
    }
    
    return hScore * m_hWeight;
}

/****************************************************************************************************
****************************************************************************************************/
std::vector<Node*> NavMap::aStarSearch(Node* startNode, Node* endNode) {
    std::list<Node*> searching;
    std::list<Node*> searched;

    startNode->gScore = 0.0f;
    startNode->parent = nullptr;
    searching.push_back(startNode);

    while (searching.empty() == false) {
        // Sort the nodes been searched by their fScore
        // Smallest value first
        searching.sort([](const Node* a, const Node* b) { return a->fScore < b->fScore; });
        Node* currentNode = searching.front();
        if (currentNode == endNode) {
            break;
        }
        searching.pop_front();
        currentNode->colour = SEARCHED;
        searched.push_back(currentNode);

        // For each path the node has
        for (Path* path : currentNode->paths) {
            Node* target = path->target;
            if (std::find(searched.begin(), searched.end(), target) == searched.end()) {
                    // Calculate the hScore if it doesn't already have one
                if (target->hScore == INFINITY) {
                    target->hScore = calcHScore(target, endNode, Heuristic::Euclidean);
                }
                // Determine if traversing the node is likely to be quicker
                if (currentNode->gScore + path->cost + target->hScore < target->fScore) {
                    searching.push_back(target);
                    target->gScore = currentNode->gScore + path->cost; // Distance traversed already
                    target->fScore = target->gScore + target->hScore; // Estimated total route length

                    target->colour = SEARCHING;
                    target->parent = currentNode;
                }
            }
        }
    }

    std::vector<Node*> outputPath;
    Node* currentNode = endNode;
    while (currentNode != nullptr) {
        outputPath.push_back(currentNode);
        currentNode->colour = PATH;
        currentNode = currentNode->parent;
    }

    return outputPath;
}

/****************************************************************************************************
****************************************************************************************************/
std::vector<Node*> NavMap::aStarSearch(Node* startNode, Node* endNode, std::vector<Creature*> enemies) {
    std::list<Node*> searching;
    std::list<Node*> searched;

    startNode->gScore = 0.0f;
    startNode->parent = nullptr;
    searching.push_back(startNode);

    while (searching.empty() == false) {
        // Sort the nodes been searched by their fScore
        // Smallest value first
        searching.sort([](const Node* a, const Node* b) { return a->fScore < b->fScore; });
        Node* currentNode = searching.front();
        if (currentNode == endNode) {
            break;
        }
        searching.pop_front();
        currentNode->colour = SEARCHED;
        searched.push_back(currentNode);

        // For each path the node has
        for (Path* path : currentNode->paths) {
            Node* target = path->target;
            if (std::find(searched.begin(), searched.end(), target) == searched.end()) {
                // Calculate the hScore if it doesn't already have one
                if (target->hScore == INFINITY) {
                    for (Creature* creature : enemies) {
                        // Calculates distance between the currentNode and each of the nearby enemies
                        creature->m_distance = math::distanceBetweenSq(((Transform*)creature->getComponent(comp::Transform))->getWorldTransform()[2], target->pos);
                        if (creature->m_distance > SHORT_DISTANCE*SHORT_DISTANCE) {
                            creature->m_distance = SHORT_DISTANCE*SHORT_DISTANCE;
                        }
                    }

                    target->hScore = calcHScore(target, endNode, Heuristic::Custom, enemies);
                }
                // Determine if traversing the node is likely to be quicker
                if (currentNode->gScore + path->cost + target->hScore < target->fScore) {
                    searching.push_back(target);
                    target->gScore = currentNode->gScore + path->cost; // Distance traversed already
                    target->fScore = target->gScore + target->hScore; // Estimated total route length

                    target->colour = SEARCHING;
                    target->parent = currentNode;
                }
            }
        }
    }

    std::vector<Node*> outputPath;
    Node* currentNode = endNode;
    while (currentNode != nullptr) {
        outputPath.push_back(currentNode);
        currentNode->colour = PATH;
        currentNode = currentNode->parent;
    }

    return outputPath;
}

/****************************************************************************************************
****************************************************************************************************/
void NavMap::resetNodes() {
    for (Node* node : m_nodes) {
        node->colour = BLACK;
        //node->colour = UNSEARCHED; // Uncomment this line to have the nodes always be drawn
        node->fScore = INFINITY;
        node->gScore = INFINITY;
        node->hScore = INFINITY;
        node->parent = nullptr;
    }
}