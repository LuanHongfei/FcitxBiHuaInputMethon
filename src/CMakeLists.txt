# Source files
set(fcitx_fcitxbihuainputmethod_sources
  fcitxbihuainputmethod-config.c
  fcitxbihuainputmethod.c
  )

# See document in FcitxMacro.cmake and the example .fxaddon file
# doc/fcitx-example.fxaddon in the fcitx repo for more information.
fcitx_add_addon_full(fcitxbihuainputmethod
  DESC # Use configdesc file
  SCAN # Generate api header
  SCAN_PRIV # Generate addfunction header
  # HEADER_DIR alternative_name # change dir name your header is installed
  # LIB_NAME lib_name # module file name (without extension)
  # HEADERS headers.h # extra public headers to install
  # EXTRA_PO extra_files # extra files to translate
  # LINK_LIBS other-lib # external libraies to link
  SOURCES ${fcitx_fcitxbihuainputmethod_sources})
