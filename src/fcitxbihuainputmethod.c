/***************************************************************************
 *   Copyright (C) YEAR~YEAR by Your Name                                  *
 *   your-email@address.com                                                *
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcitx/ime.h>
#include <fcitx-config/fcitx-config.h>
#include <fcitx-config/xdg.h>
#include <fcitx-utils/log.h>
#include <fcitx-utils/utils.h>
#include <fcitx-utils/utf8.h>
#include <fcitx/instance.h>
#include <fcitx/context.h>
#include <fcitx/module.h>
#include <fcitx/hook.h>
#include <libintl.h>

#include "config.h"
#include "fcitxbihuainputmethod-internal.h"

static void *FcitxFcitxBiHuaInputMethodCreate(FcitxInstance *instance);
static void FcitxFcitxBiHuaInputMethodDestroy(void *arg);
static void FcitxFcitxBiHuaInputMethodReloadConfig(void *arg);
CONFIG_DEFINE_LOAD_AND_SAVE(FcitxBiHuaInputMethod, FcitxFcitxBiHuaInputMethodConfig, "fcitx-fcitxbihuainputmethod")
DECLARE_ADDFUNCTIONS(FcitxBiHuaInputMethod)

FCITX_DEFINE_PLUGIN(fcitx_spell, module, FcitxModule) = {
    .Create = FcitxFcitxBiHuaInputMethodCreate,
    .Destroy = FcitxFcitxBiHuaInputMethodDestroy,
    .SetFD = NULL,
    .ProcessEvent = NULL,
    .ReloadConfig = FcitxFcitxBiHuaInputMethodReloadConfig
};

static void*
FcitxFcitxBiHuaInputMethodCreate(FcitxInstance *instance)
{
    FcitxFcitxBiHuaInputMethod *fcitxbihuainputmethod = fcitx_utils_new(FcitxFcitxBiHuaInputMethod);
    bindtextdomain("fcitx-fcitxbihuainputmethod", LOCALEDIR);
    fcitxbihuainputmethod->owner = instance;

    if (!FcitxBiHuaInputMethodLoadConfig(&fcitxbihuainputmethod->config)) {
        free(fcitxbihuainputmethod);
        return NULL;
    }

    FcitxFcitxBiHuaInputMethodAddFunctions(instance);
    return fcitxbihuainputmethod;
}

static void
FcitxFcitxBiHuaInputMethodDestroy(void *arg)
{
    FcitxFcitxBiHuaInputMethod *fcitxbihuainputmethod = (FcitxFcitxBiHuaInputMethod*)arg;
    free(fcitxbihuainputmethod);
}

static void
FcitxFcitxBiHuaInputMethodReloadConfig(void *arg)
{
    FcitxFcitxBiHuaInputMethod *fcitxbihuainputmethod = (FcitxFcitxBiHuaInputMethod*)arg;
    FcitxBiHuaInputMethodLoadConfig(&fcitxbihuainputmethod->config);
}

#include "fcitx-fcitxbihuainputmethod-addfunctions.h"
