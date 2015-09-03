#include <iostream>
#include <fstream>
#include <string>
#include "BSBFeeCat.h"
#include "BSBFeeScale.h"
#include "BSBFeeModel.h"
#include "tinyxml.h"
#include <vector>
#include <iterator>
#include <list>
#include <algorithm>
#include <sstream>
#ifdef __GNUC__
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif


const unsigned int NUM_INDENTS_PER_SPACE = 2;
std::tr1::unordered_map<std::string, std::string> my_cachce; //Cache Upper/Lower limit of invterval , [instrument]=lowerLimit

std::string GetEnv(const std::string & var) {
	const char * val = ::getenv(var.c_str());
	if (val == 0) {
		std::cout << "GetEnv=0" << std::endl ;
		return "";
	}
	else {
		std::cout << "GetEnv : " << val << std::endl ;
		return val;
	}
}

void clearDuplicate(int x) {

}

const char * getIndent(unsigned int numIndents)
{
	static const char * pINDENT = "                                      + ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents*NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

// same as getIndent but no "+" at the end
const char * getIndentAlt(unsigned int numIndents)
{
	static const char * pINDENT = "                                        ";
	static const unsigned int LENGTH = strlen(pINDENT);
	unsigned int n = numIndents*NUM_INDENTS_PER_SPACE;
	if (n > LENGTH) n = LENGTH;

	return &pINDENT[LENGTH - n];
}

int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if (!pElement) return 0;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	int i = 0;
	int ival;
	double dval;
	const char* pIndent = getIndent(indent);
	printf("\n");
	while (pAttrib)
	{
		printf("%s%s: value=[%s]", pIndent, pAttrib->Name(), pAttrib->Value());

		if (pAttrib->QueryIntValue(&ival) == TIXML_SUCCESS)    printf(" int=%d", ival);
		if (pAttrib->QueryDoubleValue(&dval) == TIXML_SUCCESS) printf(" d=%1.1f", dval);
		printf("\n");
		i++;
		pAttrib = pAttrib->Next();
	}
	return i;
}

void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0)
{
	if (!pParent) return;

	TiXmlNode* pChild;
	TiXmlText* pText;
	int t = pParent->Type();
	printf("%s", getIndent(indent));
	int num;

	switch (t)
	{
	case TiXmlNode::TINYXML_DOCUMENT:
		printf("Document");
		break;

	case TiXmlNode::TINYXML_ELEMENT:
		printf("Element [%s]", pParent->Value());
		num = dump_attribs_to_stdout(pParent->ToElement(), indent + 1);
		switch (num)
		{
		case 0:  printf(" (No attributes)"); break;
		case 1:  printf("%s1 attribute", getIndentAlt(indent)); break;
		default: printf("%s%d attributes", getIndentAlt(indent), num); break;
		}
		break;

	case TiXmlNode::TINYXML_COMMENT:
		printf("Comment: [%s]", pParent->Value());
		break;

	case TiXmlNode::TINYXML_UNKNOWN:
		printf("Unknown");
		break;

	case TiXmlNode::TINYXML_TEXT:
		pText = pParent->ToText();
		printf("Text: [%s]", pText->Value());
		break;

	case TiXmlNode::TINYXML_DECLARATION:
		printf("Declaration");
		break;
	default:
		break;
	}
	printf("\n");
	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
	{
		dump_to_stdout(pChild, indent + 1);
	}
}

