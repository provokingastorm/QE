// Copyright (C) 2006 Joe Linhoff, All Rights Reserved
// e_diag.cpp
#include "e_config.h"
#if BUILD_DIAG // compile this app
#include "qe.h"

// diagnostic file-wide
typedef struct {
   float hFont;
   float wWindow;
   float hWindow;
   float wOrg;
   float hOrg;
   float xScale;
   float yScale;
   uns   keyBuf[32];
   chr   chrBuf[32];
   uns   sndClick;
} diagRec;

diagRec diag;

// sound definition
typedef struct {
   chr *name;
   uns flags;
   uns t;
   chr *path;
} sndDefRec;

// table of sounds
sndDefRec snd[] = 
{
   {"fall", 0x0f0|M_SNDNEW_LOOPING,0,"sounds/fall.wav"},
   {"lightning-near",  0x1c0,0,"sounds/lightning_near.wav"},
   {"thunder",0x0f0,0,"sounds/thunder.wav"},
   {"rain",  0x200|M_SNDNEW_LOOPING,0,"sounds/rain.wav"},
   {"stopall",0x000|M_SNDNEW_PREDEFINED,0,"stopall"},
   {0,},
}; // snd[]

#define DIAG_COLOR_A 0.5,0.2,0.5
#define DIAG_COLOR_B 0.2,0.2,1

#define XX(_x_) ((xx+(_x_))*diag.xScale)
#define YY(_y_) ((yy+(_y_))*diag.yScale)

// JFL 24 Sep 06
int fncDiagMode(void)
{
   float xx=0,yy=0;

   // get window size
   if(qeGetWindowSize(&diag.wWindow,&diag.hWindow))
      diag.wWindow=diag.hWindow=0;
   diag.xScale=diag.wWindow/diag.wOrg;
   diag.yScale=diag.hWindow/diag.hOrg;

   qefnSysCamBitmap();

   qeFontRGB(DIAG_COLOR_A);
   qeFontPos(XX(10),YY(2*diag.hFont));
   qeFontPrintf("Diagnostic Screen (Joe Linhoff)");

   return 0; // function must return a value, 0 indicates success
} // fncDiagMode()

// JFL 24 Sep 06
void diagRect(float x1,float y1,float x2,float y2)
{
   glBegin(GL_QUADS);
   glVertex2f(x1,y1);
   glVertex2f(x1,y2);
   glVertex2f(x2,y2);
   glVertex2f(x2,y1);
   glEnd();
} // diagRect()

