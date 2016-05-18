/*
 * 
 *
 *  Created on: 9.10.2013
 *      Author: voicegroup
 */

#ifndef NORMALIZER_H_
#define NORMALIZER_H_

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

sign = ('+' | '-');

ws = ' ';
cul = ws+ (('č.'| 'číslo') ws+)?  (digit+ | cisloslovom);
title = ws+ capital small+;
word = ws+ letter{2,} ;

titul = ws+ ('JUDr.' |'MUDr.' | 'Ing.' | 'Mgr.' | 'bc.');



main := |*
('bytom' | 'sídlom') ws ('dvojbodka' | ':' | ',' | 'čiarka')  ws word{1,3}  :>> cul => { add_street(out,ts,te,"<UL1>",2);};
('bytom na' | 'sídlom na')  word{1,3}  :>> cul => { add_street(out,ts,te,"<UL5>",2);};
('bytom' | 'sídlom')  word{1,3}   cul => { add_street(out,ts,te,"<UL1>",1);};
('ulica' | 'námestie' | 'trieda' | 'ul.' | 'nám.' | 'Ulica' | 'Námestie' | 'Trieda' | 'Ul.' | 'Nám.')  word{1,3}  :>> cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('a'|'á') ws+ ('ulica' | 'námestie' | 'nábrežie' | 'trieda' | 'ul.' | 'nám.') :>> cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('a'|'á') cul => { add_street(out,ts,te,"<UL1>",0);};
letter+ 'ov' ('u'|'ú') ws+ 'ulicu' :>> cul   => { add_street(out,ts,te,"<UL2>",0);};
'na' ws ('ulici' | 'námestí' | 'nábreží'| 'triede' | 'ul.' | 'nám.' | 'Ulici' | 'Námestí' | 'Nábreží'| 'Triede' | 'Ul.' | 'Nám.') ws+ word{1,3} :>> cul  => { add_street(out,ts,te,"<UL5>",1);}; 
letter+ ('ovej'|'ového') ws+ ('ulici' | 'námestí' | 'nábreží'| 'triede' | 'ul.' | 'nám.') :>> cul  => { add_street(out,ts,te,"<UL5>",0);};

digit digit digit ws digit digit title{1,3} => { add_street(out,ts,te,"<OBEC1>",0);};
'Krajský súd' title{1,3} => { add_street(out,ts,te,"<OBEC1>",2);};
'Okresný súd' title{1,3} => { add_street(out,ts,te,"<OBEC1>",2);};
('bytom v' | 'sídlom v')  title{1,3} => { add_street(out,ts,te,"<OBEC5>",2);};


titul title title => {add_name(out,ts,te,"<MM1>","<MP1>",1);};
('poškodená' | 'maloletá' |'sudkyňa' | 'samosudkyňa' | 'opatrovníčka' | 'advokátka' | 'zástupkyňa' | 'obvinená' |'svedkyňa'| 'navrhovateľka' | 'odporkyňa' | 'žalovaná' | 'obžalovaná') titul? title title => {add_name(out,ts,te,"<ZM1>","<ZP1>",1);};
('poškodený' |'maloletý' |'sudca' |'samosudca' | 'opatrovník' | 'advokát' | 'zástupca' | 'obvinený' |'svedok'| 'navrhovateľ' | 'odporca' | 'žalovaný' | 'obžalovaný') titul? title title => {add_name(out,ts,te,"<MM1>","<MP1>",1);};
('poškodeného' |'maloletého' |'sudcu' |'samosudcu' | 'opatrovníka' | 'advokáta' | 'zástupcu' |'obvineného' |'svedka'| 'navrhovateľa' | 'odporcu' | 'žalovaného' | 'obžalovaného') titul? title title => {add_name(out,ts,te,"<MM24>","<MP24>",1);};
('poškodenej' |'sudkyne' |'samosudkyne' | 'opatrovníčky' | 'advokátky' | 'zástupkyne' |'obvinenej' |'svedkyňe'| 'navrhovateľky' | 'odporkyňe' | 'žalovanej' | 'obžalovanej') titul? title title => {add_name(out,ts,te,"<ZM24>","<ZP24>",1);};
('maloletej' |'svedkyni'| 'navrhovateľke' | 'odporkyni' | 'žalovanej' | 'obžalovanej' | 'obvinenej') titul? title title => {add_name(out,ts,te,"<ZM35>","<ZP35>",1);};
('poškodenému' |'maloletému' |'svedkovi'| 'navrhovateľovi' | 'odporcovi'| 'žalovanému' | 'obžalovanému' | 'obvinenému') titul? title title => {add_name(out,ts,te,"<ZM35>","<ZP35>",1);};
('poškodeným' |'maloletým' |'sudcom'| 'samosudcom' | 'opatrovníkom' | 'zástupcom'| 'advokátom') titul? title title => {add_name(out,ts,te,"<MM6>","<MP6>",1);};
('poškodenou' |'maloletou' |'sudkyňou'| 'samosudkyňou'| 'opatrovníčkou' | 'zástupkyňou'| 'advokátkou') titul? title title => {add_name(out,ts,te,"<ZM6>","<ZP6>",1);};
'zast.' titul title title => {add_name(out,ts,te,"<MM6>","<MP6>",1);};
'obv.' titul? title title => {add_name(out,ts,te,"<MM1>","<MP1>",1);};
'sv.' titul? title title => {add_name(out,ts,te,"<MM1>","<MP1>",1);};


ws 'IČO: ' digit+ => { out << " IČO: ";
add(out,ts+5,te,"<LONG>");};
ws 'EEČ: ' digit+ => { out << " EEČ: ";
add(out,ts+5,te,"<LONG>");};

