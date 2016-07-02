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

#include "TS_DISCO_F469NI.h"

// Constructor
TS_DISCO_F469NI::TS_DISCO_F469NI()
{
  BSP_TS_Init(100, 100);
}

// Destructor
TS_DISCO_F469NI::~TS_DISCO_F469NI()
{

}

//=================================================================================================================
// Public methods
//=================================================================================================================

uint8_t TS_DISCO_F469NI::Init(uint16_t ts_SizeX, uint16_t ts_SizeY)
{
  return BSP_TS_Init(ts_SizeX, ts_SizeY);
}

uint8_t TS_DISCO_F469NI::ITConfig(void)
{
  return BSP_TS_ITConfig();
}

uint8_t TS_DISCO_F469NI::GetState(TS_StateTypeDef *TS_State)
{
  return BSP_TS_GetState(TS_State);
}

#if (TS_MULTI_TOUCH_SUPPORTED == 1)

uint8_t TS_DISCO_F469NI::Get_GestureId(TS_StateTypeDef *TS_State)
{
  return BSP_TS_Get_GestureId(TS_State);
}

uint8_t TS_DISCO_F469NI::ResetTouchData(TS_StateTypeDef *TS_State)
{
  return BSP_TS_ResetTouchData(TS_State);
}

#endif // (TS_MULTI_TOUCH_SUPPORTED == 1)

//=================================================================================================================
// Private methods
//=================================================================================================================
