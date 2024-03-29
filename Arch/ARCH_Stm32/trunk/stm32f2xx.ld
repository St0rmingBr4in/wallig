/*
Linker script for STM32F2xx_1024K_128K with flash offset.
*/

/*
These are used by the startup in order to allocate stacks for the different modes.
*/
__Stack_Size = 1024 ;

PROVIDE ( _Stack_Size = __Stack_Size ) ;

__Stack_Init = _estack  - __Stack_Size ;

/*"PROVIDE" allows to easily override these values from an object file or the commmand line.*/
PROVIDE ( _Stack_Init = __Stack_Init ) ;

/*
There will be a link error if there is not this amount of RAM free at the end.
*/
_Minimum_Stack_Size = 0x100 ;


/* include the memory spaces definitions sub-script */
/*
Linker subscript for STM32F2xx definitions with 1024K Flash and 96K RAM */

/* Memory Spaces Definitions */

MEMORY
{
  RAM (xrw) : ORIGIN = 0x20000000, LENGTH = INTERNAL_RAM
  FLASH (rx) : ORIGIN = 0x8000000, LENGTH = INTERNAL_ROM
  FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
  EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
}

/* higher address of the user mode stack */
_estack = HIGH_STACK_ADR;



/* include the sections management sub-script for FLASH mode */
/*
Common part of the linker scripts for STR71x devices in FLASH mode
(that is, the FLASH is seen at 0)
Copyright RAISONANCE 2005
You can use, modify and distribute thisfile freely, but without any waranty.
*/



/* Sections Definitions */

SECTIONS
{
    /* for Cortex devices, the beginning of the startup code is stored in the .isr_vector section, which goes to FLASH */
    .isr_vector :
    {
	. = ALIGN(4);
        KEEP(*(.isr_vector))            /* Startup code */
	. = ALIGN(4);
    } >FLASH
 
    /* for some STRx devices, the beginning of the startup code is stored in the .flashtext section, which goes to FLASH */
    .flashtext :
    {
	. = ALIGN(4);
        *(.flashtext)            /* Startup code */
	. = ALIGN(4);
    } >FLASH
 
    
    /* the program code is stored in the .text section, which goes to Flash */
    .text :
    {
	    . = ALIGN(4);
	    
        *(.text)                   /* remaining code */
        *(.text.*)                   /* remaining code */
        *(.rodata)                 /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)

	    . = ALIGN(4);
   	 _etext = .;
	    /* This is used by the startup in order to initialize the .data secion */
   	 _sidata = _etext;
    } >FLASH
    
 

    /* This is the initialized data section
    The program executes knowing that the data is in the RAM
    but the loader puts the initial values in the FLASH (inidata).
    It is one task of the startup to copy the initial values from FLASH to RAM. */
    .data  : AT ( _sidata )
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .data secion */
        _sdata = . ;
        
        *(.data)
        *(.data.*)

	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .data secion */
   	 _edata = . ;
    } >RAM
    
    

    /* This is the uninitialized data section */
    .bss :
    {
	    . = ALIGN(4);
        /* This is used by the startup in order to initialize the .bss secion */
        _sbss = .;
        
        *(.bss)
        *(COMMON)
        
	    . = ALIGN(4);
	    /* This is used by the startup in order to initialize the .bss secion */
   	 _ebss = . ;
    } >RAM
    
    PROVIDE ( end = _ebss );
    PROVIDE ( _end = _ebss );
    
    /* This is the user stack section 
    This is just to check that there is enough RAM left for the User mode stack
    It should generate an error if it's full.
     */
    ._usrstack :
    {
	    . = ALIGN(4);
        _susrstack = . ;
        
        . = . + _Minimum_Stack_Size ;
        
	    . = ALIGN(4);
        _eusrstack = . ;
    } >RAM
    

   
    /* this is the FLASH Bank1 */
    /* the C or assembly source must explicitly place the code or data there
    using the "section" attribute */
    .b1text :
    {
        *(.b1text)                   /* remaining code */
        *(.b1rodata)                 /* read-only data (constants) */
        *(.b1rodata*)
    } >FLASHB1
    
    /* this is the EXTMEM */
    /* the C or assembly source must explicitly place the code or data there
    using the "section" attribute */
    
    /* EXTMEM Bank0 */
    .eb0text :
    {
        *(.eb0text)                   /* remaining code */
        *(.eb0rodata)                 /* read-only data (constants) */
        *(.eb0rodata*)
    } >EXTMEMB0
    
    /* EXTMEM Bank1 */
    .eb1text :
    {
        *(.eb1text)                   /* remaining code */
        *(.eb1rodata)                 /* read-only data (constants) */
        *(.eb1rodata*)
    } >EXTMEMB1
    
    /* EXTMEM Bank2 */
    .eb2text :
    {
        *(.eb2text)                   /* remaining code */
        *(.eb2rodata)                 /* read-only data (constants) */
        *(.eb2rodata*)
    } >EXTMEMB2
    
    /* EXTMEM Bank0 */
    .eb3text :
    {
        *(.eb3text)                   /* remaining code */
        *(.eb3rodata)                 /* read-only data (constants) */
        *(.eb3rodata*)
    } >EXTMEMB3
    
    
    
    /* after that it's only debugging information. */
    
    /* remove the debugging information from the standard libraries */
    DISCARD :
    {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
     }

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment       0 : { *(.comment) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}

