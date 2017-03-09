#include "SlovakReplace.h"



int main(int argc, char **argv) {
    LCParser  lp;
#ifdef NETWORKING_ENABLED
    Arg porta(lp,"-p",Arg::STRING, "Run server on port" ,"--port");
#endif
    Arg helpa(lp,"-h",Arg::BOOL, "Print help" ,"--help");
    Arg anota(lp,"-a",Arg::BOOL, "Use additional state-machine for named entity annotation" ,"--additional");
    Arg defaulta(lp,"-d",Arg::BOOL, "Add default class if no class was set" ,"--default");

    lp.parse(argc,argv);
    if (helpa.isValid){
        cout << "Text Annotator" << endl;
        cout << "\t rewrites text according to the given rules" << endl;
        lp.help(cout);
        return 0;
    }

    NgramIndex r;
    if (anota.isValid){
        r.useanotator = true;
    }
    if (defaulta.isValid){
        r.adddefault = true;
    }

    for (size_t i = 0; i < lp.unk.size(); i++) {
        ifstream ifs(lp.unk[i].start());
        LineTokenizer lt(ifs);
        r.load_rules(lt);
    }
#ifdef NETWORKING_ENABLED
    if (porta.isValid) {
        cout << "Loaded " << r.rules.size() << endl;
        return TCPFork::start(&r,(int)porta.value);
    }
    else{
#endif
        LineTokenizer lt(cin);
        while (lt.next()){
            r.annotate_line(lt.token().start(),cout);
        }
#ifdef NETWORKING_ENABLED
    }
#endif
}
