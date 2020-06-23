#ifdef PROTOTYPE
unsigned floatUnsigned2Float(unsigned);
unsigned test_floatUnsigned2Float(unsigned);
#endif
#ifdef DECL
 {"floatUnsigned2Float", (funct_t) floatUnsigned2Float, (funct_t) test_floatUnsigned2Float, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
#endif
#ifdef CODE
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatUnsigned2Float(unsigned u) {
#ifdef FIX
    unsigned exp = 127+31;
    unsigned residue;
    unsigned frac = 0;
    if (u == 0) {
	exp = 0;
	frac = 0;
    } else {
	/* Normalize so that msb = 1 */
	while ((u & (1<<31)) == 0) {
	    u = u << 1;
	    exp--;
	}
	/* Now have Bit 31 = MSB (becomes implied leading one)
	   Bits 8-30 are tentative fraction,
	   Bits 0-7 require rounding.
	*/
	residue = u & 0xFF;
	frac = (u >> 8) & 0x7FFFFF; /* 23 bits */
	if (residue > 0x80 || (residue == 0x80 && (frac & 0x1))) {
	    /* Round up */
	    frac ++;
	    /* Might need to renormalize */
	    if (frac > 0x7FFFFF) {
		frac = (frac & 0x7FFFFF) >> 1;
		exp++;
	    }
	}
    }
    return (exp << 23) | frac;
#else
    return 2;
#endif
}
#endif
#ifdef TEST
unsigned test_floatUnsigned2Float(unsigned u) {
    float f = (float) u;
    return f2u(f);
}
#endif
