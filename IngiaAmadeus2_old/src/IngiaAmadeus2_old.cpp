
#include "IngiaAmadeus2_old.h"

IngiaAmadeus2_old * IngiaAmadeus2_old::instance = NULL;

boolean IngiaAmadeus2_old::begin(uint8_t pinModeA, uint8_t pinModeB, uint8_t pinModeC, uint8_t pinModeD, uint8_t pinModeE, uint8_t pinModeF) 
{    
    pinMode(PIN_IO_A, pinModeA);
    pinMode(PIN_IO_B, pinModeB);
    pinMode(PIN_IO_C, pinModeC);
    pinMode(PIN_IO_D, pinModeD);
    pinMode(PIN_IO_E, pinModeE);
    pinMode(PIN_IO_F, pinModeF);
    pinMode(PIN_STEPPER_ENABLE, OUTPUT);
    
    return begin_CustomHardWare();
}

boolean IngiaAmadeus2_old::begin_CustomHardWare() {
    for ( int i=0 ; i<_cantMotores ; i++ ) {
        stepper[i]->setMaxSpeed(2000);      // default initial max speed
        stepper[i]->setAcceleration(1000);  // default initial acceleration
    }

    #ifdef __AVR__
        Timer1.initialize(1000);
        Timer1.attachInterrupt(timer1Isr); 
    #else
        toggleTimer();
    #endif
    encoder->setAccelerationEnabled(true);

    init(MI_FUENTE, 5, 0, 0);      // inicializo lo de AmadeusLCD
    //drawBitmap_Curtain(logo_INGIA, LOGO_INGIA_WIDTH, LOGO_INGIA_HEIGHT, 12, 6);
    drawBitmap(logo_INGIA, LOGO_INGIA_WIDTH, LOGO_INGIA_HEIGHT);
    delay(1500);
    setRain(true, 20);
    for( uint8_t i=0 ; i<15 ; i++ ) {
        print(F("www.ingia.com.ar"), 1, LCD_CENTER, true, -1, false);
        print(F("AMADEUS M2"), 0, LCD_CENTER, false, -1, true);
        delay(200);
    }
    setRain(false);
    lcdClear();

    // checkeo que ninguno de los parametros este fuera de rango
    for ( uint8_t i=0 ; i<_cantMenus ; i++ ) {
        if ( (getVal(i) < (long)pgm_read_dword(&_valmin[i])) || (getVal(i) > (long)pgm_read_dword(&_valmax[i])) )
            EEPROM.put ( sizeof(long)*i, pgm_read_dword(&_valores[i]) );
    }

    // para que suelte el boton si lo tiene presionado
    boolean mantuvoPresonado = encoder->isPressed();
    if ( mantuvoPresonado ) {
        print(F("Configuracion"), 1, LCD_CENTER, true);
        delay(1500);
        while(encoder->isPressed());
    }

    return mantuvoPresonado;
}

boolean IngiaAmadeus2_old::cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin) {
    boolean changeFlag = false;   

    long delta = encoder->getValue();
    if ( delta != 0 ) {
        if ( (delta>0) && ((*var)<valmax) ) {
            (*var) += delta;
            changeFlag = true;
        }
        if ( (delta<0) && ((*var)>valmin) ) {
            (*var) += delta;
            changeFlag = true;
        }
    }

    if ( ((*var)>valmin) && ((*var)<valmax) ) { 
        if(flagMaxMin != NULL)
            (*flagMaxMin) = 0;
    }
    else {
        if( (*var) >= valmax ) {
            (*var) = valmax;
            if(flagMaxMin != NULL)
                (*flagMaxMin) = 1;
        }
        if ( (*var) <= valmin ) { 
            (*var) = valmin;
            if(flagMaxMin != NULL)
                (*flagMaxMin) = (-1);
        } 
    }

    return changeFlag;
}

