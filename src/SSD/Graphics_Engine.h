#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <stdint.h>
#include "SSD_13XX.h"

class Graphics_Engine {

    public:

        void begin(SSD_13XX* ssd);

        void 		drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color);
        void		drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
        void		fillCircle(int16_t x, int16_t y, int16_t radius, uint16_t color);
        void		drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,int16_t radius, uint16_t color);
	void		fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,int16_t radius, uint16_t color);
void		drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);
	void		fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled=true);
	void		drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rot, uint16_t color);
	void		drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void		drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
	void		fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
    void 		drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset = -90);
	void 		drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset = -90);
void 		ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r=20, uint16_t colorScheme=4,uint16_t backSegColor=BLACK,int angle=150,uint8_t inc=5);
	void 		drawImage(int16_t x, int16_t y,const tPicture *img,const enum SSD_13XX_iconMods m=NONE, uint16_t b=BLACK, uint16_t bgColor=BLACK, uint16_t fgColor=BLACK);
void		drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,int16_t w, int16_t h, uint16_t color);
	void		drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg);

void 		drawArc(int16_t cx, int16_t cy, int16_t radius, int16_t thickness, float start, float end, uint16_t color) {
					if (start == 0 && end == _arcAngleMax) {
						drawArcHelper(cx, cy, radius, thickness, 0, _arcAngleMax, color);
					} else {
						drawArcHelper(cx, cy, radius, thickness, start + (_arcAngleOffset / (float)360)*_arcAngleMax, end + (_arcAngleOffset / (float)360)*_arcAngleMax, color);
					}
				};

    private:

    SSD_13XX* ssd_;

	float 					_arcAngleMax;
	int 					_arcAngleOffset;

    void 		drawArcHelper(int16_t cx, int16_t cy, int16_t radius, int16_t thickness, float start, float end, uint16_t color);
    void 		setArcParams(float arcAngleMax, int arcAngleOffset);
    void 		plot4points_cont(int16_t cx, int16_t cy, int16_t x, int16_t y, uint16_t color);
    void		drawCircle_cont_helper(int16_t x, int16_t y, int16_t radius, uint8_t cornername,uint16_t color);
    void		fillCircle_cont_helper(int16_t x, int16_t y, int16_t radius, uint8_t cornername,int16_t delta, uint16_t color);
    void 		fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);


// SSD interface methods: 
    bool boundaryCheck(int16_t xw,int16_t yh);
      int16_t getHeight();
    int16_t getWidth();
    bool isPortrait();
    void drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color);
    void 		drawPixel_cont(int16_t x, int16_t y, uint16_t color);
    void 		drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color);
    void		fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
    void 		drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled);
    void 		drawLine_cont(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void 		setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix = true);

    void startTransaction();
    void closeTransaction();
    void writedata16_cont(uint16_t d);
};

#endif
