#pragma once
#include <string>
#include <vector>

#ifdef FLAIDLL_EXPORTS
#   define EXPORT __declspec(dllexport)
#else
#   define EXPORT __declspec(dllimport)
#endif

namespace flai {
    // Base class for Fuzzy Logic membership functions
    class EXPORT MembershipFunction {
    public:
        // Returns the membership the function has at value
        float virtual getMembership(float value) = 0;
        // Returns the maximum membership the function can have
        float virtual getMaxMembership() = 0;
        // An identifying string
        std::string _id;
    };

    // Class which contains the logic for the Fuzzy Logic engine
    class EXPORT FuzzyLogic {
    public:
        ~FuzzyLogic();

        // The types of functions which can be used
        enum class FunctionTypes { LEFT_SHOULDER, RIGHT_SHOULDER, TRIANGLE, TRAPEZOID };

        // The contained fuzzy logic functions
        std::vector<std::vector<MembershipFunction*>> m_logicFunctions = std::vector<std::vector<MembershipFunction*>>();

        // Returns the lowest of the two values
        static float AND(float f1, float f2);
        // Returns the greatest of the two values
        static float OR(float f1, float f2);
        // Returns 1 - the value
        static float NOT(float f1);
    };

    // A function which is 1 until x0 the decreases to x1
    class EXPORT LeftShoulderFunction : public MembershipFunction {
    public:
        // Standard constructor
        LeftShoulderFunction(std::string id, float x0, float x1);

        // Returns the membership the function has at value
        float virtual getMembership(float value);
        // Returns the maximum membership the function can have
        float virtual getMaxMembership();

    protected:
        // Start
        float _x0;
        // End
        float _x1;
    };

    // A function which increases from x0 until x1 whereupon it is 1
    class EXPORT RightShoulderFunction : public MembershipFunction {
    public:
        // Standard constructor
        RightShoulderFunction(std::string id, float x0, float x1);

        // Returns the membership the function has at value
        float virtual getMembership(float value);
        // Returns the maximum membership the function can have
        float virtual getMaxMembership();

    protected:
        // Start
        float _x0;
        // End
        float _x1;
    };

    // A function which increases from x0 to x1 then decreases again until x2
    class EXPORT TriangleFunction : public MembershipFunction {
    public:
        // Standard constructor
        TriangleFunction(std::string id, float x0, float x1, float x2);

        // Returns the membership the function has at value
        float virtual getMembership(float value);
        // Returns the maximum membership the function can have
        float virtual getMaxMembership();

    protected:
        // Start
        float _x0;
        // Peak
        float _x1;
        // End
        float _x2;
    };

    // A fuction which increases from x0 to x1, remains at 1 until x2, then decreases again until x3
    class EXPORT TrapezoidFunction : public MembershipFunction {
    public:
        // Standard constructor
        TrapezoidFunction(std::string id, float x0, float x1, float x2, float x3);

        // Returns the membership the function has at value
        float virtual getMembership(float value);
        // Returns the maximum membership the function can have
        float virtual getMaxMembership();

    protected:
        // Start
        float _x0;
        // Start Peak
        float _x1;
        // End Peak
        float _x2;
        // End
        float _x3;
    };
}