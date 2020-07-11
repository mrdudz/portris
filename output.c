
/* output a string centered in the given line */
void cputsy_c(unsigned char y, register const char *ptr)
{
    register unsigned char x = 0;
    register unsigned char c;

    if((c = strlen(ptr)) < SCREENX) {
        x = (SCREENX - c) / 2;
    }
    gotoxy(x, y);

    while ((x < SCREENX)) {
        if(*ptr == 0) {
            break;
        }
        cputc(*ptr);
        ++ptr;
        ++x;
    }
}

/* output a string centered in the current line */
void cputs_c(register const char *ptr)
{
    cputsy_c(wherey(), ptr);
}




 
