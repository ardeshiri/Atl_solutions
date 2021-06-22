#include "Matrix.h"
#include "Vector.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(MatrixTest,CorrectConstruction)
try
{
    atlatec_test::Matrix<3, 5, int> m{ {1,3,4,2,-5}, {2,-3,4,5,6}, {4,5,6,-7,0} };
    atlatec_test::Matrix<2, 4, int> n{ {1,4,2,5}, {2,4,5,6} };
    atlatec_test::Matrix<5, 1, int> o{ {0}, {-233341411}, {0}, {0}, {0} };
    atlatec_test::Matrix<100,22,float> p{};
    atlatec_test::Matrix<0,0,double> q{};
    atlatec_test::Matrix<0,1,uint64_t> r{};

    std::valarray<int> v{1,2,3,4,5,6};
    atlatec_test::Matrix<3,2, int> s{v};
}
catch(const std::runtime_error& e)
{
    FAIL()<<"correct construction causing exception: "<<e.what()<<std::endl;
}

TEST(MatrixTest,IncorrectConstruction)
{
    try
    {
        atlatec_test::Matrix<3, 5, int> m{ {1,3,4,5}, {2,3,4,5,6}, {4,5,6,7,0} };
        FAIL()<<"Incorrect construction is being successfully done!"<<std::endl;
    }
    catch(const std::runtime_error& e) {}
    try
    {
        atlatec_test::Matrix<2, 4, int> n{ {1,4,2,5}, {2,4,5,6}, {14} };
        FAIL()<<"Incorrect construction is being successfully done!"<<std::endl;
    }
    catch(const std::runtime_error& e) {}
    try
    {
        atlatec_test::Matrix<0, 1, int> o{ {0}, {1}, {2}, {0}, {0}, {0} };
        FAIL()<<"Incorrect construction is being successfully done!"<<std::endl;
    }
    catch(const std::runtime_error& e) {}
    try
    {
        atlatec_test::Matrix<100,22,float> p{{}};
        FAIL()<<"Incorrect construction is being successfully done!"<<std::endl;
    }
    catch(const std::runtime_error& e) {}
    try
    {
        std::valarray<int> v{1,2,3,4,5,6,7};
        atlatec_test::Matrix<3,2, int> s{v};
        FAIL()<<"Incorrect construction is being successfully done!"<<std::endl;
    }
    catch(const std::runtime_error& e) {}
}


TEST(MatrixTest,CopyOperation)
{
    atlatec_test::Matrix<3, 5, int> m{ {1,3,4,2,-5}, {2,-3,4,5,6}, {4,5,6,-7,0} };
    auto mc = m;
    EXPECT_EQ(mc, m)<<"error in copy assign.";

    atlatec_test::Matrix<2, 4, int> n{ {1,4,2,5}, {2,4,5,6} };
    atlatec_test::Matrix<2, 4, int> nc{n};
    EXPECT_EQ(nc, n)<<"error in copy construct.";
}

TEST(MatrixTest,MoveOperation)
{
    atlatec_test::Matrix<3, 5, int> m{ {1,3,4,2,-5}, {2,-3,4,5,6}, {4,5,6,-7,0} };
    auto mc = m;
    auto mm = std::move(mc);
    EXPECT_EQ(mm, m)<<"error in move assign.";

    atlatec_test::Matrix<2, 4, int> n{ {1,4,2,5}, {2,4,5,6} };
    auto nc = n;
    atlatec_test::Matrix<2, 4, int> nm{ std::move(nc) };
    EXPECT_EQ(nm, n)<<"error in move construct.";
}

