/*
 * parallaxSerialRFID.h
 *
 *                ======================================
 *
 * Author: Nile Mittow <Nile@HackerLab.org>
 * Copyright (c) 2018, Nile Mittow
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *                ======================================
 *
 * Description:
 * Interface for Parallax Serial RFID reader on the Intel Edison platform
 *
 * Depends:
 * Relies on libmraa (https://github.com/intel-iot-devkit/mraa).  Currently
 * configured to work with Edison, working on a port to Raspberry Pi.
 *
 */

#ifndef PARALLAXSERIALRFID_H
#define PARALLAXSERIALRFID_H

#include <string>
#include <map>
#include <exception>
#include <iostream>
#include <sstream>
#include "mraa.hpp"

#define TAGLENGTH 12
#define DEBUG 1

using namespace std;

class parallaxSerialRFID {

  private:

    /** mraa UART context object for the RFID reader **/
    mraa::Uart* readerUart;

    /** mraa GPIO context object for the RFID reader's enable pin **/
    mraa::Gpio* ena;

    string lastRawTag;

  public:

    /**************************************************************************
     * CONSTRUCTORS & DESTRUCTOR
     *************************************************************************/

    /** default constructor, uses UART1 & pin 2 as enable **/
    parallaxSerialRFID();

    /** constructor taking enable gpio pin (int) as argument, uses UART1 **/
    parallaxSerialRFID(int _gpioNum);

    /** constructor taking both custom UART port & enable pin as int **/
    parallaxSerialRFID(int _gpioNum, int _uartNum);

    /** destructor, destroys all contexts **/
    ~parallaxSerialRFID();

    /**************************************************************************
     * MEMBER FUNCTIONS
     *************************************************************************/

    /** enable the RFID reader to read tags & clear UART buffer **/
    void enable();

    /** disable the RFID reader from reading tags **/
    void disable();

    /** returns true if a tag of length TAGLENGTH has been scanned **/
    bool tagScanned();

    /** returns the read tag number as a string **/
    string readTag();

    void flush();

    bool isNotGibberishTag();

}; /* End parallaxSerialRFID */

#endif
