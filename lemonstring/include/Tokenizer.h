/*
 * Tokenizer.h
 *
 *  Created on: Apr 18, 2013
 *      Author: dano
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_
#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstring>

#define LEMONSTRING_VERSION 1

using namespace std;




/**
 * Pointer form of the Lemon string.
 *
 * It is possible that it points unallocated memory.
 * Pointer to a portion of a char array - a string.
 */
class LString  {

public:
	size_t _size;
	char* _start;

	virtual ~LString() {

	}


	LString(){
		_start = NULL;
		_size = 0;
	}
	/**
	 * Wrap given c string to LString
	 */

	LString(const char* str){
		assert(str!=NULL);
		_start = (char*)str;
		_size = strlen(str);
	}

	/**
	 * Wrap given portion of string
	 */
	LString(char* str,int sz){
		assert(sz>0 ? str!= NULL:true);
		_start = str;
		_size = sz;
	}
	/**
	 * Copy constructor.
	 */

	LString(const LString& other){
		_start = other.start();
		_size = other.size();
	}
	/**
	 * Comparison operator.
	 */

	LString& operator=(const LString& other){
		_start = other.start();
		_size = other.size();
		return (*this);
	}

	/**
	 * Wrap given portion of string.
	 * @arg Iterators to start and end
	 */

	static LString create(unsigned char* start,unsigned char* end){
		return LString((char*)start,end-start);
	}
	/**
	 * Wrap give STL string
	 */

	LString(const string& str){
		_start = (char*)str.c_str();
		_size = str.size();
	}


	virtual char* start() const{
		return _start;
	}

	virtual char* end() const {
		return _start + _size;
	}
	virtual size_t size() const{
		return _size;
	}

	/**
	 * Resizes string, cuttin from right
	 */
	 LString rstrip(size_t sz) const{
		assert(size() >= sz);
		return LString(_start,_size -sz);
	}

/**
 * Resizes string cutting from left.
 */
	 LString lstrip(size_t sz) const{
		assert(size() >= sz);
		return LString(_start + sz,_size -sz);
	}

	 LString substr(size_t st,size_t sz) const{
	 		assert(size()  >= sz - st);
	 		return LString(_start + st,_size -sz);
	 	}

	 /**
	 	 * cpp string representaiotn of the lemon string
	 	 */
	 	string str() const{
	 		return string(start(),size());
	 	}

	 	/**
	 	 * Comparison operator
	 	 */
	 	virtual bool operator==(const LString& other) const{
	 		if (size() != other.size()){
	 			return false;
	 		}
	 		return memcmp(start(),other.start(),size()) == 0;
	 	}
	 	/**
	 		 * Comparison operator
	 		 */
	 		virtual bool operator==(const string& other) const{
	 			if (size() != other.size()){
	 				return false;
	 			}
	 			return memcmp(start(),other.c_str(),size()) == 0;
	 		}
	 	/**
	 		 * Comparison operator with C string
	 		 */
	 	virtual bool operator==(const char* str) const{
            if (size() != strlen(str)){
	 		    return false;
	 	    }
	 		return strncmp(start(),str,size()) == 0;
	 	}

	 		virtual bool operator!=(const LString& other) const{
	 			if (size() != other.size()){
	 				return true;
	 			}
	 			return memcmp(start(),other.start(),size()) != 0;
	 		}

	 		/**
	 		 * Access character of the string
	 		 */
	 		virtual char& operator[](size_t in) const{
	 			assert(in < size());
	 			return start()[in];
	 		}

	 		bool starts_with(const LString& other) const{
                            assert(other.size() > 0);
	 			if (other.size() <= size()){
	 				return memcmp(start(),other.start(),other.size()) == 0;
	 			}
	 			return false;

	 		}

	 		bool ends_with(const LString& other) const{
                            assert(other.size() > 0);
	 			if (other.size() <= size()){
	 				return memcmp(start() + size() - other.size(),other.start(),other.size()) == 0;
	 			}
	 			return false;
	 		}

	 		size_t  find_first(const LString& other) const{
                            assert(other.size() > 0);
	 			if (other.size() <= size()){
	 				char *s2 = other.start();
	 				int i = 0;
	 				for (char *s1 = start(); s1 < end()  ; s1++){
	 					if (*s1 == *s2){
	 						s2 += 1;
	 					}
	 					else{
	 						s2 = other.start();
	 					}
	 					if (s2 == other.end()){
	 						return i;
	 					}
	 					i += 1;
	 				}

	 			}
	 			return -1;
	 		}

};


/**
 * Tokenizer Interface
 */
class TokenizerI{
public:
	virtual bool next() = 0;
	virtual const LString& token() const = 0;
	virtual ~TokenizerI(){};

};


/**
 * Tokenizes a string according to a given character
 *
 * Empty tokens are skipped
 *
 */
class Tokenizer : public TokenizerI {
	/**
	 * Processed string pointer
	 */
	LString tok;
public:
	/**
	 * Creates default tokenizer.
	 * This constructor can be used if processed string or delimiter is not known in advance.
	 * Assigment operator can be used to assign a tokenizer to the memeory.
	 */
	Tokenizer() : str(""){
		delimiter = -1;
	};
	Tokenizer(const LString& str,char delimiter);
	virtual bool next();
	virtual const LString& token() const{
		return tok;
	}
	virtual ~Tokenizer(){};
private:
	LString str;
	char delimiter;

};


/**
 * Splits stream into lines.
 */
class LineTokenizer : public TokenizerI{
	std::istream& infile;
	int count;
	bool add_end;
	LString tok;
public:
	string line;
	LineTokenizer(std::istream& ifl,bool add_end=false) : infile(ifl){
		count = 0;
		this->add_end = add_end;
	}
	virtual const LString& token() const{
		return tok;
	}
	virtual ~LineTokenizer() {};
	virtual bool next(){
		if (!infile.good() ) {
			line = "";
			return false;
		}
		getline(infile, line);
		if (infile.eof() && line.size() == 0 ) {
			line = "";
			return false;
		}
		if (add_end){
			line += '\n';
		}
		count += 1;
		tok = LString(line);
		return true;
	}
};
/**
 * Performs conversion of string to integer.
 * @arg str input string
 * @arg result result of conversion
 * @return true on success
 */
bool to_int(const LString& str,int& result);
/**
 * Performs conversion of string to float.
 * @arg str input string
 * @arg result result of conversion
 * @return true on success
 */

bool to_float(const LString& str,float& result);

/**
 * @return first occurence of needle in heap
 */
int lstring_search(const LString& heap, const LString& needle);


double log10s(float number);
double pow10s(float number);

inline bool is_digit(char c){
	if (c > 47 && c < 58){
		return true;
	}
	return false;
}

inline bool contains_char(const LString& str,char c){
	for (size_t i = 0; i < str.size(); i++){
		if (str.start()[i] == c){
			return true;
		}
	}
	return false;
}

/**
 * Put string to the stream operator.
 *
 * Can be used for printing
 */
ostream& operator<<(ostream& os,const LString& ls);







#endif /* TOKENIZER_H_ */
