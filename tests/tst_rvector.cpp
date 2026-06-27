#include <QtTest>

#include "rbl_rvector.h"

namespace
{
constexpr double tol = 1.0e-12;
}

class TestRVector : public QObject
{
    Q_OBJECT

private slots:

    void construction();
    void positionConstructor();
    void fillAndScale();
    void lengthAndNormalize();
    void append();
    void equality();
    void dotProduct();
    void addSubtract();
    void norms();
};

void TestRVector::construction()
{
    RRVector empty;
    QCOMPARE(empty.getNRows(), 0u);

    RRVector v(4, 2.5);
    QCOMPARE(v.getNRows(), 4u);
    for (uint i = 0; i < v.getNRows(); i++)
    {
        QCOMPARE(v[i], 2.5);
    }
}

void TestRVector::positionConstructor()
{
    RRVector v(1.0, 2.0, 3.0);
    QCOMPARE(v.getNRows(), 3u);
    QCOMPARE(v[0], 1.0);
    QCOMPARE(v[1], 2.0);
    QCOMPARE(v[2], 3.0);
}

void TestRVector::fillAndScale()
{
    RRVector v(3, 1.0);
    v.fill(4.0);
    QCOMPARE(v[0], 4.0);
    QCOMPARE(v[2], 4.0);

    v.scale(0.5);
    QCOMPARE(v[0], 2.0);

    v *= 2.0;
    QCOMPARE(v[1], 4.0);
}

void TestRVector::lengthAndNormalize()
{
    RRVector v(3.0, 4.0, 0.0);
    QVERIFY(qAbs(v.length() - 5.0) < tol);

    double previousLength = v.normalize();
    QVERIFY(qAbs(previousLength - 5.0) < tol);
    QVERIFY(qAbs(v.length() - 1.0) < tol);

    RRVector zero(0.0, 0.0, 0.0);
    QCOMPARE(zero.normalize(), 0.0);
}

void TestRVector::append()
{
    RRVector a(2, 1.0);
    RRVector b(3, 2.0);
    a.append(b);
    QCOMPARE(a.getNRows(), 5u);
    QCOMPARE(a[1], 1.0);
    QCOMPARE(a[2], 2.0);
    QCOMPARE(a[4], 2.0);
}

void TestRVector::equality()
{
    RRVector a(1.0, 2.0, 3.0);
    RRVector b(1.0, 2.0, 3.0);
    RRVector c(1.0, 2.0, 4.0);

    QVERIFY(a == b);
    QVERIFY(!(a == c));
    QVERIFY(a != c);
    QVERIFY(!(a != b));
}

void TestRVector::dotProduct()
{
    RRVector a(1.0, 2.0, 3.0);
    RRVector b(4.0, 5.0, 6.0);
    QVERIFY(qAbs(RRVector::dot(a, b) - 32.0) < tol);
}

void TestRVector::addSubtract()
{
    RRVector a(1.0, 2.0, 3.0);
    RRVector b(4.0, 6.0, 8.0);
    RRVector sum;
    RRVector diff;

    RRVector::add(a, b, sum);
    QCOMPARE(sum[0], 5.0);
    QCOMPARE(sum[1], 8.0);
    QCOMPARE(sum[2], 11.0);

    RRVector::subtract(b, a, diff);
    QCOMPARE(diff[0], 3.0);
    QCOMPARE(diff[1], 4.0);
    QCOMPARE(diff[2], 5.0);
}

void TestRVector::norms()
{
    RRVector v(3.0, -4.0, 0.0);
    QVERIFY(qAbs(RRVector::euclideanNorm(v) - 5.0) < tol);
    QVERIFY(qAbs(RRVector::taxicabNorm(v) - 7.0) < tol);
}

QTEST_APPLESS_MAIN(TestRVector)

#include "tst_rvector.moc"
