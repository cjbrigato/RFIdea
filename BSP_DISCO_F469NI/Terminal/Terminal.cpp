/* mbed Terminal Library, for ANSI/VT200 Terminals and ecape codes
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Terminal.h"

#include "mbed.h"

Terminal::Terminal(PinName tx, PinName rx) : Serial(tx, rx) {}

void Terminal::cls() {
    this->printf("\033[2J");
}

void Terminal::locate(int column, int row) {
    // Cursor Home    <ESC>[{ROW};{COLUMN}H
    this->printf("\033[%d;%dH%c", row + 1, column + 1);
}

static int rgb888tobgr111(int colour) {
    int r = (colour >> 23) & 1;
    int g = (colour >> 15) & 1;
    int b = (colour >> 7) & 1;
    return (b << 2) | (g << 1) | (r << 0);
}

void Terminal::foreground(int colour) {
    // Set Attribute Mode    <ESC>[{n}m
    // Foreground Colours : 30 + bgr
    int c = 30 + rgb888tobgr111(colour);
    this->printf("\033[%dm", c);
}

void Terminal::background(int colour) {
    // Set Attribute Mode    <ESC>[{n}m
    // Background Colours : 40 + bgr
    int c = 40 + rgb888tobgr111(colour);
    this->printf("\033[%dm", c);
}
