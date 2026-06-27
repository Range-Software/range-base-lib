#include <QtTest>

#include "rbl_version.h"

class TestVersion : public QObject
{
    Q_OBJECT

private slots:

    void fromString();
    void toStringRoundTrip();
    void customDelimiter();
    void validity();
    void comparison();
};

void TestVersion::fromString()
{
    RVersion v = RVersion::fromString("1.2.3");
    QCOMPARE(v.getMajor(), 1u);
    QCOMPARE(v.getMinor(), 2u);
    QCOMPARE(v.getRelease(), 3u);
}

void TestVersion::toStringRoundTrip()
{
    RVersion v(4, 5, 6);
    QCOMPARE(v.toString(), QString("4.5.6"));

    RVersion parsed = RVersion::fromString(v.toString());
    QVERIFY(parsed == v);
}

void TestVersion::customDelimiter()
{
    RVersion v("7-8-9", '-');
    QCOMPARE(v.getMajor(), 7u);
    QCOMPARE(v.getMinor(), 8u);
    QCOMPARE(v.getRelease(), 9u);
    QCOMPARE(v.toString('-'), QString("7-8-9"));
}

void TestVersion::validity()
{
    RVersion invalid(0, 0, 0);
    QVERIFY(!invalid.isValid());

    RVersion valid(0, 0, 1);
    QVERIFY(valid.isValid());
}

void TestVersion::comparison()
{
    RVersion a(1, 2, 3);
    RVersion b(1, 2, 4);
    RVersion c(1, 2, 3);
    RVersion d(2, 0, 0);

    QVERIFY(a < b);
    QVERIFY(b > a);
    QVERIFY(a == c);
    QVERIFY(a != b);
    QVERIFY(a <= c);
    QVERIFY(a >= c);
    QVERIFY(d > a);
    QVERIFY(a < d);
}

QTEST_APPLESS_MAIN(TestVersion)

#include "tst_version.moc"
