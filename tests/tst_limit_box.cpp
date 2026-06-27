#include <QtTest>

#include "rbl_limit_box.h"

class TestLimitBox : public QObject
{
    Q_OBJECT

private slots:

    void setGetLimits();
    void limitsAreSorted();
    void scale();
    void merge();
    void intersection();
};

void TestLimitBox::setGetLimits()
{
    RLimitBox box(0.0, 1.0, 2.0, 3.0, 4.0, 5.0);
    double xl, xu, yl, yu, zl, zu;
    box.getLimits(xl, xu, yl, yu, zl, zu);
    QCOMPARE(xl, 0.0);
    QCOMPARE(xu, 1.0);
    QCOMPARE(yl, 2.0);
    QCOMPARE(yu, 3.0);
    QCOMPARE(zl, 4.0);
    QCOMPARE(zu, 5.0);
}

void TestLimitBox::limitsAreSorted()
{
    // Lower bounds given larger than upper bounds must be swapped.
    RLimitBox box(1.0, 0.0, 3.0, 2.0, 5.0, 4.0);
    double xl, xu, yl, yu, zl, zu;
    box.getLimits(xl, xu, yl, yu, zl, zu);
    QCOMPARE(xl, 0.0);
    QCOMPARE(xu, 1.0);
    QCOMPARE(yl, 2.0);
    QCOMPARE(yu, 3.0);
    QCOMPARE(zl, 4.0);
    QCOMPARE(zu, 5.0);
}

void TestLimitBox::scale()
{
    RLimitBox box(-1.0, 1.0, -2.0, 2.0, -3.0, 3.0);
    box.scale(2.0);
    double xl, xu, yl, yu, zl, zu;
    box.getLimits(xl, xu, yl, yu, zl, zu);
    QCOMPARE(xl, -2.0);
    QCOMPARE(xu, 2.0);
    QCOMPARE(yu, 4.0);
    QCOMPARE(zl, -6.0);
}

void TestLimitBox::merge()
{
    RLimitBox a(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
    RLimitBox b(2.0, 3.0, -1.0, 0.5, 0.0, 5.0);
    a.merge(b);
    double xl, xu, yl, yu, zl, zu;
    a.getLimits(xl, xu, yl, yu, zl, zu);
    QCOMPARE(xl, 0.0);
    QCOMPARE(xu, 3.0);
    QCOMPARE(yl, -1.0);
    QCOMPARE(yu, 1.0);
    QCOMPARE(zl, 0.0);
    QCOMPARE(zu, 5.0);
}

void TestLimitBox::intersection()
{
    RLimitBox a(0.0, 2.0, 0.0, 2.0, 0.0, 2.0);
    RLimitBox overlapping(1.0, 3.0, 1.0, 3.0, 1.0, 3.0);
    RLimitBox disjoint(5.0, 6.0, 5.0, 6.0, 5.0, 6.0);

    QVERIFY(RLimitBox::areIntersecting(a, overlapping));
    QVERIFY(!RLimitBox::areIntersecting(a, disjoint));
}

QTEST_APPLESS_MAIN(TestLimitBox)

#include "tst_limit_box.moc"
