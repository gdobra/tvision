#define Uses_TView
#define Uses_TFrame
#define Uses_TGroup
#include <tvision\tv.h>


void TFrame::frameLine( TDrawBuffer& frameBuf, short y, short n, uchar color )
{
  uchar frameMask[maxViewWidth];
  short i, j, k;
  TView* view;

  frameMask[0] = TFrame::initFrame[n];
  memset(&frameMask[1], initFrame[n + 1], size.x - 2);
  frameMask[size.x - 1] = initFrame[n + 2];
  view = owner->last->next;
  while (view != this)
  {
    if ((view->options & ofFramed) &&
        (view->state & sfVisible))
    {
      i = y - view->origin.y;
      if (i < 0)
      {
        i++;
        if (i == 0) i = 0x0A06; //what the hell is this?
        else i = 0;
      }
      else
      {
        if (i < view->size.y) i = 0x0005;
        else if (i == view->size.y) i = 0x0A03;
        else i = 0;
      }
      if (i != 0)
      {
        j = view->origin.x;
        k = view->size.x + j;
        if (j < 1) j = 1;
        if (k > size.x) k = size.x;
        if (k > j)
        {
          frameMask[j - 1] = frameMask[j - 1] | (i & 0xFF);
          i = ((i & 0xFF) ^ ((ushort)i >> 8)) | ((ushort)i & 0xFF00);
          frameMask[k] = frameMask[k] | (i & 0xFF);
          if (((ushort)i >> 8) != 0)
          {
            k -= j;
            do
            {
              frameMask[j] = frameMask[j] | ((ushort)i >> 8);
              j++;
              k--;
            } while (k != 0);
          }
        }
      }
    }
    view = view->next;
  }
  /*
    p:=framechars_437;
    {$ifdef unix}
    {Codepage variables are currently Unix only.}
    if internal_codepage<>cp437 then
      p:=framechars_850;
    {$endif}
  */
  for (i = 0; i < size.x; i++)
    frameBuf.moveChar(i, TFrame::frameChars[frameMask[i]], color, 1);
}