void IngiaAmadeus2_old::cambiarVarMostrando(char *nombre, long *var, long valmin, long valmax, char *unidad, boolean restart) {
    static int flagMinMax = 0;
    static boolean borderValueSituation = false;    // indica si la ultima vez estaba escrito max o min 
    
    if ( restart ) {
        encoder->setAccelerationEnabled(true);
        lcdClear();
        print(nombre, 0, LCD_CENTER);
        if ( unidad != NULL )
            print(unidad, 2, LCD_RIGHT);
    }

    boolean valueChanged = cambiarVar(var, valmin, valmax, &flagMinMax);
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

boolean IngiaAmadeus2_old::menuParametros() {
    static int topIndex = 0;
    static boolean printMenuFlag = true, currentlyChanging = false;
    static long indexMenu = 0;  // lo hago long para poder usar cambiarVar()
    static long bufferValor;
    static uint8_t lastRowIndex = 0;

    char bufferMenu[getMaxLetters()], bufferUnid[10];

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

            encoder->setAccelerationEnabled(false);
        }

        if ( cambiarVar(&indexMenu, 0, (_cantMenus-1)) ) {
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
        
        switch( encoder->getButton() ) {
            case encoder->DoubleClicked:
                printMenuFlag = true;
                lcdClear();
                return false;
            case encoder->Clicked:
                bufferValor = getVal(indexMenu);
                currentlyChanging = true;
                encoder->setAccelerationEnabled(true);

                strcpy_P(bufferMenu, (char*)pgm_read_word(&(_menus[indexMenu]))); // no tocar renglon ;)
                strcpy_P(bufferUnid, (char*)pgm_read_word(&(_unids[indexMenu]))); // no tocar renglon ;)
                delay(200);
            default:
                break;
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

        AmadeusEncoder::ButtonStatus_t boton = encoder->getButton();
        if ( (boton == encoder->DoubleClicked) || (boton == encoder->Clicked) ) {
            currentlyChanging = false;
            EEPROM_WriteValue(bufferValor, indexMenu);
        }
    }

    return true;
}

void IngiaAmadeus2_old::EEPROM_WriteValues() {
    for ( int i=0 ; i<_cantMenus ; i++ )
        EEPROM.put ( sizeof(long)*i, pgm_read_dword(&_valores[i]) );
}

void IngiaAmadeus2_old::EEPROM_WriteValue(long value, int i) {
    EEPROM.put ( sizeof(long)*i, value );
}

void IngiaAmadeus2_old::EEPROM_Clear() {
    for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

boolean IngiaAmadeus2_old::login(char const *password) { 
    uint8_t passSize = strlen(password);
    uint8_t digitoSeleccionado = 0;
    boolean correctPass = false;

    // dinamicas para adaptarse a la cantidad de digitos
    long *passArray = new long[passSize];
    uint8_t *posDigito = new uint8_t[passSize]; 

    uint8_t espacioSobrante = getMaxLetters() - ((passSize*2)-1);
    for( uint8_t i=0 ; i<passSize ; i++ ) {
        posDigito[i] = (espacioSobrante/2) + i*2;
        passArray[i] = 0;
    }

    boolean printFlag = true;
    while(!correctPass) {
        if ( printFlag ) {
            lcdClear();
            print(String("Contrase")+String(SIMB_ENIE)+String("a"), 0, LCD_CENTER, false, -1, false);
            print("^", 2, LCD_LEFT, false, posDigito[digitoSeleccionado], false);
            for( uint8_t i=0 ; i<passSize ; i++ ) {
                print(passArray[i], 1, LCD_LEFT, false, posDigito[i], true); 
            }
            printFlag = false;
        }

        if ( cambiarVar(&passArray[digitoSeleccionado], 0, 9) ) {
            print(passArray[digitoSeleccionado], 1, LCD_LEFT, false, posDigito[digitoSeleccionado]);
        }

        switch( encoder->getButton() ) {
            case AmadeusEncoder::Held: 
            // checkeo el pass
                correctPass = true;
                for( uint8_t i=0 ; i<passSize ; i++ ) {
                    if ( String(password[i]) != String(passArray[i]) ) {
                        correctPass = false;
                        printFlag = true;

                        // muestro que la metio mal
                        lcdClear();
                        print(String("Contrase")+String(SIMB_ENIE)+String("a"), 0, LCD_CENTER, false, -1, false);
                        print(F("Incorrecta"), 1, LCD_CENTER);
                        delay(1500);

                        break;
                    }
                }
                break;
            case AmadeusEncoder::Clicked: 
            // cambio de digitoSeleccionado
                print(" ", 2, LCD_LEFT, false, posDigito[digitoSeleccionado], false);
                digitoSeleccionado = (digitoSeleccionado < (passSize-1)) ? (digitoSeleccionado+1) : 0; 
                print("^", 2, LCD_LEFT, false, posDigito[digitoSeleccionado], true);
                break;
            case AmadeusEncoder::DoubleClicked:
                print("Cancelado", 1, LCD_CENTER, true);
                delay(1500);
                return false;    // cancelar

            default: break;
        }
    }

    // free assigned memory
    delete [] posDigito;
    delete [] passArray;

    return true;
}

void IngiaAmadeus2_old::jogEncoder(uint8_t motorNumber, float defaultSpeed) {
    long speed = (long)defaultSpeed;
    float dir = 1;
    stepper[motorNumber-1]->setSpeed((float)speed);
    boolean running = false;
    encoder->setHoldTime(250);
    encoder->setAccelerationEnabled(true);

    lcdClear();
    print("JOG", 0, LCD_CENTER, false, -1, false);
    print("Vel:", 1, LCD_LEFT, false, 0, false);
    print( (dir>0)?("->"):("<-"), 2, LCD_RIGHT, false, -1, false );
    print(speed, 1, LCD_RIGHT, false, -1, true);


    boolean escape = false;
    while( !escape ) {
        if( cambiarVar(&speed, 0, 4000) ) {
            print(" "+String(speed), 1, LCD_RIGHT, false, -1, true);
        }

        switch ( encoder->getButton() ) {
            case AmadeusEncoder::Held:
                if( !running ) {
                    stepper[motorNumber-1]->setSpeed((float)speed * dir);
                    running = true;
                }
                stepper[motorNumber-1]->runSpeed();
                break;
            
            case AmadeusEncoder::DoubleClicked:
                escape = true;
                break;

            case AmadeusEncoder::Clicked:
                dir = -dir;
                print( (dir>0)?("->"):("<-"), 2, LCD_RIGHT, false, -1, true );
                break;

            default:
                running = false;
        }
    }

    encoder->setHoldTime(2000);  // back to default
}

uint8_t IngiaAmadeus2_old::_getOpcionesSize(char const *str, char index) {
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

uint8_t IngiaAmadeus2_old::_splitOpciones(char const *str, char** buffer, uint8_t wordSize, char index) {
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

int IngiaAmadeus2_old::menu(const char* opciones) {
    uint8_t cantItems = _getOpcionesSize(opciones, ',');
    uint8_t wordSize = getMaxLetters()-1;
    char items[cantItems][wordSize];
    _splitOpciones(opciones, (char**)items, wordSize, ',');
    if ( _menuPos > (cantItems-1) )
        _menuPos = 0;

    encoder->setAccelerationEnabled(false);

    uint8_t topIndex = 0;
    if ( cantItems > getMaxRows() )
        topIndex = (_menuPos > (cantItems-getMaxRows())) ? (cantItems-getMaxRows()) : _menuPos;
    uint8_t lastRowIndex = _menuPos;
    boolean printMenuFlag = true;
    AmadeusEncoder::ButtonStatus_t button = encoder->getButton();
    while ( button == AmadeusEncoder::Open ) 
    {
        if (printMenuFlag) { 
            lcdClear();
            for ( int i=0 ; i<getMaxRows() ; i++ ) {
                if ( (i+topIndex) < cantItems ) {
                    print( String(" ") + String(items[i+topIndex]), i, LCD_LEFT, false, 1, false );
                }
            }
            printMenuFlag = false;  
            print(">", (_menuPos - topIndex), LCD_LEFT, false, 0);
        }

        if ( cambiarVar( (long*)&_menuPos, 0, (cantItems-1) ) ) {
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
        
        button = encoder->getButton();
    }

    // devuelvo -1 si mantuvo apretado (osea cancelo)
    return (button == AmadeusEncoder::DoubleClicked) ? (-1) : (int)_menuPos;
}

void IngiaAmadeus2_old::move(uint8_t motorNumber, long pasos) {
    if ( (motorNumber == 1) || (motorNumber == 2) ) {
        stepper[motorNumber-1]->move(pasos);
        while( stepper[motorNumber-1]->run() );
    }
}
