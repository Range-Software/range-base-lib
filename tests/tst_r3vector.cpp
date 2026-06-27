#include <QtTest>

#include <cmath>

#include "rbl_r3vector.h"

namespace
{
constexpr double tol = 1.0e-12;
}

class TestR3Vector : public QObject
{
    Q_OBJECT

private slots:

    void crossProduct();
    void distance();
    void angleBetween();
    void opposite();
    void orthogonal();
};

void TestR3Vector::crossProduct()
{
    RR3Vector x(1.0, 0.0, 0.0);
    RR3Vector y(0.0, 1.0, 0.0);
    RR3Vector result;

    RR3Vector::cross(x, y, result);
    QCOMPARE(result.getNRows(), 3u);
    QVERIFY(qAbs(result[0]) < tol);
    QVERIFY(qAbs(result[1]) < tol);
    QVERIFY(qAbs(result[2] - 1.0) < tol);
}

void TestR3Vector::distance()
{
    RR3Vector a(0.0, 0.0, 0.0);
    RR3Vector b(1.0, 2.0, 2.0);
    QVERIFY(qAbs(RR3Vector::findDistance(a, b) - 3.0) < tol);
}

void TestR3Vector::angleBetween()
{
    RR3Vector x(1.0, 0.0, 0.0);
    RR3Vector y(0.0, 1.0, 0.0);
    QVERIFY(qAbs(RR3Vector::angle(x, y) - M_PI / 2.0) < 1.0e-9);

    RR3Vector x2(2.0, 0.0, 0.0);
    QVERIFY(qAbs(RR3Vector::angle(x, x2)) < 1.0e-9);
}

void TestR3Vector::opposite()
{
    RR3Vector v(1.0, -2.0, 3.0);
    RR3Vector o = v.getOpposite();
    QCOMPARE(o[0], -1.0);
    QCOMPARE(o[1], 2.0);
    QCOMPARE(o[2], -3.0);
}

void TestR3Vector::orthogonal()
{
    RR3Vector v(1.0, 2.0, 3.0);
    RR3Vector o = v.findOrthogonal();

    // The orthogonal vector must be non-zero and perpendicular to v.
    QVERIFY(o.length() > tol);
    QVERIFY(qAbs(RRVector::dot(v, o)) < 1.0e-9);
}

QTEST_APPLESS_MAIN(TestR3Vector)

#include "tst_r3vector.moc"
