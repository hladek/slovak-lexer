/*
 * cisloslovom.cpp
 *
 *  Created on: Mar 10, 2014
 *      Author: dano
 */

#include "SlovakCisloSlovom.h"

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
        return TCPFork::start(&r,(int)porta.value);
    }
    else{
        LineTokenizer lt(cin);
        while (lt.next()){
            r.annotate(lt.token(),cout);
            cout << endl;
        }
    }
}

