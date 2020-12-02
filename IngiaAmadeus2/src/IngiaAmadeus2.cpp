
#include "IngiaAmadeus2.h"

// ################################################################
// ######################### IngiaAmadeus2 #########################
// ################################################################

void IngiaAmadeus2::jogEncoder(uint8_t motorNumber, float defaultSpeed) {
    AccelStepper *stepper = (motorNumber==1) ? &stepper1 : &stepper2;
    jogEncoder_Base(stepper, "", defaultSpeed);
}


// ################################################################
// ##################### IngiaAmadeus2_Base #######################
// ################################################################

bool IngiaAmadeus2_Base::begin() {
    init(MI_FUENTE, 5, 0, 0);      // inicializo lo de AmadeusLCD
    //drawBitmap_Curtain(logo_INGIA, LOGO_INGIA_WIDTH, LOGO_INGIA_HEIGHT, 12, 6);
    drawBitmap(logo_INGIA, LOGO_INGIA_WIDTH, LOGO_INGIA_HEIGHT);
    delay(1500);
    setRain(true, 20);
    for( uint8_t i=0 ; i<15 ; i++ ) {
        print(F("ingia.com.ar"), 1, LCD_CENTER, true, -1, false);
        print(F("AMADEUS M2"), 0, LCD_CENTER, false, -1, true);
        delay(200);
    }
    setRain(false);
    lcdClear();

    // inicializo EEPROM
    #ifndef __AVR__
    EEPROM.begin(512); // EEPROM size
    #endif
    for ( uint8_t i=0 ; i<_cantMenus ; i++ ) {
        if ( (getVal(i) < (long)pgm_read_dword(&_valmin[i])) || (getVal(i) > (long)pgm_read_dword(&_valmax[i])) )
            EEPROM.put ( sizeof(long)*i, pgm_read_dword(&_valores[i]) );
    }
    #ifndef __AVR__
    EEPROM.commit();
    #endif

    // init timer isr
    #ifdef __AVR__
        Timer1.initialize(1000);
        Timer1.attachInterrupt(timer1Isr); 
    #else
        set_timer(true);
    #endif

    // para que suelte el boton si lo tiene presionado
    bool mantuvoPresonado = encoderPtr->isPressed();
    if ( mantuvoPresonado ) {
        print(F("CONFIGURACION"), 1, LCD_CENTER, true);
        delay(1500);
        while(encoderPtr->isPressed());
    }

    return mantuvoPresonado;
}

