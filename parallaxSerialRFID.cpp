/*
 * parallaxSerialRFID.cpp
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
 * Implementation for Parallax Serial RFID reader on the Intel Edison platform
 *
 * Depends:
 * Relies on libmraa (https://github.com/intel-iot-devkit/mraa).  Currently
 * configured to work with Edison, working on a port to Raspberry Pi.
 *
 */

#include "parallaxSerialRFID.h"
#include "mraa.hpp"
#include <algorithm>

/******************************************************************************
* HELPERS
******************************************************************************/
string hexStrToIntStr(string hexStr) {
  uint32_t uint;
  stringstream ss;
  ss << hex << hexStr;
  ss >> uint;
  stringstream si;
  si << static_cast<uint32_t>(uint);
  return si.str();
}

/******************************************************************************
* CONSTRUCTORS & DESTRUCTOR
******************************************************************************/

/** default constructor, uses UART1 & pin 2 as enable **/
parallaxSerialRFID::parallaxSerialRFID() {
  try {
    readerUart = new mraa::Uart(0);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  ena = new mraa::Gpio(1);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  }
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  this->flush();
  return;
}

/** constructor taking enable gpio pin (int) as argument, uses UART1 **/
parallaxSerialRFID::parallaxSerialRFID(int _gpioNum) {
  try {
    readerUart = new mraa::Uart(0);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  ena = new mraa::Gpio(_gpioNum);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  }
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  this->flush();
  return;
}

/** constructor taking both custom UART port & enable pin as int **/
parallaxSerialRFID::parallaxSerialRFID(int _gpioNum, int _uartNum) {
  try {
    readerUart = new mraa::Uart(_uartNum);
  } catch (exception& e) {
    cerr << e.what() << "Something wrong with UART platform config" << endl;
  }
  if (readerUart->setBaudRate(2400) != mraa::SUCCESS) {
    cerr << "Error setting BaudRate on UART" << endl;
  }
  if (readerUart->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS) {
    cerr << "Error setting parity on UART" << endl;
  }
  if (readerUart->setFlowcontrol(false, false) != mraa::SUCCESS) {
    cerr << "Error setting flow control UART" << endl;
  }
  ena = new mraa::Gpio(_gpioNum);
  if (ena == NULL) {
    cerr << "Error setting up enable pin" << endl;
  }
  if (ena->dir(mraa::DIR_OUT) != mraa::SUCCESS) {
    cerr << "Error setting enable pin direction" << endl;
  }
  ena->write(1);
  this->flush();
  return;
}

/** destructor, destroys all contexts **/
parallaxSerialRFID::~parallaxSerialRFID() {
  delete readerUart;
  delete ena;
  return;
}

/******************************************************************************
* MEMBER FUNCTIONS
******************************************************************************/

/** enable the RFID reader to read tags & clear UART buffer **/
void parallaxSerialRFID::enable() {
  readerUart->flush();
  ena->write(0);
  return;
}

/** disable the RFID reader from reading tags **/
void parallaxSerialRFID::disable() {
  ena->write(1);
  return;
}

/** returns true if a tag has been scanned **/
bool parallaxSerialRFID::tagScanned() {
  return readerUart->dataAvailable();
}

/** returns the read tag number as an unsigned integer string **/
string parallaxSerialRFID::readTag() {
  lastRawTag = readerUart->readStr(TAGLENGTH);
  if(DEBUG) std::cout << "Raw: " << lastRawTag << std::endl;
  if (lastRawTag.length() >= 3) {
    string ret = hexStrToIntStr(lastRawTag.substr(3));
    readerUart->flush();
    return ret;
  }
  else {
    readerUart->flush();
    return " - ";
  }
}

void parallaxSerialRFID::flush() {
  while(readerUart->dataAvailable()) {
    readerUart->readStr(1);
    readerUart->flush();
  }
  return;
}

bool parallaxSerialRFID::isNotGibberishTag() {
  size_t n0 = std::count(lastRawTag.begin(),lastRawTag.end(),'0');
  size_t nc = std::count(lastRawTag.begin(),lastRawTag.end(),'C');
  size_t nf = std::count(lastRawTag.begin(),lastRawTag.end(),'F');
  //size_t n1 = std::count(lastRawTag.begin(),lastRawTag.end(),'1');
  //size_t n7 = std::count(lastRawTag.begin(),lastRawTag.end(),'7');

  if (((n0 + nc + nf) == 10) || (n0 >= 7)) {
    if(DEBUG) std::cout << "  Bad Read: " << lastRawTag;
    return 0;
  }
  else return 1;
}