// JFL 24 Sep 06
int fncDiagSystem(void)
{
   float xx=120,yy=40;

   // draw info
   qeFontRGB(DIAG_COLOR_A);
   qeFontPos(XX(84),YY(diag.hFont));
   qeFontPrintf("%s\nqe version %s\nOpenGL version %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));
   qeFontPrintf("time:%f\n",qeTimeFrame());

   return 0; // function must return a value, 0 indicates success
} // fncDiagSystem()

// JFL 24 Sep 06
int fncDiagKeyboard(void)
{
   float xx=120,yy=120;
   uns bt4[4],b_start,b_x,b_y,b_a,b_b;
   chr buf[32];
   int i,j,k,top;

   bt4[0]=qeInpButton(QEINPBUTTON_UP);
   bt4[1]=qeInpButton(QEINPBUTTON_DOWN);
   bt4[2]=qeInpButton(QEINPBUTTON_LEFT);
   bt4[3]=qeInpButton(QEINPBUTTON_RIGHT);
   b_start=qeInpButton(QEINPBUTTON_START);
   b_x=qeInpButton(QEINPBUTTON_X);
   b_y=qeInpButton(QEINPBUTTON_Y);
   b_a=qeInpButton(QEINPBUTTON_A);
   b_b=qeInpButton(QEINPBUTTON_B);

   // copy keybuf into keychrs
   buf[0]=0;
   if((i=qeInpOp(QEINPOP_CPYCHRBUF,buf,sizeof(buf)))>0)
   { // new chrs
      top=NUM(diag.chrBuf)-1;
      if(i>top)
         i=top; // more than fit, replace the whole buffer
      else
      {
         // make room for the new chrs in the buffer
         for(j=0;j<top-i;j++)
            diag.chrBuf[j]=diag.chrBuf[j+i];
      }

      // copy the new chrs into the buffer
      for(k=0,j=top-i;j<top;j++,k++)
         diag.chrBuf[j]=buf[k];
   } // new chrs

   glColor3f(DIAG_COLOR_B);

   // draw left button
   i = (bt4[2]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(70),YY(24),XX(70+20),YY(24+20));

   // draw down button
   i = (bt4[1]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(90),YY(44),XX(90+20),YY(44+20));

   // draw right button
   i = (bt4[3]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(110),YY(24),XX(110+20),YY(24+20));

   // draw up button
   i = (bt4[0]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(90),YY(4),XX(90+20),YY(4+20));

   // draw info
   qeFontPos(XX(144),YY(0));
   qeFontPrintf("KEYBOARD\nup:%d\ndown:%d\nleft:%d\nright:%d\nstart:%d\n" \
      "x:%d y:%d a:%d b:%d\n",
      bt4[0],bt4[1],bt4[2],bt4[3],b_start,b_x,b_y,b_a,b_b);
   qeFontPrintf("keys:'%s'\n",diag.chrBuf);

   return 0; // function must return a value, 0 indicates success
} // fncDiagKeyboard()

// JFL 24 Sep 06
int fncDiagMouse(void)
{
   float xx=10,yy=50;
   uns bt3[3];
   int xyw[3];
   int i;

   // get button counts
   bt3[0]=qeInpButton(QEINPBUTTON_MOUSELB);
   bt3[1]=qeInpButton(QEINPBUTTON_MOUSECB);
   bt3[2]=qeInpButton(QEINPBUTTON_MOUSERB);

   // get positions
   qeInpMouseXY(xyw);
   xyw[2]=qeInpMouseWheel();

   // draw mouse
   glPolygonMode(GL_FRONT,GL_LINE); // RB56, draw wireframe polygons
   glColor3f(DIAG_COLOR_B);
   diagRect(XX(0),YY(0),XX(70),YY(110));

   // draw left button
   i = (bt3[0]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(2),YY(2),XX(2+20),YY(30));

   // draw center button
   i = (bt3[1]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(24),YY(2),XX(24+20),YY(30));

   // draw right button
   i = (bt3[2]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(46),YY(2),XX(48+20),YY(30));

   // draw info
   qeFontPos(XX(84),YY(0));
   qeFontPrintf("MOUSE\nlmb:%d\ncmb:%d\nrmb:%d\nx:%d\ny:%d\nwheel:%d",
      bt3[0],bt3[1],bt3[2],xyw[0],xyw[1],xyw[2]);

   return 0; // function must return a value, 0 indicates success
} // fncDiagMouse()

#define DPADX 80

// JFL 24 Sep 06
// JFL 24 Feb 07; sticknum
int drawJoystick(int sticknum,int xx,int yy)
{
   uns numjoy,numax,numbt,btval[10],dpad[4];
   int axval[6];
   int i;
   float x,y,a;

   // note: zero values will be returned for non-existent joystick info

   // get info
   numjoy=qeInpJoyNum();
   qeInpJoyInfo(sticknum,&numax,&numbt);

   // get button counts
   btval[0]=qeInpJoyButton(sticknum,QEJOYBUTTON_X);
   btval[1]=qeInpJoyButton(sticknum,QEJOYBUTTON_A);
   btval[2]=qeInpJoyButton(sticknum,QEJOYBUTTON_B);
   btval[3]=qeInpJoyButton(sticknum,QEJOYBUTTON_Y);
   btval[4]=qeInpJoyButton(sticknum,QEJOYBUTTON_LSHOULDER);
   btval[5]=qeInpJoyButton(sticknum,QEJOYBUTTON_RSHOULDER);
   btval[6]=qeInpJoyButton(sticknum,QEJOYBUTTON_START);
   btval[7]=qeInpJoyButton(sticknum,QEJOYBUTTON_BACK);
   btval[8]=qeInpJoyButton(sticknum,QEJOYBUTTON_LTHUMB);
   btval[9]=qeInpJoyButton(sticknum,QEJOYBUTTON_RTHUMB);

   // get positions
   axval[0]=qeInpJoyAxis(sticknum,QEJOYAXIS_LEFT_X);
   axval[1]=qeInpJoyAxis(sticknum,QEJOYAXIS_LEFT_Y);
   axval[2]=qeInpJoyAxis(sticknum,QEJOYAXIS_RIGHT_X);
   axval[3]=qeInpJoyAxis(sticknum,QEJOYAXIS_RIGHT_Y);
   axval[4]=qeInpJoyAxis(sticknum,QEJOYAXIS_LEFT_TRIGGER);
   axval[5]=qeInpJoyAxis(sticknum,QEJOYAXIS_RIGHT_TRIGGER);

   // get dpad
   dpad[0]=qeInpJoyButton(sticknum,QEJOYBUTTON_DPAD_LEFT);
   dpad[1]=qeInpJoyButton(sticknum,QEJOYBUTTON_DPAD_DOWN);
   dpad[2]=qeInpJoyButton(sticknum,QEJOYBUTTON_DPAD_RIGHT);
   dpad[3]=qeInpJoyButton(sticknum,QEJOYBUTTON_DPAD_UP);

   // draw joystick
   glPolygonMode(GL_FRONT,GL_LINE); // RB56, draw wireframe polygons
   glColor3f(DIAG_COLOR_B);
   diagRect(XX(0),YY(0),XX(148),YY(144));

   // -- buttons --

   // draw button left X
   i = (btval[0]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(84),YY(24),XX(84+20),YY(24+20));

   // draw button up A
   i = (btval[1]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(104),YY(44),XX(104+20),YY(44+20));

   // draw button right B
   i = (btval[2]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(124),YY(24),XX(124+20),YY(24+20));

   // draw button down Y
   i = (btval[3]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(104),YY(4),XX(104+20),YY(4+20));

   // -- dpad --

   // draw button left
   i = (dpad[0]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(84-DPADX),YY(24),XX(84+20-DPADX),YY(24+20));

   // draw button down
   i = (dpad[1]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(104-DPADX),YY(44),XX(104+20-DPADX),YY(44+20));

   // draw button right
   i = (dpad[2]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(124-DPADX),YY(24),XX(124+20-DPADX),YY(24+20));

   // draw button up
   i = (dpad[3]&1) ? GL_FILL : GL_LINE;
   glPolygonMode(GL_FRONT,i);
   diagRect(XX(104-DPADX),YY(4),XX(104+20-DPADX),YY(4+20));

   // -- sticks --

   // draw stick 0
   a=60; // width/height of stick area
   glPolygonMode(GL_FRONT,GL_LINE);
   diagRect(XX(4),YY(80),XX(4+a),YY(80+a));
   x=(float)axval[0]/(float)QEINP_JOYAXIS_MAX;
   y=(float)axval[1]/(float)QEINP_JOYAXIS_MAX;
   x*=a/2; // scale
   y*=a/2; // scale
   x+=4+a/2; // center
   y+=80+a/2; // center
   x-=8/2; // draw width
   y-=8/2; // draw height
   glPolygonMode(GL_FRONT,GL_FILL);
   diagRect(XX(x),YY(y),XX(x+8),YY(y+8));

   // draw stick 1
   a=60; // width/height of stick area
   glPolygonMode(GL_FRONT,GL_LINE);
   diagRect(XX(84),YY(80),XX(84+a),YY(80+a));
   x=(float)axval[2]/(float)QEINP_JOYAXIS_MAX;
   y=(float)axval[3]/(float)QEINP_JOYAXIS_MAX;
   x*=a/2; // scale
   y*=a/2; // scale
   x+=84+a/2; // center
   y+=80+a/2; // center
   x-=8/2; // draw width
   y-=8/2; // draw height
   glPolygonMode(GL_FRONT,GL_FILL);
   diagRect(XX(x),YY(y),XX(x+8),YY(y+8));

   // draw info
   qeFontPos(XX(170),YY(0));
   qeFontPrintf("JOYSTICK\nbt:%d,%d,%d,%d\nshoulder:%d,%d\n" \
      "ax0:%d\nax1:%d\nax2:%d\nax3:%d\ntrig:%d,%d\nstart,back;%d,%d\nthumb:%d,%d",
      btval[0],btval[1],btval[2],btval[3],btval[4],btval[5],
      axval[0],axval[1],axval[2],axval[3],
      axval[4],axval[5],
      btval[6],btval[7],
      btval[8],btval[9]);

   return 0; // function must return a value, 0 indicates success
} // drawJoystick()

// JFL 24 Feb 07
int fncDiagJoystick(void)
{
   drawJoystick(0,10,230);
   drawJoystick(1,10,390);
   return 0;
} // fncDiagJoystick()

// JFL 24 Sep 06
int fncDiagSounds(void)
{
   float xx=160,yy=230;
   float x1,x2,y1,y2,dy;
   sndDefRec *sdr;
   uns b,id;
   int xy[2];

   b=qeInpButton(QEINPBUTTON_MOUSELB);
   if((b&1)&&(diag.sndClick!=b))
   {
      diag.sndClick=b;
      qeInpMouseXY(xy);
   }
   else
      b=0;

   qeFontPos(XX(170),YY(diag.hFont));
   qeFontPrintf("SOUNDS:\n");

   dy=YY(2*diag.hFont)-YY(diag.hFont);
   x1=XX(170);
   y1=YY(diag.hFont)+dy;
   x2=XX(170+80);
   y2=y1+dy;

   for(sdr=snd;sdr->name;sdr++)
   {
      qeFontPos(x1,y1);
      qeFontPrintf("%s f:0x%x t:%d\n",sdr->name,sdr->flags,sdr->t);

      y1-=dy;
      y2-=dy;

      if(b)
      {
         if((xy[0]<x1)||(xy[0]>x2)||(xy[1]<y1)||(xy[1]>y2))
            ;
         else
         {
            id=qeSndPlay(sdr->name);
            qeSndMasterVol(100);
         }
      }

      y1+=2*dy;
      y2+=2*dy;

   } // for

   return 0; // function must return a value, 0 indicates success
} // fncDiagSounds()

// qeMain()
// JFL 05 Aug 06
int qeMain(int argc,chr *argv[])
{
   int r,i;
   sndDefRec *sdr;
   chr *s1;

   MEMZ(diag);

   // setup diag
   diag.hFont=qeFontHeight();
   diag.wOrg=800; // original layout width
   diag.hOrg=600; // original layout height
   qeInpKeyBuf(diag.keyBuf,sizeof(diag.keyBuf));

   // fill chrBuf
   for(i=0;i<sizeof(diag.chrBuf);i++)
      diag.chrBuf[i]=' '; // spaces initially
   diag.chrBuf[sizeof(diag.chrBuf)-1]=0;

   // print information
   qePrintf("%s / qe version %s / OpenGL version %s\n",
      __FILE__,qeVersion(),glGetString(GL_VERSION));

   // add a function to the engine's object loop
   if(!qeObjAddFnc(fncDiagMode))
      BRK();
   if(!qeObjAddFnc(fncDiagSystem))
      BRK();
   if(!qeObjAddFnc(fncDiagKeyboard))
      BRK();
   if(!qeObjAddFnc(fncDiagMouse))
      BRK();
   if(!qeObjAddFnc(fncDiagJoystick))
      BRK();
   if(!qeObjAddFnc(fncDiagSounds))
      BRK();

   // create sounds
   for(sdr=snd;sdr->name;sdr++)
   {
      if(qeSndNew(sdr->name,sdr->flags,sdr->t,sdr->path)<0)
         qePrintf("** qeSndNew %s failed\n",sdr->name);
   } // for

   // turn control over to qe until the user closes the program
   qeForever();

   while((r=qeErrNext())<0)
   {
      s1=qeErrString(r);
      qePrintf("qeMainErr %s\n",s1);
      BRK();
   } // while

   return 0;
} // qeMain()

#endif // compile this app
// EOF