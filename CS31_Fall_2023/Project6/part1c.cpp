#include <iostream>
using namespace std;
    void computeCube(int n, int* ncubed)
    {
        *ncubed = n * n * n;
    }

    int main()
    {
	int num;
        computeCube(5, &num);
        cout << "Five cubed is " << num << endl;
    }