bool IngiaAmadeus2_Base::cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin) {
    bool changeFlag = false;   

    long delta = encoderPtr->getValue();
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

void IngiaAmadeus2_Base::cambiarString(const char *title, char *buffer, uint8_t size, char *opc) {
    char opc_buffer[100];
    char *m_opc = opc;
    if ( opc == NULL ) {
        uint8_t i;
        for ( i=32 ; i<=125 ; i++ ) opc_buffer[i-32] = i;
        opc_buffer[i] = '\0';
        m_opc = opc_buffer;
    }
    uint32_t opcSize = strlen(m_opc);
    setIncrementalAcceleration(false);

    long letterIndex = 0;
    uint8_t letter_startIndex = 0, last_letter = 0;
    uint8_t str_len = strlen(buffer);
    bool printFlag = true;
    lcdClear();
    print(title, 0, LCD_CENTER, false, -1, false);
    while( !backPressed() ) {
        // cambiar la letterIndex
        if ( cambiarVar(&letterIndex, 0, str_len) || printFlag ) { 
            printFlag = false;
            clearRow(1); clearRow(2);
            if ( str_len > getMaxLetters() ) {
                uint8_t rel_index = letterIndex - letter_startIndex;
                if ( last_letter < letterIndex ) {
                    if ( rel_index>(getMaxLetters()/2) && (str_len-letter_startIndex)>=getMaxLetters() )
                        letter_startIndex++;
                }
                else {
                    if ( rel_index<(getMaxLetters()/2) && letter_startIndex>0 )
                        letter_startIndex--;
                }
                last_letter = letterIndex;
            }
            if ( (str_len-letter_startIndex) >= getMaxLetters() ) print(">", 2, LCD_RIGHT, false, -1, false);
            if ( letter_startIndex > 0 ) print("<", 2, LCD_LEFT, false, 0, false);
            print("^", 2, LCD_LEFT, false, letterIndex-letter_startIndex, false);
            print( String(buffer).substring(letter_startIndex,letter_startIndex+getMaxLetters()), 1, LCD_LEFT, false, 0 );
            while(encoderPtr->isHeld()); // espero que suelte si lo tenia
        }
        // cambiar la letra
        if ( enterPressed() && letterIndex<(size-1) ) {
            if ( letterIndex == str_len ) {
                buffer[letterIndex+1] = '\0';
                str_len++;
            }
            long i = 0;
            // find i si ya habia una letra
            for ( uint8_t j=0 ; j<opcSize ; j++ ) {
                if(m_opc[j]==buffer[letterIndex]) i=j;
            }
            printFlag = true;
            print("*", 2, LCD_LEFT, false, letterIndex-letter_startIndex, false);
            while( !enterPressed() ) {
                if ( cambiarVar(&i, 0, opcSize) ) {
                    buffer[letterIndex] = m_opc[i];
                    printFlag = true;
                }
                if ( printFlag ) {
                    printFlag = false;
                    print( String(buffer).substring(letter_startIndex,letter_startIndex+getMaxLetters()), 
                        1, LCD_LEFT, false, 0 );
                }
            }
            print("^", 2, LCD_LEFT, false, letterIndex-letter_startIndex);
        }
        // borrar la letra
        if ( encoderPtr->isHeld() && letterIndex<str_len ) {
            String aux = buffer;
            aux.remove(letterIndex,1);
            aux.toCharArray(buffer, size);
            str_len--;
            printFlag = true;
        }
    }
}

bool IngiaAmadeus2_Base::login(char const *password) { 
    uint8_t passSize = strlen(password); passSize = (passSize<4)?passSize:4;
    uint8_t digitoSeleccionado = 0;
    bool correctPass = false;

    // dinamicas para adaptarse a la cantidad de digitos
    long passArray[4];
    uint8_t posDigito[4]; 

    uint8_t espacioSobrante = getMaxLetters() - ((passSize*2)-1);
    for( uint8_t i=0 ; i<passSize ; i++ ) {
        posDigito[i] = (espacioSobrante/2) + i*2;
        passArray[i] = 0;
    }

    bool printFlag = true;
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

        switch( encoderPtr->getButton() ) {
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

    return true;
}


void IngiaAmadeus2_Base::jogEncoder_Base(AccelStepper *stepper, String title, float defaultSpeed) {
    long speed = (long)defaultSpeed;
    float dir = 1;
    stepper->setSpeed((float)speed);
    boolean running = false;
    encoderPtr->setHoldTime(250);
    encoderPtr->setAccelerationEnabled(true);

    lcdClear();
    if ( title.length() == 0 )
        print("JOG", 0, LCD_CENTER, false, -1, false);
    else
        print(title, 0, LCD_CENTER, false, -1, false);
    print("Vel:", 1, LCD_LEFT, false, 0, false);
    print( (dir>0)?("->"):("<-"), 2, LCD_RIGHT, false, -1, false );
    print(speed, 1, LCD_RIGHT, false, -1, true);


    boolean escape = false;
    while( !escape ) {
        int flagMaxMin;
        if( cambiarVar(&speed, 0, 4000) ) {
            print(" "+String(speed), 1, LCD_RIGHT, false, -1, true);
        }

        switch ( encoderPtr->getButton() ) {
            case AmadeusEncoder::Held:
                if( !running ) {
                    stepper->setSpeed((float)speed * dir);
                    running = true;
                }
                stepper->runSpeed();
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

    encoderPtr->setHoldTime(2000);  // back to default
}
