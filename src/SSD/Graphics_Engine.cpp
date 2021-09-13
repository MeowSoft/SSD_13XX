#include "Graphics_Engine.h"



int16_t Graphics_Engine::getHeight() {
    return ssd_->_height;
}
int16_t Graphics_Engine::getWidth() {
    return ssd_->_width;
}
void Graphics_Engine::drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color) {
    ssd_->_drawVerticalLine(x, y, h, color);
}
void Graphics_Engine::drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color) {
    ssd_->_drawHorizontalLine(x, y, w, color);
}
void Graphics_Engine::drawPixel_cont(int16_t x, int16_t y, uint16_t color) {
    ssd_->_drawPixel(x, y, color);
}
void Graphics_Engine::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
    ssd_->fillRect(x, y, w, h, color);
}
void Graphics_Engine::drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled) {
    ssd_->_drawRectangle(x, y, w, h, color1, color2, filled);
}
void Graphics_Engine::drawLine_cont(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    ssd_->_drawLine(x0, y0, x1, y1, color);
}
bool Graphics_Engine::boundaryCheck(int16_t xw,int16_t yh) {
    return !ssd_->_checkBounds(xw, yh);
}
bool Graphics_Engine::isPortrait() {
    return ssd_->_portrait;
}
void Graphics_Engine::setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix) {
    ssd_->_setAddressWindow(x0, y0, x1, y1, rotFix);
}
uint16_t Graphics_Engine::getDefaultBackground() {
    return ssd_->_defaultBgColor;
}
uint16_t Graphics_Engine::getDefaultForeground() {
    return ssd_->_defaultFgColor;
}

/* #region Graphics_Engine */

void Graphics_Engine::begin(SSD_13XX* ssd) {
    ssd_ = ssd;

	_arcAngleMax 	= 360;
	_arcAngleOffset = -90;

    }

