#pragma once
// clang-format off
// 8-Slot Bottom-Row (z. B. V4/Q4, 60%)
// picke Core auf 1,3,4,5,8 – fülle 2,6,7
#define BR8_POS_1_3_4_5_8(MAIN, F2, F6, F7) \
  /*1*/ MAIN##_R5_LCTL, \
  /*2*/ F2, \
  /*3*/ MAIN##_R5_LALT, \
  /*4*/ MAIN##_R5_SPACE, \
  /*5*/ MAIN##_R5_RALT, \
  /*6*/ F6, \
  /*7*/ F7, \
  /*8*/ MAIN##_R5_RCTL

// 7-Slot Bottom-Row (z. B. V1, 75%)
// picke Core auf 1,3,4,5,7 – fülle 2,6
#define BR7_POS_1_3_4_5_7(MAIN, F2, F6) \
  /*1*/ MAIN##_R5_LCTL, \
  /*2*/ F2, \
  /*3*/ MAIN##_R5_LALT, \
  /*4*/ MAIN##_R5_SPACE, \
  /*5*/ MAIN##_R5_RALT, \
  /*6*/ F6, \
  /*7*/ MAIN##_R5_RCTL

  // 6-Slot Bottom-Row (z. B. BELLA, 75%)
  // picke Core auf 1,3,4,5,6 – fülle 2
  #define BR6_POS_1_3_4_5_6(MAIN, F2) \
  /*1*/ MAIN##_R5_LCTL, \
  /*2*/ F2, \
  /*3*/ MAIN##_R5_LALT, \
  /*4*/ MAIN##_R5_SPACE, \
  /*5*/ MAIN##_R5_RALT, \
  /*6*/ MAIN##_R5_RCTL
// clang-format on
