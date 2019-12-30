
void MiPrograma() {
    switch ( amadeus.menu("JOG,Loop Mov,Config") ) {
        case 0:
            // caso presione "JOG"
            amadeus.jogEncoder(1, amadeus.getVal(VEL_JOG));
            break;
        case 1:
            // caso presione "Loop Mov"
            loopMov();
            break;
        case 2:
            configuracionAvanzada_SinLogin();
            break;
        default:
            // caso que haya hecho doble click, osea cancelo el menu
            amadeus.print("Se intento", 0, LCD_CENTER, true);
            amadeus.print("salir", 1, LCD_CENTER, false);
            delay(1500);
            break;
    }
}


void loopMov() {
    // seteo las velocidades y aceleraciones con los valores guardados
    amadeus.stepper1.setAcceleration( amadeus.getVal(ACELERACION) );
    amadeus.stepper2.setAcceleration( amadeus.getVal(ACELERACION) );
    amadeus.stepper1.setMaxSpeed( amadeus.getVal(VEL_AUTO_1) );
    amadeus.stepper2.setMaxSpeed( amadeus.getVal(VEL_AUTO_2) );

    while(1) 
    {
        // el codigo comentado es si se quisiera hacer un movimiento bloqueante 
        // (no se podria cancelar manteniando apretado el encoder)
        /*
        amadeus.move( 1, amadeus.getVal(LARGO_TOTAL) );
        delay( amadeus.getVal(DELAY) );
        amadeus.move( 1, -amadeus.getVal(LARGO_TOTAL) );
        */

        // muestro un mensaje
        amadeus.print("Moviendo", 0, LCD_CENTER, true);
        amadeus.print("para un", 1, LCD_CENTER, false);
        amadeus.print("lado", 2, LCD_CENTER, false);
        amadeus.stepper1.move( amadeus.getVal(LARGO_TOTAL) );

        // hago el movimiento
        while( amadeus.stepper1.run() ) {
            // si aprieto el encoder salgo del loop
            if ( amadeus.encoder.isDoubleClicked() ) {
                amadeus.print("Cancelado", 1, LCD_CENTER, true);
                delay(1500);
                return;
            }
        }

        // espero el delay guardado en la configuracion
        amadeus.print("Esperando", 1, LCD_CENTER, true);
        delay( amadeus.getVal(DELAY) );

        // muestro un mensaje
        amadeus.print("Moviendo", 0, LCD_CENTER, true);
        amadeus.print("para el", 1, LCD_CENTER, false);
        amadeus.print("otro", 2, LCD_CENTER, false);
        
        // hago el movimiento
        amadeus.stepper1.move( -amadeus.getVal(LARGO_TOTAL) );
        while( amadeus.stepper1.run() ) {
            // si aprieto el encoder salgo del loop
            if ( amadeus.encoder.isDoubleClicked() ) {
                amadeus.print("Cancelado", 1, LCD_CENTER, true);
                delay(1500);
                return;
            }
        }

        // espero el delay guardado en la configuracion
        amadeus.print("Esperando", 1, LCD_CENTER, true);
        delay( amadeus.getVal(DELAY) );
    }
}