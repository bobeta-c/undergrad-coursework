#include "grid.h"
#include <cassert>
#include <iostream>
using namespace std;
#include <cctype>
bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);
bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg);
void plotRectangle(int r, int c, int height, int width, char ch);

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);
bool getAndMove(string commandString, int& i, int& badPos, int& value);


const int ROWS = 20;
const int COLUMNS = 30;

const int HORIZ = 0;
const int VERT = 1;

const int FG = 0;
const int BG = 1;

const string QUIT = "";
int main()
    {
        setSize(12, 15);
        assert(plotLine(3, 5, 2, HORIZ, '@', FG));
        for (int c = 5; c <= 7; c++)
            assert(getChar(3, c) == '@');
        assert(getChar(3, 8) == ' ');
        clearGrid();
        char pc = '%';
        int m = FG;
        int bad = 999;
          // A successful command string should not change bad
        assert(performCommands("V2", pc, m, bad) == 0  &&  getChar(3, 1) == '%'  &&  bad == 999);
        assert(performCommands("H4V3V-1H-9", pc, m, bad) == 3  &&  bad == 7);
	char epc = '\n';
	int em = 10;
	assert(performCommands("h1", epc, m, bad) == 2); // bad plotChar
	assert(performCommands("h1", pc, em, bad) == 2); //bad mode
	assert(performCommands("h5v3fnh-5bov-3c", pc, m, bad) == 0 && pc == '*'); //normal test case that invokes all possible commands normally
	assert(performCommands("", pc, m, bad) == 0); // empty string is syntactically valid
	assert(performCommands("V86F", pc, m, bad) == 1 && bad == 4); //precedence in error returns
	assert(performCommands("h11v99h99", pc, m, bad) == 3 && bad == 3);//precedence in out of bounds error
	assert(performCommands("Cb\t", pc, m, bad) == 1 && bad == 2); // non printable character
	assert(performCommands("F#H+25H?V3!", pc, m, bad) == 1 && bad == 3); //leftmost syntax error
	assert(performCommands("B@H", pc, m, bad) == 1 && bad == 3);//expecting - or digit after H
	assert(performCommands("C12", pc, m, bad) == 1 && bad == 1);//C is one command; 1 can't start a command
	assert(performCommands("Q3V4#", pc, m, bad) == 1 && bad == 0); //Q is not a command
	assert(performCommands("V03c H123#", pc, m, bad) == 1 && bad == 4);//space can't start a command
	assert(performCommands("H18H-123#", pc, m, bad) == 1 && bad == 7); //H-12 is one command; 3 can't start a command
	assert(performCommands("H5H-1-2", pc, m, bad) == 1 && bad == 5); //H-1 is one command; - can't start a command
	assert(performCommands("fh8", pc, m, bad) == 1 && bad == 2); //fh is one command; 8 can't start a command
	assert(performCommands("H3V99CF\t", pc, m, bad ) == 1 && bad == 7);	
		
        cout << "All tests succeeded." << endl;
	assert(performCommands("CCCV", pc, m, bad) == 1 && bad == 4);
	assert(performCommands("CCCV-", pc, m, bad) == 1 && bad == 5);

    }

