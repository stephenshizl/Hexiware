#line 1 "D:/Marko/tasks/freescale/svn/Hexiwear/MK64/5. Click Demos/1. mikroC/2. Primeri/HEXIWEAR_Motion_Click/example/ARM/HEXIWEAR_Motion_Click.c"
#line 1 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/2. primeri/common/inc/common_screen_objects.h"
#line 1 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_driver.h"
#line 1 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_types.h"
#line 1 "d:/work/mikroc pro for arm/include/stdint.h"





typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;
typedef signed long long int64_t;


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef signed int int_least16_t;
typedef signed long int int_least32_t;
typedef signed long long int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned int uint_least16_t;
typedef unsigned long int uint_least32_t;
typedef unsigned long long uint_least64_t;



typedef signed long int int_fast8_t;
typedef signed long int int_fast16_t;
typedef signed long int int_fast32_t;
typedef signed long long int_fast64_t;


typedef unsigned long int uint_fast8_t;
typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long long uint_fast64_t;


typedef signed long int intptr_t;
typedef unsigned long int uintptr_t;


typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
#line 13 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_types.h"
typedef enum
{
 OLED_TRANSITION_NONE,
 OLED_TRANSITION_TOP_DOWN,
 OLED_TRANSITION_DOWN_TOP,
 OLED_TRANSITION_LEFT_RIGHT,
 OLED_TRANSITION_RIGHT_LEFT

} oled_transition_t;

typedef enum
{
 OLED_STATUS_SUCCESS,
 OLED_STATUS_ERROR,
 OLED_STATUS_PROTOCOL_ERROR,
 OLED_STATUS_INIT_ERROR,
 OLED_STATUS_DEINIT_ERROR

} oled_status_t;
#line 41 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_types.h"
typedef uint16_t* oled_pixel_t;

typedef struct
{
 uint32_t DCpin;
 uint32_t CSpin;
 uint32_t RSTpin;

 uint32_t ENpin;

} settingsOLED_t;

typedef enum
{
 OLED_TEXT_ALIGN_NONE = 0,

 OLED_TEXT_ALIGN_LEFT = 0x1,
 OLED_TEXT_ALIGN_RIGHT = 0x2,
 OLED_TEXT_ALIGN_CENTER = 0x3,

 OLED_TEXT_VALIGN_TOP = 0x10,
 OLED_TEXT_VALIGN_BOTTOM = 0x20,
 OLED_TEXT_VALIGN_CENTER = 0x30

} oled_text_align_t;

typedef enum
{
 OLED_COLOR_BLACK = 0x0000,
 OLED_COLOR_BLUE_1 = 0x06FF,
 OLED_COLOR_BLUE = 0x001F,
 OLED_COLOR_RED = 0xF800,
 OLED_COLOR_GREEN = 0x07E0,
 OLED_COLOR_CYAN = 0x07FF,
 OLED_COLOR_MAGENTA = 0xF81F,
 OLED_COLOR_YELLOW = 0xFFE0,
 OLED_COLOR_GRAY = 0x528A,
 OLED_COLOR_WHITE = 0xFFFF

} oled_color_t;

typedef struct
{
 uint8_t xCrd;
 uint8_t yCrd;
 uint8_t width;
 uint8_t height;
 oled_pixel_t areaBuffer;

} oled_dynamic_area_t;

typedef struct
{
 const uint8_t* font;
 uint16_t fontColor;
 oled_text_align_t alignParam;
 const uint8_t* background;

} oled_text_properties_t;
#line 1 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_resources.h"
#line 1 "d:/work/mikroc pro for arm/include/stdint.h"
#line 6 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_resources.h"
extern const uint8_t guiFont_Tahoma_18_Regular[];
extern const uint8_t guiFont_Tahoma_16_Regular[];
extern const uint8_t guiFont_Tahoma_14_Regular[];
extern const uint8_t guiFont_Tahoma_10_Regular[];
extern const uint8_t guiFont_Tahoma_8_Regular[];
extern const uint8_t guiFont_Tahoma_7_Regular[];
extern const uint8_t guiFont_Tahoma_6_Regular[];
extern const uint8_t guiFont_Roboto_Mono11x23_Regular[];
extern const uint8_t guiFont_Exo_2_Condensed10x16_Regular[];
extern const uint8_t guiFont_Exo_2_Condensed15x23_Regular[];
extern const uint8_t guiFont_Exo_2_Condensed21x32_Regular[];

