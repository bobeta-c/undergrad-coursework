#include <fstream>
#include <cstring>
#include <iostream>
#include <cctype>
#include <sstream>
#include <streambuf>
#include <cassert>


using namespace std;

int render(int lineLength, istream& inf, ostream& outf);
int getWord(istream& inf, char* buffer, int size, bool& overflowed, bool& ImSoPissedThatThisHasToBeAVariable);


void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}
int main()
{
	const int MAX_FILENAME_LENGTH = 100;
	testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
        testRender(8, "  This is a test  \n", "This is\na test\n", 0);
        testRender(6, "Testing it\n", "Testin\ng it\n", 1);
        testRender(-5, "irrelevant", "irrelevant", 2);
        cerr << "Tests complete" << endl;
	for (;;)
	{
	    cout << "Enter input file name (or q to quit): ";
	    char filename[MAX_FILENAME_LENGTH];
	    cin.getline(filename, MAX_FILENAME_LENGTH);
	    if (strcmp(filename, "q") == 0)
		break;
	    ifstream infile(filename);
	    if (!infile)
	    {
		cerr << "Cannot open " << filename << "!" << endl;
		continue;
	    }
	    cout << "Enter maximum line length: ";
	    int len;
	    cin >> len;
	    cin.ignore(10000, '\n');
	    ofstream outf("output.txt");
	    if( ! outf ){
		    cerr << "Error: Cannot create output.txt" << endl;
		    return 1;
            }
	    int returnCode = render(len, infile, cout);
	    cout << "Return code is " << returnCode << endl;
	}
}



int render(int lineLength, istream& inf, ostream& outf){
	if (lineLength < 1){ //Check to verify appropriate input length
		return 2;
	
	}
	int returnState = 0; //Keep track of line overflows

	int bufferSize = lineLength+1; //our function will need to get the largest of an output line or the next word
	char buffer[251]; //dynamically allocate memory accounting for the largest possible word while at least being large enough to store @P@X
	if(buffer == nullptr) {
		return 3;//aditional error outside of spec
	}
	buffer[0]='\0'; //make the string a valid array

	const char pmarks[4] = {'.','!','?',':'}; // function declarations first define what punctuation is
	bool needToParagraph = false; //record if we need to make a paragraph before outputting text
	int currentLength = 0; // keep track of the length of our line to make sure it does not overflow
	int wordLength = 0; //keep track of how long our word is
	bool punctuation = false; // keep track of whether we need to ouput two spaces after punctuation
	bool hyphen = false; // keep track of whether we need to output no spaces after a hyphen
	bool stupidVariable = false; // a really stupid variable - check to see if paragraph is necessary
	int currentLine = 0; //make sure we do not paragraph before the first line.
	for(;;){
		bool overflowed = false;
		wordLength = getWord(inf, buffer, bufferSize, overflowed, stupidVariable); // get the next word segment also gain information if the word was cut off also get information that in the special case where the c string can only fit 3 characters, stupidVariable keeps track of whether the @P@ recorded is an @P@ or an @P@ followed by another character
		if(wordLength == 0){ //end of file
			break;
		}

	//cerr << wordLength << " : " << currentLength << " : \"" << buffer << '\"' << strcmp(buffer,"@P@") << " : " << currentLine << endl;
		if(overflowed){ // check if the word is longer than line length
			returnState = 1;
		}
		if(!hyphen && (strcmp(buffer, "@P@") == 0) && (currentLine != 0 || currentLength != 0) && !stupidVariable ){ //check for paragraph
			needToParagraph = true;
			continue;
		}
		else if (!hyphen && (strcmp(buffer, "@P@") == 0 && !stupidVariable )){ //Make sure to still not pring @P@ if it is leading the input file (the first word I get from getWord())
				continue;
		}
		currentLength += wordLength; //update current line-length

		if(currentLength <= lineLength){//If the word fits in the line
			if(needToParagraph){ // Do a paragraph
				outf << endl << endl;
				currentLength = wordLength;
			}
			if((currentLength != wordLength && hyphen)){ //if following hyphen just print
					outf << buffer;
			}
			else if((currentLength != wordLength) && !punctuation){ // regular word
				if(currentLength+1 <= lineLength){ // word fits in line
					outf << ' ' << buffer;
					currentLength++;
				}
				else{ // word doesn't fit in line - Kind of redundant because this same block is reused in the other cases of not fitting - would be better to have a check at the beginning but wtv
					outf << endl;
					currentLine++;
					currentLength = wordLength;
					outf << buffer;
				}
			}
			else if(currentLength != wordLength){ // regular word fits in line but is following a punctuation
				if(currentLength+2 <= lineLength){ // fits in line
					outf << "  " << buffer;
					currentLength+=2;
				}
				else{ // doesn't fit in line more redundancy
					outf << endl;
					currentLine++;
					currentLength = wordLength;
					outf << buffer;
				}
			}
			else{// first word of the line no need to check
				outf << buffer;
			}

		}
		else{//line is too long with the word attached
			if(needToParagraph){//is it time to paragraph?
				outf << endl;
			}
			outf << endl;
			currentLine++;
			currentLength = wordLength;
			outf << buffer;
		}
		punctuation = false; //reset all of the flags that could be set
		needToParagraph = false;
		hyphen = false;
		stupidVariable = false;
		for(int j = 0; j < 4; j++){ // Check for Punctuation
			if(buffer[wordLength-1] == pmarks[j]){
				punctuation = true;
	//			cerr << buffer[wordLength-1] << " : " << pmarks[j] ;
			}
		}
		if(buffer[wordLength-1] == '-'){ // check for hyphen
			hyphen = true;
		}
	}
	outf << endl; //end the final output with a newline

	return returnState; // return the proper state
}
bool getchars(istream& inf, char* container, int chars){ // vestigial function - from a previous attempt - it hold sentimental value
	container[0] = '\0';
	for(int i = 0; i < chars-1; i++){
		if(!inf.get(container[i])){
			container[i+1] = '\0';
			return false;
		}
	}
	container[chars-1] = '\0';
	return true;
}

