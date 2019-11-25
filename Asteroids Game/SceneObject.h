#pragma once
#include <vector>
#include <Vector3.h>
#include <Matrix3.h>

const float PI = 3.1415927410125732421875;

class SceneObject {
public:
    SceneObject(SceneObject* parent);
    virtual ~SceneObject();

    SceneObject* m_parent = nullptr;
    std::vector<SceneObject*> m_children;

    struct State {
        math::Vector3 pos{ 0,0,1 };
        math::Vector3 scale{ 1,1,1 };
        float rot = 0;
    };

    State& getState() {
        m_dirty = true;
        return m_state;
    }
    const State& getState() const {
        return m_state;
    }

    SceneObject* getWorldRoot();
    void setParent(SceneObject* newParent);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Updating
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void update();
    virtual void draw();

    math::Matrix3 getLocalTransform() const;
    math::Matrix3 getWorldTransform() const;

    void setLocalTransform(math::Matrix3 newLocalTransform);

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Position
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void setPosition(float x, float y, float z = 1);
    void setPosition(math::Vector3 position);

    void movePosition(float x, float y, float z = 0);
    void movePosition(math::Vector3 position);

    math::Vector3 getPosition() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Rotation
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void setRotation(float angle);

    void rotateRotation(float angle);

    float getAngleRotation() const;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Scale
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    void setScale(float x, float y, float z = 1);
    void setScale(math::Vector3 position);

    // Needs to be changed to const
    math::Vector3 getScale();

private:
    State m_state;
    bool m_dirty = true;

    math::Matrix3 m_localTransform;
    math::Matrix3 m_worldTransform;

    void updateWorldTransform();
    void updateLocalTransform();
};