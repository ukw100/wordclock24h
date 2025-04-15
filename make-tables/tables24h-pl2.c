/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-pl2.c - WC24H tables (polski 2)
 *
 * Copyright (c) 2019 Frank Meyer - frank(at)fli4l.de
 *
 *   Layout is 20x20. You have to make the following changes in the wordclock source:
 *
 * display-config.h:
 *     #define DSP_DISPLAY_LEDS        400        // old: 288
 *
 * wclock24h-config.h:
 *     #define WC_ROWS                  20        // old: 16
 *     #define WC_COLUMNS               20        // old: 18
 *
 * POLSKI: 20X20
 *   JEST*SIEDEMNASTAZERO
 *   SIODMADZIESIATA*OSMA
 *   *CZWARTA*DWUDZIESTA*
 *   DRUGA*SZOSTA*TRZECIA
 *   TRZYNASTA**DWUNASTA*
 *   OSIEMNASTA*PIETNASTA
 *   JEDENASTA*SZESNASTA*
 *   *DZIEWIETNASTA*PIATA
 *   PIERWSZA*CZTERNASTA*
 *   DZIEWIATA*SZESNASCIE
 *   TRZYDZIESCI*DZIESIEC
 *   ZERO***DWADZIESCIA**
 *   PIETNASCIEJEDENASCIE
 *   ***CZTERDZIESCI*ZERO
 *   *PIECDZIESIAT**JEDEN
 *   OSIEMNASCIEPIECSZESC
 *   *CZTERY***TRZYNASCIE
 *   SIEDEMNASCIEDZIEWIEC
 *   CZTERNASCIEDWANASCIE
 *   ***DZIEWIETNASCIE*C*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H_PL2 == 1