#ws ([(),:"] | '-' | '?' | '!' | '.'| '"' | '/')+ ws => {add(out,ts,te,"<PUN>");}; 
#ws abbrev ws => {add(out,ts,te,"<SKR>");};
#ws capital capital+ ws => {add(out,ts,te,"<ACRON>");};
ws 'I.' ws => {add(out,ts,te,"<ORD>");};
ws 'II.' ws => {add(out,ts,te,"<ORD>");};
ws 'III.' ws => {add(out,ts,te,"<ORD>");};
ws 'IV.' ws => {add(out,ts,te,"<ORD>");};
ws poradieslovom ws => {add(out,ts,te,"<ORD>");};

ws mesiac ws => {add(out,ts,te,"<MESIAC>");};

ws digit digit? ws* '.' ws* digit ?digit ws* '.' ws* digit digit digit** => { add(out,ts,te,"<DATE>");}; # Datum
ws (cisloslovom ws*)+ => { add(out,ts,te,"<INT>");}; # Cislo Slovom
ws digit+ ws => { add(out,ts,te,"<INT>");};
ws digit{1,3} ('.' digit digit digit )+ :>> (ws* [,;]+ '-')? => {add_int(out,ts,te);};

ws digits '.' =>{ add(out,ts,te,"<ORD>");};
ws sign? (digits '.')? digits 'e' [+=]? digits =>{ add(out,ts,te,"<FLT>");}; # Cislo vo vedeckom formate

ws digit digit? ':' digit digit => {add_ints(out,ts,te,':');};
ws digit digit? '.' digit digit => {add_ints(out,ts,te,'.');};

ws capital capital ws* digit digit digit capital capital => {add_long(out,ts,te);}; # SPZ
ws digit+ (ws* '-' ws* digit+){2,} => {add_long(out,ts,te);}; # Cislo spisu s pomlckami
ws (digit | capital){2,} (ws* '/' ws* (digit | capital){2,}){2,} => {add_long(out,ts,te);}; # Spisova Znacka
ws digit+{8,} ws => {add_long(out,ts,te);}; # dlhe cislo

(capital letter* | [0-9]+ '.'? )  ( ws+ ([+\-,&]|letter)+){0,4} ws+ ('s.r.o.' | 'spol s r.o.'| 'spol. s r.o.' | 'spol. s.r.o.' | 'a.s.' | 'n.p.' | 'š.p.' | [Ii] 'nc.'| [Ll]'td.'| 'LTD' | 'spoločnosť s ručením obmedzeným') => { add(out,ts,te,"<FIRMA1>");};

title ' republika' => {add(out,ts,te,"<STAT1>");};
title ' republiky' => {add(out,ts,te,"<STAT2>");};
any => { out << *ts;};

*|;

}%%

%% write data;

class AnnotationLexer : public RagelProcessor{

public:
    AnnotationLexer() {
        %% write init;
    }
    
    
    void add_int(ostream& out,unsigned char* ts,unsigned char* te){
      out << ' ';
      while(ts != te){
        if (is_char_digit(*ts)){
          out << *ts;
        }
        ts++;
      }
      out << "|<INT> "; 
    }
    void add_ints(ostream& out,unsigned char* ts,unsigned char* te,char spl){
      out << ' ';
      LString t((char*)ts, te-ts);
      Tokenizer tok(t,spl);
      size_t i = 0;
      while (tok.next()){
        for (i = 0 ; i < tok.token().size(); i++){
          if (is_char_digit(tok.token()[i])){
            out << tok.token()[i];
          }
        }
        if (i > 0){
          out << "|<INT> ";
        }
      }
        
      while(ts != te){
        if (is_char_digit(*ts)){
          out << *ts;
        }
        ts++;
      }
      out << "|<INT> "; 
    }
    
    
    
    
    void add_long(ostream& out,unsigned char* ts,unsigned char* te){
      LString t((char*)ts, te-ts);
      Tokenizer tok(t,' ');
      while(tok.next()){
          out << tok.token();
      }
      out << "|<LONG> ";
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
        int v = 0;
        while (tok.next()){
             out << ' ';
            out << tok.token();
            if (i >= lstrip ){
                if (tok.token() == "č." || tok.token() ==  "číslo" ){
                    
                }
                else if (to_int(tok.token(),v)){
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
    
    void add_name(ostream& out,unsigned char* ts,unsigned char* te,const char* meno,const char* priezvisko,int lstrip){
        LString t((char*)ts, te-ts);
        int i = 0;
        int sz = 0;
        Tokenizer tok2(t,' ');
        while (tok2.next()){
          sz += 1;
        }
        Tokenizer tok(t,' ');
        int v = 0;
        while (tok.next()){
             out << ' ';
            out << tok.token();
            if (i >= lstrip && tok.token() != "JUDr." && tok.token() != "Mgr."  && tok.token() != "Ing." ){
                 out << '|';
                 if (v == 0){
                    out << meno;
                    v += 1;
                }
                else {
                    out << priezvisko;
                }
            }
            out << ' ';
            i++;
        }
    }
    void add(ostream& out,unsigned char* ts,unsigned char* te,const char* cl){
        LString t((char*)ts, te-ts);
        Tokenizer tok(t,' ');
        while (tok.next()){
             out << ' ';
            out << tok.token();
            out << '|';
            out << cl;
            out << ' ';
        }
    }
    virtual void call_ragel(ostream& out){
        %% write exec;
    }
};

#endif /* QUOTELEXER_H_ */
