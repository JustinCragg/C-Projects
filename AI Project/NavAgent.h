#pragma once
#include "Component.h"
#include "NavMap.h"

// The component which handles the navigation of an agent through a navigational grid
class NavAgent : public Component{
public:
    // The constructor for the class
    // parent is the sceneObject which owns the component, navMap is a reference to the navMap which the navAgent should use
    NavAgent(SceneObject* parent, NavMap* navMap);
    // Initialises the class
    void init(NavMap* navMap);
    // The destructor for the class
    ~NavAgent();

    // Determines the most efficient path to the target position
    // Sets m_gotPath to true
    void calculatePath(math::Vector3 target);
    // Determines the most efficient path to the target position while attempting to avoid enemies
    // Sets m_gotPath to true
    void calculatePath(math::Vector3 target, std::vector<Creature*> enemies);

    // Moves the owning SceneObject toward the target position, through the determined route
    void navMove(float step);

    // A boolean determining whether the navAgent currently has a path to follow
    bool m_gotPath = false;

    // The current path of nodes to reach the end target
    std::vector<Node*> m_nodePath;

    // A reference to the navMap which the navAgent uses
    NavMap* m_navMap = nullptr;
protected:

    // The target position
    math::Vector3 m_target;
};