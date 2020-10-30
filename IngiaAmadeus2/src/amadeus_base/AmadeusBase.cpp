#include "AmadeusBase.h"

AmadeusBase * AmadeusBase::instance = NULL;

void AmadeusBase::cambiarVarMostrando(char *nombre, long *var, long valmin, long valmax, char *unidad, bool restart) {
    static int flagMinMax = 0;
    static bool borderValueSituation = false;    // indica si la ultima vez estaba escrito max o min 
    
    if ( restart ) {
        setIncrementalAcceleration(true);
        lcdClear();
        print(nombre, 0, LCD_CENTER, false, -1, true);
        if ( unidad != NULL )
            print(unidad, 2, LCD_RIGHT);
    }

    bool valueChanged = cambiarVar(var, valmin, valmax, &flagMinMax);
    if ( valueChanged || restart ) {
        print(String(*var)+" ", 2, LCD_LEFT, false, 0);

        if ( borderValueSituation ) {
            if ( flagMinMax == 0 ) {
                clearRow(1);
                borderValueSituation = false;
            }
        }
        else {
            if ( flagMinMax == 1 ) { 
                print("(max)", 1, LCD_CENTER);
                borderValueSituation = true;
            }
            if ( flagMinMax == (-1) ) {
                print("(min)", 1, LCD_CENTER);
                borderValueSituation = true;
            }
        }   
    }
}

bool AmadeusBase::menuParametros() {
    static int topIndex = 0;
    static bool printMenuFlag = true, currentlyChanging = false;
    static long indexMenu = 0;  // lo hago long para poder usar cambiarVar()
    static long bufferValor;
    static uint8_t lastRowIndex = 0;

    char bufferMenu[25], bufferUnid[10];    // 25 por tirar un numero grande de getMaxLetters()

    if ( !currentlyChanging ) {
        if (printMenuFlag) { 
            lcdClear();
            for ( int i=0 ; i<getMaxRows(); i++ ) {
                if ( (i+topIndex) < _cantMenus ) {
                    strcpy_P(bufferMenu, (char*)pgm_read_word(&(_menus[i+topIndex]))); // no tocar renglon ;)
                    print( String(" ") + String(bufferMenu), i, LCD_LEFT, false, 1, false );
                }
            }
            printMenuFlag = false;  
            print(">", (indexMenu - topIndex), LCD_LEFT, false, 0);

            setIncrementalAcceleration(false);
        }

        if ( cambiarVarIncremental(&indexMenu, 0, (_cantMenus-1)) ) {
            // si me pase de la zona imprimible
            if ( (indexMenu+1) > (topIndex + getMaxRows()) ) {
                topIndex++;
                printMenuFlag = true;
            }
            else if ( (indexMenu < topIndex) ) {
                topIndex--;
                printMenuFlag = true;
            }
            else {
                print(" ", (lastRowIndex - topIndex), LCD_LEFT, false, 0, false); 
                print(">", (indexMenu - topIndex), LCD_LEFT, false, 0);
            }
            lastRowIndex = indexMenu;
        }
        
        if ( backPressed() ) {
            printMenuFlag = true;
            lcdClear();
            return false;
        }
        else if ( enterPressed() ) {
            bufferValor = getVal(indexMenu);
            currentlyChanging = true;
            setIncrementalAcceleration(true);

            strcpy_P(bufferMenu, (char*)pgm_read_word(&(_menus[indexMenu]))); // no tocar renglon ;)
            strcpy_P(bufferUnid, (char*)pgm_read_word(&(_unids[indexMenu]))); // no tocar renglon ;)
            delay(200);
        }
    }
    else {     
        if ( printMenuFlag ) {
            cambiarVarMostrando( bufferMenu, &(bufferValor), pgm_read_dword(&_valmin[indexMenu]),
                pgm_read_dword(&_valmax[indexMenu]), bufferUnid );
        }
        else {
            printMenuFlag = true;
            cambiarVarMostrando( bufferMenu, &(bufferValor), pgm_read_dword(&_valmin[indexMenu]),
                pgm_read_dword(&_valmax[indexMenu]), bufferUnid, true );
        }

        if ( backPressed() || enterPressed() ) {
            currentlyChanging = false;
            EEPROM_WriteValue(bufferValor, indexMenu);
        }
    }

    return true;
}

