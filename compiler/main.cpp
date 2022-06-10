#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <vector>
using namespace std;

ifstream input("while.pas");

enum KeyWords {
    assignSy = 51,
    beginSy = 17,
    endSy = 13, 
    evenSy = 16,
    leftBrSy = 9,
    rightBrSy = 4,
    ifSy = 56, 
    thenSy = 52,
    doSy = 54,
    colonSy = 5, semiColonSy = 14, commaSy = 20,
    programSy = 3, dotSy = 61,
    varSy, andSy, orSy, notSy, lessSy, moreSy, lessEvenSy, moreEvenSy, notEvenSy,
    plusSy, minusSy, multSy, subSy, divSy, modSy,
    elseSy,
    whileSy,
    quotSy,
};

map<string, KeyWords> keywords
{
    {"var", varSy},
    {"begin", beginSy},
    {"end", endSy},
    {"and", andSy},
    {"or", orSy},
    {"not", notSy},
    {"=", evenSy},
    {"<", lessSy},
    {">", moreSy},
    {"<=", lessEvenSy},
    {">=", moreEvenSy},
    {"<>", notEvenSy},
    {":=", assignSy},
    {"+", plusSy},
    {"-", minusSy},
    {"*", multSy},
    {"/", subSy},
    {"div", divSy},
    {"mod", modSy},
    {"(", leftBrSy},
    {")", rightBrSy},
    {"if", ifSy},
    {"then", thenSy},
    {"else", elseSy},
    {"while", whileSy},
    {"do", doSy},
    {"\"", quotSy},
    {":", colonSy},
    {";", semiColonSy},
    {".", dotSy},
    {",", commaSy},
    {"program", programSy}
};


#define DECL_PTR(ClassName) using ClassName ## Ptr = unique_ptr<ClassName>;

class CTextPosition {
private:
    int line_number;
    int ch_number;
public:
    CTextPosition(int _line_number, int _ch_number)
    {
        line_number = _line_number;
        ch_number = _ch_number;
    }
    int getLineNumber() { return line_number; };
    int getChNumber() { return ch_number; };
};
typedef CTextPosition* CTextPositionPtr;

#define PURE = 0
enum TokenType {
    ttIdent = 2,
    ttKeyword,
    ttConst
};

class CToken {
private:
    TokenType tType;
    CTextPositionPtr text_position;
public:
    CToken(TokenType _type, CTextPositionPtr tp)
    {
        tType = _type; 
        text_position = tp;
    }
    TokenType getType() { return tType; };
    CTextPositionPtr getTextPosition()
    {
        return text_position;
    }
    virtual string ToString() PURE;
};

typedef CToken* CTokenPtr;
//DECL_PTR(CToken);

class CIdentToken : public CToken
{
private:
    string ident;
public:
    CIdentToken(string _ident, CTextPositionPtr tp): CToken(ttIdent, tp), ident(_ident)  {}
    string ToString() 
    {
        return ident;
    }
};
typedef CIdentToken* CIdentTokenPtr;
//DECL_PTR(CIdentToken);

class CKeywordToken : public CToken
{
private:
    string keyword;
    KeyWords code;
public:
    CKeywordToken(string _keyword, CTextPositionPtr tp) : CToken(ttKeyword, tp), keyword(_keyword), code(keywords[_keyword]) {}
    KeyWords getCode()
    {
        return code;
    }
    string ToString()
    {
        return keyword;
    }
};
typedef CKeywordToken* CKeywordTokenPtr;
//DECL_PTR(CKeywordToken);

class CIntConstToken : public CToken
{
private:
    int value;
public:
    CIntConstToken(int val, CTextPositionPtr tp) : CToken(ttConst, tp), value(val) {}
    string ToString()
    {
        return to_string(value);
    }
};
typedef CIntConstToken* CIntConstTokenPtr;
//DECL_PTR(CIntConstToken);

class CRealConstToken : public CToken
{
private:
    double value;
public:
    CRealConstToken(double val, CTextPositionPtr tp) : CToken(ttConst, tp), value(val) {}
    string ToString()
    {
        return to_string(value);
    }
};
typedef CRealConstToken* CRealConstTokenPtr;
//DECL_PTR(CRealConstToken);

