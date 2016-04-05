/*
 * RagelProcessor.h
 *
 *  Created on: 17.10.2013
 *      Author: voicegroup
 */

#ifndef RAGELPROCESSOR_H_
#define RAGELPROCESSOR_H_

#include "UTokenizer.h"
#include <cassert>
#include <cstdlib>
#include <sstream>


class RagelProcessor {
public:
	typedef unsigned char atype;
	//string buf;
	vector<atype> buf;
	atype *te;
	atype *ts;
	atype *p;
	atype *pe;
	atype *eof;
	int act;
	int cs;
	virtual ~RagelProcessor(){};

	virtual void pop(ostream& out){
        out.flush();
    }

	virtual void call_ragel(ostream& out)=0;

		/**
		 * Imput chunk with endline
		 */
	virtual void annotate_line(LString chunk,ostream& out){
		size_t have = buf.size();
		buf.resize(have + chunk.size());
		memcpy((char*)buf.data()+ have,chunk.start(),chunk.size());

		//buf += chunk;
		if (buf.size() > 0){
			ts = buf.data();
			te = ts + have;
			p = buf.data() + have;
			//te = p;
			//cout << p << endl;
			pe = buf.data() + buf.size();
			eof = NULL;
			if (chunk.size() == 0 || (buf.size() > 1000000)){
				eof = pe;
			}
			//int sz = buf.size() -have;

			call_ragel(out);

			if (ts == 0){
				have = 0;
			}
			else{
				have = pe - ts;
			}
			assert(have >= 0);
			assert(have <= buf.size());

			//cerr << have << endl;
			if (have > 0){
				if (eof != NULL){
					cerr << "?????????????????????" << endl;
					//cout << buf;
					cerr << endl;
					abort();
				}
				memmove(buf.data(),buf.data() + buf.size() -have,have);
				buf.resize(have);

				//buf = buf.substr(buf.size() - have,have);

				//if (buf.size() > 0 &&buf.c_str()[0] == 0x0d){
				//	buf.erase(buf.begin(),buf.begin()+1);
				//}
				//assert(pe - buf > have );
			}
			else {
				buf.clear();
			}
		}
		if ((unsigned int)chunk.size() == 0 || eof != NULL){
			assert(buf.size() == 0);
			pop(out);
		}
	}

};


#endif /* RAGELPROCESSOR_H_ */