uint8_t AmadeusBase::_getOpcionesSize(char const *str, char index) {
    uint16_t i=0;
    uint8_t size = (str[0]!='\0') ? 1 : 0;

    // recorro para conocer el largo
    while( str[i] != '\0' ) {
        if( str[i] == index ) 
            size++;
        i++;
    }

    return size;
}

uint8_t AmadeusBase::_splitOpciones(char const *str, char** buffer, uint8_t wordSize, char index) {
    uint8_t size = _getOpcionesSize(str, index);
    uint16_t i=0, baseIndex = 0;
    uint8_t relativeIndex = 0;

    for( uint8_t itemIndex=0 ; itemIndex<size ; itemIndex++ ) {
        baseIndex = itemIndex * wordSize;
        relativeIndex = 0;
        while( (str[i]!='\0') && (str[i]!=index) ) {
            if ( relativeIndex < (wordSize-1) ) {
                ((char*)buffer)[relativeIndex + baseIndex] = str[i];
                relativeIndex++;
            }
            i++;
        }
        ((char*)buffer)[relativeIndex + baseIndex] = '\0';
        i++;
    }

    return size;   // devuelve el tamaño final
}

int AmadeusBase::menu(const char* opciones) {
    uint8_t cantItems = _getOpcionesSize(opciones, ',');        //Serial.println(String("cantItems: ")+cantItems);
    uint8_t wordSize = getMaxLetters()-1;                       //Serial.println(String("wordSize: ")+wordSize);
    char items[cantItems][wordSize];                            //Serial.println(String("_menuPos: ")+_menuPos);
    _splitOpciones(opciones, (char**)items, wordSize, ',');     //for(uint8_t i=0;i<cantItems;i++) Serial.println(String(i)+": "+items[i]);
    if ( _menuPos > (cantItems-1) )
        _menuPos = 0;

    setIncrementalAcceleration(false);

    uint8_t topIndex = 0;
    if ( cantItems > getMaxRows() ) 
        topIndex = (_menuPos > (cantItems-getMaxRows())) ? (cantItems-getMaxRows()) : _menuPos;     
    uint8_t lastRowIndex = _menuPos;
    bool printMenuFlag = true;                                  //Serial.println(String("topIndex: ")+topIndex);
    while ( !enterPressed() ) 
    {
        if (printMenuFlag) { 
            lcdClear();
            for ( int i=0 ; i<getMaxRows() ; i++ ) {
                if ( (i+topIndex) < cantItems ) {
                    print( String(" ") + String(items[i+topIndex]), i, LCD_LEFT, false, 1, false );     //Serial.println(String(i)+": "+items[i+topIndex]);
                }
            }
            printMenuFlag = false;  
            print(">", (_menuPos - topIndex), LCD_LEFT, false, 0);
        }

        if ( cambiarVar( &_menuPos, 0, (cantItems-1) ) ) {   //Serial.println(String("_menuPos: ")+_menuPos);
            // si me pase de la zona imprimible
            if ( (_menuPos+1) > (topIndex + getMaxRows()) ) {
                topIndex++;
                printMenuFlag = true;
            }
            else if ( (_menuPos < topIndex) ) {
                topIndex--;
                printMenuFlag = true;
            }
            else {
                print(" ", (lastRowIndex - topIndex), LCD_LEFT, false, 0, false); 
                print(">", (_menuPos - topIndex), LCD_LEFT, false, 0);
            }
            lastRowIndex = _menuPos;
        }

        // devuelvo -1 si cancelo
        if( backPressed() ) return (-1);
        //if(enterPressed()) { while(enterPressed()); Serial.println("ENTER"); }
        //if(backPressed()) { while(backPressed()); Serial.println("BACK"); }
    }
    return (int)_menuPos;
}
