/*
 * Copyright (c) 2013 by Anuj Deshpande <anujdeshpande92@gmail.com>
 * Copyright (c) 2013 by Siddharth Bharat Purohit <sidbpurohit@gmail.com>
 * SPI Library for Userspace Arduino
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */
#include "SPI.h"
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "linux-virtual.h"
#include <assert.h>

static void pabort(const char *s)
{
	perror(s);
	abort();
}

SPIClass SPI;

SPIClass::SPIClass()
{
  ret = 0;
  fd = -1;
  device = "/dev/spidev1.0";
  bitOrder = 0;
  tr.len = 1;
  tr.speed_hz = 4000000; // default set to 4 Mhz
  tr.bits_per_word = 8;
  tr.cs_change = 0;
  tr.delay_usecs = 0;
  tr.pad = 0;
  tr.rx_nbits = 0;
  tr.tx_nbits = 0;
}

void SPIClass::beginTransaction(SPISettings settings)
{
    setDataMode(settings.m_dataMode);
    setBitOrder(settings.m_bitOrder);
    setClockDivider(settings.m_clock);
}

void SPIClass::endTransaction(void)
{
}

void writeMuxFile(char *fileName, char *value) 
{
  //FILE *outfile = fopen(fileName, "w");
  int outfile = open(fileName, O_SYNC | O_WRONLY);
  if (outfile >= 0) {
      write(outfile, value, strlen(value));
      close(outfile);
  }
}

void SPIClass::begin()
{
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_17_pinmux/state", "spi_cs");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_18_pinmux/state", "spi");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_21_pinmux/state", "spi");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_22_pinmux/state", "spi_sclk");

    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Can't open device");
        abort();
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &tr.bits_per_word);
    if (ret == -1)
        perror("SPI_IOC_WR_BITS_PER_WORD not set");
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tr.bits_per_word);
    if (ret == -1)
        perror("can't get bits per word");
}

void SPIClass::begin(SPISettings settings)
{
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_17_pinmux/state", "spi_cs");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_18_pinmux/state", "spi");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_21_pinmux/state", "spi");
    writeMuxFile("/sys/devices/platform/ocp/ocp:P9_22_pinmux/state", "spi_sclk");

    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("Can't open device");
        abort();
    }

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &tr.bits_per_word);
    if (ret == -1)
        perror("SPI_IOC_WR_BITS_PER_WORD not set");
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tr.bits_per_word);
    if (ret == -1)
        perror("can't get bits per word");
    setDataMode(settings.m_dataMode);
    setBitOrder(settings.m_bitOrder);
    setClockDivider(settings.m_clock);
}

byte SPIClass::transfer(byte txData)
{
    byte rxData = 0xFF;
    tr.tx_buf = (__u64) &txData;
    tr.rx_buf = (__u64) &rxData;
    tr.len = sizeof(byte);
    //tr.cs_change = 0;
    //tr.delay_usecs = 0xFFFF;
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        perror("SPI_IOC_MESSAGE not sent");

    return rxData;
}

void SPIClass::transfer(uint8_t *buf, uint32_t len, uint32_t speed,
                          uint16_t delay, uint8_t bitsPerWord, uint8_t cs_change)
{
  struct spi_ioc_transfer transfer[len];
  for (uint32_t i = 0; i < len; i++) {
    transfer[i].tx_buf = (__u64) (buf + i);
    transfer[i].rx_buf = (__u64) (buf + i);
    transfer[i].len = 1; //number of bytes in vector
    transfer[i].speed_hz = speed;
    transfer[i].delay_usecs = delay;
    transfer[i].bits_per_word = bitsPerWord;
    transfer[i].cs_change = cs_change;
    transfer[i].rx_nbits = 0;
    transfer[i].tx_nbits = 0;
    transfer[i].pad = 0;
  }
  int status = ioctl(fd, SPI_IOC_MESSAGE(len), &transfer);
  if (status < 0)
  {
      perror("SPI_IOC_MESSAGE not sent");
      /*std::string errMessage(strerror(errno));
      throw std::runtime_error("Failed to do full duplex read/write operation "
                               "on SPI Bus " + this->deviceNode + ". Error message: " +
                               errMessage);*/
  }
}

/*
void SPIClass::transfer(std::vector<std::vector<uint8_t> > &data, uint32_t speed,
                          uint16_t delay, uint8_t bitsPerWord, uint8_t cs_change)
{
    struct spi_ioc_transfer transfer[data.size()];
    int i = 0;
    for (std::vector<uint8_t> &d : data)
    {
        //see <linux/spi/spidev.h> for details!
        transfer[i].tx_buf = reinterpret_cast<__u64>(d.data());
        transfer[i].rx_buf = reinterpret_cast<__u64>(d.data());
        transfer[i].len = d.size(); //number of bytes in vector
        transfer[i].speed_hz = speed;
        transfer[i].delay_usecs = delay;
        transfer[i].bits_per_word = bitsPerWord;
        transfer[i].cs_change = cs_change;
        i++;
    }
    int status = ioctl(fd, SPI_IOC_MESSAGE(data.size()), &transfer);
    if (status < 0)
    {
        perror("SPI_IOC_MESSAGE not sent");
//        std::string errMessage(strerror(errno));
//        throw std::runtime_error("Failed to do full duplex read/write operation "
//                                 "on SPI Bus " + this->deviceNode + ". Error message: " +
//                                 errMessage);
    }
}
*/

void SPIClass::setBitOrder(uint8_t bOrder)
{
    if (bOrder == LSBFIRST) {
        bitOrder = SPI_LSB_FIRST; // TODO: this fails as the HW doesn't support it
                                  // code could be modified to do a bit reversal instead
    }
    else if (bOrder == MSBFIRST) {
        bitOrder = 0;
    }
    else {
        assert(0); // invalid bOrder provided
    }

    if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &bitOrder) < 0) {
        perror("Failed to set SPI bit justification\n");
    }
}

void SPIClass::setDataMode(uint8_t mode)
{
    if ((mode == SPI_MODE0) | (mode == SPI_MODE1) |
        (mode == SPI_MODE2) | (mode == SPI_MODE3)
        ) {
      /*
       * spi mode
        ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
        if (ret == -1)
          pabort("can't set spi mode");

        ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
        if (ret == -1)
          pabort("can't get spi mode");
       */
        ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
        if (ret == -1)
          perror("SPI_IOC_WR_MODE not set");
        ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
        if (ret == -1)
          perror("can't get spi mode");
        uint32_t mode32;
        ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode32);
        if (ret == -1)
          perror("can't get spi mode");
        m_mode32 = mode32;
    } else {
        perror("INVALID DATA_MODE set");
    }
}

void SPIClass::setClockDivider(uint32_t rate) {
   if (rate > 0) {
    tr.speed_hz = rate;
  	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &rate);
    if (ret == -1)
        perror("SPI_IOC_WR_MAX_SPEED_HZ not set");
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &rate);
    if ((ret == -1) || (tr.speed_hz != rate))
        pabort("can't get max speed hz");
  }
  else
  	perror("INVALID SPI_CLOCK_DIV set");
}

void SPIClass::end() {
  close(fd);
}
