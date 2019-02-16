#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>     //memset

//#define TIMEOUT_DELAY   250
#define TIMEOUT_BLOCK   -1
//#define KEY_BACKSPACE   127
//#define KEY_ENTER       10
//#define KEY_CR          13

typedef unsigned char byte;
typedef unsigned short word;
//typedef byte BOOL;

#define TRUE 1
#define FALSE 0

//#define STATUS_X    20
//#define STATUS_Y    0
#define MOVE_X      0
#define MOVE_Y      13


byte aa[100];
byte xx[5] = {0,1,9,10,11};
byte openingMove[] = {34,35,45,46,47,54,55,56,57,66};

byte g=0;   //last player move?
byte k=0;
byte e=0;
byte a=0;
byte n=0;
byte z=0;
byte l=0;
byte t=0;
byte h1=0;
byte x=0;
byte m=0;

byte empty = '.';
byte human = 'H';
byte computer = 'C';

byte playing = TRUE;

int rnd(int min,int max);
void humanMove();
void computerMove();
void showBoard();

void initSystemSupport() {
#ifdef _COCO_BASIC_
    initCoCoSupport();
#else
    initscr();
    timeout(TIMEOUT_BLOCK);
#endif
}

void deinitSystemSupport() {
#ifndef _COCO_BASIC_
    endwin();
#endif
}

int readNumber() {
    int ch = 0;
    while(ch < '0' || ch > '9') {
        ch = getch();
    }

    return ch-'0';
}

word readword() {
    byte high = readNumber();
    byte low = readNumber();
    return high*10+low;
}

byte inkey() {return (byte)getch();}

void locate(int x,int y) {
    move(y,x);
}

void cls(int color) {
    clear();
}

//Returns a number between min and max inclusive.
//If min=1 and max=10 numbers returned would be 1 through 10
int rnd(int min,int max)
{
    int n = rand()%(max+1-min)+min;
    return n;
}

//in 40 & 80 column mode, makes the cursor visible again.
void showCursor()
{
//    for(byte i=0; i<sizeof cursoraddr; i++)
//    {
//        byte* addr = cursoraddr[i];
//        *addr = 4;
//    }
}

void hideCursor()
{
//    for(byte i=0; i<sizeof cursoraddr; i++)
//    {
//        byte* addr = cursoraddr[i];
//        *addr = 0;
//    }
}

//void cursorBlinkRate(byte rate)
//{
//    byte* addr = 0xF78C;
//    *addr = rate;
//}
//
//void resetCursorBlinkRate()
//{
//    byte* addr = 0xF787;
//    *addr = 10;
//    *(addr+1) = 148;
//}
//
void init()
{
    cls(1);
    memset(aa,0,100);

    for(byte c=1; c<=8; c++)
    {
        for(byte b=2; b<=9; b++)
        {
            aa[c*10+b]=empty;
        }
    }

    printw("YOU WANT THE FIRST MOVE? (Y OR N) ");
    unsigned seed=1;
    byte ch = inkey();
    while(ch != 'Y' && ch != 'N' && ch != 'y' && ch != 'n')
    {
        ++seed;
        ch = inkey();
    }
    //srand(seed);

    //make the computers first move if user doesn't want first move
    if('Y' != ch && 'y' != ch)
    {
        aa[openingMove[rnd(0,9)]] = computer;
    }
}

void humanMove()
{
    showCursor();
    g = 0;
    while(g<12 || g > 89)
    {
        locate(MOVE_X,MOVE_Y);
        printw("YOUR MOVE? ");
        g = (byte)readword();
        g++;
        if(g < 12 || g > 89 || aa[g] != empty)
        {
            locate(MOVE_X,MOVE_Y+1);
            printw("INVALID MOVE\n");
        }
    }
    z = human;
    aa[g] = human;
    hideCursor();
}

void countSequence()
{
    //printw("CS start a=%d n=%d z=%d k=%d\n",a,n,z,k);
    e = a;
    while(1)
    {
        e += n;
        if(aa[e] != z)
        {
//            locate(20,15); printw("K=%d",k);
//            locate(20,16); printw("E=%d",e);
            break;
        }
        k++;
    }
}

//g is the last move the human made
void computerMove()
{
    a=g;
    l=0;
    locate(MOVE_X,MOVE_Y);
    printw("MY MOVE...       ");
    for(byte x=1; x<=4; x++)
    {
        k = 0;
        n = xx[x];
        countSequence();
        n = -n;
        countSequence();
        if(k>l)
            l=k;
    }

    if(l>3)
    {
        printw("YOU WIN\n");
        playing = FALSE;
        return;
    }

    t=1;
    while(t != 4)
    {
        if(t != 2)
            z = computer;
        if(t == 2)
            z = human;

        g = 0;
        h1 = 0;
        l = 0;
        for(a=12; a<=89; a++)
        {
            m = 0;
            if(aa[a] == 46)     //460 IF A(A)<>46 THEN 570
            {
                for(x=1; x<=4; x++)
                {
                    k = 0;
                    n = xx[x];
                    countSequence();
                    n = -n;
                    countSequence();
                    if(k > l)
                    {
                        h1 = 0;
                        l = k;
                    }
                    if(l != k)
                        continue;   //next loop iteration
                    if(t==1 && l<4 || (t==2 || t==3) && l<2)
                        continue;   //next loop iteration
                    m++;
                }
                if(m > h1)  //550 IF M<=H1 THEN 570
                {
                    h1 = m;
                    g = a;
                }
            }
        }
        if(h1 != 0)
            break;
        t++;    //590 T=T+1 : IF T<>4 THEN 410 'this plus the while loop is the same logic
    }

    if(h1 == 0)
    {
//        printw("random move\n");
        a = 1;
        do
        {
            g = (byte)rnd(13,77); //610 G=INT(RND*77)+13
            a++;
            if(a > 100)
            {
                printw("I CONCEDE THE GAME\n");
                playing = FALSE;
                return;
            }
        } while(aa[g] != 46);
    }

    aa[g] = computer;
    z = computer;
    a = g;
    l = 0;
    for(x=1; x<=4; x++)
    {
        k = 0;
        n = xx[x];
        countSequence();
        n = -n;
        countSequence();
        if(k > l)
            l = k;
    }
}

void showBoard()
{
    locate(0,0);
    printw("GOMOKU C\n\n");
    printw("  1 2 3 4 5 6 7 8\n");
    for(byte a=1; a<=8; a++)
    {
        printw("%d ",a);
        for(byte b=2; b<=9; b++)
        {
            byte n = aa[a*10+b];
            printw("%c ",n);
        }
        printw("%d\n",a);
    }
    printw("  1 2 3 4 5 6 7 8\n");
}


int main() {
    initSystemSupport();

    init();
    while(playing)
    {
        showBoard();
        humanMove();
        if(!playing)
            break;
        showBoard();
        computerMove();
        if(l>3)
        {
            showBoard();
            printw("I WIN\n");
            playing = FALSE;
            getch();
            break;
        }
    }

    deinitSystemSupport();
    return 0;
}