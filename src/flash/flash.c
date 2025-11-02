/*------------------------------------------------------------------------------------------------------------------------------------
 * flash.c - emulate an EEPROM with FLASH memory and cache the data
 *
 * W25Q16 - 16 MBit -> 2 Mbyte -> 8192 pages of 256 bytes
 *       512 sectors of 4096 bytes
 *      8192 pages of 256 bytes
 *
 * Here we use only 4 sectors: 16384 bytes
 *
 * Copyright (c) 2018-2025 Frank Meyer - frank(at)uclock.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *------------------------------------------------------------------------------------------------------------------------------------
 */
#if defined(BLACK_BOARD)                                    // flash only on STM32F407 Black Board

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "w25qxx.h"
#include "flash.h"

#define SECTORSIZE                  4096                    // sector size of flash
#define PAGESIZE                    256                     // page size of flash
#define PAGES_PER_SECTOR            16                      // 16 * 256 = 4096
#define SECTORS                     4                       // hold 4 sectors (4 * 4096 = 16384) in RAM
#define FLUSH_TIME                  5                       // flush flash 5 seconds after last cache update

static uint8_t                      cache[SECTORS * SECTORSIZE];
static uint8_t                      sector_is_dirty[SECTORS];
static uint32_t                     last_update;            // timestamp of last update

uint_fast8_t                        flash_is_up             = 0;

/*------------------------------------------------------------------------------------------------------------------------------------
 * flash_flush () - flush all dirty cache slots
 *------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
flash_flush (uint_fast8_t force_flush)
{
    uint32_t    sector;
    uint32_t    page;
    uint32_t    idx;

    if (force_flush || (last_update > 0 && last_update + FLUSH_TIME < uptime))
    {                                                                                   // flush cache x seconds after last cache update
        for (sector = 0; sector < SECTORS; sector++)
        {
            if (sector_is_dirty[sector])
            {
                w25qxx_erase_sector (sector);                                           // set 4096 bytes to 0xFF

                for (page = 0; page < PAGES_PER_SECTOR; page++)
                {
                    uint8_t * p = cache + sector * SECTORSIZE + page;                   // find a byte != 0xff within the cache

                    for (idx = 0; idx < PAGESIZE; idx++)
                    {
                        if (*p != 0xFF)
                        {
                            break;                                                      // found, we must write the page
                        }
                        p++;
                    }

                    if (idx < PAGESIZE)
                    {
                        w25qxx_write_page (cache + sector * SECTORSIZE + page * PAGESIZE, sector, page);
                    }
                }
                sector_is_dirty[sector] = 0;
            }
        }

        last_update = 0;                                                                // reset timestamp of last cache update
    }

    return 1;
}

/*------------------------------------------------------------------------------------------------------------------------------------
 * flash_read () - read data into buffer
 *------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
flash_read (uint32_t start_addr, uint8_t * buffer, uint32_t cnt)
{
    if (start_addr < SECTORS * SECTORSIZE)                                                      // start address valid?
    {
        if (start_addr + cnt > SECTORS * SECTORSIZE)                                            // data behind cache size?
        {
            cnt = SECTORS * SECTORSIZE - start_addr;
        }

        memcpy (buffer, cache + start_addr, cnt);
        return 1;
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------
 * flash_write () - write buffer into cache
 *------------------------------------------------------------------------------------------------------------------------------------
 */
uint_fast8_t
flash_write (uint32_t start_addr, uint8_t * buffer, uint32_t cnt)
{
    uint32_t   start_sector;
    uint32_t   end_sector;
    uint32_t   sector;

    if (start_addr < SECTORS * SECTORSIZE)                                                      // start address valid?
    {
        if (start_addr + cnt > SECTORS * SECTORSIZE)                                            // data behind cache size?
        {
            cnt = SECTORS * SECTORSIZE - start_addr;
        }

        start_sector = start_addr / SECTORSIZE;
        end_sector   = (start_addr + cnt - 1) / SECTORSIZE;

        if (memcmp (cache + start_addr, buffer, cnt) != 0)
        {
            memcpy (cache + start_addr, buffer, cnt);

            for (sector = start_sector; sector <= end_sector; sector++)
            {
                sector_is_dirty[sector] = 1;
            }
            last_update = uptime;
        }

        return 1;
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------
 * flash_init () - initialize flash routines
 *------------------------------------------------------------------------------------------------------------------------------------
 */
void
flash_init (void)
{
    uint32_t    sector;
    uint32_t    page;
    uint32_t    offset = 0;

    w25qxx_init ();
    flash_is_up = 1;

    for (sector = 0; sector < SECTORS; sector++)                                        // copy sectors into RAM
    {
        for (page = 0; page < PAGES_PER_SECTOR; page++)
        {
            w25qxx_read_page (cache + offset, sector, page);
            offset += PAGESIZE;
        }
    }
}

#endif // BLACK_BOARD
