#include <iostream>
#include <string>
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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
          // Return true if there is a path from (sr,sc) to (er,ec)
          // through the maze; return false otherwise
{
	maze[sr][sc] = '#';
	if(sr == er && sc == ec){
		return true;
	}
	if(maze[sr-1][sc] == '.'){
		if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec)){
			return true;
		}
	}
	if(maze[sr][sc-1] == '.'){
		if(pathExists(maze,nRows,nCols,sr,sc-1,er,ec)){
			return true;
		}
	}
	if(maze[sr+1][sc] == '.'){
		if(pathExists(maze,nRows,nCols,sr+1,sc,er,ec)){
			return true;
		}
	}
	if(maze[sr][sc+1] == '.'){
		if(pathExists(maze,nRows,nCols,sr,sc+1,er,ec)){
			return true;
		}
	}
	return false;
}