void Graphics_Engine::drawArcHelper(int16_t cx, int16_t cy, int16_t radius, int16_t thickness, float start, float end, uint16_t color)
{
	if (cx + radius > getWidth() ) return;
	if (cy + radius > getHeight()) return;
	int16_t xmin = 65535;
	int16_t xmax = -32767;
	int16_t ymin = 32767;
	int16_t ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;
	int16_t ir2, or2;//int
	int16_t x, y, x2, y2;//int
	int16_t y1s, y1e, y2s;//int
	float sslope, eslope;
	bool y1StartFound, y2StartFound, y1EndFound, y2EndSearching;

	startAngle = (start / _arcAngleMax) * 360;	// 252
	endAngle = (end / _arcAngleMax) * 360;		// 807

	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;

	if (startAngle > endAngle) {
		drawArcHelper(cx, cy, radius, thickness, ((startAngle / 360.0) * _arcAngleMax), _arcAngleMax, color);
		drawArcHelper(cx, cy, radius, thickness, 0, ((endAngle / 360.0) * _arcAngleMax), color);
	} else {
		// Calculate bounding box for the arc to be drawn
		cosStart = SSD_Util::cosDeg_helper(startAngle);
		sinStart = SSD_Util::sinDeg_helper(startAngle);
		cosEnd = SSD_Util::cosDeg_helper(endAngle);
		sinEnd = SSD_Util::sinDeg_helper(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		sslope = (float)cosStart / (float)sinStart;
		eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		ir2 = (radius - thickness) * (radius - thickness);
		or2 = radius * radius;
		ssd_->_startTransaction();
		for (x = xmin; x <= xmax; x++) {
			y1StartFound 	= false;
			y2StartFound 	= false;
			y1EndFound 		= false;
			y2EndSearching 	= false;
			y1s = 0;
			y1e = 0;
			y2s = 0;
			for (y = ymin; y <= ymax; y++) {
				x2 = x * x;
				y2 = y * y;
				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine_cont(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						} else {
							break;
						}
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine_cont(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine_cont(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine_cont(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
			#if defined(ESP8266)
				yield();
			#endif
		}
		ssd_->_closeTransaction();
	}
}

void Graphics_Engine::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngleMax = arcAngleMax;
	_arcAngleOffset = arcAngleOffset;
}




//fast
void Graphics_Engine::drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color)
{
	if (cx + radiusW >= getWidth()) return;
	if (cy + radiusH >= getHeight()) return;
	int16_t x,y;
	int16_t twoASquare,twoBSquare;
	int32_t stoppingX,stoppingY;
    int32_t xchange,ychange,ellipseError;
    twoASquare = 2 * (radiusW * radiusW);
    twoBSquare = 2 * (radiusH * radiusH);
    x = radiusW;
    y = 0;
    xchange = (radiusH * radiusH) * (1 - (2 * radiusW));
    ychange = (radiusW * radiusW);
    ellipseError = 0;
    stoppingX = (twoBSquare * radiusW);
    stoppingY = 0;
	ssd_->_startTransaction();
    while (stoppingX >= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		y++;
		stoppingY += twoASquare;
		ellipseError += ychange;
		ychange += twoASquare;
		if ((2 * ellipseError) + xchange > 0) {
			x--;
			stoppingX -= twoBSquare;
			ellipseError += xchange;
			xchange += twoBSquare;
		}
		#if defined(ESP8266)
			yield();
		#endif
    }
    x = 0;
    y = radiusH;
    xchange = (radiusH * radiusH);
    ychange = (radiusW * radiusW) * (1 - (2 * radiusH));
    ellipseError = 0;
    stoppingX = 0;
    stoppingY = (twoASquare * radiusH);
    while (stoppingX <= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		x++;
		stoppingX += twoBSquare;
		ellipseError += xchange;
		xchange += twoBSquare;
		if (((2 * ellipseError) + ychange) > 0) {
			y--;
			stoppingY -= twoASquare;
			ellipseError += ychange;
			ychange += twoASquare;
		}
		#if defined(ESP8266)
			yield();
		#endif
    }
	ssd_->_closeTransaction();
}

//fast
void Graphics_Engine::plot4points_cont(int16_t cx, int16_t cy, int16_t x, int16_t y, uint16_t color)
{
	drawPixel_cont(cx + x, cy + y, color);
	if (x != 0) drawPixel_cont(cx - x, cy + y, color);
	if (y != 0) drawPixel_cont(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel_cont(cx - x, cy - y, color);
}

//fast
void Graphics_Engine::drawCircle_cont_helper(int16_t x,int16_t y,int16_t radius,uint8_t cornername,uint16_t color)
{
	int16_t f			= 1 - radius;
	int16_t ddF_x		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;
	if (cornername > 253){
		drawPixel_cont(x  		, y+radius	, color);
		drawPixel_cont(x  		, y-radius	, color);
		drawPixel_cont(x+radius	, y  		, color);
		drawPixel_cont(x-radius	, y  		, color);
	}
	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername > 253){
			drawPixel_cont(x + deltax, y + deltay, color);
			drawPixel_cont(x - deltax, y + deltay, color);
			drawPixel_cont(x + deltax, y - deltay, color);
			drawPixel_cont(x - deltax, y - deltay, color);
			drawPixel_cont(x + deltay, y + deltax, color);
			drawPixel_cont(x - deltay, y + deltax, color);
			drawPixel_cont(x + deltay, y - deltax, color);
			drawPixel_cont(x - deltay, y - deltax, color);
		} else {
			if (cornername & 0x4) {
				drawPixel_cont(x + deltax, y + deltay, color);
				drawPixel_cont(x + deltay, y + deltax, color);
			}
			if (cornername & 0x2) {
				drawPixel_cont(x + deltax, y - deltay, color);
				drawPixel_cont(x + deltay, y - deltax, color);
			}
			if (cornername & 0x8) {
				drawPixel_cont(x - deltay, y + deltax, color);
				drawPixel_cont(x - deltax, y + deltay, color);
			}
			if (cornername & 0x1) {
				drawPixel_cont(x - deltay, y - deltax, color);
				drawPixel_cont(x - deltax, y - deltay, color);
			}
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}



//fast
void Graphics_Engine::fillCircle_cont_helper(int16_t x, int16_t y, int16_t radius, uint8_t cornername, int16_t delta, uint16_t color)
{
	int16_t f     		= 1 - radius;
	int16_t ddF_x 		= 1;
	int16_t ddF_y 		= -2 * radius;
	int16_t deltax		= 0;
	int16_t deltay		= radius;

	while (deltax < deltay) {
		if (f >= 0) {
			deltay--;
			ddF_y += 2;
			f     += ddF_y;
		}
		deltax++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine_cont(x+deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x+deltay, y-deltax, 2*deltax+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine_cont(x-deltax, y-deltay, 2*deltay+1+delta, color);
			drawFastVLine_cont(x-deltay, y-deltax, 2*deltax+1+delta, color);
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}

//fast
void Graphics_Engine::drawCircle(int16_t x, int16_t y, int16_t radius, uint16_t color)
{
	if (x+(radius) >= getWidth() || y+(radius) >= getHeight()) return;//radius*2

	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t xrel = 0;
	int16_t yrel = radius;
	ssd_->_startTransaction();
	drawPixel_cont(x  , y+radius, color);
	drawPixel_cont(x  , y-radius, color);
	drawPixel_cont(x+radius, y  , color);
	drawPixel_cont(x-radius, y  , color);

	while (xrel<yrel) {
		if (f >= 0) {
			yrel--;
			ddF_y += 2;
			f += ddF_y;
		}
		xrel++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel_cont(x + xrel, y + yrel, color);
		drawPixel_cont(x - xrel, y + yrel, color);
		drawPixel_cont(x + xrel, y - yrel, color);
		drawPixel_cont(x - xrel, y - yrel, color);
		drawPixel_cont(x + yrel, y + xrel, color);
		drawPixel_cont(x - yrel, y + xrel, color);
		drawPixel_cont(x + yrel, y - xrel, color);
		drawPixel_cont(x - yrel, y - xrel, color);
	}
	ssd_->_closeTransaction();
}

void Graphics_Engine::fillCircle(int16_t x, int16_t y, int16_t radius,uint16_t color)
{
	if (x+(radius) >= getWidth() || y+(radius) >= getHeight()) return;//radius*2
	ssd_->_startTransaction();//open SPI comm
	drawFastVLine_cont(x, y-radius, (2*radius)+1, color);
	fillCircle_cont_helper(x, y, radius, 3, 0, color);
	ssd_->_closeTransaction();
}


//fast
void Graphics_Engine::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color)
{
	if (x+w >= getWidth() || y+h > getHeight()) return;
	//if (x+w-radius-1 < 0 || y+h-radius-1 < 0) return;
	ssd_->_startTransaction();
	drawFastHLine_cont(x+radius  , y    	 , w-2*radius, color); // Top
	drawFastHLine_cont(x+radius  , y+h-1	 , w-2*radius, color); // Bottom
	drawFastVLine_cont(x    	 , y+radius  , h-2*radius, color); // Left
	drawFastVLine_cont(x+w-1	 , y+radius  , h-2*radius, color); // Right
  // draw four corners
	drawCircle_cont_helper(x+radius    , y+radius    , radius, 1, color);
	drawCircle_cont_helper(x+w-radius-1, y+radius    , radius, 2, color);
	drawCircle_cont_helper(x+w-radius-1, y+h-radius-1, radius, 4, color);
	drawCircle_cont_helper(x+radius    , y+h-radius-1, radius, 8, color);
	ssd_->_closeTransaction();
}


//fast
void Graphics_Engine::fillRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t radius, uint16_t color)
{
	if (x+w >= getWidth() || y+h > getHeight()) return;
	if (radius < 2) {
		fillRect(x,y,w,h,color);
		return;
	}
	ssd_->_startTransaction();
	drawRect_cont(x+radius, y, w-2*radius, h, color, color,1);
	//fillRect_cont(x+radius, y, w-2*radius, h, color, color);
	fillCircle_cont_helper(x+w-radius-1, y+radius, radius, 1, h-2*radius-1, color);
	fillCircle_cont_helper(x+radius    , y+radius, radius, 2, h-2*radius-1, color);
	ssd_->_closeTransaction();
}


void Graphics_Engine::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color)
{
	ssd_->_startTransaction();//open SPI comm
	drawLine_cont(x0, y0, x1, y1, color/*,20*/);//low 1
	drawLine_cont(x1, y1, x2, y2, color/*,20*/);//high 1
	drawLine_cont(x2, y2, x3, y3, color/*,20*/);//high 2
	drawLine_cont(x3, y3, x0, y0, color/*,20*/);//low 2
	ssd_->_closeTransaction();
}


void Graphics_Engine::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color,bool triangled)
{
	ssd_->_startTransaction();//open SPI comm
    fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);
	if (triangled) fillTriangle_cont(x2, y2, x3, y3, x0, y0, color);
    fillTriangle_cont(x1,y1,x2,y2,x3,y3,color);
	ssd_->_closeTransaction();
}

void Graphics_Engine::drawPolygon(int16_t x, int16_t y, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{
	sides = (sides > 2 ? sides : 3);
	const float dtr = (PI/180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	ssd_->_startTransaction();
	for (i = 0; i < sides; i++) {
		drawLine_cont(
			x + (sin((i*rads + rot) * dtr) * diameter),
			y + (cos((i*rads + rot) * dtr) * diameter),
			x + (sin(((i+1)*rads + rot) * dtr) * diameter),
			y + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color//,20
			);
	}
	ssd_->_closeTransaction();
}

void Graphics_Engine::drawMesh(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if (boundaryCheck(x,y)) return;
	//if (((x + w) - 1) >= _width)  w = _width  - x;
	//if (((y + h) - 1) >= _height) h = _height - y;
	w = SSD_Util::sizeCheck(x,w,getWidth());
	h = SSD_Util::sizeCheck(y,h,getHeight());

	int16_t n, m;

	if (w < x) {n = w; w = x; x = n;}
	if (h < y) {n = h; h = y; y = n;}
	ssd_->_startTransaction();
	for (m = y; m <= h; m += 2) {
		for (n = x; n <= w; n += 2) {
			drawPixel_cont(n, m, color);
		}
	}
	ssd_->_closeTransaction();
}

void Graphics_Engine::drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	if (x0 > getWidth() || x1 > getWidth() || x2 > getWidth()) return;
	if (y0 > getHeight() || y1 > getHeight() || y2 > getHeight()) return;
	ssd_->_startTransaction();
	drawLine_cont(x0, y0, x1, y1, color/*,800*/);
	drawLine_cont(x1, y1, x2, y2, color/*,800*/);
	drawLine_cont(x2, y2, x0, y0, color/*,800*/);
	ssd_->_closeTransaction();
}

//85% fast
void Graphics_Engine::fillTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	ssd_->_startTransaction();
	fillTriangle_cont(x0,y0,x1,y1,x2,y2,color);//
	ssd_->_closeTransaction();
}

void Graphics_Engine::fillTriangle_cont(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
	int16_t a, b, y, last;

	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}
	if (y1 > y2) {swapVals(y2, y1); swapVals(x2, x1);}
	if (y0 > y1) {swapVals(y0, y1); swapVals(x0, x1);}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a){
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
		if (x2 < a){
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
		drawFastHLine_cont(a, y0, b-a+1, color);
		return;
	}

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa   = 0,
		sb   = 0;

	if (y1 == y2) {
		last = y1;
	} else {
		last = y1-1;
	}

	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y <= y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swapVals(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}
}


