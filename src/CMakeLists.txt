aux_source_directory(. FCITX_BIHUA_SOURCES)

fcitx_add_addon_full(bh
  SOURCES ${FCITX_BIHUA_SOURCES}
  IM_CONFIG bh.conf)
