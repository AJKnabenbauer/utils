#include "Vector_Math.h"



namespace vmath
{

    // vec2f start
    // -------------------------------------------------------------------------------------------------------------------
    
    // vec2f
    // Constructors
    // -------------------------------------------------------------------------------------------------------------------
    
    vec2f::vec2f() {}

    vec2f::vec2f(float x0, float y0) :x(x0), y(y0) {}

    vec2f::vec2f(const vec2f& v) :x(v.x), y(v.y) {}

    vec2f::vec2f(const vec2f* v) 
    {
        if (v) {
            x = v->x;
            y = v->y;
        };
    }

    vec2f::vec2f(const D2D1_POINT_2F& v) :x(v.x), y(v.y) {}

    vec2f::vec2f(const D2D1_POINT_2F* v) 
    {
        if (v) {
            x = v->x;
            y = v->y;
        };
    }

    vec2f::vec2f(const POINT& p) :x((float)p.x), y((float)p.y) {}

    // -------------------------------------------------------------------------------------------------------------------
    // 

    
    // vec2f
    // Operator Overloads
    // -------------------------------------------------------------------------------------------------------------------

    vec2f vec2f::operator - () const 
    {
        return vec2f(-x, -y);
    }



    vec2f vec2f::operator + (vec2f v) const
    {
        return vec2f(x+v.x, y+v.y);
    }

    vec2f vec2f::operator - (vec2f v) const
    {
        return vec2f(x-v.x, y-v.y);
    }

    vec2f vec2f::operator * (vec2f v) const
    {
        return vec2f(x*v.x, y*v.y);
    }

    vec2f& vec2f::operator += (vec2f v)
    {
        *this = vec2f(x, y) + v; 
        return *this;
    }

    vec2f& vec2f::operator -= (vec2f v)
    {
        *this = vec2f(x, y) - v;
        return *this;
    }

    vec2f& vec2f::operator *= (vec2f v)
    {
        *this = vec2f(x, y) * v;
        return *this;
    }



    vec2f vec2f::operator * (float s) const
    {
        return vec2f(x*s, y*s);
    }

    vec2f vec2f::operator / (float s) const
    {
        return fIsZero(s) ? vec2f(x, y) : vec2f(x, y) * (1/s);
    }

    vec2f& vec2f::operator *= (float s)
    {
        *this = vec2f(x, y) * s;
        return *this;
    }

    vec2f& vec2f::operator /= (float s)
    {
        *this = vec2f(x, y) / s;
        return *this;
    }



    vec2f vec2f::operator * (D2D1_MATRIX_3X2_F& m) const
    {
        return vec2f(
            (x * m._11) + (y * m._21) + m._31,
            (x * m._12) + (y * m._22) + m._32
        );
    }

    vec2f& vec2f::operator *= (D2D1_MATRIX_3X2_F& m)
    {
        *this = vec2f(x, y) * m; 
        return *this;
    }



    vec2f::operator bool() const
    {
        return ((fabs(x) > FLT_EPSILON) || (fabs(y) > FLT_EPSILON));
    }

    vec2f::operator D2D1_POINT_2F() const
    {
        return D2D1::Point2F(x, y);
    }

    // -------------------------------------------------------------------------------------------------------------------
    //


    // vec2f
    // Non-Static Member Functions
    // -------------------------------------------------------------------------------------------------------------------

    float vec2f::dotProduct(const vec2f& v)
    {
        return (x*v.x) + (y*v.y);
    }


    float vec2f::length()
    {
        return hypot(x, y); // equivilant to sqrt(x^2 + y^2)
    }


    float vec2f::lengthApprox()
    {
        // Error between -5% and +3% (+0.043% average). See https://gamedev.stackexchange.com/a/69255 
        float dx = fabs(x), dy = fabs(y);
        return 0.394f * (dx + dy) + 0.554f * max(dx, dy);
    }


    float vec2f::distance(const vec2f& v)
    {
        return (v-*this).length();
    }


    float vec2f::distanceApprox(const vec2f& v)
    {
        return (v-*this).lengthApprox();
    }


    vec2f vec2f::unitLengthVector()
    {
        float n = this->dotProduct(*this);
        return (n > 0) ? vec2f(x, y) * (1/sqrt(n)) : vec2f(0, 0);
    }


    vec2f& vec2f::normalize()
    {
        *this = this->unitLengthVector();
        return *this;
    }


    vec2f& vec2f::setLength(float len)
    {
        *this = this->unitLengthVector() * len;
        return *this;
    }


