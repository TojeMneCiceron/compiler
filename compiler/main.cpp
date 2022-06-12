#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <iomanip>
#include <vector>
//#include "compiler.h"
using namespace std;

string filename = "syntaxerrors.pas";

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
enum TokenType {
    ttIdent = 2,
    ttKeyword,
    ttConst
};
enum VariantType {
    vtInt, vtReal, vtChar, vtBoolean, vtUndefined
};
map<string, VariantType> types
{
    {"integer", vtInt},
    {"real", vtReal},
    {"char", vtChar},
    {"Boolean", vtBoolean}
};

#define PURE = 0

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

class CToken {
private:
    TokenType tType;
    VariantType vType;
    CTextPositionPtr text_position;
public:
    CToken(TokenType _type, VariantType _vType, CTextPositionPtr tp) : tType(_type), vType(_vType), text_position(tp) {}
    TokenType getType() { return tType; };
    VariantType getVarType() { return vType; };
    void setVarType(VariantType _vType) { vType = _vType; };
    CTextPositionPtr getTextPosition() { return text_position; }
    virtual string ToString() PURE;
};
typedef CToken* CTokenPtr;

class CIdentToken : public CToken
{
private:
    string ident;
public:
    CIdentToken(string _ident, CTextPositionPtr tp) : CToken(ttIdent, vtUndefined, tp), ident(_ident) {}
    string ToString() { return ident; }
};
typedef CIdentToken* CIdentTokenPtr;

class CKeywordToken : public CToken
{
private:
    string keyword;
    KeyWords code;
public:
    CKeywordToken(string _keyword, CTextPositionPtr tp) : CToken(ttKeyword, vtUndefined, tp), keyword(_keyword), code(keywords[_keyword]) {}
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

class CIntConstToken : public CToken
{
private:
    int value;
public:
    CIntConstToken(int val, CTextPositionPtr tp) : CToken(ttConst, vtInt, tp), value(val) {}
    VariantType getType() { return vtInt; }
    string ToString()
    {
        return to_string(value);
    }
};
typedef CIntConstToken* CIntConstTokenPtr;

class CRealConstToken : public CToken
{
private:
    double value;
public:
    CRealConstToken(double val, CTextPositionPtr tp) : CToken(ttConst, vtReal, tp), value(val) {}
    VariantType getType() { return vtReal; }
    string ToString()
    {
        return to_string(value);
    }
};
typedef CRealConstToken* CRealConstTokenPtr;

class CCharConstToken : public CToken
{
private:
    char value;
public:
    CCharConstToken(char val, CTextPositionPtr tp) : CToken(ttConst, vtChar, tp), value(val) {}
    VariantType getType() { return vtChar; }
    string ToString()
    {
        return "" + value;
    }
};
typedef CCharConstToken* CCharConstTokenPtr;

class CBoolConstToken : public CToken
{
private:
    bool value;
public:
    CBoolConstToken(bool val, CTextPositionPtr tp) : CToken(ttConst, vtBoolean, tp), value(val) {}
    VariantType getType() { return vtBoolean; }
    string ToString()
    {
        return value ? "true" : "false";
    }
};
typedef CBoolConstToken* CBoolConstTokenPtr;

class CIOModule {
private:
    ifstream input;
    string buffer;
    int cur_ch;
    int cur_line = 0;
public:
    CIOModule(string filename) : input(filename) {}
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
        cout << "****ERROR " << error << "****   line: " << setw(3) << tp->getLineNumber() << "   symbol position: " << setw(3) << tp->getChNumber() << endl;
    }
    ~CIOModule()
    {
        //cout << endl << "deleting io...";
        input.close();
    }
};

