#pragma once
#include <string>
#include <glm/vec4.hpp>

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    class EXPORT Need {
    public:
        Need(std::string name) { m_name = name; }

        std::string m_name = "";
        float m_value = 1.0f;
        float m_decay = 0.01f;

        glm::vec4 m_colour = glm::vec4(1);
    };
}