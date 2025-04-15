/*-------------------------------------------------------------------------------------------------------------------------------------------
 * tables24h-pl.c - WC24H tables (polski)
 *
 * Copyright (c) 2018-2019 Frank Meyer - frank(at)fli4l.de
 *
 * Layout is 18x18. You have to make the following changes in the wordclock source:
 *
 * display-config.h:
 *     #define DSP_DISPLAY_LEDS        324         // old: 288
 *
 * wclock24h-config.h:
 *     #define WC_ROWS                  18         // old: 16
 *
 * POLSKI: 18X18
 *   JEST*TRZECIASZOSTA
 *   DZIEWIATADZIESIATA
 *   SIODMACZWARTAOSMA*
 *   PIATADRUGAPIERWSZA
 *   DWUNASTAJEDENASTA*
 *   *PIECDZIESIAT*****
 *   ****DWADZIESCIA***
 *   ZEROCZTERDZIESCI**
 *   *TRZYDZIESCI*ZERO
 *   DZIESIECTRZYNASCIE
 *   JEDENASCIEDZIEWIEC
 *   *****SZESNASCIE***
 *   ***DZIEWIETNASCIE*
 *   CZTERNASCIE*SZESC*
 *   ******PIETNASCIE**
 *   OSIEMNASCIE*CZTERY
 *   PIECDWANASCIE*****
 *   SIEDEMNASCIE*PM*AM
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *-------------------------------------------------------------------------------------------------------------------------------------------
 */

#if WCLOCK24H_PL == 1

#include "tables24h-pl.h"
#include "tables-gen.h"
#include "tables-ext.h"

const DISPLAY_MODES tbl_modes[DISPLAY_MODES_COUNT] =
{
    { HM_1, MM_0,   "Polski"            },
    { HM_0, MM_1,   "Temperatur"        },
};

const uint8_t tbl_hours[HOUR_MODES_COUNT][HOUR_COUNT][MAX_HOUR_WORDS]=
{
    {   // tbl_hours[0][] = Temperature
        { WP_JEST                                                                           },      // 00:mm in Mode HM_0
        { WP_JEST                                                                           },      // 01:mm in Mode HM_0
        { WP_JEST                                                                           },      // 02:mm in Mode HM_0
        { WP_JEST                                                                           },      // 03:mm in Mode HM_0
        { WP_JEST                                                                           },      // 04:mm in Mode HM_0
        { WP_JEST                                                                           },      // 05:mm in Mode HM_0
        { WP_JEST                                                                           },      // 06:mm in Mode HM_0
        { WP_JEST                                                                           },      // 07:mm in Mode HM_0
        { WP_JEST                                                                           },      // 08:mm in Mode HM_0
        { WP_JEST                                                                           },      // 09:mm in Mode HM_0
        { WP_JEST                                                                           },      // 10:mm in Mode HM_0
        { WP_JEST                                                                           },      // 11:mm in Mode HM_0
        { WP_JEST                                                                           },      // 12:mm in Mode HM_0
        { WP_JEST                                                                           },      // 13:mm in Mode HM_0
        { WP_JEST                                                                           },      // 14:mm in Mode HM_0
        { WP_JEST                                                                           },      // 15:mm in Mode HM_0
        { WP_JEST                                                                           },      // 16:mm in Mode HM_0
        { WP_JEST                                                                           },      // 17:mm in Mode HM_0
        { WP_JEST                                                                           },      // 18:mm in Mode HM_0
        { WP_JEST                                                                           },      // 19:mm in Mode HM_0
        { WP_JEST                                                                           },      // 20:mm in Mode HM_0
        { WP_JEST                                                                           },      // 21:mm in Mode HM_0
        { WP_JEST                                                                           },      // 22:mm in Mode HM_0
        { WP_JEST                                                                           },      // 23:mm in Mode HM_0
    },
    {   // tbl_hours[1][] = Mode 1 POLSKI
        { WP_JEST, WP_DWUNASTA, WP_AM                                                       },     // 00:mm in Mode HM_1
        { WP_JEST, WP_PIERWSZA, WP_AM                                                       },     // 01:mm in Mode HM_1
        { WP_JEST, WP_DRUGA, WP_AM                                                          },     // 02:mm in Mode HM_1
        { WP_JEST, WP_TRZECIA, WP_AM                                                        },     // 03:mm in Mode HM_1
        { WP_JEST, WP_CZWARTA, WP_AM                                                        },     // 04:mm in Mode HM_1
        { WP_JEST, WP_PIATA, WP_AM                                                          },     // 05:mm in Mode HM_1
        { WP_JEST, WP_SZOSTA, WP_AM                                                         },     // 06:mm in Mode HM_1
        { WP_JEST, WP_SIODMA, WP_AM                                                         },     // 07:mm in Mode HM_1
        { WP_JEST, WP_OSMA, WP_AM                                                           },     // 08:mm in Mode HM_1
        { WP_JEST, WP_DZIEWIATA, WP_AM                                                      },     // 09:mm in Mode HM_1
        { WP_JEST, WP_DZIESIATA, WP_AM                                                      },     // 10:mm in Mode HM_1
        { WP_JEST, WP_JEDENASTA, WP_AM                                                      },     // 11:mm in Mode HM_1
        { WP_JEST, WP_DWUNASTA, WP_PM                                                       },     // 12:mm in Mode HM_1
        { WP_JEST, WP_PIERWSZA, WP_PM                                                       },     // 13:mm in Mode HM_1
        { WP_JEST, WP_DRUGA, WP_PM                                                          },     // 14:mm in Mode HM_1
        { WP_JEST, WP_TRZECIA, WP_PM                                                        },     // 15:mm in Mode HM_1
        { WP_JEST, WP_CZWARTA, WP_PM                                                        },     // 16:mm in Mode HM_1
        { WP_JEST, WP_PIATA, WP_PM                                                          },     // 17:mm in Mode HM_1
        { WP_JEST, WP_SZOSTA, WP_PM                                                         },     // 18:mm in Mode HM_1
        { WP_JEST, WP_SIODMA, WP_PM                                                         },     // 19:mm in Mode HM_1
        { WP_JEST, WP_OSMA, WP_PM                                                           },     // 20:mm in Mode HM_1
        { WP_JEST, WP_DZIEWIATA, WP_PM                                                      },     // 21:mm in Mode HM_1
        { WP_JEST, WP_DZIESIATA, WP_PM                                                      },     // 22:mm in Mode HM_1
        { WP_JEST, WP_JEDENASTA, WP_PM                                                      },     // 23:mm in Mode HM_1
    },
};

