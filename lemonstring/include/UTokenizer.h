/*
 * UTokenizer.h
 *
 *  Created on: 25.10.2013
 *      Author: voicegroup
 */

#ifndef UTOKENIZER_H_
#define UTOKENIZER_H_
#include "Tokenizer.h"

static const int delimss[16] = { 0x20, 0xa0, 0x09, 0x1680, 0x180E, 0x3000, 0xFEFF,
		0x2000, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x2006, 0x2007, 0x2008 };

/**
 * Tokenizer for splitting utf-8 string to characters
 */
class UTokenizer {
/**
 * Processed utf-8 string
 */
	LString str;
	/**
	 * Current token
	 */
	LString tok;
public:
	/**
	 * Number of bytes to be processed
	 */
	int remaining;
	/**
	 * Number of bytes already processed
	 */
	int count;
	/**
	 * Code of current character
	 */
	int character;
	/**
	 * Number of valid characters already processed
	 */
	int good_chars;
	/**
	 * New unicode tokenizer
	 */
	UTokenizer(const LString& ls);

	virtual ~UTokenizer(){};
	/**
	 * Finds next valid character.
	 * @return false if no new valid character can be found
	 */
	virtual bool next();
	/**
	 * @return pointer to current character
	 */
	virtual const LString& token() const {
		return tok;
	}
};
/**
 * Splits utf-8 encoded string according to the given utf-8 character
 */
class UCharTokenizer  {
	LString outtok;
	UTokenizer tok;
	bool is_delimiter(int chara);
public:
	/**
	 * Vector of delimiter codes
	 */
	vector<int> delims;
	/**
	 * @arg processed string
	 */
	UCharTokenizer(const LString& ls);
	void add_delimiters(const char* delimiters);
	virtual bool next();
	virtual const LString& token() const {
		return outtok;
	}
	virtual ~UCharTokenizer(){};
};


class USpaceTokenizer : public UCharTokenizer{
public:

	USpaceTokenizer(const LString& ls)
: UCharTokenizer(ls)
{
		delims.push_back( 0x20);
		delims.push_back( 0xa0);
		delims.push_back( 0x09);
		delims.push_back( 0x1680);
		delims.push_back( 0x180E);
		delims.push_back( 0x3000);
		delims.push_back( 0xFEFF);
		delims.push_back( 0x2000);
		delims.push_back( 0x2001);
		delims.push_back( 0x2002);
		delims.push_back( 0x2003);
		delims.push_back( 0x2004);
		delims.push_back( 0x2005);
		delims.push_back( 0x2006);
		delims.push_back( 0x2007);
		delims.push_back( 0x2008);
		 //  0x20 [0x00-0x0B]


	};

	virtual ~USpaceTokenizer(){};

};


int char_to_ascii(int character);

void char_to_stream(ostream& os, int character);
/**
 * @return lowercse string
 */
string to_lower(const LString& input);
/**
 * @return string with removed diacritics
 */
string to_ascii(const LString& input);
/**
 * Removes bad characters from string.
 * @return Normalized utf-8 string
 */
string to_normalized(const LString& input);

string to_reversed(const LString& input);


bool is_char_digit(char input);

bool is_digit(const LString& input);
bool is_alpha(const LString& input);
bool is_lower(const LString& input);
bool is_upper(const LString& input);
bool is_title(const LString& input);

#endif /* UTOKENIZER_H_ */
