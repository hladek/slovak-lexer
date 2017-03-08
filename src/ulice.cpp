/*
 * ulice.cpp

 *
 *  Created on: Dec 8, 2014
 *      Author: dano
 */
#include "slovaklexer/Ulice.h"

int main(int argc, char **argv) {

Ulice ul;
LineTokenizer lt(cin,true);
while (lt.next()){
    ul.annotate_line(lt.token(),cout);

}
}




