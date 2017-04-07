package com.bitc.jk.xmis.util;

import java.awt.Color;

import jxl.format.Colour;

/**
 * 将十六进制颜色转换为jxl可用的颜色
 * @author liuxc
 */
public class ColourUtil {
    public static Colour getNearestColour(String strColor) {
        Color cl = Color.decode(strColor);
        Colour color = null;
        Colour[] colors = Colour.getAllColours();
        if ((colors != null) && (colors.length > 0)) {
           Colour crtColor = null;
           int[] rgb = null;
           int diff = 0;
           int minDiff = 999;
           for (int i = 0; i < colors.length; i++) {
                crtColor = colors[i];
                rgb = new int[3];
                rgb[0] = crtColor.getDefaultRGB().getRed();
                rgb[1] = crtColor.getDefaultRGB().getGreen();
                rgb[2] = crtColor.getDefaultRGB().getBlue();
    
                diff = Math.abs(rgb[0] - cl.getRed())
                  + Math.abs(rgb[1] - cl.getGreen())
                  + Math.abs(rgb[2] - cl.getBlue());
                if (diff < minDiff) {
                 minDiff = diff;
                 color = crtColor;
                }
           }
        }
        if (color == null)
           color = Colour.BLACK;
        return color;
    }
    
    public static void main(String[] args){
        System.out.print(getNearestColour("#FFFFFF"));
    }
}
