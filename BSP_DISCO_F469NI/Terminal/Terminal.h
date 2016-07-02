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

#include "mbed.h"

#ifndef MBED_TERMINAL_H
#define MBED_TERMINAL_H

/** Control and display on an ANSI/VT100 Terminal
 *
 * This allows you to control an ANSI serial terminal
 */ 
class Terminal : public Serial {
public:
    /** Create the Terminal interface 
     * 
     * @param tx Serial transmit
     * @param rx Serial recieve
     */    
    Terminal(PinName tx, PinName rx);

#if DOXYGEN_ONLY
    /** Write a character to the terminal
     *
     * @param c The character to write to the display
     */
    int putc(int c);

    /** Write a formated string to the terminal
     *
     * @param format A printf-style format string, followed by the
     *               variables to use in formating the string.
     */
    int printf(const char* format, ...);
    
    // also baud etc
#endif

    /** Locate to a screen column and row
     *
     * @param column  The horizontal position from the left, indexed from 0
     * @param row     The vertical position from the top, indexed from 0
     */
    void locate(int column, int row);

    /** Clear the screen and locate to 0,0 */
    void cls();

    /** Set the foreground colour */
    void foreground(int colour);

    /** Set the background colour */
    void background(int colour);
};

#endif