bool getAndMove(string commandString, int& i, int& badPos, int& value){
	int negate = 1;
	int d1 = -1, d2 = -1;
	int total = 0;
	// inside box
	if(i>= commandString.size()){
		badPos = commandString.size();
		return false;
	}
	// negative?
	if(commandString.at(i) == '-'){
		negate = -1;
		i++;
		if(i>= commandString.size()){
			badPos = commandString.size();
			return false;
		}
	}
	// numbers?
	if((isdigit(commandString.at(i)))){
		d1 = commandString.at(i)-'0';
		i++;
		if(i < commandString.size() && isdigit(commandString.at(i))){
			d2 = commandString.at(i)-'0';
			i++;
		}
	}
	else{
		badPos = i;
		return false;
	}
	//compute value
	if(d2 != -1){
		total += d2;
		total += 10*d1;
	}
	else{
		total += d1;
	}
	value = total*negate;
	return true;

	
}
int performCommands(string commandString, char& plotChar, int& mode, int& badPos){
	//setup initialization values
	int staticBadPos = badPos;
	int returnValue = 0;
	//check to make sure the plotChar and mode are correct
	if(!isprint(plotChar) || (mode != FG && mode != BG)){
		return 2;
	}
	int tempBadPos;
	bool commandDidntWork = false;
	int tracker = 0;
	int r = 1, c = 1;
	for(int i = 0; i < commandString.size();){
		int value;
		//horizontal line case
		if(toupper(commandString.at(i)) == 'H'){
			tracker = i;
			i++;
			//get the numbers
			if(!getAndMove(commandString, i, badPos, value)){
				badPos = i;
				return 1;
			}
			//exucte successfully?
			if(plotLine(r,c,value,HORIZ,plotChar,mode)){
				c = c+value;
			}
			//if not keep track of it for later
			else{
				if(!commandDidntWork){
					tempBadPos = tracker;
					commandDidntWork = true;
				}
				returnValue =  3;

			}
		}
		//vertical line case
		else if(toupper(commandString.at(i)) == 'V'){
			tracker = i;
			i++;
			//get the numbers
			if(!getAndMove(commandString, i, badPos, value)){
				badPos = i;
				returnValue =  1;
			}
			//execute successfully?
			if(plotLine(r,c,value,VERT,plotChar,mode)){
				r = r+value;
			}
			//if not keep track of it for later
			else{
				if(!commandDidntWork){
					tempBadPos = tracker;
					commandDidntWork = true;
				}
				returnValue =  3;
			}
		}
		//foreground case
		else if(toupper(commandString.at(i)) == 'F'){
			i++;
			//check for in string
			if(i >= commandString.size()){
				badPos = i;
				return 1;
			}
			//check for valid character
			if(!isprint(toupper(commandString.at(i)))){
				badPos = i;
				return 1;
			}
			mode = FG;
			plotChar = commandString.at(i);
			i++;
		}
		//background case
		else if(toupper(commandString.at(i)) == 'B'){
			i++;
			//check for in string
			if(i >= commandString.size()){
				badPos = i;
				return 1;
			}
			//check for valid character
			if(!isprint(toupper(commandString.at(i)))){
				badPos = i;
				return 1;
			}
			mode = BG;
			plotChar = commandString.at(i);
			i++;
		}
		//clear case
		else if(toupper(commandString.at(i)) == 'C'){
			i++;
			clearGrid();
			r = 1;
			c = 1;
			plotChar = '*';
			mode = FG;
		}
		//error case
		else{
			badPos = i;
			return 1;
		}
	}
	if(commandDidntWork && badPos == staticBadPos ){
		badPos = tempBadPos;
	}		
	return returnValue;

	
}
bool plotHorizontalLine(int r, int c, int distance, char ch, int fgbg){
	bool passed = true;
	if(r<1 || r > getRows()) return false;
	for(int i = 0; i <= distance; i++){
		if((c+i)>getCols()){
			passed = false;
			break;
		}
		if((c+i)<1){
			passed = false;
			continue;
		}
		if(fgbg == BG){
			if(getChar(r,c+i) == ' ')
				setChar(r,c+i,ch);
		}
		else{
			setChar(r,c+i,ch);
		}
	}
	return passed;
}

bool plotVerticalLine(int r, int c, int distance, char ch, int fgbg){
	bool passed = true;
	if(c<1 || c > getCols()){
		passed = false;       
		return passed;
	}
	for(int i = 0; i <= distance; i++){
		if((r+i)>getRows()){
			passed = false;
			break;
		}
		if((r+i)<1){
			passed = false;
			continue;
		}
		if(fgbg == BG){
			if(getChar(r+i,c) == ' ')
				setChar(r+i,c,ch);
		}
		else
			setChar(r+i,c,ch);
	}
	return passed;
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg){
	//check that the line is inside of the box
	if((dir != HORIZ && dir != VERT) || (fgbg != FG && fgbg!= BG) || !isprint(plotChar))
	{
		return false;
	}
	if(r < 1 || r > getRows() || c < 1 || c > getCols())
	{
		return false;
	}
	if((dir == HORIZ && (c+distance < 1 || c+distance > getCols())) || (dir == VERT && (r+distance < 1 || r+distance > getRows())))
	{	
		return false;
	}
	//horizontal case
	if(dir == HORIZ){
		if(distance < 0){
			plotHorizontalLine(r,c+distance,-distance,plotChar,fgbg);
		}
		else{
			plotHorizontalLine(r,c,distance,plotChar,fgbg);
		}
	}
	//vertical case
	else{
		if(distance < 0){
			plotVerticalLine(r+distance,c,-distance,plotChar,fgbg);
		}
		else{
			plotVerticalLine(r,c,distance,plotChar,fgbg);
		}
	}
	return true;
}
