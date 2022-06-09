#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

ifstream input("whileif.pas");

int main() {
    /*CTokenPtr token = nullptr;
    while (token = lexer->getNextToken())
    {
        cout << token->ToString() << "\t";
        switch (token->getType())
        {
        case ttConst:
            cout << "c" << endl;
            break;
        case ttIdent:
            cout << "i" << endl;
            break;
        case ttKeyword:
            cout << "kw" << endl;
            break;
        }
    }*/

    auto syntax = new CSyntax();
    syntax->program();

    cout << "d o n e";

    delete io;
    delete lexer;
    delete syntax;
    input.close();
}