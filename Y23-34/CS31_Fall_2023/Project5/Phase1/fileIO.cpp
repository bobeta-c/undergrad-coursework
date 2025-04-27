#include <iostream>
#include <fstream>
using namespace std;

void greet(ostream& outf)    // outf is a name of our choosing
{
    outf << "Hello" << endl;
}

int countLines(istream& inf)   // inf is a name of our choosing
{
    int lineCount = 0;
    string line;
    while (getline(inf, line))
	    lineCount++;
    return lineCount;
}

int main()
{
    ifstream infile("test.txt");
    if ( ! infile )
    {
	cerr << "Error: Cannot open data.txt!" << endl;
	return 1;
    }
    int fileLines = countLines(infile);  // reads from the file data.txt
    cout << "data.txt has " << fileLines << " lines." << endl;
    cout << "Type lines, then ctrl-Z (Windows) or ctrl-D (macOS/Linux):" << endl;
    int keyboardLines = countLines(cin); // reads from keyboard
    cout << "You typed " << keyboardLines << " lines." << endl;
}
