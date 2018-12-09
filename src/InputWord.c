#include "bihua.h"
#include "Thesaurus.h"
INPUT_RETURN_VALUE BHGetCandWord (void* arg, FcitxCandidateWord* candWord)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	FcitxInputState* input = FcitxInstanceGetInputState (bhstate->owner);
	char* word = strdup (candWord->strWord);
	strcpy (FcitxInputStateGetOutputString (input),
			word);
	if (bhstate->lastInput) {
		free (bhstate->lastInput);
		bhstate->lastInput = NULL;
	}
	if (BHGetRemindCandWords (bhstate, word) == IRV_DISPLAY_CANDWORDS) {
		FcitxMessages* preedit = FcitxInputStateGetPreedit (input);
		FcitxMessagesSetMessageCount (preedit, 0);
		FcitxMessagesAddMessageStringsAtLast (preedit, MSG_TIPS, "联想");
		FcitxInputStateSetRawInputBufferSize (input, 0);
		FcitxInputStateGetRawInputBuffer (input) [0] = 0;
		bhstate->lastInput = word;
		return IRV_COMMIT_STRING_REMIND;
	}
	free (word);
	return IRV_COMMIT_STRING;
}

INPUT_RETURN_VALUE BHGetRemindCandWord (void* arg, FcitxCandidateWord* candWord)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	FcitxInputState* input = FcitxInstanceGetInputState (bhstate->owner);
	char* word = strdup (candWord->strWord);
	strcpy (FcitxInputStateGetOutputString (input),
			word);
	int retVal = IRV_COMMIT_STRING;
	char* sInput = malloc (strlen (word) + strlen (bhstate->lastInput) + 1);
	sprintf (sInput, "%s%s", bhstate->lastInput, word);
	free (bhstate->lastInput);
	bhstate->lastInput = sInput;
	if (BHGetRemindCandWords (bhstate, bhstate->lastInput) == IRV_DISPLAY_CANDWORDS) {
		retVal = IRV_COMMIT_STRING_REMIND;
	}
	free (word);
	return retVal;
}

INPUT_RETURN_VALUE BHGetCandWords (void* arg)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	FcitxInputState* input = FcitxInstanceGetInputState (bhstate->owner);
	FcitxCandidateWordList* cand_list = FcitxInputStateGetCandidateList (input);
	FcitxCandidateWordSetPageSize (cand_list, 5);
	FcitxCandidateWordSetChoose (cand_list, DIGIT_STR_CHOOSE);
	char* raw_buff = FcitxInputStateGetRawInputBuffer (input);
	if (FcitxInputStateGetRawInputBufferSize (input) < 50) {
		getCandidateWord (bhstate, cand_list, raw_buff);
	}
	char* msg = keyCode2Msg (raw_buff);
	FcitxInputStateSetCursorPos (input,
								 strlen(msg));
	FcitxMessages* preedit = FcitxInputStateGetPreedit (input);
	FcitxMessagesSetMessageCount (preedit, 0);
	FcitxMessagesAddMessageStringsAtLast (preedit, MSG_INPUT, msg);
	free (msg);
	return IRV_DISPLAY_CANDWORDS;
}

INPUT_RETURN_VALUE BHGetRemindCandWords (FcitxBiHuaState* bhstate, const char* word)
{
	FcitxInputState* input = FcitxInstanceGetInputState (bhstate->owner);
	FcitxCandidateWordList* cand_list = FcitxInputStateGetCandidateList (input);
	FcitxCandidateWordReset (cand_list);
	FcitxCandidateWordSetPageSize (cand_list, 5);
	FcitxCandidateWordSetChoose (cand_list, DIGIT_STR_CHOOSE);
	int n = getCandidateWordForThesaurus (bhstate, cand_list, word);
	if (n) {
		return IRV_DISPLAY_CANDWORDS;
	}
	return IRV_DO_NOTHING;
}

void getCandidateWord (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* keyCode)
{
	if (cand_list == NULL)
		return;
	BWord* bw = bhstate->words;

	for (int i = 0; i < bhstate->wordSize; ++i) {
		if (matchKeyCode (keyCode, bw[i].keyCode)) {
			FcitxCandidateWord candWord;
			candWord.callback = BHGetCandWord;
			candWord.owner = bhstate;
			candWord.priv = NULL;
			candWord.strExtra = NULL;
			candWord.strWord = strdup (bw[i].word);
			candWord.wordType = MSG_OTHER;
			FcitxCandidateWordAppend (cand_list, &candWord);
		}
	}
}

int getCandidateWordForThesaurus (FcitxBiHuaState* bhstate, FcitxCandidateWordList* cand_list, const char* word)
{
	if (cand_list == NULL)
		return 0;
	int iThesSize;
	const char* const* thesaurus = getThesaurus (&iThesSize);
	if (thesaurus == NULL)
		return 0;
	int n = 0;
	int iSize = strlen (word);
	for (int i = 0; i < iThesSize; ++i) {
		if (strncmp (word, thesaurus[i], iSize) == 0) {
			if (strlen(thesaurus[i] + iSize)==0)
				continue;
			FcitxCandidateWord candWord;
			candWord.callback = BHGetRemindCandWord;
			candWord.owner = bhstate;
			candWord.priv = NULL;
			candWord.strExtra = NULL;
			candWord.strWord = strdup (thesaurus[i] + iSize);
			candWord.wordType = MSG_OTHER;
			FcitxCandidateWordAppend (cand_list, &candWord);
			++n;
		}
	}
	return n;
}
