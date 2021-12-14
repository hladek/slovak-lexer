#include <unordered_set>
#include <string>
#include <iostream>
using namespace std;

int main() {
    hash<string> hasher;
    unordered_set<size_t> hashes;                                // empty
    for (string line; getline(cin, line);) {
        size_t hh = hasher(line);
        auto res = hashes.emplace(hh);
        if (res.second){
            cout << line << endl;
        }
    }
}