/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void Graphics_Engine::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t length, uint16_t color,int offset)
{

    ssd_->_startTransaction();
	if (length < 2) {//NEW
		drawPixel_cont(x,y,color);
	} else {
		drawLine_cont(
		x,
		y,
		x + (length * SSD_Util::cosDeg_helper(angle + offset)),//_angle_offset
		y + (length * SSD_Util::sinDeg_helper(angle + offset)),
		color);
	}
    ssd_->_closeTransaction();
}

/**************************************************************************/
/*!
      Basic line by using Angle as parameter
	  Parameters:
	  x: horizontal start pos
	  y: vertical start
	  angle: the angle of the line
	  start: where line start
	  length: lenght of the line
	  color: RGB565 color
	  from my RA8875 library
*/
/**************************************************************************/
void Graphics_Engine::drawLineAngle(int16_t x, int16_t y, int angle, uint8_t start, uint8_t length, uint16_t color,int offset)
{

    ssd_->_startTransaction();
	if (start - length < 2) {//NEW
		drawPixel_cont(x,y,color);
	} else {
		drawLine_cont(
		x + start * SSD_Util::cosDeg_helper(angle + offset),//_angle_offset
		y + start * SSD_Util::sinDeg_helper(angle + offset),
		x + (start + length) * SSD_Util::cosDeg_helper(angle + offset),
		y + (start + length) * SSD_Util::sinDeg_helper(angle + offset),
		color);
	}

    ssd_->_closeTransaction();
}