const MINUTEDISPLAY tbl_minutes[MINUTE_MODES_COUNT][MINUTE_COUNT]=
{
    {   // tbl_minutes[0][] = Mode 1 POLSKI
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_ZERO_1                              }},    // hh:00 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_JEDEN                               }},    // hh:01 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_DWA                                 }},    // hh:02 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_TRZY                                }},    // hh:03 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_CZTERY                              }},    // hh:04 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_PIEC                                }},    // hh:05 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_SZESC                               }},    // hh:06 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_SIEDEM                              }},    // hh:07 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_OSIEM                               }},    // hh:08 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_ZERO, WP_DZIEWIEC                            }},    // hh:09 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DZIESIEC                                     }},    // hh:10 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_JEDENASCIE                                   }},    // hh:11 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWANASCIE                                    }},    // hh:12 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYNASCIE                                   }},    // hh:13 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERNASCIE                                  }},    // hh:14 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIETNASCIE                                   }},    // hh:15 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_SZESNASCIE                                   }},    // hh:16 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_SIEDEMNASCIE                                 }},    // hh:17 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_OSIEMNASCIE                                  }},    // hh:18 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DZIEWIETNASCIE                               }},    // hh:19 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA                                  }},    // hh:20 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_JEDEN                        }},    // hh:21 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_DWA                          }},    // hh:22 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_TRZY                         }},    // hh:23 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_CZTERY                       }},    // hh:24 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_PIEC                         }},    // hh:25 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_SZESC                        }},    // hh:26 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_SIEDEM                       }},    // hh:27 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_OSIEM                        }},    // hh:28 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_DWADZIESCIA, WP_DZIEWIEC                     }},    // hh:29 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI                                  }},    // hh:30 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_JEDEN                        }},    // hh:31 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_DWA                          }},    // hh:32 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_TRZY                         }},    // hh:33 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_CZTERY                       }},    // hh:34 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_PIEC                         }},    // hh:35 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_SZESC                        }},    // hh:36 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_SIEDEM                       }},    // hh:37 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_OSIEM                        }},    // hh:38 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_TRZYDZIESCI, WP_DZIEWIEC                     }},    // hh:39 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI                                 }},    // hh:40 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_JEDEN                       }},    // hh:41 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_DWA                         }},    // hh:42 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_TRZY                        }},    // hh:43 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_CZTERY                      }},    // hh:44 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_PIEC                        }},    // hh:45 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_SZESC                       }},    // hh:46 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_SIEDEM                      }},    // hh:47 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_OSIEM                       }},    // hh:48 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_CZTERDZIESCI, WP_DZIEWIEC                    }},    // hh:49 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT                                 }},    // hh:50 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_JEDEN                       }},    // hh:51 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_DWA                         }},    // hh:52 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_TRZY                        }},    // hh:53 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_CZTERY                      }},    // hh:54 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_PIEC                        }},    // hh:55 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_SZESC                       }},    // hh:56 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_SIEDEM                      }},    // hh:57 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_OSIEM                       }},    // hh:58 in Mode MM_0
        { MDF_IT_IS_1 | MDF_HOUR_OFFSET_0, {WP_PIECDZIESIAT, WP_DZIEWIEC                    }},    // hh:59 in Mode MM_0
    },
    
};

