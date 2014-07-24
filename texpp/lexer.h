/*  This file is part of texpp library.
    Copyright (C) 2009 Vladimir Kuznetsov <ks.vladimir@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __TEXPP_LEXER_H
#define __TEXPP_LEXER_H

#include <texpp/common.h>
#include <texpp/token.h>

#include <istream>

namespace texpp {

class Lexer;
class Context;

class Lexer
{
public:
    Lexer(const string& fileName, std::istream* file,
                bool interactive = false, bool saveLines = false);
    Lexer(const string& fileName, shared_ptr<std::istream> file,
                bool interactive = false, bool saveLines = false);
    ~Lexer();

    // return next Token from tex
    Token::ptr nextToken();

    bool interactive() const { return m_interactive; }

    string jobName() const;
    const string& fileName() const { return *m_fileName; }
    shared_ptr<string> fileNamePtr() const { return m_fileName; }

    size_t linePos() const { return m_linePos; }
    size_t lineNo() const { return m_lineNo; }
    const string& line() const { return m_lineOrig; }
    const string& line(size_t n) const;

    int endlinechar() const { return m_endlinechar; }
    void setEndlinechar(int endlinechar) { m_endlinechar = endlinechar; }

    int getCatCode(int ch) const { return m_catCodeTable[ch]; }
    void assignCatCode(int ch, int code) { m_catCodeTable[ch] = code; }

protected:
    void init();

    Token::ptr newToken(Token::Type type,
                    const string& value = string());

    bool nextLine();
    bool nextChar();

protected:
    enum State {
        ST_EOF = 0,         // end of file
        ST_EOL = 1,         // end of line
        ST_NEW_LINE = 2,    // new line
        ST_SKIP_SPACES = 3, // skip spaces
        ST_MIDDLE = 4       // middle of line
    };

    shared_ptr<std::istream> m_fileShared;

    std::istream*   m_file;     // .tex file source
    shared_ptr<string> m_fileName;

    string  m_lineOrig; // current line
    string  m_lineTex;

    size_t  m_linePos;
    size_t  m_lineNo;   // current line number
    size_t  m_charPos;  // actual position of next char in line
    size_t  m_charEnd;  // position of the last char in line

    State   m_state;    // processing state
    int     m_char;
    Token::CatCode m_catCode;

    int     m_endlinechar;
    short int     m_catCodeTable[256];  // character-code–category-code pairs <char, CatCode>

    bool    m_interactive;
    bool    m_saveLines;

    vector<string> m_lines; // massive of already cinsidered text lines
};

} // namespace

#endif