// load the named file and dump its structure to STDOUT
void dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		dump_to_stdout(&doc); // defined later in the tutorial
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}
void gen_tierfee_xml(std::vector<FeeScaleXML*> & feeScale)
{
	// Make xml: <?xml ..><Hello>World</Hello>
	std::tr1::unordered_map<std::string, std::string> gen_tier_cache;
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * element = new TiXmlElement("import");

	//TiXmlText * text = new TiXmlText("World");
	element->SetAttribute("mode", "synchronize");
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);

	{
		for (std::vector < FeeScaleXML* >::iterator iter = feeScale.begin();
			iter != feeScale.end(); iter++) {

			FeeScaleXML *feescale = *iter;
			std::tr1::unordered_map<std::string, std::string>::const_iterator got = gen_tier_cache.find(feescale->name);
			if (got == gen_tier_cache.end()) {

				TiXmlElement * feescales = new TiXmlElement("feescale");
				element->LinkEndChild(feescales);
				TiXmlElement * name = new TiXmlElement("name");
				TiXmlElement * currency = new TiXmlElement("currency");
				TiXmlElement * baseValue = new TiXmlElement("baseValue");
				TiXmlElement * calculationBase = new TiXmlElement("calculationBase");

				TiXmlElement * calculationMethod = new TiXmlElement("calculationMethod");
				TiXmlElement * orderEstimationMode = new TiXmlElement("orderEstimationMode");
				TiXmlElement * overlappingIntervals = new TiXmlElement("overlappingIntervals");
				TiXmlElement * FeeAggregationScheme = new TiXmlElement("FeeAggregationScheme");



				feescales->LinkEndChild(name);
				feescales->LinkEndChild(currency);
				feescales->LinkEndChild(baseValue);
				feescales->LinkEndChild(calculationBase);
				feescales->LinkEndChild(calculationMethod);
				feescales->LinkEndChild(orderEstimationMode);
				feescales->LinkEndChild(overlappingIntervals);
				feescales->LinkEndChild(FeeAggregationScheme);


				//feescales->LinkEndChild(name);
				name->LinkEndChild(new TiXmlText((feescale->name).c_str()));
				currency->LinkEndChild(new TiXmlText((feescale->currency).c_str()));
				baseValue->LinkEndChild(new TiXmlText((feescale->baseValue).c_str()));
				calculationBase->LinkEndChild(new TiXmlText((feescale->calculationBase).c_str()));
				calculationMethod->LinkEndChild(new TiXmlText((feescale->calculationMethod).c_str()));
				orderEstimationMode->LinkEndChild(new TiXmlText((feescale->orderEstimationMode).c_str()));
				overlappingIntervals->LinkEndChild(new TiXmlText((feescale->overlappingIntervals).c_str()));
				FeeAggregationScheme->LinkEndChild(new TiXmlText((feescale->FeeAggregationScheme).c_str()));
				for (std::vector <interval> ::iterator interval_it = feescale->intervals.begin();
					interval_it != feescale->intervals.end();
					interval_it++) {

					TiXmlElement * intervals = new TiXmlElement("intervals");
					feescales->LinkEndChild(intervals);
					interval feeInterval = *interval_it;

					//-----------------------------------------------------
					TiXmlElement * value = new TiXmlElement("value");
					value->LinkEndChild(new TiXmlText(feeInterval.value.c_str()));
					//-----------------------------------------------------
					TiXmlElement * lowerLimit = new TiXmlElement("lowerLimit");

					if (feeInterval.lowerLimit == "true") {
						lowerLimit->SetAttribute("nil", feeInterval.lowerLimit.c_str());
					}
					else {
						lowerLimit->LinkEndChild(new TiXmlText(feeInterval.lowerLimit.c_str()));
					}
					//-----------------------------------------------------
					TiXmlElement * upperLimit = new TiXmlElement("upperLimit");

					if (feeInterval.upperLimit == "true") {
						upperLimit->SetAttribute("nil", feeInterval.upperLimit.c_str());
					}
					else {
						upperLimit->LinkEndChild(new TiXmlText(feeInterval.upperLimit.c_str()));
					}
					//-----------------------------------------------------
					intervals->LinkEndChild(lowerLimit);
					intervals->LinkEndChild(upperLimit);
					intervals->LinkEndChild(value);

					std::pair<std::string, std::string> cache(feescale->name, "exist");
					gen_tier_cache.insert(cache);
				}
				
			}
			else {

			}



		}

		doc.SaveFile("BSBFeeScale.xml");
	}
}
void gen_catfee_xml(std::vector<FeeCatXML*>& feeCat)
{
	// Make xml: <?xml ..><Hello>World</Hello>
	std::tr1::unordered_map<std::string, std::string> gen_cat_cache;
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement * element = new TiXmlElement("import");

	//TiXmlText * text = new TiXmlText("World");
	element->SetAttribute("mode", "synchronize");
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);

	{
		
		for (std::vector < FeeCatXML* >::iterator iter = feeCat.begin();
			iter != feeCat.end(); iter++) {
			
			FeeCatXML *feeCat = *iter;
			//Check Duplicate
			std::tr1::unordered_map<std::string, std::string>::const_iterator got = gen_cat_cache.find(feeCat->id);
			if (got == gen_cat_cache.end()) {
				TiXmlElement * feescats = new TiXmlElement("clientfeecat");
				element->LinkEndChild(feescats);

				TiXmlElement * id = new TiXmlElement("id");
				TiXmlElement * name = new TiXmlElement("name");

				feescats->LinkEndChild(id);
				feescats->LinkEndChild(name);

				id->LinkEndChild(new TiXmlText((feeCat->id).c_str()));
				name->LinkEndChild(new TiXmlText((feeCat->name).c_str()));
				std::pair<std::string, std::string> cache(feeCat->id, "exist");
				gen_cat_cache.insert(cache);
			}

			
		}


		doc.SaveFile("BSBFeeCat.xml");
	}
}


