#include <QtTest>

#include "rbl_rmatrix.h"
#include "rbl_rvector.h"

namespace
{
constexpr double tol = 1.0e-9;
}

class TestRMatrix : public QObject
{
    Q_OBJECT

private slots:

    void construction();
    void identity();
    void transpose();
    void determinant();
    void invert();
    void matrixVectorMultiply();
    void trace();
    void equality();
};

void TestRMatrix::construction()
{
    RRMatrix m(2, 3, 1.5);
    QCOMPARE(m.getNRows(), 2u);
    QCOMPARE(m.getNColumns(), 3u);
    QCOMPARE(m.getValue(1, 2), 1.5);
    QVERIFY(!m.isSquare());

    m.setValue(0, 1, 7.0);
    QCOMPARE(m.getValue(0, 1), 7.0);
}

void TestRMatrix::identity()
{
    RRMatrix m;
    m.setIdentity(3);
    QCOMPARE(m.getNRows(), 3u);
    QVERIFY(m.isSquare());
    for (uint i = 0; i < 3; i++)
    {
        for (uint j = 0; j < 3; j++)
        {
            QCOMPARE(m.getValue(i, j), i == j ? 1.0 : 0.0);
        }
    }
}

void TestRMatrix::transpose()
{
    RRMatrix m(2, 3, 0.0);
    m.setValue(0, 0, 1.0);
    m.setValue(0, 1, 2.0);
    m.setValue(0, 2, 3.0);
    m.setValue(1, 0, 4.0);
    m.setValue(1, 1, 5.0);
    m.setValue(1, 2, 6.0);

    m.transpose();
    QCOMPARE(m.getNRows(), 3u);
    QCOMPARE(m.getNColumns(), 2u);
    QCOMPARE(m.getValue(0, 0), 1.0);
    QCOMPARE(m.getValue(2, 0), 3.0);
    QCOMPARE(m.getValue(2, 1), 6.0);
}

void TestRMatrix::determinant()
{
    RRMatrix m(2, 2, 0.0);
    m.setValue(0, 0, 4.0);
    m.setValue(0, 1, 3.0);
    m.setValue(1, 0, 6.0);
    m.setValue(1, 1, 3.0);
    // 4*3 - 3*6 = -6
    QVERIFY(qAbs(m.getDeterminant() - (-6.0)) < tol);
}

void TestRMatrix::invert()
{
    RRMatrix m(2, 2, 0.0);
    m.setValue(0, 0, 4.0);
    m.setValue(0, 1, 7.0);
    m.setValue(1, 0, 2.0);
    m.setValue(1, 1, 6.0);

    RRMatrix inv(m);
    inv.invert();

    RRMatrix product;
    RRMatrix::mlt(m, inv, product);

    QCOMPARE(product.getNRows(), 2u);
    QCOMPARE(product.getNColumns(), 2u);
    QVERIFY(qAbs(product.getValue(0, 0) - 1.0) < tol);
    QVERIFY(qAbs(product.getValue(0, 1)) < tol);
    QVERIFY(qAbs(product.getValue(1, 0)) < tol);
    QVERIFY(qAbs(product.getValue(1, 1) - 1.0) < tol);
}

void TestRMatrix::matrixVectorMultiply()
{
    RRMatrix m(2, 2, 0.0);
    m.setValue(0, 0, 1.0);
    m.setValue(0, 1, 2.0);
    m.setValue(1, 0, 3.0);
    m.setValue(1, 1, 4.0);

    RRVector x(2, 0.0);
    x[0] = 5.0;
    x[1] = 6.0;

    RRVector y;
    RRMatrix::mlt(m, x, y);
    QCOMPARE(y.getNRows(), 2u);
    QVERIFY(qAbs(y[0] - 17.0) < tol); // 1*5 + 2*6
    QVERIFY(qAbs(y[1] - 39.0) < tol); // 3*5 + 4*6
}

void TestRMatrix::trace()
{
    RRMatrix m(3, 3, 0.0);
    m.setValue(0, 0, 1.0);
    m.setValue(1, 1, 2.0);
    m.setValue(2, 2, 3.0);
    QVERIFY(qAbs(RRMatrix::trace(m) - 6.0) < tol);
}

void TestRMatrix::equality()
{
    RRMatrix a(2, 2, 1.0);
    RRMatrix b(2, 2, 1.0);
    RRMatrix c(2, 2, 1.0);
    c.setValue(1, 1, 2.0);

    QVERIFY(a == b);
    QVERIFY(a != c);
}

QTEST_APPLESS_MAIN(TestRMatrix)

#include "tst_rmatrix.moc"