TEST(MatrixTest,SubscriptOperation)
{
    atlatec_test::Matrix<3, 5, int> m{ {1,3,4,2,-5}, {2,-3,4,5,6}, {4,5,6,-7,0} };
    EXPECT_EQ(-5, m[4])<<"error in subscription.";
    EXPECT_EQ(1, m[0])<<"error in subscription.";
    EXPECT_EQ(0, m[14])<<"error in subscription.";

    const atlatec_test::Matrix<3, 5, int> mc{ {1,3,4,2,-5}, {2,-3,4,5,6}, {4,5,6,-7,0} };
    EXPECT_EQ(-5, mc[4])<<"error in subscription.";
    EXPECT_EQ(1, mc[0])<<"error in subscription.";
    EXPECT_EQ(0, mc[14])<<"error in subscription.";

    atlatec_test::Matrix<2, 4, int> n{ {1,4,2,5}, {2,4,5,6} };
    EXPECT_EQ(1, n.at(0,0))<<"error in subscription.";
    EXPECT_EQ(5, n.at(1,2))<<"error in subscription.";
    EXPECT_EQ(6, n.at(1,3))<<"error in subscription.";
    try{ n.at(1,4); FAIL()<<"wrong index accepted."; } catch(...){}
    try{ n.at(4,1); FAIL()<<"wrong index accepted."; } catch(...){}

    const atlatec_test::Matrix<2, 4, int> nc{ {1,4,2,5}, {2,4,5,6} };
    EXPECT_EQ(1, nc.at(0,0))<<"error in subscription.";
    EXPECT_EQ(5, nc.at(1,2))<<"error in subscription.";
    EXPECT_EQ(6, nc.at(1,3))<<"error in subscription.";
    try{ nc.at(1,6); FAIL()<<"wrong index accepted."; } catch(...){}
    try{ nc.at(7,0); FAIL()<<"wrong index accepted."; } catch(...){}
}

TEST(MatrixTest,MatrixMultiplication)
{
    atlatec_test::Matrix<3, 5, int> m0{ {1,3,4,2,5}, {2,3,4,5,6}, {4,5,6,7,0} };
    atlatec_test::Matrix<5, 2, int> n0{ {1,5}, {3,6}, {4,0}, {2,5}, {3,2} };
    auto res01 = m0*n0;
    atlatec_test::Matrix<3,2, int> res02{{45,43}, {55,65}, {57,85}};
    EXPECT_EQ(res01, res02)<<"wrong multiplication outcome.";

    atlatec_test::Matrix<4, 2, int> m1{ {1,2}, {2,-1}, {3,4}, {-4, 5} };
    atlatec_test::Matrix<2, 4, int> n1{ {8,-9,2,0}, {0,1,-2,9} };
    auto res11 = m1*n1;
    atlatec_test::Matrix<4,4, int> res12{{8,-7,-2,18}, {16,-19,6,-9}, {24,-23,-2,36}, {-32,41,-18,45}};
    EXPECT_EQ(res11, res12)<<"wrong multiplication outcome.";

    atlatec_test::Matrix<4, 2, int> m2{ {1,2}, {2,-1}, {3,4}, {-4, 5} };
    auto res21 = m2*31;
    atlatec_test::Matrix<4,2, int> res22{{31,62}, {62,-31}, {93,124}, {-124,155}};
    EXPECT_EQ(res21, res22)<<"wrong multiplication outcome.";

    atlatec_test::Matrix<4, 2, int> m3{ {1,2}, {2,-1}, {3,4}, {-4, 5} };
    auto res31 = 12*m3;
    atlatec_test::Matrix<4,2, int> res32{{12,24}, {24,-12}, {36,48}, {-48,60}};
    EXPECT_EQ(res31, res32)<<"wrong multiplication outcome.";
}

TEST(MatrixTest,MatrixAddition)
{
    atlatec_test::Matrix<3, 5, int> m0{ {1,3,4,2,5}, {2,3,4,5,6}, {4,5,6,7,0} };
    atlatec_test::Matrix<3, 5, int> n0{ {0,3,2,1,5}, {4,3,5,0,1}, {3,2,55,6,7} };
    auto res01 = m0+n0;
    atlatec_test::Matrix<3, 5, int> res02{{1,6,6,3,10}, {6,6,9,5,7}, {7,7,61,13,7}};
    EXPECT_EQ(res01, res02)<<"wrong addition outcome.";

    atlatec_test::Matrix<4, 2, int> m1{ {1,2}, {2,-1}, {3,4}, {-4, 5} };
    atlatec_test::Matrix<4, 2, int> n1{ {2,2}, {6,-51}, {-3,-4}, {-4, 55} };
    auto res11 = m1+n1;
    atlatec_test::Matrix<4, 2, int> res12{{3,4}, {8,-52}, {0,0}, {-8, 60}};
    EXPECT_EQ(res11, res12)<<"wrong multiplication outcome.";

    atlatec_test::Matrix<4, 2, double> m2{ {0.4,0}, {-47.1,32.4}, {-4,4.11}, {-4, 5.01} };
    atlatec_test::Matrix<4, 2, double> n2{ {1,2}, {2,-1}, {3,4}, {-4, 5} };
    auto res21 = m2+n2;
    atlatec_test::Matrix<4,2, double> res22{{1.4,2}, {-45.1,31.4}, {-1,8.11}, {-8,10.01}};
    EXPECT_EQ(res21, res22)<<"wrong multiplication outcome.";
}

