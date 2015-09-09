#ifndef BSBFEESCALE_H
#define BSBFEESCALE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct interval {
	std::string lowerLimit;
	std::string upperLimit;
	std::string value;
};

class FeeScaleXML
{
public:

	std::string name;
	std::string description;
	std::string currency;
	std::string baseValue;
	std::string calculationBase;
	std::string calculationMethod;
	std::string orderEstimationMode;
	std::string overlappingIntervals;
	std::string FeeAggregationScheme;
	std::vector<interval> intervals;
	std::string value;
};

#endif
