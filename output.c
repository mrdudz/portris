
/* output a string centered in the given line */
void __fastcall__ cputsy_c(unsigned char y, const char *ptr)
{
    unsigned char x = 0;
    unsigned char c;

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
void __fastcall__ cputs_c(const char *ptr)
{
    cputsy_c(wherey(), ptr);
}




 
