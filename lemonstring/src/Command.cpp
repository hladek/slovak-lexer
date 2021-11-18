/*
 * Command.cpp
 *
 *  Created on: 9.9.2013
 *      Author: voicegroup
 */

#include "Command.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

Arg::Arg(LCParser& lp, const char* name, argType type,
		const char* helpstring, const char* name2) :
		name(name), name2(name2), helpstring(helpstring), type(type) {
	isValid = false;
	isSet = false;
	value = 0;
    for (size_t i = 0; i < lp.args.size(); i++) {
        if (LString(name) == lp.args[i]->name) {
            cerr << "Argument with the same name already existing " << name << endl;
            abort();
        }
        if (LString(name2) == lp.args[i]->name2) {
            cerr << "Argument with the same name already existing " << name2 << endl;
            abort();
        }
    }
	lp.args.push_back(this);
}

string Arg::help() {
	stringstream result;
	if (type != Arg::LABEL){
		result << "\t";
		//result += name;
		result << name;
		result <<"\t";
		result << name2;
		result << "\t";
		result << helpstring;
	}
	else{
		result << helpstring;
	}

	return result.str();
}

void LCParser::help(ostream& os) {
	for (size_t i = 0; i < args.size(); i++) {
		os << args[i]->help() << endl;
	}
}

int LCParser::parse(int argc, char** argv) {
	for (int i = 1; i < argc; i++) {
		add(argv[i]);
	}
	return numValid;
}

void LCParser::add(char* arg) {
	LString token(arg);
	assert(token.size() > 0);
    // Added item is argument to a flag
	if (haveArg >= 0) {
		assert(args[haveArg]->type != Arg::BOOL);
		args[haveArg]->arg = token;
		if (args[haveArg]->type == Arg::STRING) {
			args[haveArg]->isValid = true;
		} else if (args[haveArg]->type == Arg::INTEGER) {
			int v = 0;
			args[haveArg]->isValid = to_int(token, v);
			args[haveArg]->value = v;
		} else if (args[haveArg]->type == Arg::FLOAT) {
			float v = 0;
			args[haveArg]->isValid = to_float(token, v);
			args[haveArg]->value = v;
		}
        else{
            cerr << "Command line parser error" << endl;
            abort();
        }
		if(args[haveArg]->isValid){
			numValid += 1;
		}
		haveArg = -1;
	} else {
        // Added item is flag
        // Find flag in database of valid flags
		bool f = false;
		for (size_t i = 0; i < args.size(); i++) {
			if (token == args[i]->name || token == args[i]->name2) {
				args[i]->isSet = true;
				numSet += 1;
				f = true;
				haveArg = -1;
				if (args[i]->type ==  Arg::BOOL) {
					// Is bool argument, is OK
					args[i]->isValid = true;
					numValid += 1;
				}
				else{
					// Is not bool argument, argument required
					haveArg = i;
				}
				break;
			}
		}
		if (!f) {
			unk.push_back(token);
		}
	}
}

bool LCParser::find_invalid(ostream& os){
	bool good = true;
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i]->isSet && !args[i]->isValid){
			good = false;
			os << "Command " << args[i]->name <<"/"<< args[i]->name2;
			os << "requires " << args[i]->typestr() << endl;
		}

	}

	return good;

}

string Arg::typestr() {
	string result = "unk";
	if (type == Arg::BOOL){
			result =  "no parameter";
	}
	else if (type == Arg::STRING){
		result =  "string";
	}
	else if (type == Arg::INTEGER){
		result ="integer";
	}
	else if (type == Arg::FLOAT){
		result = "float";
	}

	return result;
}
