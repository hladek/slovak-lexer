/*
 * cisloslovom.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: dano
 */


#include "CisloSlovom.h"
#include "Tokenizer.h"
#include <cstdlib>
#include "Vocab3.h"
#include "UTokenizer.h"
#include "Command.h"
#include "btree.h"
#include <sstream>

#include "UvPrefork.h"

static void readpos(const LString& pos, int& rod, int& cislo,
			int& pad) {
		rod = ROD_MUZN;
		cislo = CISLO_JEDN;
		pad = PAD_NOM;
		for (size_t i = 0; i < pos.size(); i++){
			if (is_char_digit(pos[i]) && pos[i] != '0'){
				pad = pos[i] - 48;
			}
			if (pos[i] == 'p'){
				cislo = CISLO_MNOZ;
			}
			else if (pos[i] == 'm'){
				rod = ROD_MUZ;
			}
			else if (pos[i] == 'n'){
				rod = ROD_STR;
			}
			else if (pos[i] == 'f'){
				rod = ROD_ZENA;
			}

		}
	}

bool to_float(const LString& inp,int& val1,int& val2){
	Tokenizer tok(inp,',');
	tok.next();
	if (to_int(tok.token(),val1)){
		if (tok.next()){
			if (to_int(tok.token(),val2)){
				return true;
			}
		}
	}
	return false;
}

bool to_order(const LString& inp,int& val){
	if (inp.size() == 0)
		return false;
	if (inp[inp.size()-1] == '.'){
       if (to_int(inp.rstrip(1),val)){
    	   return true;
       }
	}
	return false;
}

bool to_date(const LString& inp,int& den,int& mesiac, int& rok){
 Tokenizer tok(inp,'.');
 tok.next();
 if (to_int(tok.token(),den)){
	 if(tok.next() && to_int(tok.token(),mesiac)){
		 if(tok.next() && to_int(tok.token(),rok)){
			 return true;
		 }
	 }
 }
 return false;
}

class Rewriter : public Annotator {
	Vocab3 vocab;


public:
	BtreeMap rules;

	Rewriter() :
		rules(2,1) {
	}

	void load_file(const char* filename) {
		ifstream ifs(filename);
		LineTokenizer ltok(ifs);
		while (ltok.next()) {
			int key[2] = {-1,-1};
			Tokenizer tok(ltok.token(), '\t');
			if (tok.next()) {
				key[0] = vocab.Add(tok.token());
				if (tok.next()) {
					key[1] = vocab.Add(tok.token());
					rules.insert(key);
				}
			}
		}
	}


	virtual void annotate(LString chunk,ostream& out) {
		Tokenizer tok(chunk,' ');
		int i = 0;
		while(tok.next()){
			Tokenizer items(tok.token(),'|');
			items.next();
			LString word = items.token();
			LString cl("%");
			if (items.next()){
				cl = tok.token().lstrip(word.size() + 1);
			}
			string anot;
			int key[2] = {vocab.Add(word),-1};
			BtreeIterator it = rules.find_previous(key);
			vector<int> stateset;
			while(it.next() && it.key()[0] == key[0]){
				stateset.push_back(it.key()[1]);
			}
			if (stateset.size() > 0){
				// Naslo sa pravidlo na prepis
				anot = vocab.Get(stateset[rand() % stateset.size()]).str();
			}
			else{
				// Nenaslo sa pravidlo na prepis
				stringstream ss;
				int rod, cislo, pad = 0;
				readpos(cl,rod,cislo,pad);
				int value = 0;
				int val2 = 0;
				int val3 = 0;
				// Cele cislo
				if (word.size() < 8 && to_int(word,value)){
					anot =  CisloSlovom::cisloSlovom(value, rod, pad);
				}
				// Radova cislovka alebo ciselny zoznam
				else if (word.size() < 8 && to_order(word,value)){
					if (i == 0 && word.size() < 4){
						anot = "po_" + CisloSlovom::cisloSlovomR(value, ROD_STR, PAD_NOM);
					}
					else{
						anot =  CisloSlovom::cisloSlovomR(value, rod, pad);
					}
				}
				// Desatinne cislo
				else if (word.size() < 13 && to_float(word,value,val2)){
					ss << CisloSlovom::cisloSlovom(value, ROD_MUZN, PAD_NOM);
					ss << "_";
					LString la[2] = { LString("celá"), LString("celých")};
					ss << la[rand() % 2];
					ss << "_";
					ss << CisloSlovom::cisloSlovom(val2, ROD_MUZN, PAD_NOM);
					ss << "_";
					anot = ss.str();
				}
				// Datum
				else if (word.size() < 11 && to_date(word,value,val2,val3)){
					ss << CisloSlovom::cisloSlovomR(value, ROD_MUZN, PAD_GEN);
					ss << "_";
					ss << CisloSlovom::cisloSlovomR(val2, ROD_MUZN, PAD_NOM);
					ss << "_";
					ss << CisloSlovom::cisloSlovom(val3, ROD_MUZN, PAD_NOM);
					anot = ss.str();
				}
				// DLHE CISLO
				else if (word.size() > 5){
					int numdigits = 0;
					for (size_t i = 0; i < word.size(); i++) {
						if (is_digit(word.start()[i])) {
							numdigits += 1;
							ss << CisloSlovom::cisloSlovom(word.start()[i]- 48, ROD_MUZN, PAD_NOM);
							ss << "_";
						} else if (word.start()[i] == '/') {
							LString la[3] = { LString(" lomeno"), LString(
									" lomka"), LString(" lomítko") };
							ss << la[rand() % 3];
							ss << "_";
						} else {
							ss << word.start()[i];
							ss << "_";
						}
					}
					if (numdigits > 4){
						anot = ss.str();
					}

				}
				// Zoznamy s pismenkami
				else if (word.size() < 4 && i == 0){
					if (word[word.size() -1] == ')'){
						word = word.rstrip(1);
						ss << "po_";
					}
					else if (word[word.size() -1] == '.'){
						word = word.rstrip(1);
						ss << "po_";
					}
					else if (word[word.size() -1] == '/'){
						word = word.rstrip(1);
						ss << "po_";
					}

					ss << word;
					anot = ss.str();
				}
				i+= 1;
			}

			out << word;
			out << "|" << cl;
			if (anot != ""){
				for (size_t i=0; i<anot.size(); i++){
					if (anot[i] == ' '){
						anot[i] = '_';
					}
				}
				out << "|" << anot;
			}
			else if (is_upper(word)){
				out << "|" << word;
			}
			else{
				out << "|" << to_lower(word);
			}

			out << " ";
		}
	}
};

int main(int argc, char **argv) {
	LCParser  lp;
	Arg porta(lp,"-p",Arg::STRING, "Run server on port" ,"--port");
	Arg helpa(lp,"-h",Arg::BOOL, "Print help" ,"--help");
	lp.parse(argc,argv);
	if (helpa.isValid){
		lp.help(cout);
		return 0;
	}
	Rewriter r;
	for (size_t i = 0; i < lp.unk.size(); i++) {
		r.load_file(lp.unk[i].start());
	}
	if (porta.isValid) {
		cout << "Loaded " << r.rules.size() << endl;
        return TCPForking::listen(porta.value,&r);
	}
	else{
		LineTokenizer lt(cin);
		while (lt.next()){
			r.annotate(lt.token(),cout);
		}
	}

}