#include "tables24h-pl2.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { HM_1, MM_0,   "Polski"            },
    { HM_0, MM_1,   "Temperatur"        },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS]=
{
    {   // tbl_hours[0][] = Temerature
        { WP_JEST                                                                   },      // 00:mm in Mode HM_0
        { WP_JEST                                                                   },      // 01:mm in Mode HM_0
        { WP_JEST                                                                   },      // 02:mm in Mode HM_0
        { WP_JEST                                                                   },      // 03:mm in Mode HM_0
        { WP_JEST                                                                   },      // 04:mm in Mode HM_0
        { WP_JEST                                                                   },      // 05:mm in Mode HM_0
        { WP_JEST                                                                   },      // 06:mm in Mode HM_0
        { WP_JEST                                                                   },      // 07:mm in Mode HM_0
        { WP_JEST                                                                   },      // 08:mm in Mode HM_0
        { WP_JEST                                                                   },      // 09:mm in Mode HM_0
        { WP_JEST                                                                   },      // 10:mm in Mode HM_0
        { WP_JEST                                                                   },      // 11:mm in Mode HM_0
        { WP_JEST                                                                   },      // 12:mm in Mode HM_0
        { WP_JEST                                                                   },      // 13:mm in Mode HM_0
        { WP_JEST                                                                   },      // 14:mm in Mode HM_0
        { WP_JEST                                                                   },      // 15:mm in Mode HM_0
        { WP_JEST                                                                   },      // 16:mm in Mode HM_0
        { WP_JEST                                                                   },      // 17:mm in Mode HM_0
        { WP_JEST                                                                   },      // 18:mm in Mode HM_0
        { WP_JEST                                                                   },      // 19:mm in Mode HM_0
        { WP_JEST                                                                   },      // 20:mm in Mode HM_0
        { WP_JEST                                                                   },      // 21:mm in Mode HM_0
        { WP_JEST                                                                   },      // 22:mm in Mode HM_0
        { WP_JEST                                                                   },      // 23:mm in Mode HM_0
    },
    {   // tbl_hours[1][] = Mode 1 POLSKI
        {WP_JEST, WP_ZERO                                                           },      // 00:mm in Mode HM_1
        {WP_JEST, WP_PIERWSZA                                                       },      // 01:mm in Mode HM_1
        {WP_JEST, WP_DRUGA                                                          },      // 02:mm in Mode HM_1
        {WP_JEST, WP_TRZECIA                                                        },      // 03:mm in Mode HM_1
        {WP_JEST, WP_CZWARTA                                                        },      // 04:mm in Mode HM_1
        {WP_JEST, WP_PIATA                                                          },      // 05:mm in Mode HM_1
        {WP_JEST, WP_SZOSTA                                                         },      // 06:mm in Mode HM_1
        {WP_JEST, WP_SIODMA                                                         },      // 07:mm in Mode HM_1
        {WP_JEST, WP_OSMA                                                           },      // 08:mm in Mode HM_1
        {WP_JEST, WP_DZIEWIATA                                                      },      // 09:mm in Mode HM_1
        {WP_JEST, WP_DZIESIATA                                                      },      // 10:mm in Mode HM_1
        {WP_JEST, WP_JEDENASTA                                                      },      // 11:mm in Mode HM_1
        {WP_JEST, WP_DWUNASTA                                                       },      // 12:mm in Mode HM_1
        {WP_JEST, WP_TRZYNASTA                                                      },      // 13:mm in Mode HM_1
        {WP_JEST, WP_CZTERNASTA                                                     },      // 14:mm in Mode HM_1
        {WP_JEST, WP_PIETNASTA                                                      },      // 15:mm in Mode HM_1
        {WP_JEST, WP_SZESNASTA                                                      },      // 16:mm in Mode HM_1
        {WP_JEST, WP_SIEDEMNASTA                                                    },      // 17:mm in Mode HM_1
        {WP_JEST, WP_OSIEMNASTA                                                     },      // 18:mm in Mode HM_1
        {WP_JEST, WP_DZIEWIETNASTA                                                  },      // 19:mm in Mode HM_1
        {WP_JEST, WP_DWUDZIESTA                                                     },      // 20:mm in Mode HM_1
        {WP_JEST, WP_DWUDZIESTA, WP_PIERWSZA                                        },      // 21:mm in Mode HM_1
        {WP_JEST, WP_DWUDZIESTA, WP_DRUGA                                           },      // 22:mm in Mode HM_1
        {WP_JEST, WP_DWUDZIESTA, WP_TRZECIA                                         },      // 23:mm in Mode HM_1
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT]=
{
    {   // tbl_minutes[0][] = Mode 1 POLSKI
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_ZERO_2                     }},     // hh:00 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_JEDEN                      }},     // hh:01 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_DWA                        }},     // hh:02 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_TRZY                       }},     // hh:03 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_CZTERY                     }},     // hh:04 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_PIEC                       }},     // hh:05 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_SZESC                      }},     // hh:06 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_SIEDEM                     }},     // hh:07 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_OSIEM                      }},     // hh:08 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO_1, WP_DZIEWIEC                   }},     // hh:09 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DZIESIEC                              }},     // hh:10 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_JEDENASCIE                            }},     // hh:11 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWANASCIE                             }},     // hh:12 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYNASCIE                            }},     // hh:13 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERNASCIE                           }},     // hh:14 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIETNASCIE                            }},     // hh:15 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_SZESNASCIE                            }},     // hh:16 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_SIEDEMNASCIE                          }},     // hh:17 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_OSIEMNASCIE                           }},     // hh:18 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DZIEWIETNASCIE                        }},     // hh:19 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA                           }},     // hh:20 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_JEDEN             }},     // hh:21 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_DWA               }},     // hh:22 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_TRZY              }},     // hh:23 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_CZTERY            }},     // hh:24 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_PIEC              }},     // hh:25 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_SZESC             }},     // hh:26 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_SIEDEM            }},     // hh:27 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_OSIEM             }},     // hh:28 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA,     WP_DZIEWIEC          }},     // hh:29 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,                          }},     // hh:30 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_JEDEN             }},     // hh:31 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_DWA               }},     // hh:32 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_TRZY              }},     // hh:33 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_CZTERY            }},     // hh:34 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_PIEC              }},     // hh:35 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_SZESC             }},     // hh:36 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_SIEDEM            }},     // hh:37 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_OSIEM             }},     // hh:38 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI,     WP_DZIEWIEC          }},     // hh:39 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,                         }},     // hh:40 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_JEDEN             }},     // hh:41 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_DWA               }},     // hh:42 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_TRZY              }},     // hh:43 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_CZTERY            }},     // hh:44 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_PIEC              }},     // hh:45 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_SZESC             }},     // hh:46 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_SIEDEM            }},     // hh:47 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_OSIEM             }},     // hh:48 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI,    WP_DZIEWIEC          }},     // hh:49 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,                         }},     // hh:50 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_JEDEN             }},     // hh:51 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_DWA               }},     // hh:52 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_TRZY              }},     // hh:53 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_CZTERY            }},     // hh:54 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_PIEC              }},     // hh:55 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_SZESC             }},     // hh:56 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_SIEDEM            }},     // hh:57 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_OSIEM             }},     // hh:58 in Mode MM_0
        {MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT,    WP_DZIEWIEC          }},     // hh:59 in Mode MM_0
    },
    {   // tbl_minutes[1][] = Mode 1 TEMPERATUR
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DZIESIEC,                     WP_C  }},     // hh:00 10.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DZIESIEC,                     WP_C  }},     // hh:01 10.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_JEDENASCIE,                   WP_C  }},     // hh:02 11.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_JEDENASCIE,                   WP_C  }},     // hh:03 11.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWANASCIE,                    WP_C  }},     // hh:04 12.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWANASCIE,                    WP_C  }},     // hh:05 12.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYNASCIE,                   WP_C  }},     // hh:06 13.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYNASCIE,                   WP_C  }},     // hh:07 13.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_CZTERNASCIE,                  WP_C  }},     // hh:08 14.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_CZTERNASCIE,                  WP_C  }},     // hh:09 14.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_PIETNASCIE,                   WP_C  }},     // hh:10 15.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_PIETNASCIE,                   WP_C  }},     // hh:11 15.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SZESNASCIE,                   WP_C  }},     // hh:12 16.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SZESNASCIE,                   WP_C  }},     // hh:13 16.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEDEMNASCIE,                 WP_C  }},     // hh:14 17.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_SIEDEMNASCIE,                 WP_C  }},     // hh:15 17.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_OSIEMNASCIE,                  WP_C  }},     // hh:16 18.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_OSIEMNASCIE,                  WP_C  }},     // hh:17 18.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DZIEWIETNASCIE,               WP_C  }},     // hh:18 19.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DZIEWIETNASCIE,               WP_C  }},     // hh:19 19.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,                  WP_C  }},     // hh:20 20.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,                  WP_C  }},     // hh:21 20.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_JEDEN,    WP_C  }},     // hh:22 21.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_JEDEN,    WP_C  }},     // hh:23 21.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_DWA,      WP_C  }},     // hh:24 22.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_DWA,      WP_C  }},     // hh:25 22.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_TRZY,     WP_C  }},     // hh:26 23.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_TRZY,     WP_C  }},     // hh:27 23.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_CZTERY,   WP_C  }},     // hh:28 24.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_CZTERY,   WP_C  }},     // hh:29 24.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_PIEC,     WP_C  }},     // hh:30 25.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_PIEC,     WP_C  }},     // hh:31 25.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_SZESC,    WP_C  }},     // hh:32 26.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_SZESC,    WP_C  }},     // hh:33 26.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_SIEDEM,   WP_C  }},     // hh:34 27.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_SIEDEM,   WP_C  }},     // hh:35 27.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_OSIEM,    WP_C  }},     // hh:36 28.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_OSIEM,    WP_C  }},     // hh:37 28.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_DZIEWIEC, WP_C  }},     // hh:38 29.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_DWADZIESCIA,     WP_DZIEWIEC, WP_C  }},     // hh:39 29.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,                  WP_C  }},     // hh:40 30.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,                  WP_C  }},     // hh:41 30.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_JEDEN,    WP_C  }},     // hh:42 31.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_JEDEN,    WP_C  }},     // hh:43 31.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_DWA,      WP_C  }},     // hh:44 32.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_DWA,      WP_C  }},     // hh:45 32.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_TRZY,     WP_C  }},     // hh:46 33.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_TRZY,     WP_C  }},     // hh:47 33.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_CZTERY,   WP_C  }},     // hh:48 34.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_CZTERY,   WP_C  }},     // hh:49 34.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_PIEC,     WP_C  }},     // hh:50 35.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_PIEC,     WP_C  }},     // hh:51 35.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_SZESC,    WP_C  }},     // hh:52 36.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_SZESC,    WP_C  }},     // hh:53 36.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_SIEDEM,   WP_C  }},     // hh:54 37.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_SIEDEM,   WP_C  }},     // hh:55 37.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_OSIEM,    WP_C  }},     // hh:56 38.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_OSIEM,    WP_C  }},     // hh:57 38.5 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_DZIEWIEC, WP_C  }},     // hh:58 39.0 degree
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, { WP_TRZYDZIESCI,     WP_DZIEWIEC, WP_C  }},     // hh:59 39.5 degree
    },
};

