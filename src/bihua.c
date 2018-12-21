/***************************************************************************
 *   Copyright (C) 2002~2005 by Yuking                                     *
 *   yuking_net@sohu.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/
#include "bihua.h"
#include "Thesaurus.h"
#include "FreqList.h"

FCITX_DEFINE_PLUGIN (fcitx_bh, ime, FcitxIMClass) = {
	BHCreate,
	BHDestroy
};

void* BHCreate (struct _FcitxInstance* instance)
{
	FcitxBiHuaState* bhstate = malloc (sizeof (FcitxBiHuaState));
	bhstate->words = (BWord*) getWords (&bhstate->wordSize);
	if (bhstate->words == NULL) {
		free (bhstate);
		return NULL;
	}
	FcitxInstanceRegisterIM (
		instance,
		bhstate,
		"bihua",
		"笔画",
		"bihua",
		BHInit,
		NULL,
		DoBHInput,
		BHGetCandWords,
		NULL,
		NULL,
		NULL,
		NULL,
		100 /* make quwei place at last */,
		"zh_CN"
	);
	bhstate->owner = instance;
	bhstate->lastInput = NULL;
	char * freqFile;
	newString(freqFile,strlen("/.config/bihua_freq") + strlen(getenv("HOME")));
	sprintf(freqFile,"%s/.config/bihua_freq",getenv("HOME"));
	bhstate->freqList = openFreqList(freqFile);
	free(freqFile);
	return bhstate;
}

boolean BHInit (void* arg)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	FcitxInstanceSetContext (bhstate->owner, CONTEXT_IM_KEYBOARD_LAYOUT, "us");
	boolean flag = true;
    FcitxInstanceSetContext(bhstate->owner, CONTEXT_SHOW_REMIND_STATUS, &flag);
	return true;
}

void BHDestroy (void* arg)
{
	FcitxBiHuaState* bhstate = (FcitxBiHuaState*) arg;
	saveFreqList(bhstate->freqList);
	freeFreqList(bhstate->freqList);
	free (bhstate);
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 

