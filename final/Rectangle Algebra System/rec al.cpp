#include "function.h"
#include <algorithm>
using namespace std;

Rectangle::Rectangle() : name(""), left(0), top(0), right(0), bottom(0), isValid(false) {}

Rectangle::Rectangle(string n, int _x1, int _y1, int _x2, int _y2) : name(n) {
    left   = min(_x1, _x2);
    right  = max(_x1, _x2);
    top    = max(_y1, _y2);
    bottom = min(_y1, _y2);
    isValid = (left != right && top != bottom);
}

Rectangle Rectangle::operator&(const Rectangle& other) const {
    if (!isValid || !other.isValid) return Rectangle();
    int nl = max(left,   other.left);
    int nr = min(right,  other.right);
    int nt = min(top,    other.top);
    int nb = max(bottom, other.bottom);
    if (nl >= nr || nb >= nt) return Rectangle();
    return Rectangle("", nl, nb, nr, nt);
}

Rectangle Rectangle::operator|(const Rectangle& other) const {
    if (!isValid) return other;
    if (!other.isValid) return *this;
    int nl = min(left,   other.left);
    int nr = max(right,  other.right);
    int nt = max(top,    other.top);
    int nb = min(bottom, other.bottom);
    return Rectangle("", nl, nb, nr, nt);
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    name    = other.name;
    left    = other.left;
    right   = other.right;
    top     = other.top;
    bottom  = other.bottom;
    isValid = other.isValid;
    return *this;
}

long long Rectangle::getArea() const {
    if (!isValid) return 0;
    return (long long)(right - left) * (top - bottom);
}