#ifndef __H_FREQLIST_H__
#define __H_FREQLIST_H__
#include "bihua.h"
extern void saveFreqList(BiHuaWordFreqList * list);
extern BiHuaWordFreqList * openFreqList(const char * path);
extern void freeFreqList(BiHuaWordFreqList * list);
extern int getWordFreq(BiHuaWordFreqList * list,const char * word);
extern void wordFreqAppend(BiHuaWordFreqList * list,const char * word);
#endif
