/*
 * 
 *
 *  Created on: 9.10.2013
 *      Author: voicegroup
 */

#ifndef UNORMALIZER_H_
#define UNORMALIZER_H_

#include "RagelProcessor.h"
#include "UTokenizer.h"

%%{

machine slovakparser;
include Cisla "cisla.rl";
include WChar "utf8.rl";
include Abbrevs "abbrev.rl";
alphtype unsigned char;

digits = [0-9]+;

small =  lower |ulower ;
capital =  upper | uupper ;
letter = capital | small ;
ws = ' ';

word = letter{2,} ws+;

sign = ('+' | '-');

cul = ws*  ('č. '| 'číslo '| 'č ' | 'Č ' )? ws*  (digit+ | cisloslovom);
title = ws capital small+;




main := |*
('bytom' | 'sídlom') ws ('dvojbodka' | ':' | ',' | 'čiarka')  ws word{1,3}  :>> cul => { add_street(out,ts,te,"<UL1>",2);};
('bytom na' | 'sídlom na') ws word{1,3}  :>> cul => { add_street(out,ts,te,"<UL5>",2);};
('bytom' | 'sídlom') ws word{1,3}  :>> cul => { add_street(out,ts,te,"<UL1>",1);};
('ulica' | 'námestie' | 'trieda' | 'ul.' | 'nám.') ws word{1,3}  :>> cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('a'|'á') ws+ ('ulica' | 'námestie' | 'nábrežie' | 'trieda' | 'ul.' | 'nám.') :>> cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('a'|'á') cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('u'|'ú') ws+ 'ulicu' :>> cul   => { add_street(out,ts,te,"<UL2>",0);};
'na' ws ('ulici' | 'námestí' | 'nábreží'| 'triede' | 'ul.' | 'nám.') ws+ word{1,3} :>> cul  => { add_street(out,ts,te,"<UL5>",1);}; 
letter+ ('ovej'|'ového') ws+ ('ulici' | 'námestí' | 'nábreží'| 'triede' | 'ul.' | 'nám.') :>> cul  => { add_street(out,ts,te,"<UL5>",0);};

any => { out << *ts;};

*|;

}%%

%% write data;

class Ulice : public RagelProcessor{

public:
	Ulice() {
		%% write init;
	}
	
	
	void add_street(ostream& out,unsigned char* ts,unsigned char* te,const char* cl,int lstrip){
	    LString t((char*)ts, te-ts);
	    int i = 0;
	    int sz = 0;
	    Tokenizer tok2(t,' ');
	    while (tok2.next()){
	      sz += 1;
	    }
	    Tokenizer tok(t,' ');
	    while (tok.next()){
	     	out << ' ';
	        out << tok.token();
	        if (i >= lstrip ){
		        if (tok.token() == "č." || tok.token() ==  "číslo" ){
		        	
		        }
		        else if (i == (sz -1)){
		        	out << "|<INT>";
		        }
		        else{
		        	out << '|';
		          	out << cl;
		        }
	        }
	        out << ' ';
	        i++;
	    }
	}
		
	
	virtual void call_ragel(ostream& out){
		%% write exec;
	}
	
};

#endif /* QUOTELEXER_H_ */
