a. no for example if  x = 0x80000000 or T_min,  and y = 0xFFFFFFFF or -1 then -x = x and -y = 1 so T_min is clearly not greater than 1

b. yes right shifting both is equivalent to multiplying by 16  then adding 1 y and subtracting 1 x gives 17 y + 15 x

c. ~x + ~y + 1 = -x -1 -  y -1 + 1 =  -(x+y) -1 = ~(x+y) so it is true

d. true because the negative operator is not affected by the signed or unsignedness of the int it is acting on and casting to unsigned will result in it being the same in the end

e. True because >> 2 divides by two always rounding down both positive and negative so multiplying by 4 afterwards will always again result in a less or equal number