class CLexer {
private:
    CIOModule* io;
    char cur_ch;
    CTextPositionPtr cur_tp;
public:
    CLexer(CIOModule* io) : io(io) {}
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

map<string, vector<CTokenPtr>> idents;

class CSyntax {
private:
    CIOModule* io;
    CLexer* lexer;
    CTokenPtr curToken;
public:
    CSyntax(CIOModule* io, CLexer* lexer) : io(io), lexer(lexer) {}
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
            else
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
        else
            curToken = lexer->getNextToken();
        /*if (curToken != nullptr)
            cout << curToken->ToString() << endl;*/
    }
    void var_declaration(vector<int> followers)
    {
        if (symbol() != ttIdent)
        {
            io->WriteError(ttIdent, curToken->getTextPosition());
            auto starters = followers; starters.push_back(ttIdent);
            skip_to(starters);
        }
        if (symbol() == ttIdent)
        {
            vector<CTokenPtr> new_idents;
            new_idents.push_back(curToken);

            accept(ttIdent);
            if (curToken->getType() == ttKeyword)
            {
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                while (kw->getCode() == commaSy)
                {
                    accept(commaSy);

                    if (curToken->getType() == ttIdent)
                        new_idents.push_back(curToken);
                    accept(ttIdent);

                    if (curToken->getType() == ttKeyword)
                        kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                    else
                        break;
                }
            }
            accept(colonSy);

            VariantType type = vtUndefined;

            if (curToken->getType() == ttIdent)
            {
                auto idnt = dynamic_cast<CIdentTokenPtr>(curToken);
                if (is_type(idnt->ToString()))
                    type = types[idnt->ToString()];
                else
                    //тип не существует
                    io->WriteError(10, curToken->getTextPosition());
            }
            accept(ttIdent);

            for (int i = 0; i < new_idents.size(); i++)
            {
                new_idents[i]->setVarType(type);
                string idnt = new_idents[i]->ToString();
                //повторное объявление
                if (exists_ident(idnt) && !exists_token(new_idents[i]))
                    io->WriteError(101, new_idents[i]->getTextPosition());

                idents[idnt].push_back(new_idents[i]);
            }

            skip_to_followers(followers);
        }
    }
    void var_part(vector<int> followers)
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
                var_declaration(var_followers);
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
    VariantType factor(vector<int> followers)
    {
        VariantType type = vtUndefined;

        vector<int> starters = { ttConst, ttIdent, leftBrSy };
        if (!belong(symbol(), starters))
        {
            io->WriteError(1337, curToken->getTextPosition());
            skip_to(starters, followers);
        }
        if (belong(symbol(), starters))
        {
            switch (curToken->getType())
            {
            case ttConst:
                type = curToken->getVarType();
                accept(ttConst);
                break;
            case ttIdent:                
                //если операнд не существует, добавляем в ТИ
                if (!exists_ident(curToken->ToString()))
                {
                    io->WriteError(104, curToken->getTextPosition());
                    idents[curToken->ToString()].push_back(curToken);
                }
                //берем тип первого вхождения идентификатора
                type = idents[curToken->ToString()][0]->getVarType();

                accept(ttIdent);
                break;
            case ttKeyword:
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                if (kw->getCode() == leftBrSy)
                {
                    accept(leftBrSy);
                    vector<int> expr_followers = followers; expr_followers.push_back(rightBrSy);
                    type = expr(expr_followers);
                    accept(rightBrSy);
                }
                break;
            }
            skip_to_followers(followers);
        }
        return type;
    }
    VariantType term(vector<int> followers)
    {
        vector<int> factor_followers = followers; factor_followers.insert(factor_followers.end(), { multSy, subSy, divSy, modSy, andSy });
        VariantType type = factor(factor_followers);
        if (curToken->getType() == ttKeyword && is_mult_op())
        {
            while (curToken->getType() == ttKeyword && is_mult_op())
            {
                curToken = lexer->getNextToken();
                VariantType type2 = factor(factor_followers);

                if (type2 == vtUndefined || type == vtUndefined)
                    type = vtUndefined;
                else if (type2 != type)
                {
                    type = vtUndefined;
                    //конфликт типов
                    io->WriteError(145, curToken->getTextPosition());
                }
            }
        }
        skip_to_followers(followers);

        return type;
    }
    VariantType simple_expr(vector<int> followers)
    {
        vector<int> term_followers = followers; term_followers.insert(term_followers.end(), { plusSy, minusSy, orSy });
        VariantType type = term(term_followers);
        if (curToken->getType() == ttKeyword && is_add_op())
        {
            while (curToken->getType() == ttKeyword && is_add_op())
            {
                curToken = lexer->getNextToken();
                VariantType type2 = term(term_followers);

                if (type2 == vtUndefined || type == vtUndefined)
                    type = vtUndefined;
                else if (type2 != type)
                {
                    type = vtUndefined;
                    //конфликт типов
                    io->WriteError(145, curToken->getTextPosition());
                }
            }
        }
        skip_to_followers(followers);
        return type;
    }
    bool is_relation_op()
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == evenSy || kw->getCode() == lessSy
            || kw->getCode() == lessEvenSy || kw->getCode() == moreSy
            || kw->getCode() == moreEvenSy || kw->getCode() == notEvenSy;
    }
    VariantType expr(vector<int> followers)
    {
        vector<int> se_followers = followers; se_followers.insert(se_followers.end(), { evenSy, lessSy, lessEvenSy, moreSy, moreEvenSy, notEvenSy });
        VariantType type = simple_expr(se_followers);
        if (curToken->getType() == ttKeyword && is_relation_op())
        {
            curToken = lexer->getNextToken();
            VariantType type2 = simple_expr(followers);

            if (type2 == vtUndefined || type == vtUndefined)
                type = vtUndefined;
            else if (type2 != type)
            {
                type = vtUndefined;
                //конфликт типов
                io->WriteError(145, curToken->getTextPosition());
            }
            else
                type = vtBoolean;
        }
        skip_to_followers(followers);
        return type;
    }
    void assign_statement(vector<int> followers)
    {
        CTokenPtr cur_token = curToken;
        //если переменная не существует, добавляем в ТИ
        if (cur_token->getType() == ttIdent)
        {
            if (!exists_ident(cur_token->ToString()))
            {
                io->WriteError(104, cur_token->getTextPosition());
                idents[cur_token->ToString()].push_back(cur_token);
            }
        }
        accept(ttIdent);

        accept(assignSy);
        //auto curTp = curToken->getTextPosition();
        VariantType type2 = expr(followers);

        //получаем тип необъявленного идентификатора из контекста
        if (idents[cur_token->ToString()][0]->getVarType() == vtUndefined)
            idents[curToken->ToString()][0]->setVarType(type2);

        //конфликт типов
        if (idents[cur_token->ToString()][0]->getVarType() != vtUndefined && type2 != vtUndefined 
            && idents[cur_token->ToString()][0]->getVarType() != type2)
        {
            cur_token->setVarType(type2);
            //если в ТИ не занесен вариант идентификатора с данным ошибочным типом
            if (!exists_token(cur_token))
            {
                io->WriteError(145, cur_token->getTextPosition());
                idents[cur_token->ToString()].push_back(cur_token);
            }
        }
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
                assign_statement(followers);
                break;
            case ttKeyword:
                auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                switch (kw->getCode())
                {
                case beginSy:
                    composite_statement(followers);
                    break;
                case ifSy:
                    if_statement(followers);
                    break;
                case whileSy:
                    while_statement(followers);
                    break;
                }
                break;
            }
            skip_to_followers(followers);
        }
    }
    void composite_statement(vector<int> followers)
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
    void while_statement(vector<int> followers)
    {
        accept(whileSy);
        vector<int> expr_followers = followers; expr_followers.insert(expr_followers.end(), { doSy, ttIdent, beginSy, ifSy, whileSy });
        
        auto curTp = curToken->getTextPosition();
        VariantType type = expr(expr_followers);

        //неверный тип выражения
        if (type != vtBoolean)
            io->WriteError(144, curTp);

        accept(doSy);
        statement(followers);
        skip_to_followers(followers);
    }
    void if_statement(vector<int> followers)
    {
        accept(ifSy);
        vector<int> expr_followers = followers; expr_followers.insert(expr_followers.end(), { thenSy, ttIdent, beginSy, ifSy, whileSy });
       
        auto curTp = curToken->getTextPosition();
        VariantType type = expr(expr_followers);

        //неверный тип выражения
        if (type != vtBoolean)
            io->WriteError(144, curTp);

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
    void statement_part(vector<int> followers)
    {
        composite_statement(followers);
        skip_to_followers(followers);
    }
    void block(vector<int> followers)
    {
        auto var_followers = followers;
        var_followers.push_back(beginSy);
        var_part(var_followers);
        statement_part(followers);
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

    //____________________________________семантика_____________________________________________________________________ 
    bool exists_ident(string ident)
    {
        return idents.find(ident) != idents.end();
    }
    bool exists_token(CTokenPtr token)
    {
        auto idnt = idents[token->ToString()];
        for (int i = 0; i < idnt.size(); i++)
        {            
            if (token->getVarType() == idnt[i]->getVarType())
                return true;
        }
        return false;
    }
    bool is_type(string ident)
    {
        return types.find(ident) != types.end();
    }
    bool is_logical(KeyWords op)
    {
        return op == andSy || op == orSy;
    }
};

class CCompiler
{
private:
    ifstream input;
    CIOModule* io;
    CLexer* lexer;
    CSyntax* syntax;
public:
    CCompiler(string filename)
    {
        io = new CIOModule(filename);
        lexer = new CLexer(io);
        syntax = new CSyntax(io, lexer);
    }
    void compile()
    {
        syntax->program();
    }
    ~CCompiler()
    {
        //cout << endl << "deleting compiler...";
        delete syntax;
        delete lexer;
        delete io;
    }
};

int main() {
    /*CTokenPtr token = nullptr;
    ofstream fout("output.txt");
    while (token = lexer->getNextToken())
    {
        fout << token->ToString() << "\t";
        switch (token->getType())
        {
        case ttConst:
            fout << "const" << endl;
            break;
        case ttIdent:
            fout << "ident" << endl;
            break;
        case ttKeyword:
            auto kw = dynamic_cast<CKeywordTokenPtr>(token);
            fout << (kw->getCode()) << endl;
            break;
        }
    }
    fout.close();*/

    auto compiler = new CCompiler(filename);
    compiler->compile();

    cout << endl << "d o n e";

    delete compiler;
}