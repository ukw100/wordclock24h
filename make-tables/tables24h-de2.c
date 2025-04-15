/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-de2.c - WC24H DE2 tables - 15x15 layout
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout is 15x15. You have to make the following changes in the wordclock source:
 *
 * display-config.h:
 *     #define DSP_DISPLAY_LEDS        225         // old: 225
 *
 * wclock24h-config.h:
 *     #define WC_ROWS                  15         // old: 16
 *     #define WC_COLUMNS               15         // old: 18
 *
 * Layout DE2:
 *
 *   ESBISTMNULLWEIN // ES#IST#NULL#EIN
 *   ZWEILDREIJVIERK // ZWEI#DREI#VIER#
 *   NFÜNFQHGSECHSVW // #FÜNF###SECHS##
 *   SIEBENWCFACHTGK // SIEBEN###ACHT##
 *   NEUNTDZEHNRPELF // NEUN##ZEHN##ELF
 *   VZWÖLFMDFUNDKGS // #ZWÖLF###UND###
 *   ZWANZIGTUHRNUND // ZWANZIG#UHR#UND
 *   EINEYZWEIXDREIP // EINE#ZWEI#DREI#
 *   VIERDFÜNFWSECHS // VIER#FÜNF#SECHS
 *   GRSIEBENHACHTLO // ##SIEBEN#ACHT##
 *   BNEUNAZEHNDELFS // #NEUN#ZEHN#ELF#
 *   ZHTZWÖLFKMUNDBE // ###ZWÖLF##UND##
 *   ZWANZIGVDREIßIG // ZWANZIG#DREIßIG
 *   IVIERZIGCFNULLF // #VIERZIG##NULL#
 *   FÜNFZIGTMINUTEN // FÜNFZIG#MINUTEN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H_DE2 == 1

