#pragma once
#include "Component.h"
#include "Creature.h"
#include <Vector3.h>
#include <vector>
#include <Renderer2D.h>

// Used to ensure the nodes are evenly spaced across the screen
const int NODE_BUFFER = 15;

// The colour red, for nodes which have not been searched
const math::Vector3 UNSEARCHED{ 1,0,0 };
// The colour yellow, for nodes which are being searched
const math::Vector3 SEARCHING{ 1,1,0 };
// The colour green, for nodes which have been searched
const math::Vector3 SEARCHED{ 0,1,0 };
// The colour blue, for nodes which have been chosen as the path
const math::Vector3 PATH{ 0,0,1 };
// The colour white
const math::Vector3 WHITE{ 1,1,1 };
// The colour black
const math::Vector3 BLACK{ 0,0,0 };

// Enum for choosing which heuristic to use when determining the hScore
enum Heuristic { Custom, Euclidean, Manhattan, Dijkstra};

struct Node;
// The path to a node
struct Path {
    // The constructor for the class, sets the target node and the cost to travel to that node
    Path(Node* _target, float _cost) {
        target = _target;
        cost = _cost;
    }
    // The target node
    Node* target = nullptr;
    // The cost to travel to the target node
    float cost = 0.0f;
};

// Node is a class for the nodes which makeup the navMap
struct Node {
    // The constructor for the class, sets the node's position
    Node(float x, float y) {
        pos.x = x;
        pos.y = y;
        pos.z = 1;
    }

    // Used for easily identifying the node
    int id;
    // The gScore of the node, distance already traversed
    float gScore;
    // The hScore of the node, estimated distance remaining
    float hScore;
    // The fScore of the node, estimated total route length
    float fScore;

    // The position of the node
    math::Vector3 pos;

    // Used when calculating the path, the node which arrived at this current node
    Node* parent = nullptr;
    // Each path from this node
    std::vector<Path*> paths;
    // The colour of the node
    math::Vector3 colour = UNSEARCHED;
};

// The component which contains the grid of nodes and also determines the most efficient path to a target through the grid
class NavMap : public Component {
public:
    // The constructor for the class
    // parent is the sceneobject which owns the component, mapLength and mapHeight determine the size of the area to be covered by the map - the screen size
    // density determines how many nodes are to be placed - must be between 1 and 10
    // hWeight determines how strongly the hScore of nodes affect the overall cost of a node
    NavMap(SceneObject* parent, int mapLength, int mapHeight, int density, float hWeight = 1);
    // Initialises the class
    void init(int mapLength, int mapHeight, int density, float hWeight = 1);

    // Creates the nodes and sets up the paths between them
    void createNodes();

    // Draws the nodes and their paths to the screen
    void drawNodes(aie::Renderer2D* renderer);

    // Returns the closest node to the input origin
    Node* getClosestNode(math::Vector3 origin);

    // Determines the most efficient route from the startNode to the endNode using a Eulcidean heuristic
    std::vector<Node*> aStarSearch(Node* startNode, Node* endNode);
    // Determines the most efficient route from the startNode to the endNode using a Custom heuristic
    // The custom heursitic uses the Euclidean distance and the distance to each enemy
    std::vector<Node*> aStarSearch(Node* startNode, Node* endNode, std::vector<Creature*> enemies);

    // This function resets the nodes, values and colours
    void resetNodes();

protected:
    // Returns a calculated hScore for a node, using the heuristic hType
    // enemies is only used for the custom heuristic and is defaulted to an empty vector
    float calcHScore(Node* startNode, Node* endNode, Heuristic hType, std::vector<Creature*> enemies = std::vector<Creature*>());

    // A weighting used to determine how strongly the hScore of nodes affect the overall cost of a node
    float m_hWeight = 1;

    // The max length of the nav Map
    int m_mapLength = 0;
    // The length of the map in terms of columns
    int m_length = 0;
    // The max height of the nav Map
    int m_mapHeight = 0;
    // The length of the map in terms of rows
    int m_height = 0;

    // m_density determines how many nodes are to be placed - must be between 1 and 10
    int m_density = 0;

    // A vector of every node contained in the map
    // The back of vector is the first node in the route
    std::vector<Node*> m_nodes;
};