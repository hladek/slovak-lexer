/*
 * Command.h
 *
 *  Created on: 9.9.2013
 *      Author: voicegroup
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>

using namespace std;
#include "Tokenizer.h"


class LCParser;

/**
 * One argument of the commandline parser
 */
class Arg {

public:
	/**
	 * Enum for type of an argument,
	 *
	 * BOOL - no argument required
	 * STRING - andy argument requuired
	 * INTEGER - any integer value requuired
	 * FLAOT - any real num ber argument required
	 */
	enum argType{BOOL,STRING,INTEGER,FLOAT,LABEL};
	/**
	 * Short name of an argument including slash
	 * e/g -f
	 */
	const LString name;
	/**
	 * Long name of an argument, including double slash
	 * e.g. --file
	 */
	const LString name2;
	/**
	 * Description of an argument
	 * e.g. "Name of input file"
	 */
	const char* helpstring;
	/**
	 * Type of argument
	 */
	const argType type;
	/**
	 * Set to true, if required argument is found
	 */
	bool isValid;
	/**
	 * Set to true, if found during parsing
	 */
	bool isSet;
	/**
	 * String containing found argument, if required
	 *
	 * For BOOL should be empty
	 */
	LString arg;
	/**
	 * Parsed value for INTEGER or FLOAT
	 */
	float value;
	/**
	 * Creates and insters argument to the parser
	 */

	Arg(LCParser& lp, const char* name, argType type, const char* helpstring =
			"", const char* name2 = "");
	/**
	 * Composes and returns help string for an argument
	 */
	string help();
	/**
	 * Composes string describing type of the argument
	 */
	string typestr();
};


/**
 * Commandline parser
 */
class LCParser {
	friend class Arg;
	/**
	 * Flag if next added value should be argument for previous argument
	 */
	int haveArg;
public:
	/**
	 * Vector of all argument in the parser
	 */
	vector<Arg*> args;
	/**
	 * Vector of found arguments that are not in args vector
	 */
	vector<LString> unk;
	/**
	 * Vector of arguments that are in args vector,
	 * were found during parsing, but their required argument is missing
	 */
	vector<Arg*> missingarg;

	/**
	 * Number of found arguments
	 */
	int numSet;
	/**
	 * Number of found and v alidated arguments
	 */
	int numValid;

	LCParser(){
		haveArg = -1;
		numSet = 0;
		numValid = 0;
	}

	/**
	 * Sends helpsring to the given stream
	 */
	void help(ostream& os);
	/**
	 * process fgiven commandline argumebnts
	 */
	int parse(int argc, char** argv);
	/**
	 * Prints error message if it is required
	 */
	bool find_invalid(ostream& os);
private:
	/**
	 * Adds one token to the parser
	 */
	void add(char* arg);



};


#endif /* COMMAND_H_ */
