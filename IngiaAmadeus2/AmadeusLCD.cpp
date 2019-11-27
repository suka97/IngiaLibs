#include "AmadeusLCD.h"


void AmadeusLCD::_freeBuffer() {
    for( uint8_t i=0 ; i<_bufSize_Y ; ++i ) {
        delete [] _log_buffer[i];
    }
    delete [] _log_buffer;
}

void AmadeusLCD::_makeBuffer(uint8_t charsExtra, uint8_t rowsExtra) {
    // pido memoria dinamica
    _bufSize_X = (_u8g2.getDisplayWidth() / _u8g2.getMaxCharWidth()) + charsExtra + 1;
    _bufSize_Y = (_u8g2.getDisplayHeight() / _u8g2.getMaxCharHeight()) + rowsExtra;
    _log_buffer = new uint8_t *[_bufSize_Y]; 
    for ( uint8_t i=0 ; i<_bufSize_Y ; i++ )
        _log_buffer[i] = new uint8_t[_bufSize_X];

    _clearBuffer();
}

void AmadeusLCD::init(const uint8_t *font, uint8_t charsExtra, uint8_t rowsExtra, int espacioTop) {
    _u8g2.begin();
    _u8g2.setFont(font);
    _makeBuffer(charsExtra, rowsExtra);
    _espacioTop = (espacioTop == (-1)) ? (_u8g2.getMaxCharHeight() / 3) : espacioTop;
}

void AmadeusLCD::_changeFont(const uint8_t *font, uint8_t charsExtra, uint8_t rowsExtra, int espacioTop) {
    _freeBuffer();
    _u8g2.setFont(font);
    _makeBuffer(charsExtra, rowsExtra);
    _espacioTop = (espacioTop == (-1)) ? (_u8g2.getMaxCharHeight() / 3) : espacioTop;
}

void AmadeusLCD::drawBitmap(const uint8_t *bitmap, uint16_t width, uint16_t height, uint16_t crop) {    
    _u8g2.firstPage();
    do {
        _u8g2.setDrawColor( (_colorFliped) ? 1 : 0 );
        _u8g2.drawBox(0, 0, _u8g2.getDisplayWidth(), _u8g2.getDisplayHeight());
        _u8g2.setDrawColor( (_colorFliped) ? 0 : 1 );
        _u8g2.drawXBMP((_u8g2.getDisplayWidth()-width)/2, (_u8g2.getDisplayHeight()-height)/2, width, height, bitmap);
        if ( crop > 0 ) {
            // el crop es del color del fondo
            _u8g2.setDrawColor( (_colorFliped) ? 1 : 0 );   
            _u8g2.drawBox(0, 0, crop/2, _u8g2.getDisplayHeight());
            _u8g2.drawBox(_u8g2.getDisplayWidth()-(crop/2), 0, crop/2, _u8g2.getDisplayHeight());
            _u8g2.setDrawColor( (_colorFliped) ? 0 : 1 );
        }
    } while( _u8g2.nextPage() );
}

void AmadeusLCD::drawBitmap_Curtain(const uint8_t *bitmap, uint16_t width, uint16_t height, 
    unsigned long millisLength, unsigned long millisRefresh) 
{
    uint16_t div = millisLength / millisRefresh;
    uint16_t crop = _u8g2.getDisplayWidth();
    while(1) {
        drawBitmap(bitmap, width, height, crop);
        if ( crop == 0 )
            break;
        crop = (crop > (_u8g2.getDisplayWidth()/div)) ? (crop - (_u8g2.getDisplayWidth()/div)) : 0;
        delay(millisRefresh);
    }
}

void AmadeusLCD::lcdClear() {
    _index_X = 0; _index_Y = 0;
    _clearBuffer();
    _clearScreen();
}

void AmadeusLCD::_clearBuffer() {
    for (uint16_t j=0 ; j<(_bufSize_Y) ; j++) {
        _bufferRenglonVacio(j);
    }
}