/**************************************************************************/
/*!
      ringMeter
	  (adapted from Alan Senior (thanks man!))
	  from my RA8875 library
	  it create a ring meter with a lot of personalizations,
	  it return the width of the gauge so you can use this value
	  for positioning other gauges near the one just created easily
	  Parameters:
	  val:  your value
	  minV: the minimum value possible
	  maxV: the max value possible
	  x:    the position on x axis
	  y:    the position on y axis
	  r:    the radius of the gauge (minimum 20)
	  units: a text that shows the units, if "none" all text will be avoided
	  scheme:0...7 or 16 bit color (not BLACK or WHITE)
	  0:red
	  1:green
	  2:blue
	  3:blue->red
	  4:green->red
	  5:red->green
	  6:red->green->blue
	  7:cyan->green->red
	  8:black->white linear interpolation
	  9:violet->yellow linear interpolation
	  or
      RGB565 color (not BLACK or WHITE)
	  backSegColor: the color of the segments not active (default BLACK)
	  angle:		90 -> 180 (the shape of the meter, 90:halfway, 180:full round, 150:default)
	  inc: 			5...20 (5:solid, 20:sparse divisions, default:10)
*/
/**************************************************************************/
void Graphics_Engine::ringMeter(int val, int minV, int maxV, uint8_t x, uint8_t y, uint8_t r, uint16_t colorScheme,uint16_t backSegColor,int angle,uint8_t inc)
{
	if (inc < 5) inc = 5;
	if (inc > 20) inc = 20;
	if (r < 50) r = 20;
	if (angle < 90) angle = 90;
	if (angle > 180) angle = 180;
	int i;
	int curAngle = map(val, minV, maxV, -angle, angle);
	uint16_t colour;
	x += r;
	y += r;   // Calculate coords of centre of ring
	uint8_t w = r / 4;    // Width of outer ring is 1/4 of radius
	const uint8_t seg = 5; // Segments are 5 degrees wide = 60 segments for 300 degrees
	// Draw colour blocks every inc degrees
	for (i = -angle; i < angle; i += inc) {
		colour = BLACK;
		switch (colorScheme) {
			case 0:
				colour = RED;
				break; // Fixed colour
			case 1:
				colour = GREEN;
				break; // Fixed colour
			case 2:
				colour = BLUE;
				break; // Fixed colour
			case 3:
				colour = SSD_Util::gradient(map(i, -angle, angle, 0, 127));
				break; // Full spectrum blue to red
			case 4:
				colour = SSD_Util::gradient(map(i, -angle, angle, 63, 127));
				break; // Green to red (high temperature etc)
			case 5:
				colour = SSD_Util::gradient(map(i, -angle, angle, 127, 63));
				break; // Red to green (low battery etc)
			case 6:
				colour = SSD_Util::gradient(map(i, -angle, angle, 127, 0));
				break; // Red to blue (air cond reverse)
			case 7:
				colour = SSD_Util::gradient(map(i, -angle, angle, 35, 127));
				break; // cyan to red
			case 8:
				colour = SSD_Util::colorInterpolation(0,0,0,255,255,255,map(i,-angle,angle,0,w),w);
				break; // black to white
			case 9:
				colour = SSD_Util::colorInterpolation(0x80,0,0xC0,0xFF,0xFF,0,map(i,-angle,angle,0,w),w);
				break; // violet to yellow
			default:
				if (colorScheme > 9){
					colour = colorScheme;
				} else {
					colour = BLUE;
				}
				break; // Fixed colour
		}
		// Calculate pair of coordinates for segment start
		float xStart = cos((i - 90) * 0.0174532925);
		float yStart = sin((i - 90) * 0.0174532925);
		uint8_t x0 = xStart * (r - w) + x;
		uint8_t y0 = yStart * (r - w) + y;
		uint8_t x1 = xStart * r + x;
		uint8_t y1 = yStart * r + y;

		// Calculate pair of coordinates for segment end
		float xEnd = cos((i + seg - 90) * 0.0174532925);
		float yEnd = sin((i + seg - 90) * 0.0174532925);
		uint8_t x2 = xEnd * (r - w) + x;
		uint8_t y2 = yEnd * (r - w) + y;
		uint8_t x3 = xEnd * r + x;
		uint8_t y3 = yEnd * r + y;

		if (i < curAngle) { // Fill in coloured segments with 2 triangles
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, colour, false);
		} else {// Fill in blank segments
			fillQuad(x0, y0, x1, y1, x2, y2, x3, y3, backSegColor, false);
		}
	}

}