    float vec2f::cosine()
    {
        return this->unitLengthVector().dotProduct(vec2f(1, 0));
    }


    float vec2f::cosine(vec2f& v)
    {
        float a = this->dotProduct(*this);
        float b = v.dotProduct(v);
        return (a > 0 && b > 0) ? this->dotProduct(v) / (sqrt(a)*sqrt(b)) : 0;
    }


    float vec2f::angle()
    {
        return atan2(y, x);
    }


    float vec2f::angle(vec2f& v)
    {
        return v.angle() - this->angle();
    }


    float vec2f::angleDot()
    {
        return acos(this->cosine());
    }


    float vec2f::angleDot(vec2f& v)
    {
        return acos(this->cosine(v));
    }


    vec2f& vec2f::setAngle(float angle)
    {
        float length = this->length();
        if (length > 0)
            *this = vec2f(cos(angle), sin(angle)) * length;
        return *this;
    }


    vec2f& vec2f::setAngle(float angle, float length)
    {
        *this = vec2f(cos(angle), sin(angle)) * length;
        return *this;
    }


    vec2f& vec2f::setAngle(float Sin, float Cos, float length)
    {
        *this = vec2f(Cos, Sin) * length;
        return *this;
    }


    vec2f& vec2f::rotate(float angle)
    {
        float Sin = sin(angle), Cos = cos(angle);
        *this = vec2f(x*Cos - y*Sin, x*Sin + y*Cos);
        return *this;
    }


    vec2f& vec2f::rotate(float Sin, float Cos)
    {
        *this = vec2f(x*Cos - y*Sin, x*Sin + y*Cos);
        return *this;
    }


    vec2f& vec2f::rotate(float angle, vec2f& p)
    {
        float Cos = cos(angle), Sin = sin(angle), nSin = -Sin;
        *this = vec2f(
            Cos*x + nSin*y + (p.x - Cos*p.x - nSin*p.y),
            Sin*x + Cos*y + (p.y - Sin*p.x - Cos*p.y)
        );
        return *this;
    }


    vec2f& vec2f::rotate(float Sin, float Cos, vec2f& p)
    {
        float nSin = -Sin;
        *this = vec2f(
            Cos*x + nSin*y + (p.x - Cos*p.x - nSin*p.y),
            Sin*x + Cos*y + (p.y - Sin*p.x - Cos*p.y)
        );
        return *this;
    }


    bool vec2f::isOnLine(vec2f& p0, vec2f& p1)
    {
        return between(x, p0.x, p1.x) && between(y, p0.y, p1.y);
    }


    vec2f vec2f::perp(SIDE side)
    {
        switch (side) {
            case SIDE::RIGHT: return vec2f(y, -x);
            case SIDE::LEFT: return vec2f(-y, x);
            default: return vec2f(y, -x);
        }
    }


    vec2f vec2f::perpCW()
    {
        return vec2f(y, -x);
    }


    vec2f vec2f::perpRight()
    {
        return this->perpCW();
    }


    vec2f vec2f::perpCCW()
    {
        return vec2f(-y, x);
    }


    vec2f vec2f::perpLeft()
    {
        return this->perpCCW();
    }


    vec2f vec2f::closestPoint_Line(vec2f& l0p0, vec2f& l0p1)
    {
        vec2f l1p0 = *this;
        vec2f l1p1 = l1p0 + (l0p1 - l0p0).perpCW();
        return intersectPoint(l0p0, l0p1, l1p0, l1p1);
    }


    vec2f vec2f::closestPoint_LineSegment(vec2f& l0p0, vec2f& l0p1)
    {
        vec2f pI = this->closestPoint_Line(l0p0, l0p1);
        if (pI.isOnLine(l0p0, l0p1))
            return pI;
        else {
            vec2f v1 = *this - l0p0, v2 = *this - l0p1;
            float dist0 = v1.dotProduct(v1); // v dot v gives the square of the distance (enough to compare)
            float dist1 = v2.dotProduct(v2); // v dot v gives the square of the distance (enough to compare)
            return (dist0 <= dist1) ? l0p0 : l0p1;
        }
    }


    vec2f vec2f::closestPoint_Circle(const vec2f& pCenter, float radius)
    {
        float angle = (*this - pCenter).angle();
        return angleLengthVector(angle, radius) + pCenter;
    }


    bool vec2f::insideCirlce(const vec2f& pCenter, float radius)
    {
        return this->distance(pCenter) < radius;
    }


