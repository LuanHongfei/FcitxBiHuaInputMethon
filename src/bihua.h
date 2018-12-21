#ifndef __HBIHUA_H__
#define __HBIHUA_H__
#include "config.h"

#include <string.h>

#include "fcitx/fcitx.h"
#include "fcitx-utils/utils.h"
#include "fcitx/keys.h"

#include "fcitx/ui.h"
#include "fcitx/configfile.h"
#include "fcitx/instance.h"
#include "fcitx/candidate.h"
#include "fcitx/context.h"

#define newString(name,len) do { \
	name = (char *) malloc((len) + 1); \
	} while (name == NULL); \
	bzero(name,(len) + 1)

typedef struct BWord {
	char* keyCode;
	char* word;
} BWord;

typedef struct BiHuaWordFreqList {
	char * filePath;
	char * word;
	struct BiHuaWordFreqList * next;
	int freq;
} BiHuaWordFreqList;

typedef struct FcitxBiHuaState {
	BWord* words;
	int wordSize;
	FcitxInstance* owner;
	char* lastInput;
	BiHuaWordFreqList * freqList;
} FcitxBiHuaState;

extern void* BHCreate (struct _FcitxInstance* instance);
extern INPUT_RETURN_VALUE DoBHInput (void* arg, FcitxKeySym sym, unsigned int state);
extern INPUT_RETURN_VALUE BHGetCandWords (void* arg);
extern INPUT_RETURN_VALUE BHGetCandWord (void* arg, FcitxCandidateWord* candWord);
extern INPUT_RETURN_VALUE BHGetRemindCandWord (void* arg, FcitxCandidateWord* candWord);
extern INPUT_RETURN_VALUE BHGetRemindCandWords (FcitxBiHuaState* bhstate, const char* word);
extern boolean BHInit (void* arg);
extern void getCandidateWord (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* keyCode);
extern int getCandidateWordForThesaurus (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* word);
extern char* keyCode2Msg (const char* raw_buff);
extern void BHDestroy (void* arg);
extern boolean matchKeyCode (const char* src , const char* dest);
extern boolean isKeyCode (char c);
extern BWord* getWords (int* size);
extern void candidateWordAppend(FcitxCandidateWordList * cand_list,FcitxCandidateWord *candWord, const char * remindWord);

#endif