int getWord(istream& inf, char* buffer, int size, bool& overflowed, bool& ImSoPissedThatThisHasToBeAVariable){
	overflowed = false; // record if the word is an entire word or just a segment of a large word
	char c;
	int i;
	ImSoPissedThatThisHasToBeAVariable = false; // a variable for the niche case where the buffer only fits 3 letters and the word is @P@X where x is any nonspace character
	bool brokeOut = false;// record if broke out of loop
	for(i = 0; i<(size-1); i++){
		if(!inf.get(c)){
			break;// EOF
		}
		if(size<5 && i == 0 && c == '@'){ //Check for paragraph in the case of small buffer
			if(!inf.get(c)){ // if EOF we put in the @ and leave it at that
				buffer[i] = '@';
				i=1;
				break;
			}
			if(c == 'P'){//Spooky - we are approachin @P@ but not there yet
				if(!inf.get(c)){//At this point we know that the word is @P but it is easier to go back and parse as normal
					buffer[0] = '@';
					inf.seekg(-1, ios::cur);
					continue;
				}
				if(c== '@'){// we have @P@
					if(!inf.get(c)){ //We know that it is a paragraph break
						buffer[0] = '@'; buffer[1] = 'P'; buffer[2] = '@';
						i=3;
						break;
					}
					if(isspace(c)){ //We know that it is a paragraph break
						buffer[0] = '@'; buffer[1] = 'P'; buffer[2] = '@';
						i = 3;
						break;
					}
					else{// there is a letter after @P@ so it is not a paragraph break
						if(size == 4){
							ImSoPissedThatThisHasToBeAVariable = true; //keep in mind that this is not an actual paragraph break
						}
						buffer[0] = '@';
						inf.seekg(-3,ios::cur);
						continue;
					}
				}
				else{//we have @PX so treat it as a normal word
					buffer[0] = '@';
					inf.seekg(-2,ios::cur);
					continue;
				}
			}
			else{// We go back and treat it as a normal word
				buffer[0] = '@'; 
				inf.seekg(-1,ios::cur);
				continue;
			}
			
		}//end check
		if(isspace(c)){
			if(i == 0){ //if there is a space starting the word keep on looking for the first letter
				int length = strlen(buffer);
				if(length != 0 && buffer[length-1] == '-'){
					i++;
					buffer[0] = ' ';
					brokeOut = true;
					break;
				}
				i--;
				continue;
			}
			brokeOut = true;//the word is over
			break;
		}
		buffer[i] = c;//we know it is not a space so it is part of the word-portion
		if(c == '-'){//other way to end the word
			i++;
			brokeOut = true;
			break;
		}
	}
	buffer[i] = '\0';//make sure it is a valid C string
	if(brokeOut == false){
		overflowed = true;
	}
	return i;//return the length for easy calculations
}

