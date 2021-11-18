#include "floatparse.c"
#include "Tokenizer.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>



bool to_float(const LString& ls, float& result) {
	void* p = ParseAlloc(malloc);
	//ParseTrace(stdout, "-");
	Pstate pstate;
	for (size_t i = 0; i < ls.size(); i++) {
		char s = ls[i];
		switch (s) {
		case '+':
			Parse(p, PLUS, 0, &pstate);
			break;
		case 'e':
			Parse(p, E, 0, &pstate);
			break;
		case '-':
			Parse(p, MINUS, 0, &pstate);
			break;
		case '.':
			Parse(p, DOT, 0, &pstate);
			break;
		default:
			if (s >= 48 && s < 58) {
				Parse(p, DIGIT, s - 48, &pstate);
			}
			break;

		}

		//if (pstate.state < 0)
		//	break;
	}
	//std::cout << pstate.state << std::endl;
	Parse(p, 0, 0, &pstate);
	ParseFree(p, free);
	if (pstate.state > 0) {
		result = pstate.result;
		return true;
	}
	return false;
}

double log10s(float number){
double result = log10((double)number);
return result;
}



double pow10s(float number){
	double result = pow((double)10.0,(double)number);
	return result;
}
