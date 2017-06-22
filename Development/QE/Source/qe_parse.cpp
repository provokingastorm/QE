// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// e_parse.cpp
#include "e_config.h"
#if BUILD_PARSE // compile this app
#include "qe.h"

#include <stdio.h>

// define our own sz flags -- the top nibble is reserved for system use
#define M_SZF_RESERVED 0xf0000000 // top nibble reserved for system use
#define M_SZF_ALPHA    0x00000001
#define M_SZF_NUM      0x00000002
#define M_SZF_WHITE    0x00000004
#define M_SZF_ID1      0x00000008
uns32 szFlagTable[128]; // sz flag table (assumed to cover ASCII codes)


int SetupFlagTable(void)
{
    uns j;
    chr * whitespace;

    // build the flag table
    // zero the table memory, then add in for each set
    MEMZ(szFlagTable);
    szFlagTable[0] = NUM(szFlagTable); // first is num
    for(j='a';j<='z';j++)
        szFlagTable[j]|=M_SZF_ALPHA|M_SZF_ID1;
    for(j='A';j<='Z';j++)
        szFlagTable[j]|=M_SZF_ALPHA|M_SZF_ID1;
    for(j='0';j<='9';j++)
        szFlagTable[j]|=M_SZF_NUM|M_SZF_ID1;
    whitespace=" \t\n\r";
    while((j = *whitespace++))
        szFlagTable[j]|=M_SZF_WHITE;

    return 0;
} 

enum {
   GAMETOK_NONE,
   GAMETOK_HELLO,
   GAMETOK_SET,
   GAMETOK_EQUALS,
};

chr *gameTokTable[] =
{
   "", // GAMETOK_NONE
   "hello", // GAMETOK_HELLO
   "set", // GAMETOK_SET
   "=", // GAMETOK_EQUALS
   NUL // term
}; // gameTokTable[]


int ParseBuffer(chr *buf,chr *bufx)
{
   int r,i1,tok;
   chr *s1,*s2,*n1,*n2;

   for(s1=buf;s1 && *s1;s1=s2)
   {
      // tokenize
      s1=szftill(szFlagTable,s1,bufx,M_SZF_ID1); // first id
      s2=szfskip(szFlagTable,s1,bufx,M_SZF_ID1); // first non-id
      tok=szimap(gameTokTable,s1,s2); // map to token
      qePrintf("token '%S' tok=%d ",s1,s2,tok);

      // handle tokens
      switch(tok)
      {

      case GAMETOK_HELLO:
         qePrintf("*Hello!* ");
         break;
      case GAMETOK_SET: // set NAME VAL
         s1=s2; // advance

         // get variable name & whitespace-separated value
         n1=szftill(szFlagTable,s1,bufx,M_SZF_ID1); // first id
         n2=szfskip(szFlagTable,n1,bufx,M_SZF_ID1); // first non-id
         s1=szfskip(szFlagTable,n2,bufx,M_SZF_WHITE); // skip whitespace
         s2=sztobin(s1,bufx,10,'i',&i1); // convert to int
         qePrintf("*%S=%d* ",n1,n2,i1);
         break;

      case ERR_SZ_UNDEF: // NAME=VAL
         n1=s1; // name start
         n2=s2; // name end
         s1=s2; // advance

         s1=szfskip(szFlagTable,s1,bufx,M_SZF_WHITE); // skip whitespace
         if(*s1!='=')
            break; // not an equals token, bail out
         s1=szfskip(szFlagTable,1+s1,bufx,M_SZF_WHITE); // skip equals & whitespace
         s2=sztobin(s1,bufx,10,'i',&i1); // convert to int
         qePrintf("*%S=%d* ",n1,n2,i1);

         break;      } // switch

      qePrintf("\n");
   } // for
   
   r=0;
//BAIL:
   return r;
} // gameParseBuf()


int ParseFile(chr *filePath)
{
   int returnValue;
   FILE *fileHandle = NUL;
   chr *buffer = NUL,*s1;
   long fileSize;

   // open file, find size
   fileHandle = fopen(filePath,"rb");
   if(!fileHandle)
   {
       returnValue = -1;
       goto BAIL;
   }
   if(fseek(fileHandle,0,SEEK_END))
   {
       returnValue = -2;
       goto BAIL;
   }
   fileSize = ftell(fileHandle);
   if(fileSize < 0)
   {
       returnValue = -3;
       goto BAIL;
   }
   if(fseek(fileHandle,0,SEEK_SET))
   {
       returnValue = -4;
       goto BAIL;
   }

   // alloc buffer & read file in
   returnValue = MemAlloc(&buffer,1+fileSize);
   if(returnValue < 0) // alloc +1 for terminating zero
      goto BAIL;
   if(fread(buffer,1,fileSize,fileHandle)!=fileSize)
   {
       returnValue = -6;
       goto BAIL;
   }
   buffer[fileSize] = 0; // terminating zero
   fclose(fileHandle);
   fileHandle = NUL;

   // find last occurrence of '[' and parse
   s1=szrchr(buffer,&buffer[fileSize],'[');
   if((returnValue=ParseBuffer(s1,&buffer[fileSize]))<0)
      goto BAIL;

   returnValue = 0;
BAIL:
   if(buffer) // free buf if allocated
      MemFree(buffer);
   if(fileHandle) // close all files if opened
      fclose(fileHandle);
   return returnValue;
} // gameParseFile()





int qeMain(int argc,chr *argv[])
{
   qePrintf("%s / %s / %s\n",__FILE__,qeVersion(),glGetString(GL_VERSION));
   qePrintf("[Hello, this is a test: set var1 123, var2=234, var3=345]\n");
   SetupFlagTable();
   ParseFile("qelog.txt");

   // turn control over to the engine until the user closes the program
   qeForever();

   return 0;
} 

#endif
