#pragma once
#include "Component.h"

#include <vector>
#include <Vector3.h>
#include <Matrix3.h>

// The component which handles the position, rotation and scale
// The class contatins both standard values for each as well as a local and world matrix
class Transform : public Component {
public:
    // Constructor takes the object's 'transform hierachy' parent
    Transform(SceneObject* parent, Transform* transformParent);
    // Constructor takes the object's 'transform hierachy' parent
    // As well as its new position, dimensions and rotation
    Transform(SceneObject* parent, Transform* transformParent, float x, float y, float length = 0, float height = 0, float rot = 0);
    // Initialises the class
    void init(Transform* transformParent, float x = 0, float y = 0, float length = 0, float height = 0, float rot = 0);
    // Default Destructor
    virtual ~Transform();

    // This object's transform hierachy parent
    Transform* m_transformParent = nullptr;
    // This object's transform hierachy children
    std::vector<Transform*> m_children;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Transforms
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Updates both the local and world transform
    void updateTransforms();

    // Returns the local transform matrix
    math::Matrix3 getLocalTransform() const;
    // Returns the world transform matrix
    math::Matrix3 getWorldTransform() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Position
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the object's local position
    math::Vector3 getPosition() const;

    // Sets the object's local position
    // Move to
    void setPosition(float x, float y);
    // Sets the object's local position
    // Move to
    void setPosition(math::Vector3 position);

    // Moves the object's local position
    // Move by
    void movePosition(float x, float y);
    // Moves the object's local position
    // Move by
    void movePosition(math::Vector3 offset);

    // Moves the object's local position in the direction of the target, by the step
    // Returns True if the object has reached the target
    // Move towards
    bool moveTowards(math::Vector3 target, float step);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scale
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the object's local scale
    math::Vector3 getScale() const;

    // Sets the object's local scale
    void setScale(float x, float y);
    // Sets the object's local scale
    void setScale(math::Vector3 position);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Rotation
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Returns the objects local rotation, in radians
    float getAngleRotation() const;

    // Sets the object's local rotation, in radians
    void setRotation(float angle);

    // Rotates the object's local rotation, in radians
    void rotateRotation(float angle);

protected:
    // Local Position
    math::Vector3 m_pos{ 0,0,1 };
    // Local Scale
    math::Vector3 m_scale{ 1,1,1 };
    // Local Rotation
    float m_rot = 0;

    // Boolean which shows whether the object's position will need to be updated
    bool m_dirty = true;

    // The object's local transform matrix
    math::Matrix3 m_localTransform;
    // The object's world transform matrix
    math::Matrix3 m_worldTransform;

    // Updates the object's local transform matrix
    void updateLocalTransform();
    // Updates the object's world transform matrix
    void updateWorldTransform();
};