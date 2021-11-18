/*
 * UTokenizer.cpp
 *
 *  Created on: 25.10.2013
 *      Author: voicegroup
 */

#include "UTokenizer.h"
#include "utf8proc.h"
#include <algorithm>
#include "Tokenizer.h"
#include <sstream>


UTokenizer::UTokenizer(const LString& ls) {
	str = ls;
	count = 0;
	tok._start = ls.start();
	remaining = ls.size();
	character = 0;
	good_chars = 0;
}

bool UTokenizer::next() {
	if (remaining <= 0)
		return false;
	assert(tok._start < (str._start + str.size()));
	tok._start = str._start + str._size - remaining;
	tok._size = 0;
	while (tok._size == 0 && remaining > 0){
		tok._size = utf8proc_iterate((const uint8_t*) (tok._start), remaining,
					&character);
		if (character == -1 || tok._size == 0 || !utf8proc_codepoint_valid(character)){
			tok._start += 1;
			remaining -= 1;
			tok._size = 0;
		}
		else{
			remaining -= tok.size();
		}
	}

	if (tok._size < 1){
		//cout << remaining << endl;
		//cout << character << endl;
		//cout << "--";
		//cout << tok._start << endl;
		assert(remaining == 0);
		return false;
	}
	count++;
	good_chars += tok.size();
	assert(remaining >= 0);
	return true;
}

UCharTokenizer::UCharTokenizer(const LString& ls) :
		tok(ls) {
	outtok._start = ls.start();

}

void UCharTokenizer::add_delimiters(const char* delimiters) {
	LString dl(delimiters);
	UTokenizer utok(dl);
	while (utok.next()) {
		delims.push_back(utok.character);
	}

}

bool UCharTokenizer::is_delimiter(int chara) {
	return find(delims.begin(), delims.end(),chara) != delims.end();
}

bool UCharTokenizer::next() {
	if (tok.remaining == 0){
		return false;
	}

	// Find beginning of tkoen
	while (tok.next() && is_delimiter(tok.character)  ) {

	}
	//cout << "Begin " << tok.character << endl;

	outtok._start = tok.token().start();
	outtok._size = 0;
	while (tok.next() && !is_delimiter(tok.character)  ) {

	}

	// Find end of token
	outtok._size += tok.token().start() - outtok._start + tok.token().size();
	if (outtok._size == 0){
		//assert(tok.remaining == 0);
		return false;
	}
	//cout << "End " << tok.character << endl;
	assert(tok.character != 0);
	return true;

}

string to_lower(const LString& input) {
	UTokenizer tok(input);
	stringstream output;
	unsigned char buf[8];
	while (tok.next()) {
		//cout << tok.token();
		const utf8proc_property_t* prop = utf8proc_get_property(tok.character);
		int character = prop->lowercase_mapping;
		int _size = utf8proc_encode_char(character, buf);
		if (_size > 0){
			output << LString((char*) buf, _size);
		} else{
			output << tok.token();
		}

	}
	return output.str();
}

bool is_lower(const LString& input){
	UTokenizer tok(input);
	while (tok.next()) {

		const utf8proc_property_t* prop = utf8proc_get_property(tok.character);
		if (prop->category != UTF8PROC_CATEGORY_LL){
			return false;
		}
	}
	return true;
}
bool is_upper(const LString& input){
	UTokenizer tok(input);
	while (tok.next()) {
		const utf8proc_property_t* prop = utf8proc_get_property(tok.character);
		if (prop->category != UTF8PROC_CATEGORY_LU){

			return false;
		}
	}
	return true;
}
bool is_title(const LString& input){
	UTokenizer tok(input);
	int state = 0;
	while (tok.next()) {
		const utf8proc_property_t* prop = utf8proc_get_property(tok.character);
		if (state == 0){
			if(prop->category == UTF8PROC_CATEGORY_LU){
				state = 1;
			}
			else return false;
		}
		else if (state == 1){
			if(prop->category == UTF8PROC_CATEGORY_LU){
				state = 1;
			}
			else if(prop->category == UTF8PROC_CATEGORY_LL){
				state = 2;
			}
			else return false;
		}
		else if (state == 2){
			if(prop->category == UTF8PROC_CATEGORY_LL){
				state = 2;
			}
			else return false;
		}
	}
	if (state > 0)
		return true;
	return false;

}