void AmadeusLCD::print(String const& str, uint8_t row, PrintFlag_t printFlag, boolean clearFlag, int startIndex, boolean printNow) {
    if (clearFlag) {
        _clearBuffer();
    }
    _index_Y = (row < getMaxRows()) ? row : (getMaxRows()-1);

    switch (printFlag) {
        case LCD_LEFT:
            if ( startIndex != -1 )
                _index_X = startIndex;
            break;
        case LCD_RIGHT:
            _index_X = _bufSize_X - str.length() - 1;
            break;
        case LCD_CENTER:
            _index_X = (_bufSize_X - str.length()) / 2;
            break;
    }

    for (uint8_t i=0 ; (_index_X<getMaxLetters()) && (i<str.length()) ; i++) {
        _log_buffer[_index_Y][_index_X] = (uint8_t)str.charAt(i);
        _index_X++;
    }
    if ( printNow )
        _sendBuffer();
}

void AmadeusLCD::_bufferRenglonVacio(uint8_t row) {
    for (uint16_t i=0 ; i<(_bufSize_X-1) ; i++) {
        _log_buffer[row][i] = ' ';
    }
    _log_buffer[row][_bufSize_X-1] = '\0';
}

void AmadeusLCD::_sendBuffer() {
    char letterBuffer[] = " ";

    _u8g2.firstPage();
    do {
        _u8g2.setDrawColor( (_colorFliped) ? 1 : 0 );
        _u8g2.drawBox(0, 0, _u8g2.getDisplayWidth(), _u8g2.getDisplayHeight());
        _u8g2.setDrawColor( (_colorFliped) ? 0 : 1 );

        if ( _raining ) {
            for( uint8_t f=0 ; f<_numRains ; f++)  // Draw each snowflake:
                //_u8g2.drawXBMP(_rainArray[f][0], _rainArray[f][1], 
                _u8g2.drawPixel(_rainArray[f][0], _rainArray[f][1]);
        }

        for (uint16_t i=0 ; i<(_bufSize_Y) ; i++) {
            for (uint16_t j=0 ; j<_bufSize_X ; j++ ) {
                letterBuffer[0] = _log_buffer[i][j];
                _u8g2.drawStr( getEspacioLetter()*j, getAlturaRenglon(i+1), letterBuffer );
            }
        }
    } while ( _u8g2.nextPage() );

    if ( _raining ) {
        // update position
        for( uint8_t f=0 ; f<_numRains ; f++) {
            _rainArray[f][1] -= _rainArray[f][2];
            // If snowflake is off the bottom of the screen...
            if (_rainArray[f][1] < _rainArray[f][2]) {
                // Reinitialize to a random position, just off the top
                _rainArray[f][0]   = random(0, _u8g2.getDisplayWidth());
                _rainArray[f][1]   = _u8g2.getDisplayHeight();
                _rainArray[f][2] = random(1, 4);
            }
        }
    }
}

void AmadeusLCD::_clearScreen() {
    _u8g2.firstPage();
    do {
    } while ( _u8g2.nextPage() );
}

void AmadeusLCD::setRain(boolean r, uint8_t numRains) {
    if (r && !_raining) {
        _numRains = numRains;

        // Initialize 'snowflakes'
        _rainArray = new uint16_t*[_numRains];
        for ( uint8_t i=0 ; i<_numRains ; i++ )
            _rainArray[i] = new uint16_t[3];

        for( uint8_t f=0 ; f<_numRains ; f++) {
            _rainArray[f][0]   = random(1, _u8g2.getDisplayWidth());
            _rainArray[f][1]   = random(0, _u8g2.getDisplayHeight());
            _rainArray[f][2] = random(1, 3);
        }
    }
    else if ( !r && _raining) {
        // free memory
        for( uint8_t i=0 ; i<_numRains ; ++i ) {
            delete [] _rainArray[i];
        }
        delete [] _rainArray;
    }
    else if ( r && _raining && (numRains!=_numRains) ) {
        setRain(false);
        setRain(true, numRains);
    }
    _raining = r;
}