class CCharConstToken : public CToken
{
private:
    char value;
public:
    CCharConstToken(char val, CTextPositionPtr tp) : CToken(ttConst, tp), value(val) {}
    string ToString()
    {
        return "" + value;
    }
};
typedef CCharConstToken* CCharConstTokenPtr;
//DECL_PTR(CStringConstToken);

class CBoolConstToken : public CToken
{
private:
    bool value;
public:
    CBoolConstToken(bool val, CTextPositionPtr tp) : CToken(ttConst, tp), value(val) {}
    string ToString()
    {
        return value ? "true" : "false";
    }
};
typedef CBoolConstToken* CBoolConstTokenPtr;
//DECL_PTR(CBoolConstToken);

class CioModule {
private:
    string buffer;
    int cur_ch;
    int cur_line = 0;
public:
    tuple<char, CTextPositionPtr> nextch()
    {
        if (cur_ch == buffer.size() && input.eof())
            return make_tuple(NULL, new CTextPosition(cur_line, cur_ch));
        if (cur_ch == buffer.size() && !input.eof())
        {
            getline(input, buffer);
            cur_ch = 0;
            cur_line++;
            cout << cur_line << "\t" << buffer << endl;
            return make_tuple('\n', new CTextPosition(cur_line, cur_ch));
        }
        cur_ch++;
        return make_tuple(buffer[cur_ch - 1], new CTextPosition(cur_line, cur_ch));
    }
    void WriteError(int error, CTextPositionPtr tp)
    {
        cout << "****ERROR " << error << "****  line: " << tp->getLineNumber() << "  symbol position: " << tp->getChNumber() << endl;
    }
};
//DECL_PTR(CioModule);
auto io = new CioModule();

class CLexer {
private:
    char cur_ch;
    CTextPositionPtr cur_tp;
public:
    CTokenPtr getNextToken()
    {
        CTextPositionPtr token_tp = nullptr;
        if (!cur_ch)
            tie(cur_ch, cur_tp) = io->nextch();

        if (!cur_ch)
            return nullptr;

        while (cur_ch == '\n' || cur_ch == ' ')
            tie(cur_ch, cur_tp) = io->nextch();
        
        string next_t = "";

        if (cur_ch >= 'a' && cur_ch <= 'z' ||
            cur_ch >= 'A' && cur_ch <= 'Z')
        {
            token_tp = cur_tp;
            while (cur_ch >= 'a' && cur_ch <= 'z' ||
                cur_ch >= 'A' && cur_ch <= 'Z' ||
                cur_ch >= '0' && cur_ch <= '9')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }

            if (next_t == "true")
                return new CBoolConstToken(true, token_tp);
            if (next_t == "false")
                return new CBoolConstToken(false, token_tp);

            if (keywords.count(next_t) != 0)
                return new CKeywordToken(next_t, token_tp);

            return new CIdentToken(next_t, token_tp);
        }
        if (cur_ch >= '0' && cur_ch <= '9')
        {
            token_tp = cur_tp;
            while (cur_ch >= '0' && cur_ch <= '9' || cur_ch == '.')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }

            if (next_t.find('.') == string::npos)
                return new CIntConstToken(stoi(next_t), token_tp);
            else
                return new CRealConstToken(stod(next_t), token_tp);
        }

        next_t += cur_ch;
        //cur_ch = io->nextch();
        token_tp = cur_tp;
        switch (cur_ch)
        {
        case ':':
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t, token_tp);
        case '<':
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=' || cur_ch == '>')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t, token_tp);
        case '>':
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t, token_tp);
        case '\'':
            //char
            tie(cur_ch, cur_tp) = io->nextch();
            next_t += cur_ch;
            // '
            tie(cur_ch, cur_tp) = io->nextch();
            next_t += cur_ch;
            tie(cur_ch, cur_tp) = io->nextch();
            return new CCharConstToken(next_t[1], token_tp);
       /* case ';':
            tie(cur_ch, cur_tp) = io->nextch();
            return next_t;*/

        default:
            tie(cur_ch, cur_tp) = io->nextch();
            //if (keywords.find(next_t) != keywords.end())
                return new CKeywordToken(next_t, token_tp);
        }

        //return next_t;
    };
};
auto lexer = new CLexer();

class CSyntax {
private:
    CTokenPtr curToken;
public:
    int symbol()
    {
        if (curToken->getType() != ttKeyword)
            return curToken->getType();
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode();
    }

