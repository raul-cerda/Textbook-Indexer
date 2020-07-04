#ifndef GetLine_h
#define GetLine_h

#include	<iostream>
#include	<string>
using namespace std;


bool	GetLine(istream& stream, string& text);
bool	GetLine(istream& stream, string& text, const string& delimiter);

#endif
