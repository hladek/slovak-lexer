#include <iostream>
#include <fstream>
#include "Command.h"
#include "Vocab3.h"
#include "SlovakLexer.h"
#ifdef NETWORKING_ENABLED
#include "TCPFork.h"
#endif

class LexerTokenizer: public Lexer2 
#ifdef NETWORKING_ENABLED
                      ,public Annotator
#endif
{
public:
	const Vocab3& vocab;
	Vocab3 numbers;
	LexerTokenizer(Vocab3& vocab):vocab(vocab){
		numbers.Add(LString("jedenásť"));
		numbers.Add(LString("dvanásť"));
		numbers.Add(LString("trinásť"));
        numbers.Add(LString("štrnásť"));
        numbers.Add(LString("pätnásť"));
        numbers.Add(LString("šestnásť"));
        numbers.Add(LString("sedemnásť"));
        numbers.Add(LString("osemnásť"));
        numbers.Add(LString("devätnásť"));
        numbers.Add(LString("dvadsať"));
        numbers.Add(LString("tridsať"));
        numbers.Add(LString("štyridsať"));
        numbers.Add(LString("päťdesiat"));
        numbers.Add(LString("šesťdesiat"));
        numbers.Add(LString("sedemdesiat"));
        numbers.Add(LString("osemdesiat"));
        numbers.Add(LString("devädesiat"));
        numbers.Add(LString("dvesto"));
        numbers.Add(LString("štyristo"));
        numbers.Add(LString("päťsto"));
        numbers.Add(LString("šesťsto"));
        numbers.Add(LString("sedemsto"));
        numbers.Add(LString("osemsto"));
        numbers.Add(LString("deväťsto"));
        numbers.Add(LString("jeden"));
        numbers.Add(LString("dva"));
        numbers.Add(LString("tri"));
        numbers.Add(LString("štyri"));
        numbers.Add(LString("päť"));
        numbers.Add(LString("šesť"));
        numbers.Add(LString("sedem"));
        numbers.Add(LString("osem"));
        numbers.Add(LString("deväť"));
        numbers.Add(LString("desať"));
        numbers.Add(LString("tisíc"));
        numbers.Add(LString("sto"));
	}
	virtual ~LexerTokenizer(){};

    virtual void annotate(char* instr,size_t sz,ostream& os){
        LString chunk(instr,sz);
        if (chunk.size() > 4){
            if ((chunk[0] == 'i' && chunk[1] == '\t') || chunk == "--endtext"){
                os << chunk << endl;
                return;
            }
        }
        string norm = to_normalized(chunk);
        norm += "\n";
        stringstream ss(ios_base::in | ios_base::out);
        Lexer2::annotate_line(LString(norm),ss);
        //os << ss.str();
         
   		LineTokenizer tok(ss);
    
		//os << "--------" << endl;
		//os << out.str();
		//os << "--------" << endl;
		while(tok.next()){
			//os << tok.token();
			Tokenizer tok2(tok.token(),' ');
			int count = 0;
			LString last("");
			string word;
			while (tok2.next()){
				word = tok2.token().str();
				//cout << "---" << word << "--" << endl;
				// Rozdelenie cisiel slovom na jednoptlive slova
				if (word.size() > 7){
					LString rest = LString(word);
					stringstream buf2;
					bool found = true;
					while (found && rest.size() > 0){
						found = false;
						for (size_t i = 0; i < numbers.size(); i++){
							if (rest.starts_with(numbers.Get(i))){
								//cout << "Found" << endl;
								buf2 << numbers.Get(i);
								buf2 << ' ';
								rest = rest.lstrip(numbers.Get(i).size());
								found = true;
								break;
							}
						}
					}
					if (rest.size() == 0){
						word = buf2.str();
					}
				}
				// Vypise token
				// Tokeny vacsie ako 40 sa vyhadzuju
				Tokenizer tok3(LString(word),' ');
				while (tok3.next()){
					if (tok3.token().size() < 40){
						if (count == 0){
							os << ' ';
						}
						os << tok3.token();
						os << ' ';
						count += 1;
						last = tok2.token();
                        char c = tok3.token().start()[0];
                        if (c == '?' || c == '!' || c == '.' || c == ':'){
                            os << endl;
                        }
					}
				}
			}

			if (count == 0){
				os << endl;
		    }	
			//os << count;
			//os << endl;
			//os << endl;
		}
	}
};
int main(int argc, char **argv) {
	LCParser  lp;
	Arg helpa(lp,"-h",Arg::BOOL,"Print usage and exit.","--help");
	Arg filea(lp,"-f",Arg::STRING, "Set file to process" ,"--file");
	Arg porta(lp,"-p",Arg::INTEGER, "Listen on port" ,"--port");
	Vocab3 vocab;
	lp.parse(argc,argv);
    if (helpa.isValid){
        lp.help(cout);
        return 0;
    }

    LexerTokenizer ltok(vocab);
#ifdef NETWORKING_ENABLED
    if (porta.isValid){
        return TCPFork::start(&ltok, (int)porta.value);
    }
    else
#endif
        if (filea.isValid){
        ifstream of(filea.arg.start());
        LineTokenizer lt(of);
        while(lt.next()){
            ltok.annotate(lt.token().start(),lt.token().size(),cout);
        }
    }
    else {
        LineTokenizer lt(cin);
        while(lt.next()){
            ltok.annotate(lt.token().start(),lt.token().size(),cout);
        }
    }
}

