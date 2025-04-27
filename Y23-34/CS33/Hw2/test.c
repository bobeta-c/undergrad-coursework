 long loop(long x, long n)
 {
 long result = 0;
 long mask;
 for (mask = 1; mask != 0; mask = mask << n){
 result |= x&mask;
}
return result;
}


int main(void){
    return 1;
}