extern const uint8_t apps_screen_bmp[];
#line 22 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_driver.h"
oled_status_t OLED_Init(void);
#line 37 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_driver.h"
oled_status_t OLED_DrawBox (
 uint16_t xCrd,
 uint16_t yCrd,
 uint16_t width,
 uint16_t height,
 uint16_t color
 );
#line 54 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_driver.h"
oled_status_t OLED_FillScreen( uint16_t color );
#line 68 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/1. biblioteke/oled/inc/oled_driver.h"
oled_status_t OLED_DrawPixel (
 int16_t xCrd,
 int16_t yCrd,
 uint16_t color
 );





oled_status_t OLED_AddText( const uint8_t* text );





void OLED_SetTextProperties(oled_text_properties_t *textProperties);





uint8_t OLED_GetTextWidth(const uint8_t* text);





uint8_t OLED_CharCount(uint8_t width, const uint8_t* font, const uint8_t* text, uint8_t length);





void OLED_SetFont(const uint8_t *activeFont, uint16_t font_color, uint8_t font_orientation);





void OLED_SetDynamicArea(oled_dynamic_area_t *dynamic_area);





void OLED_WriteText(uint8_t *text, uint16_t x, uint16_t y);





oled_status_t OLED_DrawImage (const uint8_t* image, uint8_t xCrd, uint8_t yCrd);






void OLED_GetImageDimensions(uint8_t *width, uint8_t *height, const uint8_t* image);
#line 8 "d:/marko/tasks/freescale/svn/hexiwear/mk64/5. click demos/1. mikroc/2. primeri/common/inc/common_screen_objects.h"
extern const uint8_t
 common_hexiwearLogo_bmp[18438];
#line 3 "D:/Marko/tasks/freescale/svn/Hexiwear/MK64/5. Click Demos/1. mikroC/2. Primeri/HEXIWEAR_Motion_Click/example/ARM/HEXIWEAR_Motion_Click.c"
sbit Motion_Sens_EN at PTB_PDOR.B19;
sbit Motion_Sens_PIN at PTB_PDIR.B8;

static uint8_t
 isMotionDetected = -1;

static uint8_t
 sampleText[] = "Motion!";

void main()
{
 OLED_Init();
 OLED_SetFont( guiFont_Tahoma_8_Regular, OLED_COLOR_WHITE, 0 );


 OLED_DrawImage( common_hexiwearLogo_bmp, 0, 0 );


 GPIO_Digital_Input( &PTB_PDOR, _GPIO_PINMASK_8 );


 GPIO_Digital_Output( &PTB_PDOR, _GPIO_PINMASK_19 );


 GPIO_Digital_Output( &PTC_PDOR, _GPIO_PINMASK_8 );
 PTC_PDOR.B8 = 1;

 EnableInterrupts();
 NVIC_IntEnable( IVT_INT_PORTB );


 PORTB_PCR8bits.IRQC = 0xB;


 Motion_Sens_EN = 1;

 while (1)
 {
 if ( 1 == isMotionDetected )
 {
 OLED_WriteText( sampleText, 35, 75 );
 PTC_PDOR.B8 = 0;
 isMotionDetected = -1;
 }
 else if ( 0 == isMotionDetected )
 {
 OLED_DrawImage( common_hexiwearLogo_bmp, 0, 0 );
 PTC_PDOR.B8 = 1;
 isMotionDetected = -1;
 }
 }
}

void MotionDetect() iv IVT_INT_PORTB ics ICS_AUTO
{
 PORTB_ISFR = 0xFFFFFFFF;

 if ( 1 == Motion_Sens_PIN )
 {
 isMotionDetected = 1;
 }
 else
 {
 isMotionDetected = 0;
 }
}
