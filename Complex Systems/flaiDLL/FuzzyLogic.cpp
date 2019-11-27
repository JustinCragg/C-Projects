#include "stdafx.h"
#include "FuzzyLogic.h"

namespace flai {
    /************************************************************************************************************
    ************************************************************************************************************/
    FuzzyLogic::~FuzzyLogic() {
        for (std::vector<MembershipFunction*> funcList : m_logicFunctions) {
            for (MembershipFunction* func : funcList) {
                delete func;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // OPERATORS
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /************************************************************************************************************
    ************************************************************************************************************/
    float FuzzyLogic::AND(float f1, float f2) {
        return fmin(f1, f2);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float FuzzyLogic::OR(float f1, float f2) {
        return fmax(f1, f2);
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float FuzzyLogic::NOT(float f1) {
        return 1 - f1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LEFT SHOULDER
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /************************************************************************************************************
    ************************************************************************************************************/
    LeftShoulderFunction::LeftShoulderFunction(std::string id, float x0, float x1) {
        _id = id;
        _x0 = x0;
        _x1 = x1;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float LeftShoulderFunction::getMembership(float value) {
        float result = 0;
        if (value <= _x0) {
            result = 1;
        }
        else if (value >= _x1) {
            result = 0;
        }
        else {
            result = (-value / (_x1 - _x0)) + (_x1 / (_x1 - _x0));
        }
        return result;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float LeftShoulderFunction::getMaxMembership() {
        return _x0 * 0.5f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // RIGHT SHOULDER
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /************************************************************************************************************
    ************************************************************************************************************/
    RightShoulderFunction::RightShoulderFunction(std::string id, float x0, float x1) {
        _id = id;
        _x0 = x0;
        _x1 = x1;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float RightShoulderFunction::getMembership(float value) {
        float result = 0;
        if (value <= _x0) {
            result = 0;
        }
        else if (value >= _x1) {
            result = 1;
        }
        else {
            result = (value / (_x1 - _x0)) - (_x0 / (_x1 - _x0));
        }
        return result;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float RightShoulderFunction::getMaxMembership() {
        return (_x1 + 1) * 0.5f;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TRIANGLE
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /************************************************************************************************************
    ************************************************************************************************************/
    TriangleFunction::TriangleFunction(std::string id, float x0, float x1, float x2) {
        _id = id;
        _x0 = x0;
        _x1 = x1;
        _x2 = x2;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float TriangleFunction::getMembership(float value) {
        float result = 0;
        if (value <= _x0 || value >= _x2) {
            result = 0;
        }
        else if (value == _x1) {
            result = 1;
        }
        else if ((value > _x0) && (value < _x1)) {
            result = (value / (_x1 - _x0)) - (_x0 / (_x1 - _x0));
        }
        else {
            result = (-value / (_x2 - _x1)) + (_x2 / (_x2 - _x1));
        }
        return result;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float TriangleFunction::getMaxMembership() {
        return _x1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TRAPEZOID
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /************************************************************************************************************
    ************************************************************************************************************/
    TrapezoidFunction::TrapezoidFunction(std::string id, float x0, float x1, float x2, float x3) {
        _id = id;
        _x0 = x0;
        _x1 = x1;
        _x2 = x2;
        _x3 = x3;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float TrapezoidFunction::getMembership(float value) {
        float result = 0;
        if (value <= _x0 || value >= _x3)
            result = 0;
        else if ((value >= _x1) && (value <= _x2))
            result = 1;
        else if ((value > _x0) && (value < _x1))
            result = (value / (_x1 - _x0)) - (_x0 / (_x1 - _x0));
        else
            result = (-value / (_x3 - _x2)) + (_x3 / (_x3 - _x2));
        return result;
    }

    /************************************************************************************************************
    ************************************************************************************************************/
    float TrapezoidFunction::getMaxMembership() {
        return (_x1 + 1) * 0.5f;
    }
}