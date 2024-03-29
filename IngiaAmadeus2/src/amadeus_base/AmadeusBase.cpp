#include "AmadeusBase.h"

AmadeusBase * AmadeusBase::instance = NULL;

void AmadeusBase::cambiarVarMostrando(const char *nombre, long *var, long valmin, long valmax, const char *unidad, bool restart, uint8_t decimales) {
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
        String num = (decimales==0) ? String(*var) : String((float)(*var)/(float)(decimales*10));
        print( num+" ", 2, LCD_LEFT, false, 0 );

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
    char buffer[200], buffer_name[20], buffer_unid[10];
    strcpy(buffer, ""); 
    for ( uint8_t i=0 ; i<_cantMenus ; i++ ) {
        #ifdef __AVR__
            strcpy_P(buffer_name, (char*)pgm_read_word(&(_menus[i])));
        #else
            strcpy_P(buffer_name, _menus[i]);
        #endif
        strcat(buffer, buffer_name);
        if ( i!=(_cantMenus-1) ) strcat(buffer, ",");
    }
    
    int opcion = menu(buffer); 
    while ( opcion != (-1) ) { 
        long bufferVal = getVal(opcion); 
        #ifdef __AVR__
            strcpy_P(buffer_name, (char*)pgm_read_word(&(_menus[opcion]))); 
            strcpy_P(buffer_unid, (char*)pgm_read_word(&(_unids[opcion]))); 
            long buffer_valMin = pgm_read_word_near(_valmin + opcion); 
            long buffer_valMax = pgm_read_word_near(_valmax + opcion); 
        #else
            strcpy_P(buffer_name, _menus[opcion]); 
            strcpy_P(buffer_unid, _unids[opcion]); 
            long buffer_valMin = _valmin[opcion]; 
            long buffer_valMax = _valmax[opcion]; 
        #endif

        cambiarVarMostrando( buffer_name, &(bufferVal), buffer_valMin, buffer_valMax, buffer_unid, true );
        bool saveVal = false;
        while ( 1 ) {
            cambiarVarMostrando( buffer_name, &(bufferVal), buffer_valMin, buffer_valMax, buffer_unid );
            if ( enterPressed() ) { saveVal=true; break; }
            if ( backPressed() ) break;
            if ( custom1Pressed() ) {
                #ifdef __AVR__
                    bufferVal = pgm_read_word_near(_valores + opcion);
                #else
                    bufferVal = _valores[opcion];
                #endif
                cambiarVarMostrando( buffer_name, &(bufferVal), buffer_valMin, buffer_valMax, buffer_unid, true );
                while(custom1Pressed());
            }
        }
        if ( saveVal ) {
            EEPROM_WriteValue(bufferVal, opcion);
            printScreen("VALOR","GUARDADO","");
            delay(1000);
        }
        opcion = menu(buffer, false);
    } 
    return false;
}


uint8_t AmadeusBase::_splitOpciones(char const *str, uint16_t *startIndexes, uint16_t *endIndexes, uint8_t buffSize, char index) {
    uint16_t i=0;
    uint8_t wordIndex = 0;

    startIndexes[0] = 0;
    for( i=0 ; wordIndex<buffSize && str[i]!='\0' ; i++ ) {
        if ( str[i] == index ) {
            endIndexes[wordIndex] = i-1;
            wordIndex++;
            startIndexes[wordIndex] = i+1;
        }
    }
    endIndexes[wordIndex] = i-1; 

    return (wordIndex+1);   // devuelve el tamaño final
}


int AmadeusBase::menu(const char* opciones, bool resetPos, uint8_t* buffer_customPressed) {
    uint8_t maxWordLen = getMaxLetters()+1;
    char wordBuffer[maxWordLen] = "";
    uint16_t startIndexes[MAX_MENU_LEN], endIndexes[MAX_MENU_LEN];      
    uint8_t cantItems = _splitOpciones(opciones, startIndexes, endIndexes, MAX_MENU_LEN, ',');  

    // Serial.println("CANT: "+String(cantItems));
    // for(uint8_t i=0 ; i<cantItems ; i++) {  
    //     for(uint8_t j=startIndexes[i] ; j<=endIndexes[i] ; j++) Serial.print(opciones[j]); 
    //     Serial.println("");
    // }

    if ( resetPos ) {
    //if ( _menuPos > (cantItems-1) )
        _menuPos = 0;
    }

    setIncrementalAcceleration(false);

    uint8_t topIndex = 0;
    if ( cantItems > getMaxRows() ) 
        topIndex = (_menuPos > (cantItems-getMaxRows())) ? (cantItems-getMaxRows()) : _menuPos;     
    uint8_t lastRowIndex = _menuPos;
    bool printMenuFlag = true;          
    while ( !enterPressed() ) 
    {
        if (printMenuFlag) { 
            lcdClear();
            for ( int i=0 ; i<getMaxRows() ; i++ ) {
                if ( (i+topIndex) < cantItems ) {
                    memset((void*)wordBuffer, '\0', maxWordLen);
                    strncpy( wordBuffer, opciones+startIndexes[i+topIndex], endIndexes[i+topIndex]-startIndexes[i+topIndex]+1 );
                    print( String(" ") + String(wordBuffer), i, LCD_LEFT, false, 1, false );     
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
        if( custom1Pressed() && (buffer_customPressed!=NULL) ) {
            *buffer_customPressed = 1;
            break;
        }
    }
    return (int)_menuPos;
}
