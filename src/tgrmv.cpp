#define Uses_TView
#define Uses_TGroup
#include <tvision\tv.h>


void TGroup::removeView(TView* view)
{
  TView *q;
  if ((view != NULL) && (last != NULL))
  {
    q = last;
    while ((q->next != view) && (q->next != last)) q = q->next;
           if (q->next == view)
    {
      if (q->next != q)
        {
          q->next = view->next;
          if (view == last) last = view->next;
        }
        else last = NULL;
      }
  }
}
