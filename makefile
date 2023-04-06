CC=g++
CFLAGS=-I. -I./include/CppUnitLite
CXXFLAGS += -std=c++20 -Wall -Werror
# -std=c++20: valid for g++ 10 and greater.
# -Wall: reports all warnings
# -Werror: warnings reported as errors

OBJ = CppUnitLite/Failure.o CppUnitLite/Test.o CppUnitLite/TestRegistry.o CppUnitLite/TestResult.o CppUnitLite/WFailure.o \
      Circle.o CircleTest.o Point.o PointTest.o Rectangle.o RectangleTest.o ShapeTest.o Triangle.o TriangleTest.o \
      Main.o 
      
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(CXXFLAGS)

Assignment8: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(CXXFLAGS)


all: Assignment8

clean:
	rm *.o
	rm ./CppUnitLite/*.o
