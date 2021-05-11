# IngiaAmadeus2 Reference

## Impresion

## Control Motores

## Metodos
- [IngiaAmadeus2 Reference](#ingiaamadeus2-reference)
  - [Impresion](#impresion)
  - [Control Motores](#control-motores)
  - [Metodos](#metodos)
  - [Control Perifericos](#control-perifericos)
    - [move](#move)
    - [setSteppersEnable](#setsteppersenable)
  - [Rutinas Predesarrolladas](#rutinas-predesarrolladas)
    - [jogEncoder](#jogencoder)
    - [cambiarVarMostrando](#cambiarvarmostrando)
    - [login](#login)
    - [menuConfig](#menuconfig)
  - [Control Pantalla](#control-pantalla)
    - [changeFont](#changefont)
    - [drawBitmap](#drawbitmap)
    - [lcdClear](#lcdclear)
    - [print](#print)
    - [println](#println)
  

## Control Perifericos
### move
* **Prototipo:**
```
void IngiaAmadeus2::move(uint8_t motorNumber, long pasos)
```
* **Descripcion:** 
Mueve el motor seleccionado una determinada cantidad de pasos. El llamado es bloqueante hasta que finalice el movimiento, en caso de ser esto un problema checkear la seccion [Control Motores](#control-motores).
* **Argumentos:** 
  * motorNumber: Numero entero cuyo valor indica el motor que se desea mover (1 o 2).
  * pasos: Cantidad de pasos que se desean mover. Valores negativos cambiarán el sentido de giro.
* **Vease También:** [Control Motores](#control-motores)
* **Ejemplo:**
```
amadeus.move(1, 200); // girará el motor 1 200 pasos
```  

### setSteppersEnable
* **Prototipo**:
```
void IngiaAmadeus2::setSteppersEnable(boolean en)
```
* **Descripcion:**
Controla la habilitación de los motores, es decir si estos se encuentran o no en torque.
* **Argumentos:**
  * en: boolean que indica el estado de la habilitacion de los motores.
* **Vease También:** -

## Rutinas Predesarrolladas
### jogEncoder
* **Prototipo**:
```
void IngiaAmadeus2::jogEncoder()
```
* **Descripcion:**
Rutina de jog utilizando el encoder del Amadeus. Bloqueante hasta que el usuario salga de la rutina presionando doble click.
* **Argumentos:** -
* **Devuelve:** -
* **Vease También:**

### cambiarVarMostrando
* **Prototipo**:
```
void IngiaAmadeus2::cambiarVarMostrando(char *nombre, long *var, long valmin, long valmax, char *unidad = NULL, boolean restart = false)
```
* **Descripcion:**
Permite modificar manualmente con el encoder rotativo el valor de una variable de tipo long apuntada por el puntero `var`. La variable podrá tomar valores enteros entre `valmin` y `valmax`, y en pantalla se mostrará el valor actual junto con su `unidad` y `nombre` correspondientes. `restart` indicara si es la primera vez que se la llama, de manera de imprimir o no toda la informacion extra (en lugar de solo refrescar el valor en pantalla). El llamado a esta funcion no es bloqueante.
* **Argumentos:**
* **Devuelve:** 
* **Vease También:** [Impresion](#impresion)
* **Ejemplo:**
```

```

### login

### menuConfig


## Control Pantalla
### changeFont

### drawBitmap

### lcdClear

### print

### println