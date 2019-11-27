
#include "IngiaAmadeus.h"

const char *IngiaAmadeus::LCD_RENGLON_LIMPIO = "                ";

void IngiaAmadeus::print(char *str, int row, PrintFlag printFlag, boolean clearFlag, int startIndex) {
    static int lastIndex = 0, lastRow = 0;
    if (clearFlag) {
        _lcd.clear();
        lastIndex = 0;
    }
    if ( lastRow != row ) {
        lastIndex = 0;
        lastRow = row;
    }
    
    switch (printFlag) {
        case LCD_LEFT:
            if ( startIndex != -1 )
                lastIndex = startIndex;
            break;
        case LCD_RIGHT:
            lastIndex = LCD_CANT_CARACTERES-strlen(str);
            break;
        case LCD_CENTER:
            lastIndex = (LCD_CANT_CARACTERES-strlen(str)) / 2;
            break;
    }
    _lcd.setCursor(lastIndex, row);
    _lcd.print(str);
    lastIndex += strlen(str);
}

void IngiaAmadeus::println(char *str, int row, PrintFlag printFlag, boolean clearFlag) {
    if(!clearFlag) {
        _lcd.setCursor(0, row);
        _lcd.print("                ");
    }    
    else
        _lcd.clear();


    print(str, row, printFlag, false, 0);
}

void IngiaAmadeus::lcdClear(boolean mantenerPos) {
    if(mantenerPos)
        _lcd.clear();
    else
        println("", 0, LCD_LEFT, true);
}

boolean IngiaAmadeus::begin(boolean writeArraysEEPROM) {
    boolean mantuvoSelectPresionado = false;
    
    pinMode(PIN_INPUT_A1, INPUT_PULLUP); _pinInOut[0] = PIN_INPUT_A1;
    pinMode(PIN_INPUT_A2, INPUT_PULLUP); _pinInOut[1] = PIN_INPUT_A2;
    pinMode(PIN_INPUT_A3, INPUT_PULLUP); _pinInOut[2] = PIN_INPUT_A3;
    
    stepper.setMaxSpeed(2000);
    stepper.setAcceleration(1000);

    if (writeArraysEEPROM)
        EEPROM_WriteValues();
    else
        EEPROM_ReadValues();

    _lcd.begin(LCD_CANT_CARACTERES, 2);
    print("INGIA srl", 0, LCD_CENTER, true);
    
    for ( long i=0 ; i<2000 ; i++) {
        buttons.update();
        if ( buttons.isPressedAfter(BUTTON_SELECT, 1800) )
            mantuvoSelectPresionado = true;
        delay(1);
    }
    return mantuvoSelectPresionado;
}

char * IngiaAmadeus::longToString(long n, char *str) {
    int index = 0, cantNumeros = 0;
    long aux;
    if ( n < 0 ) {
        str[index] = '-';
        index++;
        n = -n;
    } aux = n;
    while(n > 9) {
        n = n / 10;
        cantNumeros++;
    } n = aux;
    while (cantNumeros >= 0) {
        long _pow = mPow(10, cantNumeros); 
        aux = n / _pow; 
        n = n % _pow; 
        str[index] = aux + 48; 
        cantNumeros--; index++;
    }
    str[index] = '\0';
    return str;
}

boolean IngiaAmadeus::cambiarVar(long *var, long valmin, long valmax, int *flagMaxMin, boolean addDelay) {
    static int m=1, n=1;
    static int lastFlagMaxMin = 0;
    boolean changeFlag = false;   
    buttons.update();
    if (buttons.onPress(BUTTON_RIGHT)) { 
        if ( (*var) < valmax ) {
            (*var)++; 
            changeFlag = true;
        }
    }
    if ( buttons.isPressedAfter(BUTTON_RIGHT, 500)) {
        if ( (*var) < valmax ) 
            (*var) += m; changeFlag = true; 
    }
    if ( buttons.isPressedAfter(BUTTON_RIGHT, m * 1000) ) { 
        if (n < 4) {
            m = m * mPow(5, n-1);
            n++;
        }
    }
    if (buttons.onRelease(BUTTON_RIGHT)) {
        m = 1; n = 1;
    }

    if (buttons.onPress(BUTTON_LEFT)) {
        if ( (*var) > valmin ) {
            (*var)--;
            changeFlag = true;
        }
    }
    if ( buttons.isPressedAfter(BUTTON_LEFT, 500)) { 
        if ( (*var) > valmin ) 
            (*var) -= m; changeFlag = true; 
    }
    if (buttons.isPressedAfter(BUTTON_LEFT, m * 1000)) {
        if (n < 4) {
            m = m * mPow(5, n-1);
            n++;
        }
    }
    if (buttons.onRelease(BUTTON_LEFT)) {
        m = 1; n = 1;
    }

    // si se fue de rango
    if ( (*var) < valmin ) {
        (*var) = valmin;
        changeFlag = true;
    }
    else if ( (*var) > valmax ) { 
        (*var) = valmax;
        changeFlag = true;
    }
    // flagMaxMin
    if ( flagMaxMin != NULL ) { 
        (*flagMaxMin) = 0;
        if( (*var) == valmax ) { 
            (*flagMaxMin) = 1; 
        }
        if ( (*var) == valmin ) { 
            (*flagMaxMin) = -1; 
        }
        if ( lastFlagMaxMin != (*flagMaxMin) ) {
            lastFlagMaxMin = (*flagMaxMin); changeFlag = true;
        }
    }
    if (addDelay)
        delay(50);
    return changeFlag;
}

