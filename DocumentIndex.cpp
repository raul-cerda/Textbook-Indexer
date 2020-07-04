#include	<fstream>
#include	<iostream>
#include	<string>
#include    <sstream>
#include    <vector>

// #include	<unistd.h>

#include	"DocumentIndex.h"
#include	"GetLine.h"

using namespace std;
typedef	string::size_type	StringSize;

void DocumentFile::Close()
{
	file_.close();
	file_.clear();
	return;
}

int	DocumentFile::GetPageNumber()
{
	return(pageNumber_);
}

bool DocumentFile::excludedWord(string str)
{
	for (uint64_t i = 0; i < excludedWords.size(); i++)
	{
		if (str == excludedWords[i])
			return true;
	}
	return false;
}

//grabs word from currentLine vector, removes punctuation and checks if valid
string DocumentFile::GetWord()
{
	string word = "";
	for (uint64_t i = currentWord; i < currentLine.size(); i++)
	{
		currentWord++;
		word = currentLine[i];

		if (word.find("0") != string::npos || word.find("1") != string::npos || word.find("2") != string::npos
			|| word.find("3") != string::npos || word.find("4") != string::npos || word.find("5") != string::npos
			|| word.find("6") != string::npos || word.find("7") != string::npos || word.find("8") != string::npos
			|| word.find("9") != string::npos || word.find("-") != string::npos || word.find(".txt") != string::npos
			|| word.find(".h") != string::npos || word.find(".cpp") != string::npos || word.find(".pdf") != string::npos
			|| word.find(".com") != string::npos || word.find("/") != string ::npos)
		{
		}
		else{
			//removes any valid puncuation
			if (word.find("\"") != string::npos)
				word.replace(word.find("\""), 1, "");
			if (word.find("\"") != string::npos)
				word.replace(word.find("\""), 1, "");
			if (word.find(".") != string::npos)
				word.replace(word.find("."), 1, "");
			if (word.find(",") != string::npos)
				word.replace(word.find(","), 1, "");
			if (word.find(":") != string::npos)
				word.replace(word.find(":"), 1, "");
			if (word.find(";") != string::npos)
				word.replace(word.find(";"), 1, "");
			if (word.find("?") != string::npos)
				word.replace(word.find("?"), 1, "");
			if (word.find("!") != string::npos)
				word.replace(word.find("!"), 1, "");
			if (word.find("'s") != string::npos)
				word.replace(word.find("'s"), 2, "");
			if (word.find("(") != string::npos)
				word.replace(word.find("("), 1, "");
			if (word.find(")") != string::npos)
				word.replace(word.find(")"), 1, "");
			if (word.find("'") != string::npos)
				word.replace(word.find("'"), 1, "");
			if (!excludedWord(word))
				return word;
		}
	}
	return "";
}


bool DocumentFile::LoadExclusions(const string& name)
{
	fstream exclusionFile;
	exclusionFile.open(name);
	string temp;
	while (exclusionFile >> temp)
	{
		excludedWords.push_back(temp);
	}
	return true;
}

bool DocumentFile::Open(const string& name)
{
	file_.open(name, ios::in);
	if (!file_.fail())
	{
		//	You may add any useful initialization here.

		return(true);
	}
	else
	{
		return(false);
	}
}

//checks for double return page change/ writes current line to vector of words
bool DocumentFile::Read()
{
	if (!GetLine(file_, text_))
		return false;
	else
	{
		if (text_ == "")
		{
			GetLine(file_, text_);
			if (text_ == "")
			{
				pageNumber_++;
				GetLine(file_, text_);
			}
		}
		stringstream stram(text_);
		string temp;
		while (stram >> temp)
		{
			currentLine.push_back(temp);
		}
	}
	return true;
}

//inserts word to index, if duplicate, searches to see if duplicate page numbers
void DocumentIndex::Create(DocumentFile& documentFile)
{
	string word;

	while (documentFile.Read())
	{
		while (true)
		{
			word = documentFile.GetWord();
			if (word == "")
				break;
			else
			{
				map<string, pageAndCases>::iterator iter = index.find(word);
				if (iter != index.end())        //if word found
				{
					if (iter->second.pages.find(documentFile.GetPageNumber()) == iter->second.pages.end()) //if new pg #
						iter->second.pages.insert(documentFile.GetPageNumber());                           //add new pg
					iter->second.numOfCases++;
				}
				else          //word not in map
				{
					pageAndCases wordInfo;
					wordInfo.numOfCases = 1;
					wordInfo.pages.insert(documentFile.GetPageNumber());
					index.insert(pair<string, pageAndCases>(word, wordInfo));
				}
			}
		}
	}
	purgeExcessiveWords();
}

void DocumentIndex::purgeExcessiveWords()
{
	map<string, pageAndCases>::iterator iter = index.begin();
	while (iter != index.end())
	{
		if (iter->second.numOfCases > 10)
			iter = index.erase(iter);
		else
			++iter;
	}
}

void DocumentIndex::Write(ostream& indexStream)
{
	for (map<string, pageAndCases>::iterator iter = index.begin(); iter != index.end(); ++iter)
	{
		indexStream << iter->first << " ";
		set<int>::iterator iter2 = iter->second.pages.begin();
		indexStream << *iter2;
		iter2++;
		for (iter2; iter2 != iter->second.pages.end(); ++iter2)
			indexStream << ", " << *iter2;
		indexStream << endl;
	}
}

void DocumentIndex::Show()
{
	for (map<string, pageAndCases>::iterator iter = index.begin(); iter != index.end(); ++iter)
	{
		cout << iter->first << " ";
		set<int>::iterator iter2 = iter->second.pages.begin();
		cout << *iter2;
		iter2++;
		for (iter2; iter2 != iter->second.pages.end(); ++iter2)
			cout << ", " << *iter2;
		cout << endl;
	}
}