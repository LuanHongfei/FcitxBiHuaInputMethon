#include "FreqList.h"

#define newFreqList(name) do { \
	name = (BiHuaWordFreqList *) malloc(sizeof(BiHuaWordFreqList)); \
	} while (name == NULL); \
	bzero(name,sizeof(BiHuaWordFreqList)); \
	name->freq = -1

BiHuaWordFreqList * openFreqList(const char * path)
{
	BiHuaWordFreqList * list;
	newFreqList(list);
	list->filePath = strdup(path);
	FILE * fp = fopen(path,"rb");
	if (fp == NULL) {
		return list;
	}
	BiHuaWordFreqList * begin = list;
	unsigned char wordLen;
	while (!feof(fp)) {
		if (fread(&wordLen,1,1,fp)) {
			newString(list->word,wordLen);
			fread(list->word,1,wordLen,fp);
			fread(&list->freq,sizeof(int),1,fp);
			newFreqList(list->next);
			list = list->next;
		} else break;
	}
	fclose(fp);
	return begin;
}

void saveFreqList(BiHuaWordFreqList* list)
{
	if (list == NULL)
		return;
	FILE * fp = fopen(list->filePath,"wb");
	if (fp == NULL)
		return;
	unsigned char wordLen;
	while (list->freq != -1) {
		wordLen = strlen(list->word);
		fwrite(&wordLen,1,1,fp);
		fwrite(list->word,strlen(list->word),1,fp);
		fwrite(&list->freq,sizeof(int),1,fp);
		list =list->next;
	}
	fclose(fp);
}

void freeFreqList(BiHuaWordFreqList* list)
{
	if (list == NULL)
		return;
	BiHuaWordFreqList * prev;
	free(list->filePath);
	while (list) {
		free(list->word);
		prev = list;
		list = list->next;
		free(prev);
	}
}

int getWordFreq(BiHuaWordFreqList* list, const char* word)
{
	if (word == NULL || list == NULL)
		return -1;
	while (list->freq != -1) {
		if (!strcmp(list->word,word))
			break;
		list = list->next;
	}
	return list->freq;
}

void wordFreqAppend(BiHuaWordFreqList* list, const char* word)
{
	if (word == NULL || list == NULL)
		return;
	while (list->freq != -1) {
		if (!strcmp(list->word,word)) {
			list->freq ++;
			return;
		}
		list = list->next;
	}
	list->freq = 1;
	list->word = strdup(word);
	newFreqList(list->next);
}