//OK with SetRemap 0...3
void Graphics_Engine::drawImage(int16_t x, int16_t y,const tPicture *img,const enum SSD_13XX_iconMods m,uint16_t b)
{
	uint16_t px = 0;
	uint16_t color;
	uint8_t currentY = 0;
	uint16_t currentX = 0;
	bool skip = false;
	#if defined(_FORCE_PROGMEM__)
		const uint16_t * imageData  = PROGMEM_read(&img->data);
		uint8_t		iWidth			= pgm_read_byte(&img->image_width);
		uint8_t		iHeight			= pgm_read_byte(&img->image_height);
		uint16_t	datalen			= pgm_read_word(&img->image_datalen);
		//uint8_t		dataDepth	= pgm_read_byte(&img->image_depth);//not yet
		//boolean		dataComp	= pgm_read_byte(&(img->image_comp);//not yet
	#else
		const uint16_t * imageData	= img->data;//image data
		uint8_t		iWidth			= img->image_width;
		uint8_t		iHeight			= img->image_height;
		uint16_t	datalen			= img->image_datalen;
		//uint8_t		dataDepth	= img->image_depth;//not yet
		//uint8_t		dataComp	= img->image_comp;//not yet
	#endif
	iWidth -= 1;
	iHeight -= 1;
	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (isPortrait()) {
		swapVals(x,y);
		swapVals(iWidth,iHeight);
	}

	if (x + iWidth >= getWidth() || y + iHeight >= getHeight()) return;//cannot be

	ssd_->_startTransaction();
	setAddrWindow_cont(x,y,iWidth+x,iHeight+y,false);

	do {
		//get the pixel
		#if defined(_FORCE_PROGMEM__)
			color = pgm_read_word(&imageData[px]);
		#else
			color = imageData[px];
		#endif
		//simple color operations
		if (m == TRANSPARENT){
			if (color <= b) skip = true;
		} else if (m == REPLACE){
			if (color <= b) color = getDefaultBackground();
		} else if (m == BOTH){
			if (color <= b) {
				color = getDefaultBackground();
			} else {
				color = getDefaultForeground();
			}
		}

		if (!skip) {
			ssd_->_writedata16_cont(color);
		} else {
			setAddrWindow_cont(x+currentX,y+currentY,(iWidth+x),(iHeight+y),false);//constrain window
			skip = false;
		}

		if (currentX == iWidth){
			currentX = 0;
			currentY++;
		} else {
			currentX++;
		}

		px++;
	} while (--datalen > 0);

	ssd_->_closeTransaction();
}


void Graphics_Engine::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color)
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                ssd_->_startTransaction();
                 drawPixel_cont(x + i, y + j, color);

	ssd_->_closeTransaction();
            }
		}
		#if defined(ESP8266)
			yield();
		#endif
	}
}

void Graphics_Engine::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg)
{
	int16_t i, j;
	int16_t byteWidth = (w + 7) / 8;
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++ ) {
            ssd_->_startTransaction();
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				drawPixel_cont(x + i, y + j, color);
			} else {
				drawPixel_cont(x + i, y + j, bg);
		    }

	ssd_->_closeTransaction();
    }
	#if defined(ESP8266)
		yield();
	#endif
  }
}


/* #endregion */