TEST(VectorTest,Construction)
try
{
    atlatec_test::Vector<float> m(31);
    EXPECT_EQ(m.size(), 31)<<"wrong size.";

    atlatec_test::Vector n{1,3,4,2,-5,2,-3,4,5,6,4,5,6,-7,0};
    EXPECT_EQ(n.size(), 15)<<"wrong size.";

    std::valarray<int> v{1,0,3,0,5,0};
    atlatec_test::Vector<int> o{v};
    EXPECT_EQ(o.size(), 6)<<"wrong size.";
    EXPECT_TRUE(!(o.underlying_valarray() != v).sum());
}
catch(const std::runtime_error& e)
{
    FAIL()<<"correct construction causing exception: "<<e.what()<<std::endl;
}

TEST(VectorTest,CopyOperation)
{
    atlatec_test::Vector m{1,3,4,2,-5,2,-3,4,5,6,4,5,6,-7,0};
    auto mc = m;
    EXPECT_EQ(mc, m)<<"error in copy assign.";

    atlatec_test::Vector<int> n{36, 24, 12, 60, 108, 84};
    atlatec_test::Vector<int> nc{n};
    EXPECT_EQ(nc, n)<<"error in copy construct.";
}

TEST(VectorTest,MoveOperation)
{
    atlatec_test::Vector m{1,3,4,2,-5,2,-3,4,5,6,4,5,6,-7,0};
    auto mc = m;
    auto mm = std::move(mc);
    EXPECT_EQ(mm, m)<<"error in move assign.";

    atlatec_test::Vector<int> n{36, 24, 12, 60, 108, 84};
    auto nc = n;
    atlatec_test::Vector<int> nm{std::move(nc)};
    EXPECT_EQ(nm, n)<<"error in move construct.";
}

TEST(VectorTest,SubscriptOperation)
{
    atlatec_test::Vector m{1,3,4,2,-5,2,-3,4,5,6,4,5,6,-7,0};
    EXPECT_EQ(-5, m[4])<<"error in subscription.";
    EXPECT_EQ(1, m[0])<<"error in subscription.";
    EXPECT_EQ(0, m[14])<<"error in subscription.";

    const atlatec_test::Vector mc{1,3,4,2,-5,2,-3,4,5,6,4,5,6,-7,0};
    EXPECT_EQ(-5, mc[4])<<"error in subscription.";
    EXPECT_EQ(1, mc[0])<<"error in subscription.";
    EXPECT_EQ(0, mc[14])<<"error in subscription.";

    atlatec_test::Vector<int> n{36, 24, 12, 60, 108, 84};
    EXPECT_EQ(36, n.at(0))<<"error in subscription.";
    EXPECT_EQ(12, n.at(2))<<"error in subscription.";
    EXPECT_EQ(84, n.at(5))<<"error in subscription.";
    try{ n.at(6); FAIL()<<"wrong index accepted."; } catch(...){}
    try{ n.at(555); FAIL()<<"wrong index accepted."; } catch(...){}

    const atlatec_test::Vector<int> nc{36, 24, 12, 60, 108, 84};
    EXPECT_EQ(36, nc.at(0))<<"error in subscription.";
    EXPECT_EQ(12, nc.at(2))<<"error in subscription.";
    EXPECT_EQ(84, nc.at(5))<<"error in subscription.";
    try{ nc.at(6); FAIL()<<"wrong index accepted."; } catch(...){}
    try{ nc.at(444); FAIL()<<"wrong index accepted."; } catch(...){}
}

