#include "Point.h"
#include <string>
/****************************************************************************************************
****************************************************************************************************/
Point::Point() {
}

/****************************************************************************************************
****************************************************************************************************/
Point::Point(int ix, int iy) {
    m_x = ix;
    m_y = iy;
}

/****************************************************************************************************
****************************************************************************************************/
std::string Point::asString()const {
    std::string string = "(" + std::to_string(m_x) + "," + std::to_string(m_y) + ")";
    return string;
}

/****************************************************************************************************
****************************************************************************************************/
Point Point::operator+(const Point & p)const {
    Point point(0,0);
    point.m_x = m_x + p.m_x;
    point.m_y = m_y + p.m_y;
    return point;
}

/****************************************************************************************************
****************************************************************************************************/
Point Point::operator-(const Point & p)const {
    Point point(0,0);
    point.m_x = m_x - p.m_x;
    point.m_y = m_y - p.m_y;
    return point;
}

/****************************************************************************************************
****************************************************************************************************/
bool Point::operator==(const Point & p)const {
    bool same = false;
    if (m_x == p.m_x && m_y == p.m_y) {
        return true;
    }
    else {
        return false;
    }
}

/****************************************************************************************************
****************************************************************************************************/
bool Point::operator!=(const Point & p)const {
    bool same = false;
    if (m_x == p.m_x && m_y == p.m_y) {
        return false;
    }
    else {
        return true;
    }
}