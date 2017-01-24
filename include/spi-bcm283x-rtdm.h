/**
 * Copyright (C) 2016 Nicolas Schurando <schurann@ext.essilor.com>
 * Copyright (C) 2017 Corneliu Zuzu <corneliu.zuzu@arobs.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef BCM283X_SPI_RTDM_H
#define BCM283X_SPI_RTDM_H

/**
 * Maximum size for transmit and receive buffers.
 */
#define BCM283X_SPI_BUFFER_SIZE_MAX 1024

/**
 * IOCTL request for changing the SPI bit order.
 */
#define BCM283X_SPI_SET_BIT_ORDER 0

/**
 * IOCTL request for changing the SPI data mode.
 */
#define BCM283X_SPI_SET_DATA_MODE 1

/**
 * IOCTL request for changing the SPI bus speed.
 */
#define BCM283X_SPI_SET_SPEED 2

/**
 * IOCTL request for changing the SPI chip select polarity.
 */
#define BCM283X_SPI_SET_CS_POLARITY 3

/**
 * SPI chip select polarity.
 */
typedef enum
{
	BCM283X_SPI_CS_POL_LOW = 0,
	BCM283X_SPI_CS_POL_HIGH = 1
} bcm283x_spi_cs_polarity_e;

/**
 * SPI data bit ordering.
 */
typedef enum
{
	BCM283X_SPI_BIT_ORDER_LSBFIRST = 0,
	BCM283X_SPI_BIT_ORDER_MSBFIRST = 1
} bcm283x_spi_bit_order_e;

/**
 * SPI data mode.
 *  - Mode 0 : CPOL = 0, CPHA = 0
 *  - Mode 1 : CPOL = 0, CPHA = 1
 *  - Mode 2 : CPOL = 1, CPHA = 0
 *  - Mode 3 : CPOL = 1, CPHA = 1
 */
typedef enum
{
	BCM283X_SPI_DATA_MODE_0 = 0,
	BCM283X_SPI_DATA_MODE_1 = 1,
	BCM283X_SPI_DATA_MODE_2 = 2,
	BCM283X_SPI_DATA_MODE_3 = 3
} bcm283x_spi_mode_e;

#define BCM283X_SPI_KHZ                 (1000)
#define BCM283X_SPI_MHZ                 (1000000)
#define BCM283X_SPI_SPEED_KHZ(s)        ((double)s * BCM283X_SPI_KHZ)
#define BCM283X_SPI_SPEED_MHZ(s)        ((double)s * BCM283X_SPI_MHZ)

/*
 * As per: http://raspberrypi.stackexchange.com/questions/699/what-spi-frequencies-does-raspberry-pi-support
 * "
 *  The Raspberry Pi SPI runs at APB clock speed, which is equivalent to
 *  core clock speed, 250 MHz. This can be divided by _any even number from
 *  2 to 65536_ for the desired speed. Odd numbers are rounded down, and
 *  0 (or 1) is equivalent to 65536.
 *  This makes the frequency range be from 3.814 kHz to 125 MHz, with 32768
 *  steps in between.
 * "
 */
#define BCM283X_SPI_MIN_SPEED           (3814.69)
#define BCM283X_SPI_MAX_SPEED           BCM283X_SPI_SPEED_MHZ(125)

static inline __attribute__((always_inline))
int bcm283x_spi_speed_to_clock_divider(double spi_speed)
{
    double d;
    int divider;

    if ( spi_speed < BCM283X_SPI_MIN_SPEED ||
         spi_speed > BCM283X_SPI_MAX_SPEED )
        return -1;

    /* Calculate divider. */
    d = BCM283X_SPI_MAX_SPEED / spi_speed;
    divider = (int) d + ( (d - (int) d) < 0.5 ? 0 : 1 ); /* round */
    divider = (divider << 1) & 0xFFFF;

    return divider;
}

#endif /* BCM283X_SPI_RTDM_H */
