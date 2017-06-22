// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// e_scantags.cpp
#include "e_config.h"
#if BUILD_SCANTAGS // compile this app
#include "qe.h"
#include "qebase.h"

///////////////////////////////////////////////////////////////////////////////

// define our own sz flags -- the top nibble is reserved for system use
#define M_SZF_RESERVED 0xf0000000 // top nibble reserved for system use
#define M_SZF_ALPHA    0x00000001
#define M_SZF_NUM      0x00000002
#define M_SZF_WHITE    0x00000004
#define M_SZF_ID1      0x00000008
uns32 szFlagTable[128]; // sz flag table (assumed to cover ASCII codes)

// JFL 04 Feb 07
int gameSetupSzFlagTable(void)
{
   uns j;
   chr *s1;

   MEMZ(szFlagTable);
   szFlagTable[0]=NUM(szFlagTable); // first is num
   for(j='a';j<='z';j++)
      szFlagTable[j]|=M_SZF_ALPHA|M_SZF_ID1;
   for(j='A';j<='Z';j++)
      szFlagTable[j]|=M_SZF_ALPHA|M_SZF_ID1;
   for(j='0';j<='9';j++)
      szFlagTable[j]|=M_SZF_NUM|M_SZF_ID1;
   s1=" \t\n\r";
   while((j=*s1++))
      szFlagTable[j]|=M_SZF_WHITE;

   return 0;
} // gameSetupSzFlagTable()

#define GAMETAG_START "</game"
#define GAMETAG_END "/>"

// JFL 04 Feb 07
int gameScanTags(void)
{
   int r;
   qeObjScan srmem,*sr=&srmem; // scan mem & pointer
   qeLibChunk libmem,*lib=&libmem; // libmem & pointer
   chr scanpath[512]; // used by scanner to build the object path
   qeLibTag1 *tag; // chunk pointer
   chr *s1,*s1x;

   // load library -- name the loaded lib 'env'
   if(qeLibNew("env",0,"libs/test.lib")<0)
      bret(-1);
   if(qeLibLoad("env")<0)
      bret(-2);

   // scan the library, and create an object to display each element
   MEMZ(*sr); // zero scan record memory
   sr->buf=scanpath; // buffer for scanner
   sr->bufSize=sizeof(scanpath); // size of buffer

   // run through library chunks in the env lib
   while(qeLibScanNext(sr,lib,"env/*",NUL)>=0)
   {
      qePrintf("LIB %C %s %s\n",lib->type,lib->name,scanpath);
      switch(lib->type)
      {
      case C4TYPE_TAG1: // this chunk is used to build environments
         // get record for this library node
         tag=(qeLibTag1*)lib->data; // get pointer to data

         // find first substring that starts with GAMETAG_START
         if(!(s1=szsub(GAMETAG_START,NUL,tag->tags,NUL)))
            break; // not found, break

         // find string inside the brackets
         s1+=NUM(GAMETAG_START); // skip past GAMETAG_START
         s1x=szsub(GAMETAG_END,NUL,s1,NUL); // find GAMETAG_END
         s1=szfskip(szFlagTable,s1,s1x,M_SZF_WHITE); // skip whitespace

         // print tag info (%S uses string start & end pointers)
         qePrintf("TAG1 for name=%s field is '%S'\n",lib->name,s1,s1x);

      } // switch
   } // while

   r=0;
BAIL:
   return r;
} // gameScanTags()

// qeMain()
// JFL 05 Jan 07
int qeMain(int argc,chr *argv[])
{
   int r;

   // print information
   qePrintf("%s / %s / %s\n",__FILE__,glGetString(GL_VERSION),qeVersion());

   // setup flag table for program and scan tags
   gameSetupSzFlagTable();
   gameScanTags();

   // turn control over to the engine until the user closes the program
   qeForever();

   // print errors
   while((r=qeErrNext())<0)
      qePrintf("qeMainErr2 %s\n",qeErrString(r));

   // check for unbalanced allocations
   if(MemAllocCount())
      BRK();

   return 0;
} // qeMain()

#endif // compile this app
// EOF