/*
 * parallaxSerialRFID-example.cpp
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
 *  	list of conditions and the following disclaimer.
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
 * Example code for using the parallaxSerialRFID Library on the Intel Edison
 * Arduino Breakout platform.
 *
 * Pins:
 * RFID      Edison Arduino Breakout
 * ======    =======================
 * VCC       5V
 * /ENABLE   1
 * SOUT      0
 * GND       GND
 *
 */

#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "parallaxSerialRFID.h"
#define RFID_ENABLE_PIN 1

parallaxSerialRFID rfid(RFID_ENABLE_PIN);

void cleanExit(int signum) {
  rfid.disable();
  if (DEBUG) std::cout << "Exiting..." << std::endl;
  exit(signum);
}

int main() {

  signal(SIGINT, cleanExit);
  sleep(2);
  rfid.enable();
  std::cout << "Listening..." << std::endl;
  while(1) {
    if(rfid.tagScanned()) {
      rfid.disable();
      string tag = rfid.readTag();
      std::cout << "Tag Read: " << tag << endl;
      sleep(3);
      rfid.flush();
      rfid.enable();
    }
    usleep(500);
  }
  return 0;
}