PROVIDE (   NMIException = 0 ) ;
PROVIDE (   HardFaultException = 0 ) ;
PROVIDE (   MemManageException = 0 ) ;
PROVIDE (   BusFaultException = 0 ) ;
PROVIDE (   UsageFaultException = 0 ) ;
PROVIDE (   SVCHandler = 0 ) ;
PROVIDE (   DebugMonitor = 0 ) ;
PROVIDE (   PendSVC = 0 ) ;
PROVIDE (   SysTickHandler = 0 ) ;
PROVIDE (   WWDG_IRQHandler = 0 ) ;
PROVIDE (   PVD_IRQHandler = 0 ) ;
PROVIDE (   TAMP_STAMP_IRQHandler = 0 ) ;
PROVIDE (   RTC_WKUP_IRQHandler = 0 ) ;
PROVIDE (   FLASH_IRQHandler = 0 ) ;
PROVIDE (   RCC_IRQHandler = 0 ) ;
PROVIDE (   EXTI0_IRQHandler = 0 ) ;
PROVIDE (   EXTI1_IRQHandler = 0 ) ;
PROVIDE (   EXTI2_IRQHandler = 0 ) ;
PROVIDE (   EXTI3_IRQHandler = 0 ) ;
PROVIDE (   EXTI4_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream0_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream1_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream2_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream3_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream4_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream5_IRQHandler = 0 ) ;
PROVIDE (   DMA1_Stream6_IRQHandler = 0 ) ;
PROVIDE (   ADC_IRQHandler = 0 ) ;
PROVIDE (   CAN1_TX_IRQHandler = 0 ) ;
PROVIDE (   CAN1_RX0_IRQHandler = 0 ) ;
PROVIDE (   CAN_RX1_IRQHandler = 0 ) ;
PROVIDE (   CAN_SCE_IRQHandler = 0 ) ;
PROVIDE (   EXTI9_5_IRQHandler = 0 ) ;
PROVIDE (   TIM1_BRK_TIM9_IRQHandler = 0 ) ;
PROVIDE (   TIM1_UP_TIM10_IRQHandler = 0 ) ;
PROVIDE (   TIM1_TRG_COM_TIM11_IRQHandler = 0 ) ;
PROVIDE (   TIM1_CC_IRQHandler = 0 ) ;
PROVIDE (   TIM2_IRQHandler = 0 ) ;
PROVIDE (   TIM3_IRQHandler = 0 ) ;
PROVIDE (   TIM4_IRQHandler = 0 ) ;
PROVIDE (   I2C1_EV_IRQHandler = 0 ) ;
PROVIDE (   I2C1_ER_IRQHandler = 0 ) ;
PROVIDE (   I2C2_EV_IRQHandler = 0 ) ;
PROVIDE (   I2C2_ER_IRQHandler = 0 ) ;
PROVIDE (   SPI1_IRQHandler = 0 ) ;
PROVIDE (   SPI2_IRQHandler = 0 ) ;
PROVIDE (   USART1_IRQHandler = 0 ) ;
PROVIDE (   USART2_IRQHandler = 0 ) ;
PROVIDE (   USART3_IRQHandler = 0 ) ;
PROVIDE (   EXTI15_10_IRQHandler = 0 ) ;
PROVIDE (   RTC_Alarm_IRQHandler = 0 ) ;
PROVIDE (  OTG_FS_WKUP_IRQHandler = 0 ) ;
PROVIDE (  TIM8_BRK_TIM12_IRQHandler = 0 ) ;
PROVIDE (  TIM8_UP_TIM13_IRQHandler = 0 ) ;
PROVIDE (  TIM8_TRG_COM_TIM14_IRQHandler = 0 ) ;
PROVIDE (  TIM8_CC_IRQHandler = 0 ) ;
PROVIDE (  DMA1_Stream7_IRQHandler = 0 ) ;
PROVIDE (  FSMC_IRQHandler = 0 ) ;
PROVIDE (  SDIO_IRQHandler = 0 ) ;
PROVIDE (  TIM5_IRQHandler = 0 ) ;
PROVIDE (  SPI3_IRQHandler = 0 ) ;
PROVIDE (  UART4_IRQHandler = 0 ) ;
PROVIDE (  UART5_IRQHandler = 0 ) ;
PROVIDE (  TIM6_DAC_IRQHandler = 0 ) ;
PROVIDE (  TIM7_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream0_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream1_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream2_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream3_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream4_IRQHandler = 0 ) ;
PROVIDE (  ETH_IRQHandler = 0 ) ;
PROVIDE (  ETH_WKUP_IRQHandler = 0 ) ;
PROVIDE (  CAN2_TX_IRQHandler = 0 ) ;
PROVIDE (  CAN2_RX0_IRQHandler = 0 ) ;
PROVIDE (  CAN2_RX1_IRQHandler = 0 ) ;
PROVIDE (  CAN2_SCE_IRQHandler = 0 ) ;
PROVIDE (  OTG_FS_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream5_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream6_IRQHandler = 0 ) ;
PROVIDE (  DMA2_Stream7_IRQHandler = 0 ) ;
PROVIDE (  USART6_IRQHandler = 0 ) ;
PROVIDE (  I2C3_EV_IRQHandler = 0 ) ;
PROVIDE (  I2C3_ER_IRQHandler = 0 ) ;
PROVIDE (  OTG_HS_EP1_OUT_IRQHandler = 0 ) ;
PROVIDE (  OTG_HS_EP1_IN_IRQHandler = 0 ) ;
PROVIDE (  OTG_HS_WKUP_IRQHandler = 0 ) ;
PROVIDE (  OTG_HS_IRQHandler = 0 ) ;
PROVIDE (  DCMI_IRQHandler = 0 ) ;
PROVIDE (  CRYP_IRQHandler = 0 ) ;
PROVIDE (  HASH_RNG_IRQHandler = 0 ) ;