void printOutput(std::string tradeType, std::string tradingChannel, FeeModel * model, std::string const &line, int &sequence_number, std::ofstream & fee_model_file) {

	std::string rule_name(line, 0, 10);
	std::stringstream ss;
	model->Transactiontype = "EEE";
	model->fee_model = "COMM & DC BROKER";
	model->rule_name = "TFEX" + line.substr(0, 10) + line.substr(10, 10) + tradingChannel;
	ss << sequence_number;
	model->sequence_number = ss.str();//std::to_string(sequence_number);
	model->exchange_id = "TFEX";
	model->trade_type = tradeType;
	model->trading_channel = tradingChannel;
	model->client_fee_category = line.substr(0, 10);
	model->fee_class_abbreviation = line.substr(50, 10);
	model->fee_scale = line.substr(50, 10);

	fee_model_file << model->Transactiontype << ",";
	fee_model_file << model->fee_model << ",";
	fee_model_file << model->rule_name << ",";
	fee_model_file << model->sequence_number << ",";
	fee_model_file << ","; // Active
	fee_model_file << model->buysell << ",";
	fee_model_file << ","; // openclose
	fee_model_file << ","; // instru_typenr
	fee_model_file << ","; // contract_type
	fee_model_file << ","; // interest_type
	fee_model_file << ","; // issue_country
	fee_model_file << ","; // issue_curr
	fee_model_file << ","; // ccp_compliant
	fee_model_file << ","; // security_depositary
	fee_model_file << ","; // index_isin
	fee_model_file << ","; // issuer_shortname
	fee_model_file << ","; // security_institution
	fee_model_file << ","; // security_id
	fee_model_file << ","; // instrument_isin
	fee_model_file << model->instrument_cluster_name << ","; // instrument_cluster_name
	fee_model_file << ","; // clearinghouse_shortname
	fee_model_file << ","; // exchange_segment_type
	fee_model_file << model->exchange_id << ","; // exchange_id
	fee_model_file << ","; // exchange_segment
	fee_model_file << ","; // exchange_private_segment
	fee_model_file << ","; // exchange_area
	fee_model_file << ","; // exchange_fee_category_instrumentd
	fee_model_file << ","; // exchange_fee_category_trade
	fee_model_file << ","; // counterparty_bic
	fee_model_file << ","; // broker_bic
	fee_model_file << ","; // trade_curr
	fee_model_file << ","; // settle_curr
	fee_model_file << model->trade_type << ","; // trade_type
	fee_model_file << ","; // trading_channel_type
	fee_model_file << model->trading_channel << ","; // trading_channel_name
	fee_model_file << ","; // settlement_interface
	fee_model_file << ","; // settlement_exchange_id
	fee_model_file << ","; // settlement_fee_category
	fee_model_file << model->client_fee_category << ","; // client_fee_category
	fee_model_file << ","; // multi_account_alocatation
	fee_model_file << model->fee_class_abbreviation << ","; // fee_class_abbreviation
	fee_model_file << ","; // fee_class_name
	fee_model_file << model->fee_model << std::endl; // fee_scale,

	sequence_number += 5;

}

