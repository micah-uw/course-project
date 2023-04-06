#pragma once

#include "Shape.h"
#include "Point.h"

class Triangle : public Shape
{
public:
	Triangle(
		const Point& v1,
		const Point& v2,
		const Point& v3,
		std::ostream& canvas);

	const Point& getV1() const;
	const Point& getV2() const;
	const Point& getV3() const;

	//TODO: implement

private:
	//TODO: implement
};

