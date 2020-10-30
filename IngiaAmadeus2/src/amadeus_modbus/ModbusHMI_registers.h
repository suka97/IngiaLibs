#ifndef ModbusHMI_registers_h
#define ModbusHMI_registers_h

#define LETTERS_X_ROW       15    
#define REGS_X_RENGLON      ((LETTERS_X_ROW/2)+(LETTERS_X_ROW%2))

// Keyboard Registers
#define BUT_CLICK_REG       0
#define BUT_RELEASE_REG     (BUT_CLICK_REG+1)
#define BUT_STATE1_REG      (BUT_RELEASE_REG+1)
#define BUT_STATE2_REG      (BUT_STATE1_REG+1)

// LCD rows Regs
#define LCD_PRINT_REG        (BUT_STATE2_REG+1)
#define LCD_ROW1_BUFF_REG    (LCD_PRINT_REG+1)
#define LCD_ROW2_BUFF_REG    (LCD_ROW1_BUFF_REG+REGS_X_RENGLON)
#define LCD_ROW3_BUFF_REG    (LCD_ROW2_BUFF_REG+REGS_X_RENGLON)

#define REGS_SIZE (LCD_ROW3_BUFF_REG+REGS_X_RENGLON)

#ifndef Keypadlib_KEY_H_
    typedef enum { 
        IDLE        = 0x00, 
        PRESSED     = 0x01, 
        HOLD        = 0x10, 
        RELEASED    = 0x11
    } KeyState;
#endif

#endif