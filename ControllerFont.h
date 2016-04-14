//--------------------------------------------------------------------------------------
// File: ControllerFont.h
//
// Class for compositing text with Xbox controller font button sprites
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//-------------------------------------------------------------------------------------

#pragma once

#include "SpriteBatch.h"
#include "SpriteFont.h"


namespace DX
{
    enum class ControllerFont : wchar_t
    {
        LeftThumb = L' ',
        DPad = L'!',
        RightThumb = L'\"',
        Back = L'#',
        Guide = L'$',
        Start = L'%',
        XButton = L'&',
        AButton = L'\'',
        YButton = L'(',
        BButton = L')',
        RightShoulder = L'*',
        RightTrigger = L'+',
        LeftTrigger = L',',
        LeftShoulder = L'-',
    };

    inline void XM_CALLCONV DrawControllerString(_In_ DirectX::SpriteBatch* spriteBatch, _In_ DirectX::SpriteFont* textFont, _In_ DirectX::SpriteFont* butnFont,
                                                 _In_z_ wchar_t const* text, DirectX::XMFLOAT2 const& position, DirectX::FXMVECTOR color = DirectX::Colors::White, float scale = 1)
    {
        using namespace DirectX;

        size_t textLen = wcslen(text);
        if (textLen >= 4096)
        {
            throw std::out_of_range("String is too long");
        }

        float buttonHeight = butnFont->GetLineSpacing();
        float buttonScale = (textFont->GetLineSpacing() * scale) / buttonHeight;
        float offsetY = buttonScale / 2.f;

        size_t j = 0;
        wchar_t strBuffer[4096] = {0};

        bool buttonText = false;

        XMFLOAT2 outPos = position;

        for (size_t ch = 0; ch < textLen; ++ch)
        {
            if (buttonText)
            {
                strBuffer[j++] = text[ch];

                if (text[ch] == L']')
                {
                    wchar_t button[2] = { 0 };

                    if (_wcsicmp(strBuffer, L"[A]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::AButton);
                    }
                    else if (_wcsicmp(strBuffer, L"[B]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::BButton);
                    }
                    else if (_wcsicmp(strBuffer, L"[X]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::XButton);
                    }
                    else if (_wcsicmp(strBuffer, L"[Y]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::YButton);
                    }
                    else if (_wcsicmp(strBuffer, L"[DPad]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::DPad);
                    }
                    else if (_wcsicmp(strBuffer, L"[Back]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::Back);
                    }
                    else if (_wcsicmp(strBuffer, L"[Start]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::Start);
                    }
                    else if (_wcsicmp(strBuffer, L"[Guide]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::Guide);
                    }
                    else if (_wcsicmp(strBuffer, L"[RThumb]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::RightThumb);
                    }
                    else if (_wcsicmp(strBuffer, L"[LThumb]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::LeftThumb);
                    }
                    else if (_wcsicmp(strBuffer, L"[RB]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::RightShoulder);
                    }
                    else if (_wcsicmp(strBuffer, L"[LB]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::LeftShoulder);
                    }
                    else if (_wcsicmp(strBuffer, L"[RT]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::RightTrigger);
                    }
                    else if (_wcsicmp(strBuffer, L"[LT]") == 0)
                    {
                        *button = static_cast<wchar_t>(ControllerFont::LeftTrigger);
                    }

                    if ( *button )
                    {
                        float bsize = XMVectorGetX(butnFont->MeasureString(button));
                        float offsetX = (bsize * buttonScale / 2.f);

                        outPos.x += offsetX;
                        outPos.y -= offsetY;
                        butnFont->DrawString(spriteBatch, button, outPos, Colors::White, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(buttonScale, buttonScale));
                        outPos.x += bsize * buttonScale;
                        outPos.y += offsetY;
                    }

                    memset(strBuffer, 0, sizeof(strBuffer));
                    j = 0;

                    buttonText = false;
                }
            }
            else
            {
                switch (text[ch])
                {
                case '\r':
                    break;

                case '[':
                    if (*strBuffer)
                    {
                        textFont->DrawString(spriteBatch, strBuffer, outPos, color, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(scale,scale));
                        outPos.x += XMVectorGetX(textFont->MeasureString(strBuffer)) * scale;
                        memset(strBuffer, 0, sizeof(strBuffer));
                        j = 0;
                    }
                    buttonText = true;
                    *strBuffer = L'[';
                    ++j;
                    break;

                case '\n':
                    if (*strBuffer)
                    {
                        textFont->DrawString(spriteBatch, strBuffer, outPos, color, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(scale, scale));
                        memset(strBuffer, 0, sizeof(strBuffer));
                        j = 0;
                    }
                    outPos.x = position.x;
                    outPos.y += textFont->GetLineSpacing() * scale;
                    break;

                default:
                    strBuffer[j++] = text[ch];
                    break;
                }
            }
        }

        if (*strBuffer)
        {
            textFont->DrawString(spriteBatch, strBuffer, outPos, color, 0.f, XMFLOAT2(0.f, 0.f), XMFLOAT2(scale, scale));
        }
    }
}