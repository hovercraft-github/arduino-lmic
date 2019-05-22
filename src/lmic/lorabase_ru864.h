/*
* Copyright (c) 2014-2016 IBM Corporation.
* All rights reserved.
*
* Copyright (c) 2017 MCCI Corporation
* All rights reserved.
*
* Copyright (c) 2019 hovercraft-github (https://github.com/hovercraft-github).
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  * Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*  * Neither the name of the <organization> nor the
*    names of its contributors may be used to endorse or promote products
*    derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _lorabase_ru864_h_
#define _lorabase_ru864_h_

#ifndef _LMIC_CONFIG_PRECONDITIONS_H_
# include "lmic_config_preconditions.h"
#endif

/****************************************************************************\
|
| Basic definitions for RU864 (always in scope)
|
\****************************************************************************/

enum _dr_ru864_t {
        RU864_DR_SF12 = 0,      // DR0
        RU864_DR_SF11,          // DR1
        RU864_DR_SF10,          // DR2
        RU864_DR_SF9,           // DR3
        RU864_DR_SF8,           // DR4
        RU864_DR_SF7,           // DR5
        RU864_DR_SF7B,          // DR6 250kHz
        RU864_DR_FSK,           // DR7
        RU864_DR_NONE
};

// Band max EIRP attribution for channels:
//  g1 :   1%  14dBm
//  g2 : 0.1%  14dBm
//  g3 :  10%  20dBm
//
/*enum { // Corresponds to https://www.smartiko.ru/about/chastotnyy-plan/
        RU864_F1 = 868900000,      	//  g2	SF7-12 (DR0-5)
        RU864_J1 = RU864_F1,		//  g3	used for join, ping
        RU864_F2 = 869100000,      	//  g2	SF7-12 (DR0-5)
        RU864_J2 = RU864_F2,		//  g3	used for join, beacon, RX2
        RU864_F3 = 864100000,      	//  g2	SF7-12 (DR0-5)
        RU864_F4 = 864300000,      	//  g2	SF7-12 (DR0-5)
        RU864_F5 = 864500000,      	//  g2	SF7-12 (DR0-5)
        RU864_F6 = 864700000,      	//  g2	SF7-12 (DR0-5)
        RU864_F7 = 864900000,      	//  g2	SF7-12 (DR0-5)
        //RU864_F8 = 864600000,      	//  g2	SF7B / 250 kHz
        //RU864_FSK = 864800000,        //  g2	FSK 250 kHz, 50kbps
};*/
enum { // Corresponds to Vega/ttn plan
        RU864_F1 = 864100000,           //  g2  SF7-12 (DR0-5)
        RU864_F2 = 864300000,           //  g2  SF7-12 (DR0-5)
        RU864_F3 = 864500000,           //  g2  SF7-12 (DR0-5)
        RU864_F4 = 864700000,           //  g2  SF7-12 (DR0-5)
        RU864_F5 = 864900000,           //  g2  SF7-12 (DR0-5)
        RU864_F6 = 869100000,           //  g2  SF7-12 (DR0-5)
        RU864_J1 = RU864_F6,            //  g3  used for join, beacon, RX2
        RU864_F7 = 868900000,           //  g2  SF7-12 (DR0-5)
        RU864_J2 = RU864_F7,            //  g3  used for join, ping
        //RU864_F8 = 864600000,         //  g2  SF7B / 250 kHz
        //RU864_FSK = 864800000,        //  g2  FSK 250 kHz, 50kbps
};
enum {
        RU864_FREQ_B1B = 864000000,
        RU864_FREQ_B1T = 865000000,
        RU864_FREQ_B2B = 866000000,
        RU864_FREQ_B2T = 868000000,
        RU864_FREQ_B3B = 868700000,
        RU864_FREQ_B3T = 869200000
};

// max power: 14dBM (this constant only influence join operation currently)
enum {
        RU864_TX_EIRP_MAX_DBM = 14      // 14 dBm
};

enum { DR_PAGE_RU864 = 0x10 * (LMIC_REGION_ru864 - 1) };

enum { RU864_LMIC_REGION_EIRP = 1 };         // region uses EIRP

#endif /* _lorabase_ru864_h_ */
