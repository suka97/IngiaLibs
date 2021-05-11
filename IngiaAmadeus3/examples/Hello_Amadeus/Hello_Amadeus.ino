#include <IngiaAmadeus2.h>

// nombres de los parametros
const char menu1[] PROGMEM = "Vel JOG";
const char menu2[] PROGMEM = "Vel Auto 1";
const char menu3[] PROGMEM = "Vel Auto 2";
const char menu4[] PROGMEM = "Aceleracion";
const char menu5[] PROGMEM = "Largo Total";
const char menu6[] PROGMEM = "Delay";
const char * const menus[] PROGMEM =    { menu1,      menu2,    menu3,      menu4,      menu5,    menu6 };

// unidades que se van a utilizar
const char unid_null[] PROGMEM = "";
const char unid_ms[] PROGMEM = "ms";
const char unid_vel[] PROGMEM = "p/s";
const char unid_dist[] PROGMEM = "p";
// asigno su unidad a cada parametro
const char * const unids[] PROGMEM =    { unid_vel,  unid_vel,  unid_vel,   unid_null,  unid_dist,  unid_ms };
                  
// valores de los parametros
const long valoresDefault[] PROGMEM  =  { 1000L,     1000L,     1000L,      2000L,      1000L,      2000L };                                  
const long valmin[] PROGMEM =           { 100L,      100L,      100L,       100L,       1L,         10L   };        
const long valmax[] PROGMEM =           { 4000L,     4000L,     4000L,      50000L,     100000L,    36000L }; 
                  
// asigno un numero a cada parametro para despues poder pedirselos a la clase por medio de getVal()
enum Valores { VEL_JOG, VEL_AUTO_1, VEL_AUTO_2, ACELERACION, LARGO_TOTAL, DELAY
  ,CANT_MENUS};

// instancio la clase IngiaAmadeus2 con esto
IngiaAmadeus2 amadeus(menus, valoresDefault, unids, valmin, valmax, CANT_MENUS);

void setup() {
  // Si se quiere guardar los valores default de los parametros de la EEPROM quitar el comentario
  // amadeus.EEPROM_WriteValues();

  // Iniciañizo la clase IngiaAmadeus2, esto devuelve true si mantuvo presionado el encoder luego la pantalla de inicio
  // Aqui designare los estados de cada entrada/salida (por default todos son OUTPUTS)
  if ( amadeus.begin(INPUT_PULLUP /*pinA*/, INPUT_PULLUP /*pinB*/, INPUT_PULLUP /*pinC*/) ) 
  {
    configuracionAvanzada();
  }
}

void loop() {
  MiPrograma();
}


void configuracionAvanzada() {
  // si se quiere un menu de login
  if ( amadeus.login("111" /*contraseña numerica*/) ) {
    // muestro un mensaje que diga que logro entrar al "menu de parametros"
    amadeus.print("Menu", 0, LCD_CENTER, true);
    amadeus.print("Parametros", 1, LCD_CENTER, false);
    delay(1500);
    
    configuracionAvanzada_SinLogin();
  }    
}

void configuracionAvanzada_SinLogin(){
  // menuParametros() no es bloquante, devolvera false cuando el usuario decida salir
  while( amadeus.menuParametros() ); 

  // muestro un mensaje que diga que salio del menu
  amadeus.print("Saliendo", 1, LCD_CENTER, true);
  delay(1500);
}