const WORD_ILLUMINATION illumination[WP_COUNT]=
{
    {  0,  0,  0 },                                                                         // 00 = WP_END_OF_WORDS = ""
    {  0,  0,  4  | ILLUMINATION_FLAG_IT_IS},                                               // 01 = WP_JEST = "ES IT"
    {  8,  0,  8 },                                                                         // 02 = WP_PIERWSZA
    {  3,  0,  5 },                                                                         // 03 = WP_DRUGA
    {  3, 13,  7 },                                                                         // 04 = WP_TRZECIA
    {  2,  1,  7 },                                                                         // 05 = WP_CZWARTA
    {  7, 15,  5 },                                                                         // 06 = WP_PIATA
    {  3,  6,  6 },                                                                         // 07 = WP_SZUSTA
    {  1,  0,  6 },                                                                         // 08 = WP_SIODMA
    {  1, 16,  4 },                                                                         // 09 = WP_OSMA
    {  9,  0,  9 },                                                                         // 10 = WP_DZIEWIATA
    {  1,  6,  9 },                                                                         // 11 = WP_DZIESIATA
    {  6,  0,  9 },                                                                         // 12 = WP_JEDENASTA
    {  4, 11,  8 },                                                                         // 13 = WP_DWUNASTA
    {  4,  0,  9 },                                                                         // 14 = WP_TRZYNASTA
    {  8,  9, 10 },                                                                         // 15 = WP_CZTERNASTA
    {  5, 11,  9 },                                                                         // 16 = WP_PIETNASTA
    {  6, 10,  9 },                                                                         // 17 = WP_SZESNASTA
    {  0,  5, 11 },                                                                         // 18 = WP_SIEDEMNASTA
    {  5,  0, 10 },                                                                         // 19 = WP_OSIEMNASTA
    {  7,  1, 13 },                                                                         // 20 = WP_DZIEWIETNASTA
    {  2,  9, 10 },                                                                         // 21 = WP_DWUDZIESTA
    {  0, 16,  4 },                                                                         // 22 = WP_ZERO
    { 11,  0,  4 },                                                                         // 23 = WP_ZERO_1
    { 13, 16,  4 },                                                                         // 24 = WP_ZERO_2
    { 14, 15,  5 },                                                                         // 25 = WP_JEDEN
    { 18, 11,  3 },                                                                         // 26 = WP_DWA
    { 16, 10,  4 },                                                                         // 27 = WP_TRZY
    { 16,  1,  6 },                                                                         // 28 = WP_CZTERY
    { 15, 11,  4 },                                                                         // 29 = WP_PIEC
    { 15, 15,  5 },                                                                         // 30 = WP_SZESC
    { 17,  0,  6 },                                                                         // 31 = WP_SIEDEM
    { 15,  0,  5 },                                                                         // 32 = WP_OSIEM
    { 17, 12,  8 },                                                                         // 33 = WP_DZIEWIEC
    { 10, 12,  8 },                                                                         // 34 = WP_DZIESIEC
    { 12, 10, 10 },                                                                         // 35 = WP_JEDENASCIE
    { 18, 11,  9 },                                                                         // 36 = WP_DWANASCIE
    { 16, 10, 10 },                                                                         // 37 = WP_TRZYNASCIE
    { 18,  0, 11 },                                                                         // 38 = WP_CZTERNASCIE
    { 12,  0, 10 },                                                                         // 39 = WP_PIETNASCIE
    {  9, 10, 10 },                                                                         // 40 = WP_SZESNASCIE
    { 17,  0, 12 },                                                                         // 41 = WP_SIEDEMNASCI
    { 15,  0, 11 },                                                                         // 42 = WP_OSIEMNASCIE
    { 19,  3, 14 },                                                                         // 43 = WP_DZIEWIETNASCIE
    { 11,  7, 11 },                                                                         // 44 = WP_DWADZIESCIA
    { 10,  0, 11 },                                                                         // 45 = WP_TRZYDZIESCI
    { 13,  3, 12 },                                                                         // 46 = WP_CZTERDZIESCI
    { 14,  1, 12 },                                                                         // 47 = WP_PIEDZIESIAT
    { 19, 18,  1 },                                                                         // 48 = WP_C
 };

#endif // WCLOCK24H == 1
