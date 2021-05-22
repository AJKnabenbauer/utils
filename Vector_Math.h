#pragma once

#ifndef VECTOR_MATH
#define VECTOR_MATH

#include <d2d1_1.h>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#define Radians180 M_PI // Flips an angle defined in radians (ie a 45 degree angle becomes 225 degrees) 
#define Radians90 M_PI_2
#define Radians45 M_PI_4

#define between(a,b,c) min(b,c) <= (a) && (a) <= max(b,c)   // test if value (a) is between values (b) and (c)

namespace vmath 
{
    const double PI = M_PI;
    const double DegPerRad = 180.0 * M_1_PI;
    const double RadPerDeg = 1.0 / DegPerRad;

    const int UP = 1;
    const int NEAREST = 0;
    const int DOWN = -1;

    enum class SIDE;
    class vec2f;

    enum class SIDE {
        UNKOWN,
        CW,
        CCW,
        RIGHT = CW,
        LEFT = CCW
    }; 

    class vec2f
    {
    public:
        float x = 0;
        float y = 0;

        vec2f();
        vec2f(float x0, float y0);
        vec2f(const vec2f& v);
        vec2f(const vec2f* v);
        vec2f(const D2D1_POINT_2F& v);
        vec2f(const D2D1_POINT_2F* v);
        vec2f(const POINT& p);

        vec2f operator - () const;
        vec2f operator + (vec2f v) const;
        vec2f operator - (vec2f v) const;
        vec2f operator * (vec2f v) const;
        vec2f& operator += (vec2f v);
        vec2f& operator -= (vec2f v);
        vec2f& operator *= (vec2f v);
        vec2f operator * (float s) const;
        vec2f operator / (float s) const;
        vec2f& operator *= (float s);
        vec2f& operator /= (float s);
        vec2f operator * (D2D1_MATRIX_3X2_F& m) const;
        vec2f& operator *= (D2D1_MATRIX_3X2_F& m);
        explicit operator bool() const;
        operator D2D1_POINT_2F() const;

        float dotProduct(const vec2f& v);

        float length();
        float lengthApprox();

        float distance(const vec2f& v);
        float distanceApprox(const vec2f& v);

        vec2f unitLengthVector();

        vec2f& normalize();

        vec2f& setLength(float len);

        float cosine();
        float cosine(vec2f& v);

        float angle();
        float angle(vec2f& v);

        float angleDot();
        float angleDot(vec2f& v);

        vec2f& setAngle(float angle);
        vec2f& setAngle(float angle, float length);
        vec2f& setAngle(float Sin, float Cos, float length);

        vec2f& rotate(float angle);
        vec2f& rotate(float Sin, float Cos);
        vec2f& rotate(float angle, vec2f& p);
        vec2f& rotate(float Sin, float Cos, vec2f& p);

        bool isOnLine(vec2f& p0, vec2f& p1);

        vec2f perp(SIDE side);
        vec2f perpCW();		// vector perpendicular to this one -90 degrees  
        vec2f perpRight();	// vector perpendicular to this one -90 degrees
        vec2f perpCCW();	// vector perpendicular to this one +90 degrees  
        vec2f perpLeft();	// vector perpendicular to this one +90 degrees

        vec2f closestPoint_Line(vec2f& l0p0, vec2f& l0p1);
        vec2f closestPoint_LineSegment(vec2f& l0p0, vec2f& l0p1);
        vec2f closestPoint_Circle(const vec2f& pCenter, float radius);

        bool insideCirlce(const vec2f& pCenter, float radius);

        SIDE determineSide(vec2f v);
        SIDE determineSide(vec2f p0, vec2f p1);

        float distFrom_Line(vec2f& l0p0, vec2f& l0p1);
        float distFrom_LineSegment(vec2f& l0p0, vec2f& l0p1);
        float distFrom_Circle(vec2f& pCenter, float radius);

        std::string string();
        std::wstring wstring();
    };

