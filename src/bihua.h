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

typedef struct BHWord {
	char* keyCode;
	char* word;
} BWord;

typedef struct FcitxBiHuaState {
	BWord* words;
	int wordSize;
	FcitxInstance* owner;
	char* lastInput;
} FcitxBiHuaState;

extern void* BHCreate (struct _FcitxInstance* instance);
extern INPUT_RETURN_VALUE DoBHInput (void* arg, FcitxKeySym sym, unsigned int state);
extern INPUT_RETURN_VALUE BHGetCandWords (void* arg);
extern INPUT_RETURN_VALUE BHGetCandWord (void* arg, FcitxCandidateWord* candWord);
extern INPUT_RETURN_VALUE BHGetRemindCandWord (void* arg, FcitxCandidateWord* candWord);
extern INPUT_RETURN_VALUE BHGetRemindCandWords (FcitxBiHuaState* bhstate, const char* word);
//extern INPUT_RETURN_VALUE BHGetSpitCandidateWord (FcitxBiHuaState* bhstate,const char* keyCode);
extern boolean BHInit (void* arg);
extern void getCandidateWord (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* keyCode);
extern int getCandidateWordForThesaurus (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* word);
extern char* keyCode2Msg (const char* raw_buff);
extern void BHDestroy (void* arg);
extern boolean matchKeyCode (const char* src , const char* dest);
inline boolean isKeyCode (char c)
{
	switch (c) {
		case 'i':
		case 'o':
		case 'p':
		case 'k':
		case 'l':
		case 'm':
			return true;
	}
	return false;
}

extern struct BWord* getWords (int* size);

#endif
