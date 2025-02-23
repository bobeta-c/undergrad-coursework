#include <iostream>
#include <queue>
using namespace std;

class Coord
{
  public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
          // Return true if there is a path from (sr,sc) to (er,ec)
          // through the maze; return false otherwise

int main()
{
    string maze[10] = {
	"XXXXXXXXXX",
	"X........X",
	"XXXXX.X.XX",
	"X...X.X..X",
	"X.X...X..X",
	"X.XXX.XXXX",
	"XXX....X.X",
	"X.XX.XXX.X",
	"X....X...X",
	"XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 6,5, 1,8))
	cout << "Solvable!" << endl;
    else
	cout << "Out of luck!" << endl;
}


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
	queue<Coord> queueStack; //stack of coords
	queueStack.push(Coord(sr,sc));
	maze[sr][sc] = '#';

	while(!queueStack.empty())
	{
		Coord current = queueStack.front();
		queueStack.pop();
		int r = current.r();
		int c = current.c();
		if(r == er && c == ec){
			return true;
		}
		if(maze[r-1][c] == '.'){
			//cout << "(" << r-1 << ',' << c << ")" << endl;
			queueStack.push(Coord(r-1,c));
			maze[r-1][c] = '#';
		}
		if(maze[r][c-1] == '.'){
			queueStack.push(Coord(r,c-1));
			maze[r][c-1] = '#';
		}
		if(maze[r+1][c] == '.'){
			queueStack.push(Coord(r+1,c));
			maze[r+1][c] = '#';
		}
		if(maze[r][c+1] == '.'){ 
			queueStack.push(Coord(r,c+1));
			maze[r][c+1] = '#';
		}
	}
	return false;
}
