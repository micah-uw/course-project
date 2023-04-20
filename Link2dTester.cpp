#include "TestHarness.h"
#include "Link2d.h"

TEST(chainConstruction, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	for (size_t i = 0; i < chain.chain.size(); i++)
	{
		CHECK_EQUAL(100.f, chain.chain[i].len);
	}
}

TEST(chainConstruction_onelink, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	CHECK_EQUAL(100.f, chain.chain[0].len);
	chain.ForwardKinematics(0);
	CHECK_EQUAL(0, chain.xEnd);
	CHECK_EQUAL(100.f, chain.yEnd);
}

TEST(chainConstruction_twolink, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	CHECK_EQUAL(100.f, chain.chain[0].len);
	CHECK_EQUAL(100.f, chain.chain[1].len)
	chain.ForwardKinematics(0);
	CHECK_EQUAL(0, chain.xEnd);
	CHECK_EQUAL(200.f, chain.yEnd);
}

TEST(IKsolver1, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	chain.ForwardKinematics(0);
	CHECK_EQUAL(0, chain.xEnd);
	CHECK_EQUAL(200.f, chain.yEnd);
	auto result = chain.InverseKinematics(100.f, 100.f, 100);
	CHECK(result);
	CHECK_EQUAL(100, (int)chain.xEnd);
	CHECK_EQUAL(99, (int)chain.yEnd);
}

TEST(IKsolver2, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	chain.ForwardKinematics(0);
	CHECK_EQUAL(0, chain.xEnd);
	CHECK_EQUAL(200.f, chain.yEnd);
	auto result = chain.InverseKinematics(-100.f, -100.f, 100);
	CHECK(result);
	CHECK_EQUAL(-99, (int)chain.xEnd);
	CHECK_EQUAL(-99, (int)chain.yEnd);
}

TEST(IKsolver_no_solution, Link2d)
{
	Link2d chain;
	chain.AddLink(100.f);
	chain.AddLink(100.f);
	chain.ForwardKinematics(0);
	CHECK_EQUAL(0, chain.xEnd);
	CHECK_EQUAL(200.f, chain.yEnd);
	auto result = chain.InverseKinematics(-300.f, -300.f, 100);
	CHECK(!result);
}