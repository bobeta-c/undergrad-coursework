#include <stdio.h>

int main(void){
    int x = 0x80000000; //T-MIN (-x) == T-MIN
    int y = 0xffffffff; // -1
    int z = 0x00000001; //  1


    printf("%d\n", x<y); //1
    printf("%d\n", (-x) > (-y)); //0
    printf("%d, %d, %d, %d\n", x, y, -x, -y);// T-Min , -1, T-min, 1
    printf("%d - %d \n", -2147483648 > 1, -2147483648); //0, T-min
    printf("%d\n", (-x) > z); //0
    printf("%d %d\n", (-y) == z, (-x) > (-y));//1 0
}