#include "tables24h-de2.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { HM_1, MM_0,   "Uhrzeit"           },
    { HM_0, MM_1,   "Temperatur"        },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS]=
{
    {   // tbl_hours[0][] = Mode 0 leer, fuer Temperaturanzeige!
        { WP_ES, WP_IST, 0                                                                                              },     // 00:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 01:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 02:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 03:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 04:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 05:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 06:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 07:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 08:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 09:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 10:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 11:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 12:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 13:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 14:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 15:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 16:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 17:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 18:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 19:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 20:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 21:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 22:mm in Mode HM_0
        { WP_ES, WP_IST, 0                                                                                              },     // 23:mm in Mode HM_0
    },
    {   // tbl_hours[1][] = HH - MM
        { WP_ES, WP_IST, WP_NULL_1,    WP_UHR_1                                                                         },     // 00:mm in Mode HM_1
        { WP_ES, WP_IST, WP_EIN_1,     WP_UHR_1                                                                         },     // 01:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ZWEI_1,    WP_UHR_1                                                                         },     // 02:mm in Mode HM_1
        { WP_ES, WP_IST, WP_DREI_1,    WP_UHR_1                                                                         },     // 03:mm in Mode HM_1
        { WP_ES, WP_IST, WP_VIER_1,    WP_UHR_1                                                                         },     // 04:mm in Mode HM_1
        { WP_ES, WP_IST, WP_FUENF_1,   WP_UHR_1                                                                         },     // 05:mm in Mode HM_1
        { WP_ES, WP_IST, WP_SECHS_1,   WP_UHR_1                                                                         },     // 06:mm in Mode HM_1
        { WP_ES, WP_IST, WP_SIEBEN_1,  WP_UHR_1                                                                         },     // 07:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ACHT_1,    WP_UHR_1                                                                         },     // 08:mm in Mode HM_1
        { WP_ES, WP_IST, WP_NEUN_1,    WP_UHR_1                                                                         },     // 09:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ZEHN_1,    WP_UHR_1                                                                         },     // 10:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ELF_1,     WP_UHR_1                                                                         },     // 11:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ZWOELF_1,  WP_UHR_1                                                                         },     // 12:mm in Mode HM_1
        { WP_ES, WP_IST, WP_DREI_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 13:mm in Mode HM_1
        { WP_ES, WP_IST, WP_VIER_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 14:mm in Mode HM_1
        { WP_ES, WP_IST, WP_FUENF_1,   WP_ZEHN_1,  WP_UHR_1                                                             },     // 15:mm in Mode HM_1
        { WP_ES, WP_IST, WP_SECH_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 16:mm in Mode HM_1
        { WP_ES, WP_IST, WP_SIEB_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 17:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ACHT_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 18:mm in Mode HM_1
        { WP_ES, WP_IST, WP_NEUN_1,    WP_ZEHN_1,  WP_UHR_1                                                             },     // 19:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ZWANZIG_1, WP_UHR_1                                                                         },     // 20:mm in Mode HM_1
        { WP_ES, WP_IST, WP_EIN_1,     WP_UND_1,   WP_ZWANZIG_1,   WP_UHR_1                                             },     // 21:mm in Mode HM_1
        { WP_ES, WP_IST, WP_ZWEI_1,    WP_UND_1,   WP_ZWANZIG_1,   WP_UHR_1                                             },     // 22:mm in Mode HM_1
        { WP_ES, WP_IST, WP_DREI_1,    WP_UND_1,   WP_ZWANZIG_1,   WP_UHR_1                                             },     // 23:mm in Mode HM_1
    }
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT]=
{
    {
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {0                                                                           }},    // hh:00 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_EINE_1, WP_MINUTE_1                                           }},    // hh:01 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWEI_2, WP_MINUTEN_1                                          }},    // hh:02 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_MINUTEN_1                                          }},    // hh:03 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_MINUTEN_1                                          }},    // hh:04 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_MINUTEN_1                                         }},    // hh:05 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECHS_2, WP_MINUTEN_1                                         }},    // hh:06 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEBEN_2, WP_MINUTEN_1                                        }},    // hh:07 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_MINUTEN_1                                          }},    // hh:08 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_MINUTEN_1                                          }},    // hh:09 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZEHN_2, WP_MINUTEN_1                                          }},    // hh:10 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ELF_2, WP_MINUTEN_1                                           }},    // hh:11 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWOELF_2, WP_MINUTEN_1                                        }},    // hh:12 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:13 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:14 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_ZEHN_2, WP_MINUTEN_1                              }},    // hh:15 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECH_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:16 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEB_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:17 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:18 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_ZEHN_2, WP_MINUTEN_1                               }},    // hh:19 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWANZIG_2, WP_MINUTEN_1                                       }},    // hh:20 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_EIN_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                   }},    // hh:21 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWEI_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                  }},    // hh:22 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                  }},    // hh:23 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                  }},    // hh:24 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                 }},    // hh:25 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECHS_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                 }},    // hh:26 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEBEN_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                }},    // hh:27 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                  }},    // hh:28 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_UND_3, WP_ZWANZIG_2, WP_MINUTEN_1                  }},    // hh:29 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREISSIG_1, WP_MINUTEN_1                                      }},    // hh:30 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_EIN_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                  }},    // hh:31 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWEI_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                 }},    // hh:32 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                 }},    // hh:33 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                 }},    // hh:34 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                }},    // hh:35 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECHS_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                }},    // hh:36 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEBEN_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1               }},    // hh:37 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                 }},    // hh:38 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_UND_3, WP_DREISSIG_1, WP_MINUTEN_1                 }},    // hh:39 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIERZIG_1, WP_MINUTEN_1                                       }},    // hh:40 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_EIN_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                   }},    // hh:41 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWEI_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                  }},    // hh:42 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                  }},    // hh:43 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                  }},    // hh:44 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                 }},    // hh:45 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECHS_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                 }},    // hh:46 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEBEN_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                }},    // hh:47 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                  }},    // hh:48 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_UND_3, WP_VIERZIG_1, WP_MINUTEN_1                  }},    // hh:49 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENFZIG_1, WP_MINUTEN_1                                      }},    // hh:50 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_EIN_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                  }},    // hh:51 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ZWEI_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                 }},    // hh:52 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_DREI_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                 }},    // hh:53 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_VIER_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                 }},    // hh:54 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_FUENF_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                }},    // hh:55 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SECHS_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                }},    // hh:56 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_SIEBEN_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1               }},    // hh:57 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_ACHT_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                 }},    // hh:58 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_UND_2, WP_NEUN_2, WP_UND_3, WP_FUENFZIG_1, WP_MINUTEN_1                 }},    // hh:59 in Mode MM_0
    },
    {
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZEHN_2                                                                  }},    // hh:00 10.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZEHN_2                                                                  }},    // hh:01 10.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ELF_2                                                                   }},    // hh:02 11.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ELF_2                                                                   }},    // hh:03 11.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWOELF_2                                                                }},    // hh:04 12.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWOELF_2                                                                }},    // hh:05 12.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_ZEHN_2                                                    }},    // hh:06 13.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_ZEHN_2                                                    }},    // hh:07 13.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_ZEHN_2                                                    }},    // hh:08 14.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_ZEHN_2                                                    }},    // hh:09 14.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_ZEHN_2                                                    }},    // hh:10 15.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_ZEHN_2                                                    }},    // hh:11 15.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECH_2,    WP_ZEHN_2                                                    }},    // hh:12 16.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECH_2,    WP_ZEHN_2                                                    }},    // hh:13 16.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEB_2,    WP_ZEHN_2                                                    }},    // hh:14 17.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEB_2,    WP_ZEHN_2                                                    }},    // hh:15 17.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_ZEHN_2                                                    }},    // hh:16 18.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_ZEHN_2                                                    }},    // hh:17 18.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_ZEHN_2                                                    }},    // hh:18 19.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_ZEHN_2                                                    }},    // hh:19 19.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWANZIG_2                                                               }},    // hh:20 20.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWANZIG_2                                                               }},    // hh:21 20.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_EIN_2,     WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:22 21.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_EIN_2,     WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:23 22.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWEI_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:24 22.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWEI_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:25 22.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:26 23.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:27 23.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:28 24.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:29 24.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:30 25.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:31 25.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECHS_2,   WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:32 26.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECHS_2,   WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:33 26.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEBEN_2,  WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:34 27.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEBEN_2,  WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:35 27.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:36 28.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:37 28.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:38 29.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_UND_3,    WP_ZWANZIG_2                                    }},    // hh:39 29.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREISSIG_1                                                              }},    // hh:40 30.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREISSIG_1                                                              }},    // hh:41 30.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_EIN_2,     WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:42 31.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_EIN_2,     WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:43 32.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWEI_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:44 32.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ZWEI_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:45 32.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:46 33.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DREI_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:47 33.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:48 34.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_VIER_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:49 34.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:50 35.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_FUENF_2,   WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:51 35.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECHS_2,   WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:52 36.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SECHS_2,   WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:53 36.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEBEN_2,  WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:54 37.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEBEN_2,  WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:55 37.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:56 38.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_ACHT_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:57 38.5 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:58 39.0 C
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_NEUN_2,    WP_UND_3,    WP_DREISSIG_1                                   }},    // hh:59 39.5 C
    },

};