    bool belong(int symbol, vector<int> starters)
    {
        return find(starters.begin(), starters.end(), symbol) != starters.end();
    }
    void skip_to(vector<int> followers)
    {
        while (curToken != nullptr && !belong(symbol(), followers))
            curToken = lexer->getNextToken();
    }
    void skip_to(vector<int> starters, vector<int> followers)
    {
        while (curToken != nullptr && !belong(symbol(), starters) && !belong(symbol(), followers))
            curToken = lexer->getNextToken();
    }

    void skip_to_followers(vector<int> followers)
    {
        if (!belong(symbol(), followers))
        {
            io->WriteError(6, curToken->getTextPosition());
            skip_to(followers);
        }
    }

    void accept(KeyWords keyword)
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            if (kw->getCode() != keyword)
                io->WriteError((int)keyword, curToken->getTextPosition());
            curToken = lexer->getNextToken();
            /*if (curToken != nullptr)
                cout << curToken->ToString() << endl;*/
        }
        else
            io->WriteError((int)keyword, curToken->getTextPosition());
    }

    void accept(TokenType tt)
    {
        if (curToken->getType() != tt)
            io->WriteError((int)tt, curToken->getTextPosition());
        curToken = lexer->getNextToken();
        /*if (curToken != nullptr)
            cout << curToken->ToString() << endl;*/
    }
    void vardeclaration(vector<int> followers)
    {
        if (symbol() != ttIdent)
        {
            io->WriteError(ttIdent, curToken->getTextPosition());
            auto starters = followers; starters.push_back(ttIdent);
            skip_to(starters);
        }
        if (symbol() == ttIdent)
        {
            accept(ttIdent);
            if (curToken->getType() == ttKeyword)
            {
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                while (kw->getCode() == commaSy)
                {
                    accept(commaSy);
                    accept(ttIdent);
                    if (curToken->getType() == ttKeyword)
                        kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                    else
                        break;
                }
            }
            accept(colonSy);
            accept(ttIdent);
            skip_to_followers(followers);
        }
    }
    void varpart(vector<int> followers)
    {
        if (symbol() != varSy)
        {
            io->WriteError(varSy, curToken->getTextPosition());
            auto starters = followers; starters.push_back(varSy);
            skip_to(starters);
        }
        if (symbol() == varSy)
        {
            accept(varSy);
            while (curToken->getType() == ttIdent)
            {
                vector<int> var_followers = followers; var_followers.push_back(semiColonSy);
                vardeclaration(var_followers);
                accept(semiColonSy);
            }

            skip_to_followers(followers);
        }
    }
    bool is_add_op()
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == plusSy || kw->getCode() == minusSy || kw->getCode() == orSy;
    }
    bool is_mult_op()
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == multSy || kw->getCode() == subSy 
            || kw->getCode() == divSy || kw->getCode() == modSy || kw->getCode() == andSy;
    }
    void factor(vector<int> followers)
    {
        vector<int> starters = { ttConst, ttIdent, leftBrSy };
        if (!belong(symbol(), starters))
        {
            io->WriteError(144, curToken->getTextPosition());
            skip_to(starters, followers);
        }
        if (belong(symbol(), starters))
        {
            switch (curToken->getType())
            {
            case ttConst:
                accept(ttConst);
                break;
            case ttIdent:
                accept(ttIdent);
                break;
            case ttKeyword:
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                if (kw->getCode() == leftBrSy)
                {
                    accept(leftBrSy);
                    vector<int> expr_followers = followers; expr_followers.push_back(rightBrSy);
                    expr(expr_followers);
                    accept(rightBrSy);
                }
                break;
            }
            skip_to_followers(followers);
        }
    }
    void term(vector<int> followers)
    {
        vector<int> factor_followers = followers; factor_followers.insert(factor_followers.end(), { multSy, subSy, divSy, modSy, andSy });
        factor(factor_followers);
        if (curToken->getType() == ttKeyword && is_mult_op())
        {
            while (curToken->getType() == ttKeyword && is_mult_op())
            {
                curToken = lexer->getNextToken();
                factor(factor_followers);
            }
        }
        skip_to_followers(followers);
    }
    void simple_expr(vector<int> followers)
    {
        vector<int> term_followers = followers; term_followers.insert(term_followers.end(), { plusSy, minusSy, orSy });
        term(term_followers);
        if (curToken->getType() == ttKeyword && is_add_op())
        {
            while (curToken->getType() == ttKeyword && is_add_op())
            {
                curToken = lexer->getNextToken();
                term(term_followers);
            }
        }
        skip_to_followers(followers);
    }
    bool is_relation_op()
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == evenSy || kw->getCode() == lessSy
            || kw->getCode() == lessEvenSy || kw->getCode() == moreSy 
            || kw->getCode() == moreEvenSy || kw->getCode() == notEvenSy;
    }
    void expr(vector<int> followers)
    {
        vector<int> se_followers = followers; se_followers.insert(se_followers.end(), { evenSy, lessSy, lessEvenSy, moreSy, moreEvenSy, notEvenSy });
        simple_expr(se_followers);
        if (curToken->getType() == ttKeyword && is_relation_op())
        {
            curToken = lexer->getNextToken();
            simple_expr(followers);
        }
        skip_to_followers(followers);
    }
    void assignstatement(vector<int> followers)
    {
        accept(ttIdent);
        accept(assignSy);
        expr(followers);
        skip_to_followers(followers);
    }
    void statement(vector<int> followers)
    {
        vector<int> starters = { ttIdent, beginSy, ifSy, whileSy };
        if (!belong(symbol(), starters))
        {
            io->WriteError(183, curToken->getTextPosition());
            skip_to(starters, followers);
        }
        if (belong(symbol(), starters))
        {
            switch (curToken->getType())
            {
            case ttIdent:
                assignstatement(followers);
                break;
            case ttKeyword:
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                switch (kw->getCode())
                {
                case beginSy:
                    compositestatement(followers);
                    break;
                case ifSy:
                    condition(followers);
                    break;
                case whileSy:
                    preccycle(followers);
                    break;
                }
                break;
            }
            skip_to_followers(followers);
        }
    }
    void compositestatement(vector<int> followers)
    {
        if (symbol() != beginSy)
        {
            io->WriteError(ttIdent, curToken->getTextPosition());
            auto starters = followers; starters.push_back(beginSy);
            skip_to(starters);
        }
        if (symbol() == beginSy)
        {
            accept(beginSy);
            vector<int> st_followers = followers; st_followers.insert(st_followers.end(), { semiColonSy, endSy });
            statement(st_followers);
            if (curToken->getType() == ttKeyword)
            {
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                while (kw->getCode() == semiColonSy)
                {
                    accept(semiColonSy);
                    vector<int> st_followers = followers; st_followers.insert(st_followers.end(), { semiColonSy, endSy });
                    statement(st_followers);

                    if (curToken->getType() == ttKeyword)
                        kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                }
            }
            accept(endSy);
            skip_to_followers(followers);
        }
    }
    void preccycle(vector<int> followers)
    {
        accept(whileSy);
        vector<int> expr_followers = followers; expr_followers.push_back(doSy); 
        expr(expr_followers);
        accept(doSy);
        statement(followers);
        skip_to_followers(followers);
    }
    void condition(vector<int> followers)
    {
        accept(ifSy);
        vector<int> expr_followers = followers; expr_followers.push_back(thenSy);
        expr(expr_followers);
        accept(thenSy);
        vector<int> st_followers = followers; st_followers.push_back(elseSy);
        statement(st_followers);
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            if (kw->getCode() == elseSy)
            {
                accept(elseSy);
                statement(followers);
            }
        }
        skip_to_followers(followers);
    }
    void statementpart(vector<int> followers)
    {
        compositestatement(followers);
        skip_to_followers(followers);
    }
    void block(vector<int> followers)
    {
        auto var_followers = followers;
        var_followers.push_back(beginSy);
        varpart(var_followers);
        statementpart(followers);
        skip_to_followers(followers);
    }
    void program()
    {
        curToken = lexer->getNextToken();
        if (curToken == nullptr)
            return;

        if (symbol() != programSy)
        {
            io->WriteError(programSy, curToken->getTextPosition());
            vector<int> starters = { /*starters*/programSy, /*followers*/ varSy, beginSy, dotSy };
            skip_to(starters);
        }
        if (symbol() == programSy)
        {
            accept(programSy);
            accept(ttIdent);
            accept(semiColonSy);
        }
        vector<int> followers;
        followers.push_back(dotSy);
        block(followers);
        accept(dotSy);
    }
};

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