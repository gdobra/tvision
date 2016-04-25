#define Uses_TView
#define Uses_TGroup
#define Uses_THardwareInfo
#define Uses_TScreen
#include <tvision\tv.h>


//TODO: optimize & rearrange this if possible
void TView::resetCursor()
{
  TView *p, *p2;
  TGroup *grp;
  TPoint crs;
  int res = 0;
  const ushort sfV_CV_F = sfVisible | sfCursorVis | sfFocused;
  if ((state & sfV_CV_F) == sfV_CV_F)
  {
    p = this;
    crs = cursor;
    while (res != 2)
    {
      res = 0;
      if ((crs.x < 0) || (crs.x >= p->size.x) ||
          (crs.y < 0) || (crs.y >= p->size.y)) break;
      crs += p->origin;
      p2 = p;
      grp = p->owner;
      if (grp == NULL)
      {
        THardwareInfo::setCaretPosition(crs.x, crs.y);
        if (state & sfCursorIns) THardwareInfo::setCaretSize(100);
        else THardwareInfo::setCaretSize(TScreen::cursorLines);
        return;
      }
      if ((grp->state & sfVisible) == 0) res = 2;
      p = grp->last;

      while (res == 0)
      {
        p = p->next;
        if (p == p2)
        {
          p = p->owner;
          res = 1;
        }
        else if ((p->state & sfVisible) &&
                 (crs.x >= p->origin.x) && (crs.x < p->size.x + p->origin.x) &&
                 (crs.y >= p->origin.y) && (crs.y < p->size.y + p->origin.y)) res = 2;
      }
    }
  }
  THardwareInfo::setCaretSize(0);
}