const WORD_ILLUMINATION illumination[WP_COUNT]=
{
    {  0,  0,  0 },                                                                         // WP_END_OF_WORDS
    {  0,  0,  2 | ILLUMINATION_FLAG_IT_IS },                                               // WP_ES
    {  0,  3,  3 | ILLUMINATION_FLAG_IT_IS },                                               // WP_IST
    {  0,  7,  4 },                                                                         // WP_NULL_1
    {  0, 12,  3 },                                                                         // WP_EIN_1
    {  1,  0,  4 },                                                                         // WP_ZWEI_1
    {  1,  5,  4 },                                                                         // WP_DREI_1
    {  1, 10,  4 },                                                                         // WP_VIER_1
    {  2,  1,  4 },                                                                         // WP_FUENF_1
    {  2,  8,  4 },                                                                         // WP_SECH_1
    {  2,  8,  5 },                                                                         // WP_SECHS_1
    {  3,  0,  4 },                                                                         // WP_SIEB_1
    {  3,  0,  6 },                                                                         // WP_SIEBEN_1
    {  3,  9,  4 },                                                                         // WP_ACHT_1
    {  4,  0,  4 },                                                                         // WP_NEUN_1
    {  4,  6,  4 },                                                                         // WP_ZEHN_1
    {  4, 12,  3 },                                                                         // WP_ELF_1
    {  5,  1,  5 },                                                                         // WP_ZWOELF_1
    {  5,  9,  3 },                                                                         // WP_UND_1
    {  6,  0,  7 },                                                                         // WP_ZWANZIG_1
    {  6,  8,  3 },                                                                         // WP_UHR_1
    {  6, 12,  3 },                                                                         // WP_UND_2
    {  7,  0,  3 },                                                                         // WP_EIN_2
    {  7,  0,  4 },                                                                         // WP_EINE_1
    {  7,  5,  4 },                                                                         // WP_ZWEI_2
    {  7, 10,  4 },                                                                         // WP_DREI_2
    {  8,  0,  4 },                                                                         // WP_VIER_2
    {  8,  5,  4 },                                                                         // WP_FUENF_2
    {  8, 10,  4 },                                                                         // WP_SECH_2
    {  8, 10,  5 },                                                                         // WP_SECHS_2
    {  9,  2,  4 },                                                                         // WP_SIEB_2
    {  9,  2,  6 },                                                                         // WP_SIEBEN_2
    {  9,  9,  4 },                                                                         // WP_ACHT_2
    { 10,  1,  4 },                                                                         // WP_NEUN_2
    { 10,  6,  4 },                                                                         // WP_ZEHN_2
    { 10, 11,  3 },                                                                         // WP_ELF_2
    { 11,  3,  5 },                                                                         // WP_ZWOELF_2
    { 11, 10,  3 },                                                                         // WP_UND_3
    { 12,  0,  7 },                                                                         // WP_ZWANZIG_2
    { 12,  8,  7 },                                                                         // WP_DREISSIG_1
    { 13,  1,  7 },                                                                         // WP_VIERZIG_1
    { 13, 10,  4 },                                                                         // WP_NULL_2
    { 14,  0,  7 },                                                                         // WP_FUENFZIG_1
    { 14,  8,  6 },                                                                         // WP_MINUTE_1
    { 14,  8,  7 },                                                                         // WP_MINUTEN_1
 };

#endif // WCLOCK24H == 1

