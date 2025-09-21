// keyboards/keychron/v1_max/iso_encoder/keymaps/neo/compat_v1max.h
#pragma once

// Falls der V1 Max kein LAYOUT_iso_83 kennt,
// mappen wir es auf das Encoder-Layout.
#ifndef LAYOUT_iso_83
#define LAYOUT_iso_83 LAYOUT_iso_encoder
#endif