    vec2f angleLengthVector(float angle, float length);
    vec2f angleLengthVector(float Sin, float Cos, float length);
    vec2f midPoint(vec2f& p0, vec2f& p1);
    vec2f pointAtScalar(vec2f& p0, vec2f& p1, float scalar);
    vec2f pointAtLength(vec2f& v, float len);
    vec2f pointAtLength(vec2f& p0, vec2f& p1, float len);
    vec2f intersectPoint(vec2f& l0p0, vec2f& l0p1, vec2f& l1p0, vec2f& l1p1);
    vec2f threePointCenter(vec2f& p1, vec2f& p2, vec2f& p3);

    bool fSame(float a, float b);

    bool fIsZero(float a);

    double degrees(double angle_Radians);
    float degrees(float angle_Radians);

    double radians(double angle_Degrees);
    float radians(float angle_Degrees);

    double roundTo(double val, double mul, int dir = vmath::NEAREST);


} // !namespace vmath







/*

namespace vvmath {


    


    template<typename T> class Vec2;

    template<typename T> T distance(T x, T y)					{ return sqrt(sqr(x) + sqr(y)); }
    template<typename T> T distance(Vec2<T>& v)					{ return sqrt(sqr(v.x) + sqr(v.y)); }
    template<typename T> T distance(Vec2<T>& v0, T x1, T y1)	{ return sqrt(sqr(x1 - v0.x) + sqr(y1 - v0.y)); }
    template<typename T> T distance(T x0, T y0, Vec2<T>& v1)	{ return sqrt(sqr(v1.x - x0) + sqr(v1.y - y0)); }

    template<typename T> T dotProduct(T x, T y)								{ return (x*x) + (y*y); } // v dot v = v length^2
    template<typename T> T dotProduct(T x, T y, T z)						{ return (x*x) + (y*y) + (z*z); } // v dot v = v length^2
    template<typename T> T dotProduct(T x0, T y0, T x1, T y1)				{ return (x0*x1) + (y0*y1); }
    template<typename T> T dotProduct(T x0, T y0, T z0, T x1, T y1, T z1)	{ return (x0*x1) + (y0*y1) + (z0*z1); }

    // Fast approximate of vector length with error between -5% and +3% (+0.043% average). See https://gamedev.stackexchange.com/a/69255 
    template<typename T>
    T distanceApprox(T x, T y) {
        T dx = abs(x);
        T dy = abs(y);
        return 0.394 * (dx + dy) + 0.554 * max(dx, dy);
    }


    // Faster approximate of length using all integer operations. See https://www.flipcode.com/archives/Fast_Approximate_Distance_Functions.shtml
    template<typename T>
    T distanceApprox_Int(T x, T y) {
        T max = abs(x); // dx
        T min = abs(y); // dy      
        if (max < min)
            swap(max, min);

        T approx = (max * 1007) + (min * 441);
        if (max < (min << 4))
            approx -= (max * 40);

        // add 512 for proper rounding
        return ((approx + 512) >> 10);
    }


    template<typename T>
    Vec2<T> unitLengthVector(T x, T y) {
        T n = dotProduct(x, y);
        if (n > 0) return Vec2<T>(x, y, 1/sqrt(n));
        else return Vec2<T>(0, 0);
    }


    // --------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // Angle functions
    //  
    // --------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T> T angle(T x, T y) { return atan2(y, x); }
    template<typename T> T angleD(T x, T y) { return acos(cosBetween(x, y, 1, 0)); } // angle between a vector and the +x axis using the dot product instead of atan2

    template<typename T> T angleBetween(T x0, T y0, T x1, T y1) { return atan2((y1 - y0), (x1 - x0)); }
    template<typename T> T angleBetweenD(T x0, T y0, T x1, T y1) { return acos(cosBetween(x0, y0, x1, y1)); } // angle between two vectors using the dot product instead of atan2


    // cosine between two vectors
    template<typename T>
    T cosBetweenUL(T x0, T y0, T x1, T y1) { return dotProduct(x0, y0, x1, y1); } // Vectors are unit length
    // Vectors are arbitrary length
    template<typename T>
    T cosBetween(T x0, T y0, T x1, T y1) {
        T a = dotProduct(x0, y0);
        T b = dotProduct(x1, y1);
        return (!a || !b) ? 0 : dotProduct(x0, y0, x1, y0) / (sqrt(a) * sqrt(b));
    }
   

    // vector of a given length and angle
    template<typename T> Vec2<T> angleLengthVector(T angle, T len) { return Vec2<T>(cos(angle) * len, sin(angle) * len); } // angle is specified in radians

    template<typename T> Vec2<T> perpendicularRight(T x, T y) { return Vec2(y, -x); } // vector perpendicular to the right (90 degrees cw) of input vector
    template<typename T> Vec2<T> perpendicularRight(Vec2<T>& v) { return Vec2(v.y, -v.x); } // vector perpendicular to the right (90 degrees cw) of input vector
   
    template<typename T> Vec2<T> perpendicularLeft(T x, T y) { return Vec2(-y, x); } // vector perpendicular to the left (90 degrees ccw) of input vector
    template<typename T> Vec2<T> perpendicularLeft(Vec2<T>& v) { return Vec2(-v.y, v.x); } // vector perpendicular to the right (90 degrees cw) of input vector

    // --------------------------------------------------------------------------------------------------------------------------------------------------
    //
    // Line related functions
    //  
    // --------------------------------------------------------------------------------------------------------------------------------------------------

    namespace Line 
    {
        template<typename T> bool pointOnLine(T x0, T y0, T x1, T y1, T x2, T y2) { return btwn(x0, x1, x2) && btwn(y0, y1, y2); } // test if (x0,y0) lies on line (x1,y1),(x2,y2)
        template<typename T> bool pointOnLine(Vec2<T>& v0, Vec2<T>& v1, Vec2<T>& v2) { return pointOnLine<T>(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y); } // test if v0 lies on line v1,v2


        template<typename T>
        Vec2<T> intersectPoint(T x0, T y0, T x1, T y1, T x2, T y2, T x3, T y3) {
            // Line 1 (p1, p2) = a1x + b1y = c1
            T a1 = y1 - y0;
            T b1 = x0 - x1;
            T c1 = a1*x0 + b1*y0;

            // Line 2 (p3, p4) = a2x + b2y = c2
            T a2 = y3 - y2;
            T b2 = x2 - x3;
            T c2 = a2*x2 + b2*y2;

            T d = a1*b2 - a2*b1;
            
            if (d != 0) 
                return Vec2<T>((b2*c1 - b1*c2), (a1*c2 - a2*c1), (1/d));
            else
                return Vec2<T>(0, 0);
        }

        template<typename T>
        Vec2<T> intersectPoint(Vec2<T>& v0, Vec2<T>& v1, Vec2<T>& v2, Vec2<T>& v3) { return intersectPoint<T>(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y); }



        template<typename T>
        T distFromLine(T x0, T y0, T x1, T y1, T x2, T y2) {
            Vec2<T> p4(x0, y0);
            p4 += perpendicularRight(x2 - x1, y2 - y1); //vector(p0, p1).perpendicular();
            Vec2<T> pInt = intersectPoint(x1, y1, x2, y2, x0, y0, p4.x, p4.y);
            return distance(pInt.x - x0, pInt.y - y0);
        }

        template<typename T>
        T distFromSeg(T x0, T y0, T x1, T y1, T x2, T y2) {
            if (pointOnLine(x0, y0, x1, y1, x2, y2)) return distFromLine(x0, y0, x1, y1, x2, y2);

            T dist0 = distance(x1 - x0, y1 - y0), dist1 = distance(x2 - x0, y2 - y0);
            return min(dist0, dist1);
        }
    } // end namespace Line



    namespace Circle 
    {
        template<typename T>
        Vec2<T> threePointCenter(Vec2<T>& p1, Vec2<T>& p2, Vec2<T>& p3) {
            Vec2<T> mp1(p1, p2, 0.5), mp2(p2, p3, 0.5);
            Vec2<T> v1 = mp1 + (p2 - p1).right();
            Vec2<T> v2 = mp2 + (p3 - p2).left();
            return Line::intersectPoint(mp1, v1, mp2, v2);
        }

        template<typename T>
        Vec2<T> threePointCenter(T x0, T y0, T x1, T y1, T x2, T y2) {
            Vec2<T> mp1(x0, y0, x1, y1, 0.5), mp2(x1, y1, x2, y2, 0.5);
            return Line::intersectPoint(mp1, mp1 + perpendicularRight(x0, y0, x1, y1), mp2, mp2 + perpendicularLeft(x1, y1, x2, y2));
        }

        template<typename T> T distanceFromEdge(Vec2<T>& pTest, Vec2<T>& pCenter, T radius) { return abs(distance(pTest, pCenter) - radius); }
        template<typename T> T distanceFromEdge(T x0, T y0, T x1, T y1, T radius) { return abs(distance(x1 - x0, y1 - y0) - radius); }

    } // end namespace Circle

    template<typename T>
    class Vec2
    {
    public:
        T x, y;
        Vec2() :x(0), y(0){}
        Vec2(T x0, T y0) :x(x0), y(y0) {}
        Vec2(T x0, T y0, T s) :x(x0*s), y(y0*s) {} // vector v times scalar s              
        Vec2(T x0, T y, T x1, T y1) :x(x1-x0), y(y1-y0) {} // vector from (x0, y0) to (x1, y1)       
        Vec2(T x0, T y, T x1, T y1, T s) :x((x1-x0)*s), y((y1-y0)*s) {} // vector from (x0, y0) to (x1, y1)       
        Vec2(Vec2<T>& v) :x(v.x), y(v.y) {}
        Vec2(Vec2<T>& v, T s) :x(v.x* s), y(v.y* s) {} // vector v times scalar s    
        Vec2(const Vec2<T>& v0, const Vec2<T>& v1) :x(v1.x-v0.x), y(v1.y-v0.y) {} // vector from v0 to v1      
        Vec2(const Vec2<T>& v0, const Vec2<T>& v1, T s) :x((v1.x - v0.x)*s), y((v1.y - v0.y)*s) {} // vector from v0 to v1 times scalar s

        void operator = (const Vec2& v) { x = v.x; y = v.y; }
        Vec2<T> operator + (const Vec2<T>& v) const { return Vec2<T>(x + v.x, y + v.y); }
        Vec2<T> operator - (const Vec2<T>& v) const { return Vec2<T>(x - v.x, y - v.y); }
        Vec2<T> operator * (const T s) const { return Vec2<T>(x*s, y*s); }
        Vec2<T> operator * (const D2D1_MATRIX_3X2_F& m) const { return Vec2<T>(x*m._11 + y*m._21 + m._31, x*m._12 + y*m._22 + m._32); }
        Vec2<T> operator - () const { return Vec2<T>(-1*x, -1*y); }
        Vec2<T> operator / (const T& s) { if (s != 0) return Vec2<T>(x, y, 1/s); else return *this; }
        void operator += (const Vec2<T>& v) { *this = Vec2(x + v.x, y + v.y); }
        void operator -= (const Vec2<T>& v) { *this = *this - v; }
        void operator *= (const T& s) { *this = *this * s; }
        void operator *= (const D2D1_MATRIX_3X2_F& m) { *this = *this * m; }
        void operator /= (const T& s) { *this = *this / s; }



        T dot(Vec2<T>& v) { return dotProduct(x, y, v.x, v.y); }

        T length() { return sqrt(x*x + y*y); }

        T angle() { return angle(x, y); }

        Vec2<T> unitVector() { return unitLengthVector(x, y); }


        //Vec2<T> perpendicular(int dir = 1) { return (dir >= 0) ? Vec2<T>(-y, x) : Vec2<T>(y, -x); }	// dir >= 0 returns +90 degrees, dir < 0 returns -90 degrees
        Vec2 right()  { return Vec2(y, -x); }								// vector perpendicular to this one on the right side
        Vec2 left()  { return Vec2(-y, x); }								// vector perpendicular to this one on the left side

        D2D1_POINT_2F Point2F() { return D2D1::Point2F(x, y); }
        D2D1_ELLIPSE Ellipse(float radius = 1) { return D2D1::Ellipse(Point2F(), radius, radius); }

        Vec2<T>& normalize() {
            *this = this->unitVector();
            return *this;
        }

        Vec2<T>& setLength(T len) {
            *this = this->normalize() * len;
            return *this;
        }

        Vec2<T>& setAngle(T dir) {
            *this = angleLengthVector(dir, this->length());
            return *this;
        }

        Vec2<T>& setAngle(T dir, T len) {
            *this = angleLengthVector(dir, len);
            return *this;
        }


        // Rotate the vector around 0,0
        Vec2<T>& rotate(T angle) {
            T Sin = sin(angle), Cos = cos(angle);
            *this = Vec2<T>(x*Cos - y*Sin, x*Sin + y*Cos);
            return *this;
        }

        // use pre calculated sin and cos values so that we can rotate many objects without having to recalculate is to many times
        Vec2<T>& rotate(T Cos, T Sin) {
            *this = Vec2<T>(x*Cos - y*Sin, x*Sin + y*Cos);
            return *this;
        }

        // Rotate the vector a specified amount about a specified point p
        Vec2<T>& rotate(T angle, Vec2<T>& p) {
            T Cos = cos(angle), Sin = sin(angle); // nSin = -Sin;
            *this = Vec2<T>(Cos*x + -Sin*y + (p.x - Cos*p.x - -Sin*p.y), Sin*x + Cos*y + (p.y - Sin*p.x - Cos*p.y));
            return *this;
        }

        // use pre calculated sin and cos values so that we can rotate many objects without having to recalculate is to many times
        Vec2<T>& rotate(T Cos, T Sin, Vec2<T>& p) {
            // nSin = -Sin;
            *this = Vec2<T>(Cos*x + -Sin*y + (p.x - Cos*p.x - -Sin*p.y), Sin*x + Cos*y + (p.y - Sin*p.x - Cos*p.y));
            return *this;
        }

    };

} // end namespace vmath


*/



