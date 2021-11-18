/*
 * HMap.h
 *
 *  Created on: 10.10.2013
 *      Author: voicegroup
 */

#ifndef HMAP_H_
#define HMAP_H_

#include <vector>
#include "Tokenizer.h"
#include <cassert>
#include <cstdlib>

using namespace std;

class StringStore{
	public:
		vector<char> store;
		int _size;
		StringStore(){
			_size = 0;
			store.resize(2048);
		}
		int push(const LString& in){
			int value = in.size();;
			if (store.size() <= (value + _size+ sizeof(int))){
				store.resize((value +sizeof(int)+ _size) *2 );
			}
			assert(_size < (int)store.size());
			int start = _size;
			assert(store.size() > (start + sizeof(int) + value));
			memcpy(store.data() + _size,(char*)&value,sizeof(int));
			_size += sizeof(int);
			memcpy(store.data() + _size,in.start(),value);
			_size += value;
			return start;
		}
	};

class Item2 : public LString{
	const StringStore& buf;
public:
	int buffer_pos;
	Item2(const StringStore& buf,int buf_pos) : buf(buf){
		buffer_pos =buf_pos;
		assert(size() > 0);
	};

	virtual ~Item2(){};
	virtual char* start() const{
		return (char*)buf.store.data() + buffer_pos + sizeof(int);
	}

	virtual size_t size() const{
		return *(int*)(buf.store.data() + buffer_pos);
	}


};



class HMap3 {

protected:
	StringStore buf;
	class item{
		int buffer_pos;
		int string_id;
	public:
		item(){
			buffer_pos = -1;
			string_id = -1;
		}
		virtual ~item(){};
		bool is_empty() const{
			return buffer_pos == -1;
		}
		int create(const LString& other, StringStore& buf,int string_id){
			assert(buffer_pos == -1);
			buffer_pos = buf.push(other);
			this->string_id = string_id;
			return buffer_pos;
		}

		int get_id() const{
			return string_id;
		}

		bool equals(const LString& other, const StringStore& buf) const{
			assert(buffer_pos != -1);
			Item2 it(buf,buffer_pos);
			return it == other;
		}

		Item2 get_item(const StringStore& buf) const{
			return Item2(buf,buffer_pos);
		}
	};
	vector <item>  table;

	static size_t hash(const LString& ls){
		 unsigned long i = 0;
		 char* s = ls.start();
		 const char *end = s + ls.size();
		 for (; s != end; s++)
		     i += (i << 3) + *s;
		 return i * 1103515245;
	}

	int find_pos(const LString& ls) const{
		assert(table.size() > 0);
		assert(ls.size()>0);
		int table_pos = hash(ls) % table.size();
		int skip = 1;
		while(!table[table_pos].is_empty() && !table[table_pos].equals(ls,buf)  ){
			table_pos = (table_pos + skip) % table.size();
			skip += 1;
		}

		return table_pos;
	}

	item& _find(const LString& ls)  {
		return table[find_pos(ls)];
	}

	const item& _find(const LString& ls) const {
		return table[find_pos(ls)];
	}

public:

	int capacity() const{
		return table.size();
	}

	/**
	 * Assures that table can hold at leeast size items
	 * without reallocation
	 */
	void reserve(int size){
		if (size > capacity()){
			vector <item>  old(size,item());
					table.swap(old);
					//cout << old.size() << endl;
					for (size_t i = 0; i < old.size(); i++){
						item oldi = old[i];
						if (!oldi.is_empty()){
							item& it = _find(oldi.get_item(buf));
							assert(it.is_empty());
							it = oldi;
						}
					}
		}
	}

	HMap3(){
		reserve(512000);
	}

};

class Vocab3 : public HMap3{
	int _size;
	vector<int> id2buf_pos;
public:
	/**
	 * COnstructs empty dictionary
	 */
	Vocab3() {
		_size = 0;
	}
	/**
	 * Adds string to the dictionary.
	 * @return id key of the string
	 */
	int Add(const LString& val){
		assert(val.size() > 0);
		assert(capacity() > 0);
		//cout << val << endl;
		item& it = _find(val);
		//cout << it.first._start << endl;
		//assert(it.is_empty());
		if (it.is_empty()){
			int buf_pos =it.create(val,buf,_size);
			id2buf_pos.push_back(buf_pos);
			_size += 1;
			if ((float)_size / (float)capacity() > 0.5){
				// Pozor, it je potom neplatne
				reserve(capacity() * 2);
			}
			return id2buf_pos.size() -1;
		}
		assert(_size < capacity());
		assert (_find(val).get_id() == it.get_id());
		return it.get_id();
	}

	/**
	 * FInds a string in a dictionary
	 * @return id if the string is found, -1 if is not in the dictionary
	 */

	int Find(const LString& val) const{
		if (val.size() <= 0){
			return -1;
		}
		const item& it = _find(val);
		if (it.is_empty()){
			return -1;
		}
		return it.get_id();
	}

	/**
	 * Find string according to the given id
	 * Id must be positive an smaller than dictionary size
	 * @returns string with the given id
	 */

	Item2 Get(int id) const {
		assert(id >= 0);
		assert(id < (int)size());
		Item2 res(buf,id2buf_pos[id]);

		return res;
	}

	/**
	 * Size of the dictionary
	 */
	size_t size() const{
		return _size;
	}

	/**
	 * Saves the dictionary to disk in binary format
	 */

	void serialize(ostream& os){
		char header[30]="Lemon Dictionary v1 ";
		os.write(header,strlen(header));
		os.write((char*)&_size,4);
		short sz = 0;
		for (int i = 0 ; i < _size; i++){
			Item2 it = Get(i);
			sz = it.size();
			assert(sz < 10000);
			os.write((char*)&sz,2);
			os.write(it.start(),sz);
		}
	}

	/**
	 * Loads the dictionary from binary format
	 */
	void deserialize(istream& is){
		char buffer[10000];
		int sz = 0;
		is.read(buffer,20);
		is.read((char*)&sz,4);
		reserve(sz);

		short csz = 0;
		for (int i = 0 ; i < sz; i++){
			is.read((char*)&csz,2);
			assert(csz > 0 && csz < 10000);
			is.read(buffer,csz);
			Add(LString(buffer,csz));
		}
	}
private:
	void qswap(vector<int>& ar,size_t from,size_t to){
		int buf = ar[from];
		ar[from] = ar[to];
		ar[to] = buf;
	}

	size_t partition(vector<int>& ar,size_t from,size_t to){
		size_t pivot = (from+ to) / 2;
		LString pv = Get(ar[pivot]);
		size_t si = from;
		qswap(ar,pivot,to);
		for (size_t i = from; i < (to -1); i++){
			LString s = Get(ar[i]);
			if (memcmp(s.start(),pv.start(),min(s.size(),pv.size())) < 0){
				qswap(ar,i,si);
				si += 1;
			}
		}
		qswap(ar,si,to);
		return si;
	}

	void qsort(vector<int>& ar,size_t from,size_t to){
		if (from < to){
			size_t p = partition(ar,from,to);
			if (p > 0)
				qsort(ar,from,p-1);
			if (p < to)
				qsort(ar,p+1,to);
		}

	}
public:
	Vocab3 sort(){
		Vocab3 voc;
		vector<int> res(size());
		for (size_t i = 0; i < size(); i++){
			res[i] = i;
		}
		if (size() > 0){
			qsort(res,0,size()-1);
		}
		for (size_t i = 0; i<size(); i++){
			voc.Add(Get(res[i]));
		}
		return voc;

	}

};


#endif /* HMAP_H_ */
