%include {
#include <cassert>
#include <cmath>
#include <iostream>
#include "floatparse.h"
#include "Tokenizer.h"
struct Pstate {
	int state;
	float result;
	Pstate(){
	state = 0;
	result = 0;
	}
};

}

%extra_argument {Pstate* pstate}

%parse_accept {
pstate->state = 1;
}

%parse_failure {
pstate->state = -1;
}


%token_type {int}
%type float {float}
%type digits {float}
%type int {float}
%type exponent {float}
%type fdigits {float}
%type fraction {float}


string ::= float(A). { pstate->result = A;}
string ::= MINUS float(A). { pstate->result = -A;}
float(V) ::= digits(A). {V = A;}
float(V) ::= digits(A) fraction(B). { V = A + B;}
float(V) ::= digits(A) exponent(B). { V = (float)A * pow(10.0f,(float)B);} 
float(V) ::= digits(A) fraction(B) exponent(C). { V =  (A + B) *pow(10.0f,(float)C);}
digits(V) ::= digits(A) DIGIT(B). { V = 10.0 * A + B; }
digits(V) ::= DIGIT(A). { V = A; }
int(V) ::= MINUS digits(A). {V = -A;}
int(V) ::= PLUS digits(A). { V = A;}
int(V) ::= digits(A). {V = A;}
exponent(V) ::= E int(A). {V = A;}
fdigits(V) ::= DIGIT(A). {V =  A;}
fdigits(V) ::= DIGIT(A) fdigits(B). {V = A + 0.1 * B;}
fraction(V) ::= DOT fdigits(A). {V = 0.1 * A;}


