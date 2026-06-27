#include <QtTest>

#include "rbl_statistics.h"
#include "rbl_rvector.h"

namespace
{
constexpr double tol = 1.0e-12;

RRVector makeValues()
{
    RRVector values(5, 0.0);
    values[0] = 1.0;
    values[1] = 2.0;
    values[2] = 3.0;
    values[3] = 4.0;
    values[4] = 5.0;
    return values;
}
}

class TestStatistics : public QObject
{
    Q_OBJECT

private slots:

    void staticHelpers();
    void aggregateValues();
};

void TestStatistics::staticHelpers()
{
    RRVector values = makeValues();
    QVERIFY(qAbs(RStatistics::findMinimumValue(values) - 1.0) < tol);
    QVERIFY(qAbs(RStatistics::findMaximumValue(values) - 5.0) < tol);
    QVERIFY(qAbs(RStatistics::findAverageValue(values) - 3.0) < tol);

    RRVector signedValues(3, 0.0);
    signedValues[0] = -7.0;
    signedValues[1] = 2.0;
    signedValues[2] = -1.0;
    QVERIFY(qAbs(RStatistics::findMinimumAbsoluteValue(signedValues) - 1.0) < tol);
}

void TestStatistics::aggregateValues()
{
    RStatistics stats(makeValues(), 10, true);
    QCOMPARE(stats.getNValues(), 5u);
    QVERIFY(qAbs(stats.getMin() - 1.0) < tol);
    QVERIFY(qAbs(stats.getMax() - 5.0) < tol);
    QVERIFY(qAbs(stats.getAvg() - 3.0) < tol);
    QVERIFY(qAbs(stats.getMed() - 3.0) < tol);
}

QTEST_APPLESS_MAIN(TestStatistics)

#include "tst_statistics.moc"
