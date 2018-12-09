#include "bihua.h"


INPUT_RETURN_VALUE DoBHInput (void* arg, FcitxKeySym sym, unsigned int state)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	FcitxInputState* input = FcitxInstanceGetInputState (bhstate->owner);
	char* strCodeInput = FcitxInputStateGetRawInputBuffer (input);
	INPUT_RETURN_VALUE retVal;

	retVal = IRV_TO_PROCESS;
	if (FcitxHotkeyIsHotKeyDigit (sym, state)) {	//按下数字键
		retVal = IRV_TO_PROCESS;	//让fcitx自行处理
	} else if (FcitxHotkeyIsHotKey (sym, state, FCITX_BACKSPACE)) {	//按下退格键
		//输入缓冲区中没有数据
		if (!FcitxInputStateGetRawInputBufferSize (input)) {	//缓冲区内没有数据
			if (!FcitxCandidateWordGetListSize (FcitxInputStateGetCandidateList (input)))
				return IRV_DONOT_PROCESS_CLEAN;	//候选词表中没有数据,删除一个已输入法的字符
			return IRV_CLEAN;	//清空候选词表
		}
		//将输入缓冲区的长度减一
		FcitxInputStateSetRawInputBufferSize (input, FcitxInputStateGetRawInputBufferSize (input) - 1);
		//将输入缓冲区的最后一个字符置零
		strCodeInput[FcitxInputStateGetRawInputBufferSize (input)] = '\0';

		if (!FcitxInputStateGetRawInputBufferSize (input))
			retVal = IRV_CLEAN;	//没有数据,关闭候选词框
		else {
			retVal = IRV_DISPLAY_CANDWORDS;	//还有数据,更新候选词表
		}
	} else if (FcitxHotkeyIsHotKey (sym, state, FCITX_SPACE)) {
		//按下空格键
		if (!FcitxCandidateWordGetListSize (FcitxInputStateGetCandidateList (input)))
			return IRV_TO_PROCESS;	//候选词表中没有数据,输出空格
		retVal = FcitxCandidateWordChooseByIndex (FcitxInputStateGetCandidateList (input), 0); //输出候选词表中第一个词
	} else if (FcitxHotkeyIsHotKeyLAZ (sym, state)) {
		//按下字母键(小写)
		if (isKeyCode (sym)) {	//是输入键
			strCodeInput[FcitxInputStateGetRawInputBufferSize (input)] = sym;	//将键值插入缓冲区末尾
			FcitxInputStateSetRawInputBufferSize (input, FcitxInputStateGetRawInputBufferSize (input) + 1);	//将缓冲区长度加一
			strCodeInput[FcitxInputStateGetRawInputBufferSize (input)] = '\0';	//将缓冲区末尾置零
			retVal = IRV_DISPLAY_CANDWORDS;	//更新候选词表
		} else if (FcitxInputStateGetRawInputBufferSize (input)) {
			retVal = IRV_DO_NOTHING;	//什么都不做
		}
	} else {	//按下其它键
		FcitxCandidateWordList* canList = FcitxInputStateGetCandidateList (input);
		if (!FcitxCandidateWordGetListSize (canList) && !FcitxInputStateGetRawInputBufferSize (input)) {	//缓冲区或候选词表内没有数据
			return IRV_TO_PROCESS;	//让fcitx自行处理
		}
		if (FcitxInputStateGetRawInputBufferSize(input)) {	//缓冲区内还有数据
			strCodeInput[0] = 0;	//将缓冲区第一个字符置零
			FcitxInputStateSetRawInputBufferSize (input, 0);	//将缓冲区长度置零
			return IRV_CLEAN;
		}
		if (FcitxCandidateWordGetListSize (canList)) { //候选词表中还有数据
			FcitxCandidateWordReset (FcitxInputStateGetCandidateList (input));	//清空候选词表
		}
		return IRV_CLEAN;
	}
	return retVal;
}

char* keyCode2Msg (const char* raw_buff)
{
	//一丨丿丶ㄥ
	int len = strlen (raw_buff);
	char* res = malloc (len * 4 + 1);
	bzero (res, len * 4 + 1);
	for (int i = 0; i < len; ++i) {
		switch (raw_buff[i]) {
			case 'i':
				strcat (res, "一");
				break;
			case 'o':
				strcat (res, "丨");
				break;
			case 'p':
				strcat (res, "丿");
				break;
			case 'k':
				strcat (res, "丶");
				break;
			case 'l':
				strcat (res, "ㄥ");
				break;
			case 'm':
				strcat (res, "＊");
				break;
		}
	}
	return res;
}

boolean matchKeyCode (const char* src , const char* dest)
{
	int len = strlen (src);
	for (int i = 0; i < len; ++i) {
		if (dest[i] == 0)
			return false;
		if (src[i] == 'm')
			continue;
		if (src[i] != dest[i])
			return false;
	}
	return true;
}
