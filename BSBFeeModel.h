#ifndef BSBFEEMODEL_H
#define BSBFEEMODEL_H

#include <iostream>
#include <fstream>
#include <string>



class FeeModel
{
public:

	std::string Transactiontype;
	std::string	fee_model;
	std::string	rule_name;
	std::string	sequence_number;
	std::string buysell;
	std::string instrument_cluster_name;
	std::string	exchange_id;
	std::string	trade_type;
	std::string	trading_channel;
	std::string	client_fee_category;
	std::string	fee_class_abbreviation;
	std::string	fee_scale;

};
#endif
