#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

ifstream input("program.txt");

class CTextPosition {
private:
    int line_number;
    int ch_number;
public:
    int getLineNumber() { return line_number; };
    int getChNumber() { return ch_number; };
};

class CError {
private:
    int error_code;
    CTextPosition text_position;
public:
    int getErrorCode() { return error_code; };
    CTextPosition getTextPosition() { return text_position; };
};

class CioModule {
private:
    string buffer;
    int cur_ch;
    int cur_line;
public:
    char nextch()
    { 
        if (cur_ch == buffer.size() && input.eof())
            return NULL;
        if (cur_ch == buffer.size() && !input.eof())
        {
            getline(input, buffer);
            cur_ch = 0;
            cur_line++;
        }        
        cur_ch++;
        return buffer[cur_ch - 1];
    }
    void WriteError(CError error);
};
//DECL_PTR(CioModule);

class CLexer {
    //...
public:
    //CTokenPtr getNextToken(CioModule* io);
    char getNextToken(CioModule* io)
    {
        return io->nextch();
    };
};

int main() {
    auto io = new CioModule();
    //CTokenPtr token = nullptr;
    auto lexer = new CLexer();
    char ch;

    while (ch = io->nextch())
    {
        //cout << token->ToString();
        cout << ch << endl;
    }

    //while (ch = lexer->getNextToken(io))
    //{
    //    //cout << token->ToString();
    //    cout << ch;
    //}

    cout << "d o n e";

    delete io;
    delete lexer;
    input.close();
}