bool is_char_digit(char input){
	if (input < 48 || input > 58){
			return false;
		}
	return true;
}


bool is_digit(const LString& input){
	if (input.size() == 0)
		return false;
	for (size_t i = 0 ; i < input.size(); i++){
		if (input.start()[i] < 48 || input.start()[i] > 58){
			return false;
		}
	}
	return true;
}


int char_to_ascii(int character){
	int result = 0;
	int chsize = utf8proc_decompose_char(character, &result, 4,
					UTF8PROC_STRIPMARK | UTF8PROC_DECOMPOSE, NULL);
			assert(chsize == 1);
			return result;

}

string to_ascii(const LString& input) {
	UTokenizer tok(input);
	stringstream output;

	unsigned char buf[9];
	while (tok.next()) {
		int result = char_to_ascii(tok.character);
		int _size = utf8proc_encode_char(result, buf);
		assert(_size > 0);
		buf[_size] = 0;
		output << buf;
		//		buffer.append((const char*)buf,_size);
	}
	return output.str();
}
/*
 *  Writes a decomposition of the unicode char 'uc' into the array being
 *  pointed to by 'dst'.
 *  Following flags in the 'options' field are regarded:
 *  REJECTNA:  an unassigned unicode code point leads to an error
 *  IGNORE:    "default ignorable" chars are stripped
 *  CASEFOLD:  unicode casefolding is applied
 *  COMPAT:    replace certain characters with their
 *             compatibility decomposition
 *  CHARBOUND: Inserts 0xFF bytes before each grapheme cluster
 *  LUMP:      lumps certain different characters together
 *  STRIPMARK: removes all character marks
 *  The pointer 'last_boundclass' has to point to an integer variable which
 *  is storing the last character boundary class, if the CHARBOUND option
 *  is used.
 *  In case of success the number of chars written is returned,
 *  in case of an error, a negative error code is returned.
 *  If the number of written chars would be bigger than 'bufsize',
 *  the buffer (up to 'bufsize') has inpredictable data, and the needed
 *  buffer size is returned.
 *  WARNING: The parameter 'uc' has to be in the range of 0x0000 to
 *           0x10FFFF, otherwise the program might crash!
 */
string to_normalized(const LString& input) {
	UTokenizer tok(input);
	stringstream output;
	int32_t res[10];
	unsigned char buf[10];
	int last_boundclass;
	int ressize = 0;
	while (tok.next()) {
		if (utf8proc_codepoint_valid(tok.character)) {
			//output += tok.token();
			ressize = utf8proc_decompose_char(tok.character, res, sizeof(int),
					UTF8PROC_REJECTNA | UTF8PROC_IGNORE |
					//  UTF8PROC_CASEFOLD |
							UTF8PROC_COMPAT | UTF8PROC_LUMP, &last_boundclass);
			//cout << "--" << ressize << "--";
			for (int i = 0; i < ressize; i++) {
				int _size = utf8proc_encode_char(res[i], buf);
				assert(_size > 0);
				buf[_size] = 0;
				output << buf;
			}
		}
	}
	return output.str();
}

void char_to_stream(ostream& os, int character){
	uint8_t buf[10];
    size_t sz = utf8proc_encode_char(character,buf);
    assert(sz < 10);
    os.write((const char*)buf,sz);
}
bool is_alpha(const LString& input){
	UTokenizer tok(input);
	while (tok.next()){
		const utf8proc_property_t* prop = utf8proc_get_property(tok.character);
//UTF8PROC_CATEGORY_LU  1
//UTF8PROC_CATEGORY_LL  2
//UTF8PROC_CATEGORY_LT  3
//UTF8PROC_CATEGORY_LM  4
//UTF8PROC_CATEGORY_LO  5
		if (prop->category >5 ){
			return false;
		}

	}
	return true;
}
string to_reversed(const LString& input){
	UTokenizer utok(input);
	vector<LString> chars;
	while (utok.next()){
		chars.push_back(utok.token());
	}
	stringstream ss;
	for (size_t i = 0; i < chars.size(); i++){
		//cout << chars[i] << " ";
		ss << chars[chars.size() -1 - i];
	}
	return ss.str();
}

