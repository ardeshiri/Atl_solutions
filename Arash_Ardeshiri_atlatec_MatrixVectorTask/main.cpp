#include <gtest/gtest.h>
#include "tests.h"

///There are tons of things that can be done to further enhance these classes but they take lots of time, for example making them more general, being able to have any number of
///dimensions in matrix, adding more functionalities, defining iterators, adding specialization for specific types and specializing operations for different types, using parallelism
///for some operations in some problem sizes, either through parallel algorithms or even trough using lock-less parallelism on some operations, defining more detailed and accurate
///concepts, defining deduction guides, etc.
///Vectors and matrices can get very complicated, for example a vector from a mathematical point of view can be very different from std::vector with different properties, it can be
///seen as an specialization of a matrix but since you mentioned "two classes" I went a different way. Vectors also have different definitions of product(dot and cross) that are performed
///depending on properties like being a horizontal or vertical vector. since they are related to linear algebra I factored them and just tried to define something with a minimum set of operations
///between a c++ and a mathematical vector.
///Each test, tests one of the requested functionalities or one that they depend on.


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(& argc, argv);
    return RUN_ALL_TESTS();
}