const WORD_ILLUMINATION illumination[WP_COUNT]=
{
    {  0,  0,  0 },                                                                         // 00 = WP_END_OF_WORDS      = ""
    {  3, 10,  8 },                                                                         // 01 = WP_PIERWSZA
    {  3,  5,  5 },                                                                         // 02 = WP_DRUGA
    {  0,  5,  7 },                                                                         // 03 = WP_TRZECIA
    {  2,  6,  7 },                                                                         // 04 = WP_CZWARTA
    {  3,  0,  5 },                                                                         // 05 = WP_PIATA
    {  0, 12,  6 },                                                                         // 06 = WP_SZUSTA
    {  2,  0,  6 },                                                                         // 07 = WP_SIUDMA
    {  2, 13,  4 },                                                                         // 08 = WP_OSMA
    {  1,  0,  9 },                                                                         // 09 = WP_DZIEWIATA
    {  1,  9,  9 },                                                                         // 10 = WP_DZIESIATA
    {  4,  8,  9 },                                                                         // 11 = WP_JEDENASTA
    {  4,  0,  8 },                                                                         // 12 = WP_DWUNASTA
    {  7,  0,  4 },                                                                         // 13 = WP_ZERO
    {  8, 14,  4 },                                                                         // 14 = WP_ZERO_1
    { 10,  0,  5 },                                                                         // 15 = WP_JEDEN
    { 16,  4,  3 },                                                                         // 16 = WP_DWA
    {  9,  8,  4 },                                                                         // 17 = WP_TRZY
    { 15, 12,  6 },                                                                         // 18 = WP_CZTERY
    { 16,  0,  4 },                                                                         // 19 = WP_PIEC
    { 13, 12,  5 },                                                                         // 20 = WP_SZESC
    { 17,  0,  6 },                                                                         // 21 = WP_SIEDEM
    { 15,  0,  5 },                                                                         // 22 = WP_OSIEM
    { 10, 10,  8 },                                                                         // 23 = WP_DZIEWIEC
    {  9,  0,  8 },                                                                         // 24 = WP_DZIESIEC
    { 10,  0, 10 },                                                                         // 25 = WP_JEDENASCIE
    { 16,  4,  9 },                                                                         // 26 = WP_DWANASCIE
    {  9,  8, 10 },                                                                         // 27 = WP_TRZYNASCIE
    { 13,  0, 11 },                                                                         // 28 = WP_CZTERNASCIE
    { 14,  6, 10 },                                                                         // 29 = WP_PIETNASCIE
    { 11,  5, 10 },                                                                         // 30 = WP_SZESNASCIE
    { 17,  0, 12 },                                                                         // 31 = WP_SIEDEMNASCI
    { 15,  0, 11 },                                                                         // 32 = WP_OSIEMNASCIE
    { 12,  3, 14 },                                                                         // 33 = WP_DZIEWIETNASCIE
    {  6,  4, 11 },                                                                         // 34 = WP_DWADZIESCIA
    {  8,  1, 11 },                                                                         // 35 = WP_TRZYDZIESCI
    {  7,  4, 12 },                                                                         // 36 = WP_CZTERDZIESCI
    {  5,  1, 12 },                                                                         // 37 = WP_PIEDZIESIAT
    { 17, 13,  2 },                                                                         // 38 = WP_PM
    { 17, 16,  2 },                                                                         // 39 = WP_AM
    {  0,  0,  4 | ILLUMINATION_FLAG_IT_IS},                                                // 40 = WP_JEST             = "ES IT"
 };

#endif // WCLOCK24H == 1

