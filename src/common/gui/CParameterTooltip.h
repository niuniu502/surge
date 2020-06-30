//-------------------------------------------------------------------------------------------------------
//	Copyright 2005 Claes Johanson & Vember Audio
//-------------------------------------------------------------------------------------------------------
#pragma once
#include "vstcontrols.h"
#include "Parameter.h"
#include <iostream>
#include "SkinSupport.h"

class CParameterTooltip : public VSTGUI::CControl, public Surge::UI::SkinConsumingComponnt
{
public:
   CParameterTooltip(const VSTGUI::CRect& size) : VSTGUI::CControl(size, 0, 0, 0)
   {
      label[0][0] = 0;
      label[1][0] = 0;
      visible = false;
      last_tag = -1;
   }

   void setLabel(const char* txt1, const char* txt2, const char* txt2left = nullptr)
   {
      if (txt1)
         strncpy(label[0], txt1, 256);
      else
         label[0][0] = 0;
      if (txt2)
         strncpy(label[1], txt2, 256);
      else
         label[1][0] = 0;
      if (txt2left)
         strncpy(label2left, txt2left, 256);
      else
         label2left[0] = 0;
      
      setDirty(true);
   }

   int getMaxLabelLen() {
      auto l0len = strlen(label[0]);
      auto l1len = strlen(label[1]) + strlen(label2left);
      return std::max( l0len, l1len );
   }
   
   void Show()
   {
      visible = true;
      invalid();
      setDirty(true);
   }
   void Hide()
   {
      visible = false;
      invalid();
      setDirty(true);
   }
   bool isNewTag(long tag)
   {
      bool b = (last_tag != tag);
      last_tag = tag;
      return b;
   }
   bool isVisible()
   {
      return visible;
   }

   virtual void draw(VSTGUI::CDrawContext* dc) override
   {
      if (visible)
      {
         dc->setFont(VSTGUI::kNormalFontSmall);

         auto frameCol = skin->getColor( "infowindow.border", VSTGUI::kBlackCColor );
         auto bgCol = skin->getColor( "infowindow.background", VSTGUI::kWhiteCColor );

         auto txtCol = skin->getColor( "infowindow.foreground", VSTGUI::kBlackCColor );
         auto mpCol = skin->getColor( "infowindow.foreground.modulationpositive", txtCol );
         auto mnCol = skin->getColor( "infowindow.foreground.modulationnegative", txtCol );
         auto mpValCol = skin->getColor( "infowindow.foreground.modulationvaluepositive", mpCol );
         auto mnValCol = skin->getColor( "infowindow.foreground.modulationvaluenegative", mnCol );
         
         
         
         auto size = getViewSize();
         size = size.inset(0.75, 0.75);
         dc->setFrameColor(frameCol);
         dc->drawRect(size);
         VSTGUI::CRect sizem1(size);
         sizem1.inset(1, 1);
         dc->setFillColor(bgCol);
         dc->drawRect(sizem1, VSTGUI::kDrawFilled);
         dc->setFontColor(txtCol);
         VSTGUI::CRect trect(size);
         trect.inset(4, 1);
         VSTGUI::CRect tupper(trect), tlower(trect);
         tupper.bottom = tupper.top + 13;
         tlower.top = tlower.bottom - 15;

         if( hasiwstrings )
         {
            VSTGUI::CRect tmid(trect);
            tmid.bottom = trect.bottom - 18;
            tmid.top = trect.top + 15;
            if( ! extendedwsstrings )
            {
               tmid = tlower;
            }
            if (label[0][0])
            {
               dc->drawString(label[0], tupper, VSTGUI::kLeftText, true);
            }

            if( ! extendedwsstrings )
            {
               dc->drawString( iwstrings.val.c_str(), tmid, VSTGUI::kLeftText, true );
               dc->setFontColor( mpCol );
               dc->drawString( iwstrings.dvalplus.c_str(), tmid, VSTGUI::kRightText, true );
               dc->setFontColor( txtCol );
            }
            else
            {
               dc->drawString( iwstrings.val.c_str(), tmid, VSTGUI::kCenterText, true );
               dc->setFontColor( mpCol );
               dc->drawString( iwstrings.dvalplus.c_str(), tmid, VSTGUI::kRightText, true );
               dc->setFontColor( mnCol );
               dc->drawString( iwstrings.dvalminus.c_str(), tmid, VSTGUI::kLeftText, true );

               dc->setFontColor( mpValCol );
               dc->drawString( iwstrings.valplus.c_str(), tlower, VSTGUI::kRightText, true );
               dc->setFontColor( mnValCol );
               dc->drawString( iwstrings.valminus.c_str(), tlower, VSTGUI::kLeftText, true );
               dc->setFontColor( txtCol );
            }
         }
         else
         {
            if (label[0][0])
            {
               dc->drawString(label[0], tupper, VSTGUI::kLeftText, true);
            }
            // dc->drawString(label[1],tlower,false,label[0][0]?kRightText:kCenterText);
            dc->drawString(label[1], tlower, VSTGUI::kRightText, true);
                     
            if( label2left[0] )
            {
               dc->drawString(label2left, tlower, VSTGUI::kLeftText, true);
            }
         }
      }
      setDirty(false);
   }

   void setMDIWS( const ModulationDisplayInfoWindowStrings &i )
      {
         iwstrings = i;
         hasiwstrings = true;
      }

   void setExtendedMDIWS(bool b)
      {
         extendedwsstrings = b;
      }
   
   void clearMDIWS() { hasiwstrings = false; }
   bool hasMDIWS() { return hasiwstrings; }
   
protected:
   char label[2][256], label2left[256];
   bool visible;
   int last_tag;
   
   ModulationDisplayInfoWindowStrings iwstrings;
   bool hasiwstrings = false;
   bool extendedwsstrings = false;
   
   CLASS_METHODS(CParameterTooltip, VSTGUI::CControl)
};