    SIDE vec2f::determineSide(vec2f v)
    {
        return (this->dotProduct(v.perp(SIDE::RIGHT)) >= 0) ? SIDE::RIGHT : SIDE::LEFT;
    }


    SIDE vec2f::determineSide(vec2f p0, vec2f p1)
    {
        vec2f v0 = (p1 - p0).perp(SIDE::RIGHT);
        vec2f v1 = *this - p0;
        return (v0.dotProduct(v1) >= 0) ? SIDE::RIGHT : SIDE::LEFT;
    }


    float vec2f::distFrom_Line(vec2f& l0p0, vec2f& l0p1)
    {
        return this->distance(this->closestPoint_Line(l0p0, l0p1));
    }


    float vec2f::distFrom_LineSegment(vec2f& l0p0, vec2f& l0p1)
    {
        return this->distance(this->closestPoint_LineSegment(l0p0, l0p1));
    }


    float vec2f::distFrom_Circle(vec2f& pCenter, float radius)
    {
        return fabs(this->distance(pCenter) - radius);
    }


    std::string vec2f::string()
    {
        return std::string("[ X:" + std::to_string(x) + " Y:" + std::to_string(y) + " ]");
    }


    std::wstring vec2f::wstring()
    {
        return std::wstring(L"[ X:" + std::to_wstring(x) + L" Y:" + std::to_wstring(y) + L" ]");
    }

    // -------------------------------------------------------------------------------------------------------------------
    //


    vec2f angleLengthVector(float angle, float length)
    {
        return vec2f(cos(angle), sin(angle)) * length;
    }


    vec2f angleLengthVector(float Sin, float Cos, float length)
    {
        return vec2f(Cos, Sin) * length;
    }


    vec2f midPoint(vec2f& p0, vec2f& p1)
    {
        return p0 + ((p1 - p0) * 0.5);
    }


    vec2f pointAtScalar(vec2f& p0, vec2f& p1, float scalar)
    {
        return p0 + ((p1 - p0) * scalar);
    }


    vec2f pointAtLength(vec2f& v, float len)
    {
        return v.unitLengthVector() * len;
    }


    vec2f pointAtLength(vec2f& p0, vec2f& p1, float len)
    {
        return p0 + ((p1 - p0).unitLengthVector() * len);
    }


    vec2f intersectPoint(vec2f& l0p0, vec2f& l0p1, vec2f& l1p0, vec2f& l1p1)
    {
        // Line 1 (p1, p2) = a1x + b1y = c1
        float a1 = l0p1.y - l0p0.y;
        float b1 = l0p0.x - l0p1.x;
        float c1 = a1*l0p0.x + b1*l0p0.y;

        // Line 2 (p3, p4) = a2x + b2y = c2
        float a2 = l1p1.y - l1p0.y;
        float b2 = l1p0.x - l1p1.x;
        float c2 = a2*l1p0.x + b2*l1p0.y;

        float d = a1*b2 - a2*b1;

        if (!fIsZero(d))
            return vec2f(b2*c1 - b1*c2, a1*c2 - a2*c1) * (1/d);
        else
            return vec2f(0, 0);
    }


    vec2f threePointCenter(vec2f& p1, vec2f& p2, vec2f& p3)
    {
        vec2f mp1 = midPoint(p1, p2);
        vec2f mp2 = midPoint(p2, p3);
        vec2f v1 = mp1 + (p2 - p1).perpCW();
        vec2f v2 = mp2 + (p3 - p2).perpCCW();
        return intersectPoint(mp1, v1, mp2, v2);
    }
    

    bool fSame(float a, float b)
    {
        return fabs(a - b) <= FLT_EPSILON;
    }


    bool fIsZero(float a)
    {
        return fabs(a) <= FLT_EPSILON;
    }


    double degrees(double angle_Radians)
    {
        return angle_Radians * DegPerRad;
    }

    float degrees(float angle_Radians)
    {
        return angle_Radians * static_cast<float>(DegPerRad);
    }


    double radians(double angle_Degrees)
    {
        return angle_Degrees * RadPerDeg;
    }

    float radians(float angle_Degrees)
    {
        return angle_Degrees * static_cast<float>(RadPerDeg);
    }


    double roundTo(double val, double mul, int dir)
    {
        if (dir == 0) 
            return round(val/mul) * mul;
        else if (dir > 0) 
            return ceil(val/mul) * mul;
        else 
            return floor(val/mul) * mul;
    }


} // !namespace vmath