FeeModel * procFeeModel(std::string const &line, std::ofstream & fee_model_file)
{
	std::string trade_category(line, 48, 1);

	if (trade_category != "F") {
		return NULL;
	}

	FeeModel * model = new FeeModel();
	int sequence_number = 5;
	
	std::string channel_code(line, 20, 1);
	std::string buy_sell(line, 49, 1);

	if (buy_sell == "A") model->buysell = "X";
	else if (buy_sell == "B") model->buysell = "B";
	else if (buy_sell == "S") model->buysell = "S";

	//=============== Read Cluster File ========================
	
	std::string cluster_line;
	std::ifstream infile;
	std::string OUTPUT_CLUSTER_NAME_TXT = getenv("OUTPUT_CLUSTER_NAME_TXT");

	infile.open(OUTPUT_CLUSTER_NAME_TXT.c_str());
	//FeeScaleXML * feeScale = new FeeScaleXML();
	while (std::getline(infile, cluster_line))
	{
		if (line.empty())  continue;
		
		model->instrument_cluster_name = cluster_line;
		std::cout << "model->instrument_cluster_name " << model->instrument_cluster_name << std::endl;
		if (channel_code == "A") {

			printOutput("TaTrans", "DECIDE", model, line, sequence_number, fee_model_file);

			printOutput("", "DECIDE", model, line, sequence_number, fee_model_file);

			printOutput("TaTrans", "DROPCOPY", model, line, sequence_number, fee_model_file);
			printOutput("", "DROPCOPY", model, line, sequence_number, fee_model_file);

			printOutput("TaTrans", "INTERNET", model, line, sequence_number, fee_model_file);
			printOutput("", "INTERNET", model, line, sequence_number, fee_model_file);



		}
		else if (channel_code == "I"){
			printOutput("TaTrans", "INTERNET", model, line, sequence_number, fee_model_file);
			printOutput("", "INTERNET", model, line, sequence_number, fee_model_file);

		}
		else if (channel_code == "C") {

			printOutput("TaTrans", "DECIDE", model, line, sequence_number, fee_model_file);
			printOutput("", "DECIDE", model, line, sequence_number, fee_model_file);

			printOutput("TaTrans", "DROPCOPY", model, line, sequence_number, fee_model_file);
			printOutput("", "DROPCOPY", model, line, sequence_number, fee_model_file);

		}


	}

	return NULL;
}



FeeCatXML * procFeeCat(std::string &line)
{
	FeeCatXML * xml = new FeeCatXML();
	std::string id(line, 0, 10);
	std::string name(line, 0, 10);
	name.erase(name.find_last_not_of(" \n\r\t") + 1);
	id.erase(id.find_last_not_of(" \n\r\t") + 1);

	xml->id = id;
	xml->name = name;
	//xml->print();

	return xml;
}

FeeScaleXML * procFeeScale(std::string &line)
{
	FeeScaleXML * xml = new FeeScaleXML();
	interval feeInterval;
	feeInterval.lowerLimit = "true";
	feeInterval.upperLimit = "true";
	std::string name(line, 50, 9);
	std::string firstPos(line, 32, 7);
	std::string secondPos(line, 32, 7);
	//std::cout << line.substr(10, 7) << std::endl;
	xml->name = name;
	xml->currency = "THB";
	xml->baseValue = "F";
	xml->calculationBase = "E";
	xml->calculationMethod = "L";
	xml->overlappingIntervals = "N";
	xml->FeeAggregationScheme = "Client Aggregation";
	xml->intervals.push_back(feeInterval);

	if (line.substr(10, 7) == "STOCKFT") { //Single Stock
		//std::cout << "Single Stock" << std::endl ;
		xml->orderEstimationMode = "S";
		return xml;
	}
	else if (line.substr(21, 1) == "2") {
		//std::cout << "NON SINGLE STOCK : NO TIER" << std::endl ;

		xml->orderEstimationMode = "F";
		return xml;

	}
	// In others cases,return NULL
	return NULL;

}

void procFeeScaleFromTier(std::string &line, std::vector<FeeScaleXML*> &FeeSclaeList)
{
	FeeScaleXML * xml = new FeeScaleXML();


	std::string name(line, 0, 10);
	name.erase(name.find_last_not_of(" \n\r\t") + 1);
	std::string firstPos(line, 32, 7);
	std::string secondPos(line, 32, 7);
	std::string lowerLimit(line, 10, 5);
	std::string upperLimit(line, 15, 5);

	std::string FirstValue(line, 20, 8);
	std::string SecondValue(line, 28, 8);

	std::string val = "0";

	if (FirstValue != "0000.00") {
		val = FirstValue;
	}
	else if (SecondValue != "0000.00"){
		val = SecondValue;
	}



	if (FeeSclaeList.size() != 0){

		interval feeInterval;
		feeInterval.value = val;
		feeInterval.upperLimit = upperLimit;

		std::vector<FeeScaleXML*>::iterator it = FeeSclaeList.end();
		--it;
		if (name.compare((*it)->name) != 0) { //New Fee tier ,set upper limit of last fee as "true"
			std::vector<interval>::iterator feeIt = (*it)->intervals.end();
			--feeIt;

			feeIt->upperLimit = "true";
			feeInterval.lowerLimit = "true";

			xml->name = name;
			xml->description = "description";
			xml->currency = "THB";
			xml->baseValue = "F";
			xml->calculationBase = "A";
			xml->calculationMethod = "L";
			xml->orderEstimationMode = "F";
			xml->overlappingIntervals = "N";
			xml->intervals.push_back(feeInterval);
			xml->FeeAggregationScheme = "Client Aggregation";


			FeeSclaeList.push_back(xml);
		}
		else { // Fee existed ,adds more tier
			std::vector<interval>::iterator feeIt = (*it)->intervals.end();
			--feeIt;
			feeInterval.lowerLimit = lowerLimit;
			(*it)->intervals.push_back(feeInterval);


		}

	}
	/*

	*/

}
void ReadBSBTIER(std::vector<FeeScaleXML*> &feeScaleList){
	std::string line;
	std::ifstream infile;
	std::string BSB_TIER = GetEnv("BSB_TIER_FILE");

	infile.open(BSB_TIER.c_str());
	//FeeScaleXML * feeScale = new FeeScaleXML();
	while (std::getline(infile, line))
	{
		if (line.empty())  continue;
		//if EOF
		//feeScaleList.push_back(procFeeScaleFromTier(line)); //generate FeeScale XML
		//line = line.erase(line.find_last_not_of(" \n\r\t") + 1);// Trim Space
		procFeeScaleFromTier(line, feeScaleList);
	}

}


