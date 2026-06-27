#include <QtTest>

#include "rbl_uvector.h"

class TestUVector : public QObject
{
    Q_OBJECT

private slots:

    void construction();
    void fill();
    void assignment();
};

void TestUVector::construction()
{
    RUVector empty;
    QCOMPARE(empty.getNRows(), 0u);

    RUVector v(3, 7);
    QCOMPARE(v.getNRows(), 3u);
    QCOMPARE(v[0], 7u);
    QCOMPARE(v[2], 7u);
}

void TestUVector::fill()
{
    RUVector v(4, 0);
    v.fill(9);
    for (uint i = 0; i < v.getNRows(); i++)
    {
        QCOMPARE(v[i], 9u);
    }
}

void TestUVector::assignment()
{
    RUVector a(2, 5);
    RUVector b = a;
    QCOMPARE(b.getNRows(), 2u);
    QCOMPARE(b[1], 5u);

    b[1] = 11;
    QCOMPARE(b[1], 11u);
    QCOMPARE(a[1], 5u); // original unchanged
}

QTEST_APPLESS_MAIN(TestUVector)

#include "tst_uvector.moc"
