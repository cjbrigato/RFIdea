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

#include "LCD_DISCO_F469NI.h"

// Constructor
LCD_DISCO_F469NI::LCD_DISCO_F469NI()
{
  BSP_LCD_Init();
  BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
  BSP_LCD_SetFont(&Font20);
  BSP_LCD_Clear(LCD_COLOR_WHITE);
}

// Destructor
LCD_DISCO_F469NI::~LCD_DISCO_F469NI()
{

}

//=================================================================================================================
// Public methods
//=================================================================================================================

uint8_t LCD_DISCO_F469NI::Init(void)
{
  return BSP_LCD_Init();
}

uint8_t LCD_DISCO_F469NI::InitEx(LCD_OrientationTypeDef orientation)
{
  return BSP_LCD_InitEx(orientation);
}

void LCD_DISCO_F469NI::Reset(void)
{
  BSP_LCD_Reset();
}

uint32_t LCD_DISCO_F469NI::GetXSize(void)
{
  return BSP_LCD_GetXSize();
}

uint32_t LCD_DISCO_F469NI::GetYSize(void)
{
  return BSP_LCD_GetYSize();
}

void LCD_DISCO_F469NI::SetXSize(uint32_t imageWidthPixels)
{
  BSP_LCD_SetXSize(imageWidthPixels);
}

void LCD_DISCO_F469NI::SetYSize(uint32_t imageHeightPixels)
{
  BSP_LCD_SetYSize(imageHeightPixels);
}

void LCD_DISCO_F469NI::LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
  BSP_LCD_LayerDefaultInit(LayerIndex, FB_Address);
}

void LCD_DISCO_F469NI::SelectLayer(uint32_t LayerIndex)
{
  BSP_LCD_SelectLayer(LayerIndex);
}

void LCD_DISCO_F469NI::SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  BSP_LCD_SetLayerVisible(LayerIndex, State);
}

void LCD_DISCO_F469NI::SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
  BSP_LCD_SetTransparency(LayerIndex, Transparency);
}

void LCD_DISCO_F469NI::SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  BSP_LCD_SetLayerAddress(LayerIndex, Address);
}

void LCD_DISCO_F469NI::SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_SetLayerWindow(LayerIndex, Xpos, Ypos, Width, Height);
}

void LCD_DISCO_F469NI::SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{
  BSP_LCD_SetColorKeying(LayerIndex, RGBValue);
}

void LCD_DISCO_F469NI::ResetColorKeying(uint32_t LayerIndex)
{
  BSP_LCD_ResetColorKeying(LayerIndex);
}

void LCD_DISCO_F469NI::SetTextColor(uint32_t Color)
{
  BSP_LCD_SetTextColor(Color);
}

uint32_t LCD_DISCO_F469NI::GetTextColor(void)
{
  return BSP_LCD_GetTextColor();
}

void LCD_DISCO_F469NI::SetBackColor(uint32_t Color)
{
  BSP_LCD_SetBackColor(Color);
}

uint32_t LCD_DISCO_F469NI::GetBackColor(void)
{
  return BSP_LCD_GetBackColor();
}

void LCD_DISCO_F469NI::SetFont(sFONT *fonts)
{
  BSP_LCD_SetFont(fonts);
}

sFONT *LCD_DISCO_F469NI::GetFont(void)
{
  return BSP_LCD_GetFont();
}

uint32_t LCD_DISCO_F469NI::ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
  return BSP_LCD_ReadPixel(Xpos, Ypos);
}

void LCD_DISCO_F469NI::Clear(uint32_t Color)
{
  BSP_LCD_Clear(Color);
}

void LCD_DISCO_F469NI::ClearStringLine(uint32_t Line)
{
  BSP_LCD_ClearStringLine(Line);
}

void LCD_DISCO_F469NI::DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
  BSP_LCD_DisplayChar(Xpos, Ypos, Ascii);
}

void LCD_DISCO_F469NI::DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
  BSP_LCD_DisplayStringAt(Xpos, Ypos, Text, Mode);
}

void LCD_DISCO_F469NI::DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
  BSP_LCD_DisplayStringAtLine(Line, ptr);
}

void LCD_DISCO_F469NI::DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  BSP_LCD_DrawHLine(Xpos, Ypos, Length);
}

void LCD_DISCO_F469NI::DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  BSP_LCD_DrawVLine(Xpos, Ypos, Length);
}

void LCD_DISCO_F469NI::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  BSP_LCD_DrawLine(x1, y1, x2, y2);
}

void LCD_DISCO_F469NI::DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_DrawRect(Xpos, Ypos, Width, Height);
}

void LCD_DISCO_F469NI::DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  BSP_LCD_DrawCircle(Xpos, Ypos, Radius);
}

void LCD_DISCO_F469NI::DrawPolygon(pPoint Points, uint16_t PointCount)
{
  BSP_LCD_DrawPolygon(Points, PointCount);
}

void LCD_DISCO_F469NI::DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  BSP_LCD_DrawEllipse(Xpos, Ypos, XRadius, YRadius);
}

void LCD_DISCO_F469NI::DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
  BSP_LCD_DrawBitmap(Xpos, Ypos, pbmp);
}

void LCD_DISCO_F469NI::FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  BSP_LCD_FillRect(Xpos, Ypos, Width, Height);
}

void LCD_DISCO_F469NI::FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  BSP_LCD_FillCircle(Xpos, Ypos, Radius);
}

void LCD_DISCO_F469NI::FillPolygon(pPoint Points, uint16_t PointCount)
{
  BSP_LCD_FillPolygon(Points, PointCount);
}

void LCD_DISCO_F469NI::FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
  BSP_LCD_FillEllipse(Xpos, Ypos, XRadius, YRadius);
}

void LCD_DISCO_F469NI::DisplayOn(void)
{
  BSP_LCD_DisplayOn();
}

void LCD_DISCO_F469NI::DisplayOff(void)
{
  BSP_LCD_DisplayOff();
}

void LCD_DISCO_F469NI::DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
  BSP_LCD_DrawPixel(Xpos, Ypos, RGB_Code);
}

//=================================================================================================================
// Private methods
//=================================================================================================================
