#ifndef _PRIMITIVE_H
    #define _PRIMITIVE_H

/*********************************************************************
* Overview: Primitive lines are drawn using line type and line thickness.
*			There are 3 line styles and 2 types of line thickness.
*
*********************************************************************/
// Current cursor coordinates
extern SHORT    _cursorX;
extern SHORT    _cursorY;

// Font orientation
extern BYTE     _fontOrientation;

    #define ORIENT_HOR  0
    #define ORIENT_VER  1


/*********************************************************************
* Overview: Structure describing the font header.
*
*********************************************************************/
typedef struct
{
    BYTE    fontID;     // User assigned value
    BYTE    res1   : 4;         // Reserved for future use (must be set to 0).
    BYTE    orient : 2;         // Orientation
    BYTE    res2   : 2;         // Reserved for future use (must be set to 0).
    WORD    firstChar;  // Character code of first character (e.g. 32).
    WORD    lastChar;   // Character code of last character in font (e.g. 3006).
    BYTE    height;     // Font characters height in pixels.
    BYTE    reserved;   // Reserved for future use (must be set to 0).
} FONT_HEADER;

// Structure describing font glyph entry
typedef struct
{
    BYTE    width;
    BYTE    offsetLSB;
    WORD    offsetMSB;
} GLYPH_ENTRY;

/*********************************************************************
* Overview: Structure for font stored in FLASH memory.
*
*********************************************************************/


/*********************************************************************
* Overview: Structure for font stored in RAM memory.
*
*********************************************************************/
typedef struct
{
    TYPE_MEMORY type;       // must be RAM
    const char  *address;   // bitmap image address in RAM
} FONT_RAM;

// Structure for font stored in EXTERNAL memory
    #define FONT_EXTERNAL   EXTDATA

/*********************************************************************
* Overview: This defines the size of the buffer used by font functions
*			to retrieve font data from the external memory. The buffer 
*			size can be increased to accommodate large font sizes. 
*			The user must be aware of the expected glyph sizes of the 
*			characters stored in the font table.
*
********************************************************************/
    #define EXTERNAL_FONT_BUFFER_SIZE   600

// Pointer to the current font image
extern void     *_font;

// First and last characters in the font
extern WORD     _fontFirstChar;
extern WORD     _fontLastChar;

// Installed font height
extern SHORT    _fontHeight;

/*********************************************************************
* Function:  void InitGraph(void)
*
********************************************************************/
void            InitGraph(void);

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor x-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetX()  _cursorX

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor y-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetY()  _cursorY

/*********************************************************************
* Macros:  MoveTo(x,y)
*
* Overview: This macro moves the graphic cursor to new x,y position.
*
* PreCondition: none
*
* Input: x - Specifies the new x position of the graphic cursor.
*        y - Specifies the new y position of the graphic cursor.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveTo(x, y) \
    _cursorX = x;        \
    _cursorY = y;

/*********************************************************************
* Macros:  MoveRel(dX,dY)
*
* Overview: This macro moves the graphic cursor relative to the 
*			current location. The given dX and dY displacement can 
*			be positive or negative numbers.
*
* PreCondition: none
*
* Input: dX - Specifies the displacement of the graphic cursor for
*			  the horizontal direction.
*		 dY - Specifies the displacement of the graphic cursor for
*			  the vertical direction.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define MoveRel(dX, dY) \
    _cursorX += dX;         \
    _cursorY += dY;

/*********************************************************************
* Macro: SetFontOrientation(orient)
*
* Overview: This macro sets font orientation vertical or horizontal.
*
* PreCondition: none
*
* Input: orient - should be non-zero if the font orientation is vertical
*
* Output: none
*
********************************************************************/
    #define SetFontOrientation(orient)  _fontOrientation = orient;

/*********************************************************************
* Macro: GetFontOrientation()
*
* Overview: This macro returns font orientation (0 == horizontal, 1 == vertical).
*
* PreCondition: none
*
* Input: none
*
* Output: font orientation (0 == horizontal, 1 == vertical)
*
********************************************************************/
    #define GetFontOrientation()    _fontOrientation

