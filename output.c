void cputs_c(register const char *ptr)
{
    register unsigned char x=0;
    register unsigned char c;
    //int c;

//      gotoxy(x,wherey());
//     cputs(ptr);
//     return;

    if((c=strlen(ptr))<SCREENX)
    {
        x=(SCREENX-c)/2;
    }
     //cputs(ptr);
     gotoxy(x,wherey());
//    printf("(%d,%d-%d)",c,x,wherey());
//    printf("%d-",ptr[0]);
//    printf("%d-",ptr[1]);
//    printf("%d-",ptr[2]);
//    printf("%d-",ptr[3]);
//     return;
//    while ((x < SCREENX)&((c = *ptr) != '\0'))
    while ((x < SCREENX))
//    while ((x < SCREENX)/*&((c = *ptr) != '\0')*/)
    {
        //cputc (c);
        if(*ptr==0) break;
        cputc (*ptr);
        ++ptr;
        ++x;
    }
}

void cputsy_c(unsigned char y,register const char *ptr)
{
    register unsigned char x=0;
    register unsigned char c;

//      gotoxy(x,y);
//     cputs(ptr);
//     return;

    if((c=strlen(ptr))<SCREENX)
    {
        x=(SCREENX-c)/2;
    }
     gotoxy(x,y);
    while ((x < SCREENX)&((c = *ptr) != '\0')) {
        cputc (c);
        ++ptr;
        ++x;
    }
}



 
