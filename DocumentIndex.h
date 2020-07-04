#ifndef	DocumentIndex_h
#define	DocumentIndex_h

#include	<fstream>
#include	<map>
#include	<set>
#include	<string>
#include    <sstream>
#include    <vector>

using namespace std;
typedef	string::size_type	StringSize;

class	DocumentFile
{
	public:
		DocumentFile() : pageNumber_(1) { }
		void	Close();
		int		GetPageNumber();			//	Returns the current pge number.
		string	GetWord();					//	Returns the next legal word not on the exception list;
											//	returns an empty string if there are no more words in
											//	the line.
		bool	LoadExclusions(const string& name);	//	Loads a list of words to be excluded from the index
													//	from a file of the given name.
		bool	Open(const string& name);	//	Opens a document file of the given name.
		bool	Read();						//	Reads the next line of the document file, skipping over
											//	the double empty lines that mark page separations.
											//	Returns false if there are no more lines in the file.
		
	private:
		StringSize	beginPosition_;
		fstream		file_;
		int			pageNumber_;
		string		text_;
		vector<string> currentLine;         //strings that make up current line
		int currentWord = 0;                //index of current word in current line/resets to 0 with read()
		vector<string> excludedWords;       //collection of "bad words"
		bool excludedWord(string str);      //checks if word is on list of excluded   
};

struct pageAndCases                        //struct stores page numbers and number of times a word appears total
{										   //map will store this along with word key
	set<int> pages;
	int numOfCases = 0;
};

class	DocumentIndex
{
	public:
		void	Create(DocumentFile& documentFile);	//	Creates an index for the given document file.
		void	Show();
		void	Write(ostream& indexFile);	//	Writes the index to the given file.
											//	The argument is a stream so that this function
											//	can be called to write its output to cout for
											//	test purposes.
	private:
		void purgeExcessiveWords();         //gets rid of index word with 11 or more occurences
		map<string, pageAndCases> index;
};

#endif
