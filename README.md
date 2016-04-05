# slovak-lexer

Tools and library for lexical processing of the Slovak language. 

# Features

- Word and sentence boundary identification
- Number normalization
- Abbreviation identification
- Number rewritting to spoken form
- Fast string replacement
- Includes Forking TCP server based on LibUV


# Requirements

cmake
C++ compiler
lemonstring library
Ragel (optional)
libuv (optional)

# Installation

1. Install lemonstring
2. mkdir bin
3. cd bin
4. cmake ../src
5. make
6. sudo make install

# Usage

cat slovaktext.txt | slovaktokenizer > tokenizedtext.txt

# License

GPLv3
for other uses contact the author at daniel.hladek@tuke.sk
