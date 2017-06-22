// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qesnd.h
#ifndef QESND_H
#define QESND_H 1

// C in C++ wrapper
#ifdef __cplusplus
extern "C" {
#endif

enum {
   QESNDOP_NONE,
   QESNDOP_NAME_TIME_OF_WHOLE_SOUND, // chr* name, float *resultp -- in seconds
   QESNDOP_TIME_OF_WHOLE_SOUND, // float *resultp -- in seconds
   QESNDOP_TIME_OF_START, // float *resultp -- qeTimeFrame() of start
   QESNDOP_DEBUG, // int 0=off 1=on
   QESNDOP_GETPAN,
};

#define QESNDPAN_MAXLEFT -255
#define QESNDPAN_MAXRIGHT 255

#define QESNDFREQ_MIN 100
#define QESNDFREQ_MAX 100000

// note: play id is returned from qeSndPlay()
// assume that sounds can end, be killed, or replaced by the engine 
// at almost any time -- to address this, all functions that accept play ids
// will only perform the operation if the sound is still playing -- if the
// is no longer playing, the function will return a positive, non-zero result

QE_API int qeSndPlayIdOp(int idPlay,uns op,...); // operations
QE_API int qeSndDelete(chr *name); // kill playing sound, remove resources
QE_API int qeSndLoad(chr *name); // load initial sound buffers
QE_API int qeSndUnload(chr *name); // unload buffer
QE_API int qeSndChStop(uns ch); // stop a channel
QE_API int qeSndPlayIdStop(int idPlay); // stop an id (if found)
QE_API int qeSndVol(chr *name,int vol); // vol for next time sound is played
QE_API int qeSndChVol(uns ch,int vol); // adjust vol on playing channel
QE_API int qeSndPlayIdVol(int idPlay,uns vol); // adjust vol of playing sound
QE_API int qeSndChLoops(uns ch); // return the number of loops
QE_API int qeSndPlayIdCh(int idPlay); // find channel sound is on
QE_API int qeSndPlayIdLoops(int idPlay); // number of loops
QE_API uns qeSndPlayingIds(int *idList,int firstChannel,int numChannels);
QE_API int qeSndChPan(uns ch,int pan); // pan playing channel (untested)
QE_API int qeSndPlayIdPan(int idPlay,int pan); // pan playing sound (untested)
QE_API int qeSndPlayIdGetPan(int idPlay); // get current pan
QE_API int qeSndChFreq(uns ch,int hertz); // playing channel
QE_API int qeSndPlayIdFreq(int idPlay,int hertz); // playing sound
QE_API int qeSndPlayIdGetFreq(int idPlay); // playing sound

// C in C++ wrapper
#ifdef __cplusplus
}
#endif

#endif // QESND_H

