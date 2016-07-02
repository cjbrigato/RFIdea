/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef __TS_DISCO_F469NI_H
#define __TS_DISCO_F469NI_H

#ifdef TARGET_DISCO_F469NI

#include "mbed.h"
#include "stm32469i_discovery_ts.h"

/*
  This class drives the touch screen module ( device) of the LCD display
  present on DISCO_F469NI board.

  Usage:

  #include "mbed.h"
  #include "TS_DISCO_F469NI.h"

  TS_DISCO_F469NI ts;

  DigitalOut led1(LED1);

  int main()
  {
      TS_StateTypeDef TS_State;
    
      ts.Init(420, 272);
    
      while(1)
      {
        ts.GetState(&TS_State);
        if ((TS_State.touchDetected) && (TS_State.touchX[0] > 240))
        {
          led1 = 1;
        }
        else
        {
          led1 = 0;
        }
      }
  }
*/
class TS_DISCO_F469NI
{
  
public:
  //! Constructor
  TS_DISCO_F469NI();

  //! Destructor
  ~TS_DISCO_F469NI();

  /**
    * @brief  Initializes and configures the touch screen functionalities and
    *         configures all necessary hardware resources (GPIOs, I2C, clocks..);.
    * @param  ts_SizeX : Maximum X size of the TS area on LCD
    * @param  ts_SizeY : Maximum Y size of the TS area on LCD
    * @retval TS_OK if all initializations are OK. Other value if error.
    */
  uint8_t Init(uint16_t ts_SizeX, uint16_t ts_SizeY);

  /**
    * @brief  Configures and enables the touch screen interrupts both at GPIO level and
    * in TouchScreen IC driver configuration.
    * @retval TS_OK if all initializations are OK.
    */
  uint8_t ITConfig(void);

  /**
    * @brief  Returns status and positions of the touch screen.
    * @param  TS_State: Pointer to touch screen current state structure
    * @retval TS_OK if all initializations are OK. Other value if error.
    */
  uint8_t GetState(TS_StateTypeDef *TS_State);

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

  /**
    * @brief  Update gesture Id following a touch detected.
    * @param  TS_State: Pointer to touch screen current state structure
    * @retval TS_OK if all initializations are OK. Other value if error.
    */
  uint8_t Get_GestureId(TS_StateTypeDef *TS_State);

  /**
    * @brief  Function used to reset all touch data before a new acquisition
    *         of touch information.
    * @param  TS_State: Pointer to touch screen current state structure
    * @retval TS_OK if OK, TE_ERROR if problem found.
    */
  uint8_t ResetTouchData(TS_StateTypeDef *TS_State);

#endif // (TS_MULTI_TOUCH_SUPPORTED == 1)

private:

};

#else
#error "This class must be used with DISCO_F469NI board only."
#endif // TARGET_DISCO_F469NI

#endif  
