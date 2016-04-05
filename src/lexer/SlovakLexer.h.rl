/*
 * Lexer2.h
 *
 *  Created on: 9.10.2013
 *      Author: voicegroup
 */

#ifndef LEXER2_H_
#define LEXER2_H_

#include "RagelProcessor.h"
#include "Tokenizer.h"

%%{
	
machine slovaklexer;
include WChar "utf8.rl";
include Abbrevs "abbrev.rl";
include Cisla "cisla.rl";

alphtype unsigned char;

action add {
   Tokenizer tok(LString::create(ts,te),' ');
   //out << '>';
	while(tok.next()){
		out << tok.token();
		out << ' ';
	}
	//out += '<';
}

action add_nospace {
	Tokenizer tok(LString::create(ts,te),' ');
	while(tok.next()){
		out << tok.token();
	}
	out << ' ';
}


action add_end {
    // Normalizuju mnohonasobne znamienka
	Tokenizer tok(LString::create(ts,te),' ');
	int counts[4] = {0,0,0,0};
	const unsigned char signs[5] = ".:?!";
	unsigned char* c = ts;
	while(c != te){
		for (int i =0 ; i < 4; i++){
		  if (*c == signs[i]){
		      counts[i]++;
		      continue;
		  }
	    }
	   c++;
	}
	int maxV = 0;
	int maxC = 0;
	for (int i =0 ; i < 4; i++){
		if (counts[i] > maxC){
		  maxV = i;
		  maxC = counts[i];
		}
	}
	out << signs[maxV];
//	if (maxV == 3 && counts[2] > 0 ){
//	   out << '?';
//	}
//	if (maxV == 2 && counts[3] > 0 ){
//	   out << '!';
//	}
	out << ' ';
	//if (!(lastquote > 0 && (out.str().size() -lastquote) < 30)){
	//	out << '\n';
	//	lastquote = 0;
	//}
}

action add_list {
	out << '\n';
	Tokenizer tok(LString::create(ts,te),' ');
	while(tok.next()){
		out << tok.token();
	}
	out << ' ';
}

action add_raw {
	out << LString::create(ts,te);
}

action add_split{
	tok = Tokenizer(LString::create(ts,te),'\n');
	tok.next();
	Tokenizer tok2(tok.token(),'-');
	tok2.next();
	out << tok2.token();
	tok.next();
	out << tok.token();
	out << ' ';
}

action add_uv {
	out << ' ';
	out << '\"';
	out << ' ';
	if (lastquote > 0){
	  lastquote = 0;
	}
	else{
	  lastquote = out.str().size();
	}
}



small =  lower |ulower ;
capital =  upper | uupper ;
letter = capital | small ;

dot = ('.');

# Problem je Windows/ Unix koniec riadku. V pripade Win konca riadku, treba vyhodit CR (0x0D) a nechat LF 0x0A
endline =  0x0a ;
# 0x0d by sa malo vyhodit v ramci any

# a0 je tiez ws(0x20 [0x00-0x0B]) |
ws = (0x20 | 0xa0 |  0x09|  (0x16 0x80) | (0x18 0x0E) |  (0x30 0x00) | (0xFE 0xFF))+  ;

leftquote =  '"' | '"' | '„' | '"'| ',,'| '„' ;

rightquote = '"' | '"' | '“' | '"' | '“';

quote = leftquote | rightquote | "'";

special = ( ','| '-'| '+'|   '(' | ')' |  '§' | '$' | '%' | '=' | '/'| '&');

word = letter (letter | "'" | digit | '_') *;

main := |*
'[' any {1,6} ']' => add; # sumova znacka
( ('?' | '!' | dot | ':'| ';')  ws*)+  => add_end;
special => add; # Specialny znak
abbrev => add;
word => add;
letter letter+ '-' letter letter+ => add; # Slovo s pomlckou,napr. e-mail
letter letter+  ws* '-' ws* endline ws* small small+ => add_split; # Rozdelene slovo
#quote => add_uv;
'-' ('-'| ws)* '-' => { out << " - ";}; # Ciara z pomlciek
'X'+ ws* ','? ws* dot => add_nospace; # Anonymizacna znacka
'rok' [ua] ws digit digit digit digit => add; # Dalsie cislo nie je radova cislovka
'čísl' [ua] ws digit+ ws => add;
(mesiac | ('rok' [ua]?)) ws digit digit digit digit => add;
'O\'' capital small+ => add; # Irske meno
word ('|' (letter | digit | [+-_<>%*]){1,10} ){1,10} => add_nospace; # TAG


('j' ws 'e' ws) | ('s' ws [aio] ws) => add_nospace; # druhe r i e d k e slovo
letter ws letter ws letter  (ws letter)+  => add_nospace; # R i e d k e  slovo
letter ws* dot ws* (letter ws* dot)*  => add_nospace; # dvojpismenova alebo trojpismenova  skratka
's.' ws* 'r.' ws* 'o.' => add_nospace;	# Aby boli caste dlhe skratky samostatny token
[Oo] '.' ws* 's.' ws* 'p' ? dot => add_nospace; # Obciansky SUdny Poriadok
'O.' ws* 's.' ws* 's.' => add_nospace;
'a.' ws* 's.' => add_nospace;
'z.' ws* 'z.' => add_nospace;
'Z.'	ws*	'z.' => add_nospace;
capital ws* dot => add_nospace; # jednopismenova inicialka
[XIVMLC]+ dot => add_nospace; # Rimske radove cislo

'ČVS' ws* ':' => add_nospace; # cislo vysetrovacieho spisu
'IČO' ws* ':' => add_nospace; # ICO
'EEČ' ws* ':' => add_nospace; # ICO
'PSČ' ws* ':' => add_nospace; # ICO


endline ws* (digit+| letter|([XIVMLC])+) dot? ws* (')' | '/') ws => add_list;
endline ws* '(' ws* digit+ ws* ')' ws => add_list;
endline '-' ws  => add_list; # hlava zoznamu na zaciatku riadka
endline => {out << ' ';}; # Ostatne konce riadku sa pocitaju ako medzera

#digit+ ws+ '-' ws+ digit+ ('-' (digit digit) (digit digit)? )? => add; # Ciselny rozsah, mozno s rokom
digit+ ws* ':' ws* digit+ (':' ws* digit+)? (',' digit+)?  => add_nospace; # cas 20:30
#(('-' | '+') ws*)? 



# Sumy a velke cisla
digit+ => add;
digit+ ('.' digit digit digit ){2,} => {add_int(out,ts,te);};
digit+ ('.' digit digit digit )+ (ws* [,;]+ '-') => {add_int(out,ts,te);};
digit+ ws* ',-' => {add_int(out,ts,te);};
(digit ws*)+ dot ws* digit+ :>> (ws* [,;]+ ws* '-'+)? => { add_int(out,ts,te);}; # Cislo s desatinnou ciarkou alebo cislovanie
digit+ :>> (ws+ digit digit digit)+ :>> (ws* [,;]+ ws* '-'+)? => { add_int(out,ts,te);}; # Cislo s desatinnou ciarkou alebo cislovanie

digit+ (ws* '.'? ws* digit digit digit)* ',' (digit+) :>> (ws* [,;]+ '-'*)?  => { add_float(out,ts,te,',');}; # cislo viac castove s bodkami +1.000.000 , 1234
digit+ (ws* ','? ws* digit digit digit)*  (ws* '.' (ws* digit+)) :>> (ws* [,;]+ '-'*)? => { add_float(out,ts,te,'.');}; # cislo viac castove s ciarkami +1,000,000.1234
(digit ws*)+ ',' digit+ :>> (ws* [,;]+ '-'*)? => { add_float(out,ts,te,',');};


digit digit? ws* dot ws* digit digit? (ws* dot ws* digit digit (digit digit)?)? => add_nospace; # datum

digit ws* '-' ws* nasobky => add_nospace; # nasobnost
digit digit? ws* dot ws* digit digit? ws* dot ((ws* digit digit (digit digit)? ))?  => add_nospace; # datum a cas
digit+ ws* dot => add_nospace; # Radova cislovka

digit digit ws+ digit digit digit ws+ capital letter{3,} => add; # PSC a meno obce
capital letter{3,} ws+ digit+ =>add; # Ulica a cislo

(word dot)? word '@' word dot word => add; # email
('http' | 'ftp' | 'gopher' | 'telnet' ) '://' (any - (ws|endline))+  (ws | endline) => add_nospace; # url
 'www.'  (any - (ws|endline))+   (ws | endline ) => add_nospace; # www
 ws word dot small{2,3} ws => add; # url
 dot 'net'  => add; # dot net
 dot 'týždeň' => add;
 '€' => add; # Euro
 'e.on' => add; # Nazov firmy
 (letter{2,} dot)? letter{3,} dot ('cz' | 'sk' | 'com' | 'org' | 'to' | 'pl' | 'hu' | 'au' | 'eu' | 'info' | 'ru' | 'co.uk') => add; # url
 (':-)'  | ':-)' | ':-o' | ';-)' | ':)' | ';)') ')'* => add_nospace; # smajliky
 any; # Znak co sa vyhadzuje
*|;

}%%

%% write data;


class Lexer2 : public RagelProcessor{
public:
	Tokenizer tok;
	//int lastquote;
	
	Lexer2()  {
//	lastquote = 0;
		%% write init;
	}
	
	void add_int(ostream& out,unsigned char* ts,unsigned char* te){
	  while(ts != te){
	    if (is_char_digit(*ts)){
	      out << *ts;
	    }
	    ts += 1;
	  }
	  out << ' ';
	}
	
	void add_float(ostream& out,unsigned char* ts,unsigned char* te,char sep){
	  LString t((char*)ts, te-ts);
	  Tokenizer tok(t,sep);
	  tok.next(); 
	  LString item = tok.token();
	  for (size_t i = 0; i < item.size(); i++){
	    if (is_char_digit(item[i])){
	      out << item[i];
	    }
	  }
	  out << ',';
      // TODO nastane next ???
	  tok.next();
	  item = tok.token();
	  for (size_t i = 0; i < item.size(); i++){
	    if (is_char_digit(item[i])){
	      out << item[i];
	    }
	  }
	  out << ' ';
	}
		
	virtual void call_ragel(ostream& out){
		%% write exec;
	}
};

#endif /* LEXER2_H_ */