/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* Overview: This function outputs a character from the current graphic 
*		    cursor position. OutChar() uses the current active font 
*		    set with SetFont(). 
*
* PreCondition: none
*
* Input: ch - The character code to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
*
* Side Effects: After the function is completed, the graphic cursor 
*			    position is moved in the horizontal direction by the 
*			    character width. Vertical position of the graphic cursor 
*			    is not changed.
*
********************************************************************/
WORD    OutChar(XCHAR ch);

/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the current graphic cursor position. The string must 
*			be terminated by a line feed or zero. For Non-Blocking 
*			configuration, OutText() may return control to the program 
*			due to display device busy status. When this happens zero 
*			is returned and OutText() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutText() uses the current 
*			active font set with SetFont().
*
*
* Input: textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: Current horizontal graphic cursor position will be moved 
*				to the end of the text. The vertical graphic cursor 
*				position will not be changed.
*
********************************************************************/
WORD    OutText(XCHAR *textString);

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* Overview: This function outputs a string of characters starting 
*			at the given x, y position. The string must be terminated 
*			by a line feed or zero. For Non-Blocking configuration, 
*			OutTextXY() may return control to the program due to 
*			display device busy status. When this happens zero is 
*			returned and OutTextXY() must be called again to continue 
*			the outputting of the string. For Blocking configuration, 
*			this function always returns a 1. OutTextXY() uses the 
*			current active font set with SetFont().
*
* Input: x - Defines the x starting position of the string.
*		 y - Defines the y starting position of the string.
*		 textString - Pointer to the string to be displayed.
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when string is not yet outputted completely.
*         - Returns 1 when string is outputted completely.
*         For Blocking configuration:
*         - Always return 1.
*
* Example:
*   <CODE> 
*	void PlaceText(void)
*	{
*		SHORT width, height;
*		static const XCHAR text[] = "Touch screen to continue";
*		
*		SetColor(BRIGHTRED);                // set color
*		SetFont(pMyFont);                   // set font to my font
*		
*		// get string width & height
*		width = GetTextWidth(text, pMyFont);
*		height = GetTextHeight(pMyFont);
*		
*		// place string in the middle of the screen
*		OutTextXY(  (GetMaxX() - width) >> 1, \
*		(GetMaxY() – height) >> 1, \
*		(char*)text);
*	}
*	</CODE> 
*
* Side Effects: Current horizontal graphic cursor position will be 
*				moved to the end of the text. The vertical graphic 
*				cursor position will not be changed.
*
********************************************************************/
WORD    OutTextXY(SHORT x, SHORT y, XCHAR *textString);

/*********************************************************************
* Function: SHORT GetTextHeight(void* font)
*
* Overview: This macro returns the height of the specified font. 
*			All characters in a given font table have a constant 
*			height.
*
* Input: font - Pointer to the font image.
*
* Output: Returns the font height.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextHeight(void *font);

/*********************************************************************
* Function: SHORT GetTextWidth(XCHAR* textString, void* font)
*
* Overview: This function returns the width of the specified string 
*			for the specified font. The string must be terminated 
*			by a line feed or zero.
*
* Input: textString - Pointer to the string.
*		 font - Pointer to the font image.
*
* Output: Returns the string width in the specified font.
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
SHORT   GetTextWidth(XCHAR *textString, void *font);

/*********************************************************************
* Function: void SetFont(void* font)
*
* Overview: This function sets the current font used in OutTextXY(), 
*			OutText() and OutChar() functions.
*
* Input: font - Pointer to the new font image to be used.
*
* Output: none
*
* Example:
*   See OutTextXY() example.
*
* Side Effects: none
*
********************************************************************/
void    SetFont(void *font);

/*********************************************************************
* Function: void ClearDevice(void)
*
* Overview: This function clears the screen with the current color 
*			and sets the graphic cursor position to (0, 0). 
*			Clipping is NOT supported by ClearDevice().
*
* Input: none
*
* Output: none
*
* Example:
*	<CODE> 
*	void ClearScreen(void)
*	{
*		SetColor(WHITE);		// set color to WHITE
*		ClearDevice();			// set screen to all WHITE
*	}
*   </CODE>
* 
* Side Effects: none
*
********************************************************************/
void    ClearDevice(void);
#endif // _PRIMITIVE_H