/*
template<typename Type>
class Mat3x2
{
public:

    union
    {
        struct 
        {
            Type m11;	// Horizontal scaling / cosine of rotation
            Type m12;	// Vertical shear / sine of rotation
            Type m21;	// Horizontal shear / negative sine of rotation
            Type m22;	// Vertical scaling / cosine of rotation
            Type dx;	// Horizontal shift (always orthogonal regardless of rotation)
            Type dy;	// Vertical shift (always orthogonal regardless of rotation)
        };

        struct 
        {
            Type r1c1, r1c2;
            Type r2c1, r2c2;
            Type r3c1, r3c2;
        };  

        struct
        {
            Type _11, _12;
            Type _21, _22;
            Type _31, _32;
        };

        Type m[3][2] ={0};
    };

    Mat3x2() { setIdentity(); }
    Mat3x2(Type m11, Type m12, Type m21, Type m22, Type m31, Type m32) {
        m[0] ={ m11, m12 };
        m[1] ={ m21, m22 };
        m[2] ={ m31, m32 };
        //_11 = m11;
        //_12 = m12;
        //_21 = m21;
        //_22 = m22;
        //_31 = m31;
        //_32 = m32;
    }


    Mat3x2 operator + (const Mat3x2& matrix) {
        Mat3x2 result;

        result.m[0][0] = r1c1 + matrix.r1c1;
        result.m[0][1] = r1c2 + matrix.r1c2;

        result.m[1][0] = r2c1 + matrix.r2c1;
        result.m[1][1] = r2c2 + matrix.r2c2;

        result.m[2][0] = r3c1 + matrix.r3c1;
        result.m[2][1] = r3c2 + matrix.r3c2;

        return result;
    }


    Mat3x2 operator * (const Type& s) {
        Mat3x2 result;

        result.m[0][0] = r1c1 * s;
        result.m[0][1] = r1c2 * s; 

        result.m[1][0] = r2c1 * s;
        result.m[1][1] = r2c2 * s;  

        result.m[2][0] = r3c1 * s;
        result.m[2][1] = r3c2 * s;

        return result;
    }


    Mat3x2 operator * (const Mat3x2& matrix) {
        Mat3x2 result;

        result.m[0][0] = r1c1 * matrix.r1c1  +  r1c2 * matrix.r2c1;
        result.m[0][1] = r1c1 * matrix.r1c2  +  r1c2 * matrix.r2c2;

        result.m[1][0] = r2c1 * matrix.r1c1  +  r2c2 * matrix.r2c1;
        result.m[1][1] = r2c1 * matrix.r1c2  +  r2c2 * matrix.r2c2;

        result.m[2][0] = r3c1 * matrix.r1c1  +  r3c2 * matrix.r2c1  +  matrix.r3c1;
        result.m[2][1] = r3c1 * matrix.r1c2  +  r3c2 * matrix.r2c2  +  matrix.r3c2;

        return result;
    }


    Type determinant() const {
        return (_11 * _22) - (_12 * _21);
    }


    Mat3x2& setIdentity() {
        m[0] ={ 1, 0 };
        m[1] ={ 0, 1 };
        m[2] ={ 0, 0 };
        return *this;
    }


    Mat3x2& invert() {
        // 3x3 matrix - column major. X vector is 0, 1, 2, etc. (openGL prefer way)
        //    0    3    6
        //    1    4    7
        //    2    5    8



        float t1=m.matrixData[0]*m.matrixData[4];
        float t2=m.matrixData[0]*m.matrixData[7];
        float t3=m.matrixData[3]*m.matrixData[1];
        float t4=m.matrixData[6]*m.matrixData[1];
        float t5=m.matrixData[3]*m.matrixData[2];
        float t6=m.matrixData[6]*m.matrixData[2];

        //calculate the determinant

        float det=(t1*m.matrixData[8]-t2*m.matrixData[5]-t3*m.matrixData[8]+t4*m.matrixData[5]+t5*m.matrixData[7]-t6*m.matrixData[4]);

        //make sure the det is non-zero
        if (det==0.0) return;

        float invd=1.0f/det;

        float m0=(m.matrixData[4]*m.matrixData[8]-m.matrixData[7]*m.matrixData[5])*invd;
        float m3=-(m.matrixData[3]*m.matrixData[8]-m.matrixData[6]*m.matrixData[5])*invd;

        float m6=(m.matrixData[3]*m.matrixData[7]-m.matrixData[6]*m.matrixData[4])*invd;

        float m1=-(m.matrixData[1]*m.matrixData[8]-m.matrixData[7]*m.matrixData[2])*invd;

        float m4=(m.matrixData[0]*m.matrixData[8]-t6)*invd;

        float m7=-(t2-t4)*invd;

        float m2=(m.matrixData[1]*m.matrixData[5]-m.matrixData[4]*m.matrixData[2])*invd;

        float m5=-(m.matrixData[0]*m.matrixData[5]-t5)*invd;

        float m8=(t1-t3)*invd;

        matrixData[0]=m0;
        matrixData[3]=m3;
        matrixData[6]=m6;

        matrixData[1]=m1;
        matrixData[4]=m4;
        matrixData[7]=m7;

        matrixData[2]=m2;
        matrixData[5]=m5;
        matrixData[8]=m8;
    }

};
*/







#endif // !VECTOR_MATH









