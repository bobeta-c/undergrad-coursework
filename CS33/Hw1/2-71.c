// the problem with this code is that it only works for positive bytes - as masking with 0xFF
// extracts only the byte but does not convert it to a signed byte

int xbyte(packed_t word, int bytenum)
{
       int x = word;
       return (x << ((3-bytenum) << 3)) >> 24;
}
