/*
 * Copyright (c) 2013 by Anuj Deshpande <anujdeshpande92@gmail.com>
 * SPI Library for Userspace Arduino
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#ifndef _SPI_H
#define _SPI_H
//#include <iostream>
//#include <vector>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "linux-virtual.h"

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00
#define SPI_MODE1 0x04
#define SPI_MODE2 0x08
#define SPI_MODE3 0x0C

class SPISettings {
public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }
  SPISettings() {
    init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0);
  }
private:
  void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
      m_clock = clock;
      m_bitOrder = bitOrder;
      m_dataMode = dataMode;
  }
  uint32_t m_clock;
  uint8_t m_bitOrder;
  uint8_t m_dataMode;
  friend class SPIClass;
};


class SPIClass {
		int ret;
		int fd;
		const char *device;
		int bitOrder;
		struct spi_ioc_transfer tr;
		uint32_t m_mode32;
public:
		SPIClass();
		byte transfer(byte _data);
		void begin();
		void begin(SPISettings settings);
		void end();
		void setBitOrder(uint8_t);
		void setDataMode(uint8_t);
		void setClockDivider(uint32_t);
    void beginTransaction(SPISettings settings);
    void endTransaction(void);
    /*void transfer(std::vector<std::vector<uint8_t> > &data,
                  uint32_t speed,
                  uint16_t delay,
                  uint8_t  bitsPerWord,
                  uint8_t cs_change);*/
    void transfer(uint8_t *buf, uint32_t len, uint32_t speed,
                          uint16_t delay, uint8_t bitsPerWord, uint8_t cs_change);

};
extern SPIClass SPI;
#endif
