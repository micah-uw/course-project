# UW P&CE Certificate Program in C++, Assignment 8
## Extending the Language Through Inheritance
### Programming with Inheritance

Given the following `Shape` base class:
```
#pragma once

#include <ostream>

class Shape
{
public:
    Shape() = default;

    Shape(const Shape& src) = delete;
    Shape& operator=(const Shape& rhs) = delete;

    virtual ~Shape() = default;

    virtual void draw() const = 0;

    virtual void stream(std::ostream& os) const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Shape& shape)
{
    shape.stream(os);
    return os;
}
```

* Define classes `Triangle`, `Circle`, & `Rectangle`. 
* Subclass each from base class `Shape`. 
* Add the appropriate constructor to set the geometry for each subclass. 
* Add appropriate accessor member functions to get the geometry for each class.  
* Overload the virtual member functions inherited from `Shape`.

Use the `override` keyword where appropriate.

Since we don't have access to a C++ 2D graphics library in class, the derived `draw()` functions will not actually draw the geometry on the monitor. Instead, implement `draw` so it outputs the geometry in text format to a stream. So for this assignment, `draw` and `stream` perform similar functions. In a graphical implementation, we would use a graphical library to draw to our destination device.

Note this exercise uses the idea of passing derived class-specific data to the derived class constructor. A consequence of this is we can use the `Shape` abstraction as-is for a variety of different subclasses

For example, a triangle has 3 vertices:
```
class Triangle : public Shape
{
public:
    Triangle(
        const Point& v1,
        const Point& v2,
        const Point& v3,
        std::ostream& canvas); // the stream draw writes to

    const Point& getV1() const;
    const Point& getV2() const;
    const Point& getV3() const;

    // ...
};
```
A rectangle has an upper left and lower right position

A circle has an origin and radius

You should create and use class `Point` to store x,y positions.

Use the `TEST(drawUnique, Shape)` unit test, as provided in ShapeTest.cpp, to validate your `Shape` derived classes' implementation.

## Including the CppUnitLite header files
The header files for CppUnitLite are in the include\CppUnitLite directory.  In your project file or makefile, you will need to tell your compiler to seach this directory for headers

Microsoft Visual Studio: add this include directory to the "AdditionalIncludeDirectories"
makefile: add via the INC=-Iinclude/CppUnitLite flag
