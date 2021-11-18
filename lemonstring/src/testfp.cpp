/*
 * testfp.cpp
 *
 *  Created on: 13.6.2013
 *      Author: voicegroup
 */


#include "lunicode.h"
#include <iostream>
#include "Tokenizer.h"

void test1(){
	char input[10] = "-1.1e-02";
	float result = 0;
	to_float(LString(input,8),result);
	//assert(result == -1.1);
	std::cout << result << std::endl;


}


int main(int argc, char **argv) {
test1();

}
