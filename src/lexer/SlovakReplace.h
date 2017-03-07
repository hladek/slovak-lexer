#include <iostream>
#include <sstream>
#include "btree.h"
#include "AnnotatorLexer.h"
#include "UTokenizer.h"
#include "Command.h"
#include "Vocab3.h"

#ifdef NETWORKING_ENABLED
#include "TCPFork.h"
#endif

using namespace std;


class NgramReplace {
    int order;
    BtreeMap rules;
public:
    NgramReplace(int order) :rules(order + 1,sizeof(float)){
        assert(order >= 1);
        this->order = order;
    }

    void insert(int* rule){
        rules.insert(rule);
    }

    size_t size(){
        return rules.size();
    }

    void matches(int* part,vector<int>& states) const{
        vector<int> k(part,part+order);
        k.push_back(-1);
        BtreeIterator it = rules.find_previous(k.data());
        states.clear();
        while(it.next() && memcmp(it.key(),part,sizeof(int) * order) == 0){
            states.push_back(it.key()[order]);
            //cout << it.key()[order] << endl;
        }
    }

    vector<int> replace(const vector<int>& sentence)const {
        vector<int> result;
        vector<int> states;
        if ((int)sentence.size() < order){
            return sentence;
        }
        size_t i = 0;
        for (i = 0; i < (sentence.size()-order + 1); i++){
            vector<int> k(sentence.data() + i,sentence.data() + i +order);
            k.push_back(-1);
            matches(k.data(),states);
            if (states.empty()){
                result.push_back(sentence[i]);
            }
            else{
                int st = states[rand() % states.size()];
                result.push_back(st);
                i+= (order-1);
            }
        }

        for (; i < sentence.size(); i++){
            result.push_back(sentence[i]);
        }
        return result;
    }
};


class NgramIndex
#ifdef NETWORKING_ENABLED
: public Annotator
#endif
{
private:
    static const int MAXORDER = 6;
    int load_rule(const LString& line) {
        if (line.size() == 0)
            return -1;
        if (line[0] == '#') {
            return -1;
        }
        Tokenizer tok(line, '\t');
        if (!tok.next())
            return -1;
        vector<int> wordids;
        Tokenizer tok2(tok.token(), ' ');
        while (tok2.next()) {
            wordids.push_back(vocab.Add(tok2.token()));
        }
        if (wordids.empty()) {
            return -1;
        }
        int con = -1;

        if (tok.next()) {
            con = vocab.Add(tok.token());
        } else {
            return -1;
        }

        wordids.push_back(con);

        //assert(wordids.size() > 0 && wordids.size() <= MAXORDER);
        if (wordids.size() <= MAXORDER)
            rules[wordids.size() - 2]->insert(wordids.data());
        //cout << string(tok.startToken,tok.tokenSize) << endl;
        //cout << con << endl;
        //rules[key] = con;
        return 1;
    }

public:
    Vocab3 vocab;
    vector<NgramReplace*> rules;
    bool useanotator;
    bool adddefault;

    NgramIndex() {
        //toLow = false;
        useanotator = true;
        adddefault = false;
        for (int i = 1; i <= MAXORDER; i++) {
            rules.push_back(new NgramReplace(i));
        }

    }


    void load_rules(LineTokenizer& ltok) {
        while (ltok.next()){
            LString line = ltok.token();
            if (line.size() == 0)
                continue;
            if (line[0] == '#')
                continue;
            Tokenizer tok(line, '\t');
            if (!tok.next())
                continue;
            vector<int> wordids;
            Tokenizer tok2(tok.token(), ' ');
            while (tok2.next()) {
                wordids.push_back(vocab.Add(tok2.token()));
            }
            if (wordids.empty()) {
                continue;
            }
            if (tok.next()) {
                int con = vocab.Add(tok.token());
                wordids.push_back(con);
                if (wordids.size() <= MAXORDER)
                    rules[wordids.size() - 2]->insert(wordids.data());
            } else {
                continue;
            }
            //assert(wordids.size() > 0 && wordids.size() <= MAXORDER);
        }
        cerr << rules[0]->size() << endl;
    }

    virtual void annotate_line(LString lin, ostream& out) {
        string an;
        if (useanotator){
            stringstream ss;
            AnnotationLexer al;
            al.annotate_line(lin,ss);
            al.annotate_line(LString(""),ss);
            an = ss.str();
        }
        else{
            an = lin.str();
        }
        vector<int> ids;
        Tokenizer tok(LString((char*) an.c_str(), an.size()), ' ');
        while (tok.next()) {
            ids.push_back(vocab.Add(tok.token()));
        }
        vector<int> result(ids);
        //result = rules[1].replace(result);
        for (int i = MAXORDER - 1; i >= 0; i--) {
            result = rules[i]->replace(result);
        }
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i] >= 0) {
                out << vocab.Get(result[i]);
                if (adddefault && !contains_char(vocab.Get(result[i]), '|')){
                    out << "|%";
                }
                out << ' ';
            }
        }
    }
};

