/*
 * Tokenizer.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: dano
 */

#include "Tokenizer.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "utf8proc.h"

using namespace std;

//Tokenizer::Tokenizer(char* str,int size,char delimiter){
//	this->delimiter = delimiter;
//	this->str = str;
//	this->end = str+size;
//	token.start = str;
//	endToken = str;
//	count = 0;
//	token.size = 0;
//	//assert(end > str);
//}


ostream& operator<<(ostream& os,const LString& ls){
	if (ls.size() > 0)
		os.write(ls.start(),ls.size());
	return os;
}


int strip_characters(char* token,int tsize,const char* stripstring){
	int r = 0;
	char* p1 = (char*)token;
	char* p2 = (char*)token;
	char* end = (char*)token + tsize;
	char* p3 = (char*)stripstring;
	bool found = false;

	while(p1 != end) {
		p3 = (char*)stripstring;
		found = false;
		while (*p3 != 0){
			if (*p3 == *p1){
				found = true;
				r++;
				break;
			}
			p3++;
		}
		if (found){
			p1++;
		}
		else{
			*p2++ = *p1++;
		}
	}

	return r;


}


int strip_char(LString input,char c,LString output){
	assert(input.size() <= output.size());
	for(size_t i = 0; i< input.size(); i++){
		if (input.start()[i] != c){
			output.start()[i] = input.start()[i];
		}

	}

	return 0;

}



bool to_int(const LString& str,int& outresult){
	//assert(end > start);
	int result = 0;
	int n = 1;
	int v = 0;
	size_t size = str.size();
	if (size == 0  ){
		return false;
	}
	char* end = str.start() + size;
	while(size > 0){
		end--;
		size--;
		if (*end == '-'){
			result *= -1;
			break;
		}
		if (!is_digit(*end)){
			return false;
		}
		v = (*end - 48);
		if (v < 0 || v > 9){
			break;
		}
		result += v * n;
		n *= 10;
	}

	if (size == 0){
        outresult = result;
		return true;
    }
	return false;
}


Tokenizer::Tokenizer(const LString& str,char delimiter) : str(str) {
	assert(str.size() >= 0);
	this->delimiter = delimiter;
	tok._start= str.start();

	//this->end = str.start + str.size;
	//endToken = str.start;
}

//Tokenizer::Tokenizer(std::string& line,char delimiter){
//	this->delimiter = delimiter;
//	this->str = (char*)line.c_str();
//	this->end = str+ line.size();
//	token.start = this->str;
//	endToken = this->str;
//	count = 0;
//	token.size = 0;
//	//assert(end > str);
//}

bool Tokenizer::next(){
	assert(delimiter != -1);
	const char* end = str.start() + str.size();
	if (end <= tok._start){
		tok._size = 0;
		return false;
	}
	//assert(tok.start >= str && tok.start <= end);
	//assert(tok.start <= endToken);
	tok._start = tok._start + tok._size;
	while (tok._start < end && (*tok._start == delimiter)  ){
		assert(*tok._start != 0);
		tok._start++;
	}
	if (tok._start >= end){
		tok._size = 0;
		return false;
	}
	// Start tok = firts non delimiter or end
	char* endToken = tok._start;

	// End token = first delimiter or end
	while (endToken < end && *endToken != delimiter ){
		endToken++;
	}
	assert(endToken <= end);
	tok._size = endToken - tok._start;
	if (tok._size == 0)
		return false;
	return true;
}

int lstring_search(const LString& heap, const LString& needle){
	if (needle.size() > heap.size()){
		return -1;
	}
	char* res = std::search(heap.start(),heap.start() + heap.size(),needle.start(),needle.start()+needle.size());
	return res - heap.start();
}

//void LStringBuffer::append_wchar(int wchar_char){
//		unsigned char buf[8];
//		int _size = utf8proc_encode_char(wchar_char, buf);
//		assert(_size > 0);
//		buffer.append((const char*)buf,_size);
//
//	}