TEST(VectorTest,VectorScalarMultiplication)
{
    atlatec_test::Vector<int> v0{3,2,1,5,9,7};
    auto res00 = 12*v0;
    auto res01 = atlatec_test::Vector<int> {36, 24, 12, 60, 108, 84};
    EXPECT_EQ(res00, res01)<<"error in vector-scalar multiplication.";

    atlatec_test::Vector<double> v1{3.2,2.3,1.1,5.0};
    auto res10 = 2.0*v1;
    auto res11 = atlatec_test::Vector<double> {6.4, 4.6, 2.2, 10};
    EXPECT_EQ(res00, res01)<<"error in vector-scalar multiplication.";
}

TEST(VectorTest,VectorPushPop)
{
    atlatec_test::Vector<int> m{3,2,1,5,9,7};
    m.push_back(14);
    auto res0 = atlatec_test::Vector<int> {3,2,1,5,9,7,14};
    EXPECT_EQ(m, res0)<<"error in vector push_back.";

    atlatec_test::Vector<int> n{3,2,1,5,9,7};
    n.push_front(1);
    auto res1 = atlatec_test::Vector<int> {1,3,2,1,5,9,7,14};
    EXPECT_EQ(n, res1)<<"error in vector push_front.";

    atlatec_test::Vector<int> o{3,2,1,5,9,7};
    o.pop_back();
    auto res2 = atlatec_test::Vector<int> {3,2,1,5,9};
    EXPECT_EQ(o, res2)<<"error in vector pop_back.";

    atlatec_test::Vector<int> p{3,2,1,5,9,7};
    p.pop_front();
    auto res3 = atlatec_test::Vector<int> {3,2,1,5,9,7};
    EXPECT_EQ(n, res1)<<"error in vector pop_front.";

    atlatec_test::Vector<int> q{};
    std::vector<int> v0{1,2,3,4,5,6,7};
    std::copy(v0.begin(), v0.end(), std::back_inserter(q));
    auto res4 = atlatec_test::Vector<int> {1,2,3,4,5,6,7};
    EXPECT_EQ(q, res4)<<"error in vector push_back.";

    atlatec_test::Vector<int> r{};
    std::vector<int> v1{1,2,3,4,5,6,7};
    std::copy(v1.begin(), v1.end(), std::front_inserter(r));
    auto res5 = atlatec_test::Vector<int> {7,6,5,4,3,2,1};
    EXPECT_EQ(r, res5)<<"error in vector push_front.";
}

TEST(VectorTest,VectorAddition)
{
    atlatec_test::Vector<int> v00{3,2,1,5,9,7};
    atlatec_test::Vector<int> v01{13,3,5,7,8,1};
    auto res00 = v00+v01;
    auto res01 = atlatec_test::Vector<int> {16, 5, 6, 12, 17, 8};
    EXPECT_EQ(res00, res01)<<"error in vector addition.";

    atlatec_test::Vector<double> v10{3.2,2.3,1.1,5.0};
    atlatec_test::Vector<double> v11{3.5,1.3,0.1,5.8};
    auto res10 = v10+v11;
    auto res11 = atlatec_test::Vector<double> {6.7, 3.6, 1.2, 10.8};
    EXPECT_EQ(res00, res01)<<"error in vector addition.";
}


TEST(CommonTest,CommonMultiplication)
{
    atlatec_test::Vector<double> v0{2.2,1.3,7.1};
    atlatec_test::Matrix<3, 3, double> m0{{1.1,1.2,5.5}, {2.3,1.3,5}, {9.5,1.4,8.1}};
    auto res00 = v0*m0;
    atlatec_test::Vector<double> res01{72.86, 14.27, 76.11};
    EXPECT_EQ(res00,res01)<<"error in vector-matrix multiplication.";

    atlatec_test::Vector<int> v1{9,8,6,7,8,9,34};
    atlatec_test::Matrix<4, 7, int> m1{{78,68,68,56,9,4,9}, {6,7,9,5,3,2,5}, {76,74,22,3,55,1,45},{0,56,78,9,4,2,3}};
    auto res10 = m1*v1;
    atlatec_test::Vector<int> res11{2460, 411, 3408, 1131};
    EXPECT_EQ(res10,res11)<<"error in matrix-vector multiplication.";
}
