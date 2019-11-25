#pragma once
#include <string>
#include <vector>

class MembershipFunction {
public:
    float virtual getMembership(float value) = 0;
    float virtual getMaxMembership() = 0;
	std::string _id;
};

class FuzzyLogic {
public:
    enum class FunctionTypes { LEFT_SHOULDER, RIGHT_SHOULDER, TRIANGLE, TRAPEZOID };

	std::vector<std::vector<MembershipFunction*>> m_logicFunctions = std::vector<std::vector<MembershipFunction*>>();

    static float AND(float f1, float f2);
    static float OR(float f1, float f2);
    static float NOT(float f1);
};

class LeftShoulderFunction : public MembershipFunction {
public:
    LeftShoulderFunction(std::string id, float x0, float x1);

    float virtual getMembership(float value);
    float virtual getMaxMembership();

protected:
    // Start
    float _x0;
    // End
    float _x1;
};

class RightShoulderFunction : public MembershipFunction {
public:
    RightShoulderFunction(std::string id, float x0, float x1);

    float virtual getMembership(float value);
    float virtual getMaxMembership();

protected:
    // Start
    float _x0;
    // End
    float _x1;
};

class TriangleFunction : public MembershipFunction {
public:
    TriangleFunction(std::string id, float x0, float x1, float x2);

    float virtual getMembership(float value);
    float virtual getMaxMembership();

protected:
    // Start
    float _x0;
    // Peak
    float _x1;
    // End
    float _x2;
};

class TrapezoidFunction : public MembershipFunction {
public:
    TrapezoidFunction(std::string id, float x0, float x1, float x2, float x3);

    float virtual getMembership(float value);
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