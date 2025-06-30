#include <cmath>
#include <algorithm>

#include "rbl_statistics.h"
#include "rbl_logger.h"

void RStatistics::_init(const RStatistics *pStatistics)
{
    if (pStatistics)
    {
        this->nValues = pStatistics->nValues;
        this->min = pStatistics->min;
        this->max = pStatistics->max;
        this->avg = pStatistics->avg;
        this->med = pStatistics->med;
        this->p05 = pStatistics->p05;
        this->p95 = pStatistics->p95;
        this->dsv = pStatistics->dsv;
    }
}

RStatistics::RStatistics(const RRVector &values, uint nDistValues, bool sortValues)
    : nValues(0)
    , min(0.0)
    , max(0.0)
    , avg(0.0)
    , med(0.0)
    , p05(0.0)
    , p95(0.0)
{
    this->_init();
    this->calculate(values,nDistValues,sortValues);
}

RStatistics::RStatistics(const RStatistics &statistics)
{
    this->_init(&statistics);
}

RStatistics::~RStatistics()
{
}

RStatistics &RStatistics::operator =(const RStatistics &statistics)
{
    this->_init(&statistics);
    return (*this);
}

void RStatistics::print() const
{
    RLogger::info("Size:          % 14u\n",this->getNValues());
    RLogger::info("Minimum:       % 14g\n",this->getMin());
    RLogger::info("Maximum:       % 14g\n",this->getMax());
    RLogger::info("Average:       % 14g\n",this->getAvg());
    RLogger::info("Median:        % 14g\n",this->getMed());
    RLogger::info("Percentile 05: % 14g\n",this->getP05());
    RLogger::info("Percentile 95: % 14g\n",this->getP95());
}

QJsonObject RStatistics::toJson() const
{
    QJsonObject obj;

    obj["size"] = qint64(this->getNValues());
    obj["minimum"] = this->getMin();
    obj["maximum"] = this->getMax();
    obj["average"] = this->getAvg();
    obj["median"] = this->getMed();
    obj["p05"] = this->getP05();
    obj["p95"] = this->getP95();

    return obj;
}

void RStatistics::calculate(const RRVector &values, uint nDistValues, bool sortValues)
{
    this->nValues = (uint)values.size();
    this->min = RStatistics::findMinimumValue(values);
    this->max = RStatistics::findMaximumValue(values);
    this->avg = RStatistics::findAverageValue(values);
    this->dsv = RStatistics::findDistributedValues(values,nDistValues);

    if (sortValues)
    {
        RRVector sortedValues(values);
        std::sort(sortedValues.begin(),sortedValues.end());
        this->med = RStatistics::findMedianValue(sortedValues);
        this->p05 = RStatistics::findPercentileValue(sortedValues,5.0);
        this->p95 = RStatistics::findPercentileValue(sortedValues,95.0);
    }
    else
    {
        this->med = RStatistics::findMedianValue(values);
        this->p05 = RStatistics::findPercentileValue(values,5.0);
        this->p95 = RStatistics::findPercentileValue(values,95.0);
    }
}

double RStatistics::findMinimumValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    bool firstValue = true;
    for (uint i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = values[i];
            firstValue = false;
        }
        else
        {
            value = std::min(value,values[i]);
        }
    }
    return value;
}

double RStatistics::findMaximumValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    bool firstValue = true;
    for (uint i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = values[i];
            firstValue = false;
        }
        else
        {
            value = std::max(value,values[i]);
        }
    }
    return value;
}

double RStatistics::findMinimumAbsoluteValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    bool firstValue = true;
    for (uint i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = std::fabs(values[i]);
            firstValue = false;
        }
        else
        {
            value = std::min(value,std::fabs(values[i]));
        }
    }
    return value;
}

double RStatistics::findMaximumAbsoluteValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    bool firstValue = true;
    for (uint i=0;i<values.size();i++)
    {
        if (firstValue)
        {
            value = std::fabs(values[i]);
            firstValue = false;
        }
        else
        {
            value = std::max(value,std::fabs(values[i]));
        }
    }
    return value;
}

double RStatistics::findAverageValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    for (uint i=0;i<values.size();i++)
    {
        value += values[i];
    }
    if (values.size() > 0)
    {
        value /= values.size();
    }
    return value;
}

double RStatistics::findMedianValue(const RRVector &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double value = 0.0;
    uint nv = (uint)values.size();
    uint pos = 0;

    if (nv % 2 != 0) {
        pos = nv / 2;
        value = values[pos];
    }
    else {
        value = 0.0;
        pos = (int) floor((double)nv / 2.0);
        value += values[pos];
        pos = (int) ceil((double)nv / 2.0);
        value += values[pos];
        value /= 2.0;
    }

    return value;
}

double RStatistics::findPercentileValue(const RRVector &values, double percentile)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    RRVector vs(values);
    double n, p, vp, vd;
    uint k;
    uint N;

    if (percentile < 0.0)
    {
        p = 0.0;
    }
    else if (percentile > 100.0)
    {
        p = 100.0;
    }
    else {
        p = percentile;
    }

    N = (uint)vs.size();

    n = (p/100.0)*(N-1.0)+1.0;

    if (n <= 1.0)
    {
        return vs[0];
    }
    if (n >= N)
    {
        return vs[N-1];
    }

    k = (uint)floor(n);
    vd = vs[k] - vs[k-1];
    vd -= floor(vd);
    vp = vs[k-1] + vd;

    return vp;
}

RRMatrix RStatistics::findDistributedValues(const RRVector &values, uint nDistValues)
{
    RRMatrix dist(2,nDistValues);
    if (values.size() == 0)
    {
        dist.fill(0.0);
        return dist;
    }

    double llim,  ulim;
    double sllim, sulim;
    double step;
    int nvals;

    llim = RStatistics::findMinimumValue(values);
    ulim = RStatistics::findMaximumValue(values);
    step = ulim - llim;
    if (nDistValues > 1)
    {
        step /= (double)(nDistValues-1);
    }

    dist.fill(0.0);

    nvals = (uint)values.size();

    for (uint i=0;i<nDistValues;i++) {
        sllim = step*i + llim;
        sulim = sllim + step;

        dist[0][i] = sllim;

        if (i == nDistValues-1) {
            sulim = ulim + 1.0;
        }
        for (int j=0;j<nvals;j++) {
            if (sllim <= values[j] && values[j] < sulim) {
                dist[1][i]++;
            }
        }
    }

    return dist;
}

double RStatistics::findMinimumMagnitude(const std::vector<RR3Vector> &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double minMag = 0.0;
    for (uint i=0;i<values.size();i++)
    {
        if (i==0)
        {
            minMag = RR3Vector::euclideanNorm(values[i]);
        }
        else
        {
            minMag = std::min(minMag,RR3Vector::euclideanNorm(values[i]));
        }
    }
    return minMag;
}

double RStatistics::findMaximumMagnitude(const std::vector<RR3Vector> &values)
{
    if (values.size() == 0)
    {
        return 0.0;
    }
    double maxMag = 0.0;
    for (uint i=0;i<values.size();i++)
    {
        if (i==0)
        {
            maxMag = RR3Vector::euclideanNorm(values[i]);
        }
        else
        {
            maxMag = std::max(maxMag,RR3Vector::euclideanNorm(values[i]));
        }
    }
    return maxMag;
}
