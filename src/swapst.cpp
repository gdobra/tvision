#define Uses_TSystemError
#define Uses_TScreen
#define Uses_MsgBox
#include <tvision\tv.h>

#if !defined( __FLAT__ )
void TSystemError::swapStatusLine(TDrawBuffer _FAR& buf)
{
    short pos = TScreen::screenWidth * (TScreen::screenHeight - 1) * 2;


/*
        USES ESI, EDI

        MOVZX   ECX, BYTE PTR  [LARGE @TScreen@screenWidth]
        MOV     AL, BYTE PTR [LARGE @TScreen@screenHeight]
        DEC     AL
        MUL     CL
        MOVZX   EAX, AX
        SHL     EAX, 1
        LES     EDI, [LARGE @TScreen@screenBuffer]
        ADD     EDI, EAX
        MOV     ESI, DWORD PTR [Buffer]
@@1:
        MOV     AX, ES:[EDI]
        MOVSW
        MOV     [ESI-2], AX
        LOOP  @@1
        RET
*/
  messageBox("TSystemError::swapStatusLine", mfOKButton);
}
#endif
