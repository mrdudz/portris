0 rem * cbm uniloader v0.1
1 l$="portris-"
2 de=8

10 gosub 1000:rem check videoram
11 gosub 2000:rem check basic start
12 gosub 3000:rem check free basic memory

20 print chr$(147);"detected:";

30 if(bs=2049)and(sm=1024)and(fm<=38911) then 400 : rem c64
31 if(bs=4097)and(sm=3072) then print "c16 or plus4"
32 if(bs=1025)and(sm=32768)and(fm<=31743) then 700: rem pet
33 if(bs=4097)and(sm=7680)and(fm<=3583) then 650 : rem unexpanded vic20
34 if(bs=1025)and(sm=7680)and(fm<=6655) then 640 : rem expanded vic20 (+3k)
35 if(bs=4609)and(sm=4096)and(fm<=11775) then 600 : rem expanded vic20 (+8k)
36 if(bs=4609)and(sm=4096)and(fm<=19967) then 610 : rem expanded vic20 (+16k)
37 if(bs=4609)and(sm=4096)and(fm<=28159) then 620 : rem expanded vic20 (+24k)
38 if(bs=7169)and(sm=1024)and(fm<=58111) then 500: rem c128
39 if(sm=53248)and(fm<=11775)then 850: rem cbm610 * bs=768 ?!
40 if(sm=53248)and(fm<=64345) then 800: rem cbm510 * bs=768 ?!

340 print "unknown"
345 print "bs:";bs;"sm:";sm;"fm:";fm
349 end

350 rem *** load file
351 print:print"loading ";de;":";l$;"..."
352 load l$,de,1

400 rem *** c64
410 print "c64"
411 de=peek(186)
420 print:print"select:":print"1) 40cols":print"2) 80cols":print"3) 80cols (mono)"
422 poke198,0:wait198,1:get a$:if a$="2" then l$=l$+"c64-80":goto 350
423 if a$="3" then l$=l$+"c64-80m":goto 350
424 if a$<>"1" then 422
425 l$=l$+"c64":goto 350

500 rem *** c128
510 mo=peek(215):rem 40 or 80 columns
520 l$=l$+"c128":print"c128, ";
530 if mo and 128 then 550:print "80 columns (vdc)":goto550rem 80 cols if bit7 set
540 print "40 columns"

550 print:print"select:":print"1) 40cols":print"2) 80cols (vdc)"
552 get a$:if a$="2" then l$=l$+"vdc":goto 350
553 if a$="1" then 350
554 goto 552

600 rem *** expanded vic20
601 poke646,1:print "expanded vic20 (+8k)"
602 goto 699

610 rem *** expanded vic20
611 print "expanded vic20 (+16k)"
612 goto 699

620 rem *** expanded vic20
621 print "expanded vic20 (+24k)"
622 l$=l$+"vic20"
623 print:print"select:":print"1) normal":print"2) 26x25cols":print"3) 40x25cols"
624 poke198,0:wait198,1:get a$:if a$="2" then l$=l$+"-26x25":goto 350
625 if a$="3" then l$=l$+"-40x25":goto 350
626 if a$="1" then 350
627 goto 624

640 rem *** expanded vic20 (+3k)
641 print "expanded vic20 (+3k)"
642 goto 699

650 rem *** unexpanded vic20
660 print "unexpanded vic20"

699 print "error: not enough memory (need 24k)":end

700 rem *** pet
710 print "pet"
720 l$=l$+"pet":goto350

800 rem *** cbm 510
810 print "cbm2/cbm510"
820 l$=l$+"cbm510":goto350

850 rem *** cbm 610
860 print "cbm2/cbm610"
870 l$=l$+"cbm610":goto350

900 rem *** pet
910 print "pet"
920 l$=l$+"pet":goto350

1000 rem ** check screenmem **
1010 sm=0:for i = 1 to 6:read s
1030 print chr$(147);"a":b=peek(s)
1050 print chr$(147);:c=peek(s)
1080 if(b=1)and(c=32)then sm=s
1090 next:return

2000 rem ** check basic start **
2010 bs=0:for i = 1 to 6:read bp,bv
2030 b=(peek(bp+1)*256+peek(bp))
2080 if(b=bv)then bs=b
2090 next:return

3000 rem ** check mem free **
3010 fm=0:for i = 1 to 9:read f
3080 if(fre(0)<f) then fm=f
3090 next:return

10000 rem videoram address
10010 data 1024  : rem c64,c128
10020 data 3072  : rem c16,plus4
10030 data 32768  : rem pet
10040 data 53248  : rem cbm510
10050 data 4096  : rem expanded vic
10060 data 7680  : rem unexpanded vic

20000 rem basic-ram start pointer,value
20010 data 43,2049 : rem c64
20020 data 43,4609 : rem expanded vic20
20030 data 43,1025 : rem expanded vic20 (+3k)
20040 data 43,4097 : rem c16,plus4,unexpanded vic20
20050 data 40,1025 : rem pet
20060 data 45,7169 : rem c128
20070 rem data 45,768 : rem cbm2/cbm610,cbm2/cbm510 ?!

30000 rem free basic mem
30010 data 64345 : rem cbm2/cbm510
30020 data 58111 : rem c128
30030 data 38911 : rem c64
30041 data 31743 : rem pet
30052 data 28159 : rem expanded vic20 (+24k)
30063 data 19967 : rem expanded vic20 (+16k)
30074 data 11775 : rem expanded vic20 (+8k), cbm610 ?!
30085 data 6655 : rem expanded vic20 (+3k)
30096 data 3583 : rem unexpanded vic20
