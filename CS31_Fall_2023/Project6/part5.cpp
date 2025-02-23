#include <iostream>
using namespace std;

void removeS(char* c)
{
	char* p = c;
        while(*p != '\0'){
                if(*p == 's' || *p == 'S'){
                        while(*p != '\0'){
                                *p = *(p+1);
                                p++;
			}
			p=c;
                }
                p++;
        }
}

    int main()
    {	
        char msg[50] = "She'll blossom like a massless princess.";
        removeS(msg);
        cout << msg;  // prints   he'll bloom like a male prince.
    }
