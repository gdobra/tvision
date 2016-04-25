#define Uses_TView
#define Uses_TGroup
#include <tvision\tv.h>

typedef struct TStaticVar2
{
  TView* target;
  short offset;
  short y;
} TStaticVar2;

static TStaticVar2 staticVar2;


Boolean exposedRec2(TView *view, short x1, short w);

Boolean exposedRec1(TView *view, short x1, short x2)
{
  short dy, dx;
  while (1)
  {
    view = view->next;
    if (view == staticVar2.target)
    {
      return exposedRec2(view->owner, x1, x2);
    }
    dy = view->origin.y;
    dx = view->origin.x;
    if ((view->state & sfVisible) && (staticVar2.y >= dy))
    {
      if (staticVar2.y < dy + view->size.y)
      {
        if (x1 < dx)
        {
          if (x2 <= dx) continue;
          if (x2 > dx + view->size.x)
          {
            if (exposedRec1(view, x1, dx)) return True;
            x1 = dx + view->size.x;
          }
          else x2 = dx;
        }
        else
        {
          if (x1 < dx + view->size.x) x1 = dx + view->size.x;
          if (x1 >= x2) return False;
        }
      }
    }
  }
}

Boolean exposedRec2(TView *view, short x1, short w)
{
  TGroup *grp;
  TStaticVar2 savedStatic;
  short x2 = x1 + w;
  Boolean result = False;

  if (view->state & sfVisible)
  {
    grp = view->owner;
    if ((grp != NULL) && (grp->buffer == NULL))
    {
      savedStatic = staticVar2;
      staticVar2.y += view->origin.y;
      x1 += view->origin.x;
      x2 += view->origin.x;
      staticVar2.target = view;
      if ((staticVar2.y >= grp->clip.a.y) && (staticVar2.y < grp->clip.b.y))
      {
        if (x1 < grp->clip.a.x) x1 = grp->clip.a.x;
        if (x2 > grp->clip.b.x) x2 = grp->clip.b.x;
        if (x1 < x2) result = exposedRec1(grp->last, x1, x2);
      }
      staticVar2 = savedStatic;
    }
    else result = True;
  }
  return result;
}

Boolean TView::exposed()
{
  Boolean ok;
  short y;
  if ((state & sfExposed) && (size.x > 0) && (size.y > 0))
  {
    ok = False;
    y = 0;
    while ((y < size.y) && (!ok))
    {
      staticVar2.y = y;
      ok = exposedRec2(this, 0, size.x);
      y++;
    }
  }
  else ok = False;
  return ok;
}