void IngiaAmadeus::menuParametros() {
    static int indexMenu = 0, flagMaxMin = 0;
    static boolean printFlag = true, selectToggled = true;
    char bufferString[16];

    if (printFlag) {
        printFlag = false;
        lcdClear();
        print(_menus[indexMenu], 0); 
        print(longToString(_valores[indexMenu], bufferString), 1); print(_unids[indexMenu], 1, LCD_RIGHT);
        if ( flagMaxMin == 1 )
            print("max", 0, LCD_RIGHT);
        else if ( flagMaxMin == (-1) )
            print("min", 0, LCD_RIGHT);
    }
    delay(100);

    printFlag = cambiarVar(&(_valores[indexMenu]), _valmin[indexMenu], _valmax[indexMenu], &flagMaxMin);

    if (buttons.onPress(BUTTON_UP)) {
        if (indexMenu > 0) {
            indexMenu--;
            printFlag = true; flagMaxMin = 0;
        }
    }
    if (buttons.onPress(BUTTON_DOWN)) {
        if ( indexMenu < (_cantMenus-1) ) {
            indexMenu++;
            printFlag = true; flagMaxMin = 0;
        }
    }

    if (buttons.isPressedAfter(BUTTON_SELECT, 2000)) {
        if( !selectToggled ) {
            EEPROM_WriteValue(_valores[indexMenu], indexMenu);
            print("Valor guardado", 0 , LCD_CENTER, true);
            delay(1000);
        }
    }
    if (buttons.onRelease(BUTTON_SELECT)) {
        selectToggled = false;
        printFlag = true;
    }
}

void IngiaAmadeus::EEPROM_ReadValues() {
    for ( int i=0 ; i<_cantMenus ; i++ ) 
        EEPROM.get ( sizeof(long)*i, _valores[i] );
}

void IngiaAmadeus::EEPROM_WriteValues() {
    for ( int i=0 ; i<_cantMenus ; i++ )
        EEPROM.put ( sizeof(long)*i, _valores[i] );
}

void IngiaAmadeus::EEPROM_WriteValue(long value, int i) {
    EEPROM.put ( sizeof(long)*i, value );
}

void IngiaAmadeus::EEPROM_Clear() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

boolean IngiaAmadeus::jog(float defaultSpeed, Button buttonEsc, boolean restart) {
    static boolean screenChangeFlag = true;
    static long speed = -1;
    char strBuffer[16];
    static int dirFlag = 1;

    if(restart) {
        screenChangeFlag = true;
        return false;
    }

    if ( speed == (-1) ) {
        speed = (long)defaultSpeed;
        stepper.setSpeed((float)speed * (float)dirFlag);
    }

    if ( screenChangeFlag ) { 
        screenChangeFlag = false;
        lcdClear();
        print("JOG", 0, LCD_CENTER);
        print("Vel:", 1, LCD_LEFT); print(longToString(speed, strBuffer), 1, LCD_RIGHT);
        delay(150);
    }
    screenChangeFlag = cambiarVar(&speed, 100, 4000); 

    buttons.update();

    if ( buttons.isPressed(BUTTON_UP) ) {
        if ( dirFlag != 1 ) {
            dirFlag = 1;
            stepper.setSpeed((float)speed * (float)dirFlag);
        }
        stepper.runSpeed();
    }
    if ( buttons.isPressed(BUTTON_DOWN) ) {
        if ( dirFlag != (-1) ) {
            dirFlag = -1;
            stepper.setSpeed((float)speed * (float)dirFlag);
        }
        stepper.runSpeed();
    }

    boolean salida = false;
    if ( buttonEsc != (-1) ) {
        if ( buttons.isPressed(buttonEsc) ) {
            salida = true;
            speed = -1;
        }
    }
    return salida;
}