void ReadBSBCOMGROUP(std::vector<FeeCatXML*> &feeCatList, std::vector<FeeScaleXML*> &feeScaleList, std::vector<FeeModel *> &feeModelList){
	std::string line;
	std::ifstream infile;
	std::string BSB_COMGROUP = GetEnv("BSB_COMGROUP_FILE");
	std::string BSB_FEEMODEL_RULE_FILE = GetEnv("BSB_FEEMODEL_RULE_FILE");
	std::cout << "About to Read " << BSB_COMGROUP << std::endl;
	infile.open(BSB_COMGROUP.c_str());
	FeeScaleXML * FeeScale = new FeeScaleXML();
	FeeModel * model = new FeeModel();

	//------------- gen FeeModel--------
	std::ofstream fee_model_file;
	std::string header;

	header = "transactiontype,fee_model,rule_name,sequence_number,active,buysell,openclose,instru_typenr,contract_type,interest_type,issue_country,issue_curr,ccp_compliant,security_depositary,index_isin,issuer_shortname,security_institution,security_id,instrument_isin,instrument_cluster_name,clearinghouse_shortname,exchange_segment_type,exchange_id,exchange_segment,exchange_private_segment,exchange_area,exchange_fee_category_instrument,exchange_fee_category_trade,counterparty_bic,broker_bic,trade_curr,settle_curr,trade_type,trading_channel_type,trading_channel_name,settlement_interface,settlement_exchange_id,settlement_fee_category,client_fee_category,multi_account_alocatation,fee_class_abbreviation,fee_class_name,fee_scale,";

	fee_model_file.open(BSB_FEEMODEL_RULE_FILE.c_str());

	fee_model_file << header << "\n";
	//------------- gen FeeModel--------

	while (std::getline(infile, line))
	{

		if (line.empty())  continue;
		//Generate feecat XML
		//line = line.erase(line.find_last_not_of(" \n\r\t") + 1); // Trim Space 
		feeCatList.push_back(procFeeCat(line));


		//Generate FeeScale XML (Type C.)
		FeeScale = procFeeScale(line);
		if (FeeScale != NULL) feeScaleList.push_back(FeeScale);

		//Generate FeeModel List
		procFeeModel(line,fee_model_file);

	}

	fee_model_file.close();

}

// ----------------------------------------------------------------------
// main() for printing files named on the command line
// ----------------------------------------------------------------------
int main(int argc, char* argv[])

{
	std::vector<FeeCatXML*> feeCatList;
	std::vector<FeeScaleXML*> feeScaleList;
	std::vector<FeeModel *> feeModelList;
	std::vector<bool*> gen_cat_cache;

	ReadBSBCOMGROUP(feeCatList, feeScaleList,feeModelList);
	gen_catfee_xml(feeCatList);

	std::cout << feeScaleList.size() << std::endl;

	ReadBSBTIER(feeScaleList);
	gen_tierfee_xml(feeScaleList);

	std::cout << feeScaleList.size() << std::endl;

	//std::vector<FeeScaleXML*> feeScaleList = ReadBSBTIER();

	//dump_to_stdout("BSBFeeScale.xml");
	return 0;
}


