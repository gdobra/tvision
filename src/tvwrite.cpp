#define Uses_TPoint
#define Uses_TView
#define Uses_TGroup
#define Uses_TScreen
#define Uses_THardwareInfo
#define Uses_TEvent
#include <tvision\tv.h>

#include <stdio.h>
#include <windows.h>
#include <wincon.h>

extern uchar shadowAttr;
extern TPoint shadowSize;

typedef struct TStaticVar2
{
  TView* target;
  short offset;
  short y;
} TStaticVar2;

typedef const void _FAR* TStaticVar1;


static int lockRefresh = 0;
static TStaticVar2 staticVar2;
static TStaticVar1 staticVar1;

void writeViewRec2(TView* view, short x1, short x2, ushort shadowCounter);


void TView::writeLine( short x, short y, short w, short h, const void* buf )
{
  for (int i = 0; i < h; i++)
  {
    writeView(x, y + i, w, buf);
  }
}

void TView::writeBuf( short x, short y, short w, short h, const void _FAR* b )
{
  lockRefresh++;
  for (int i = 0; i < h; i++)
  {
    writeView(x, y + i, w, (ushort*)b + w * i);
  }
  lockRefresh--;
}

void TView::writeStr( short x, short y, const char *str, uchar color )
{
  int len = strlen(str);
  len = min(maxViewWidth, len);
  if (len != 0)
  {
    ushort wrbuf[len];
    color = mapColor(color);
    for (int i = 0; i < len; i++)
    {
      wrbuf[i] = (ushort)*str++ + ((ushort)color << 8);
    }
    writeView(x, y, len, wrbuf);
  }
}

void TView::writeChar( short x, short y, char c, uchar color, short count )
{
  if (count != 0)
  {
    ushort wrbuf[count];
    count = min(count, maxViewWidth);
    color = mapColor(color);
    ushort ch = (ushort)c + ((ushort)color << 8);
    for (int i = 0; i < count; i++)
    {
      wrbuf[i] = ch;
    }
    writeView(x, y, count, wrbuf);
  }
}

void TView::writeView(short x, short y, short w, const void _FAR* buf)
{
  short x2 = x + w;
  if ((y >= 0) && (y < size.y))
  {
    if (x < 0) x = 0;
    if (x2 > size.x) x2 = size.x;
    if (x < x2)
    {
      staticVar2.offset = x;
      staticVar2.y = y;
      staticVar1 = buf;
      writeViewRec2(this, x, x2, 0);
    }
  }
}

void cpywithshadow(void* dest, const void* src, int len)
{
  while (len-- != 0)
  {
    *((ushort *)dest) = (*((ushort *)src) & 0xFF) + ((ushort)shadowAttr << 8);
    dest = (ushort *)dest + 1;
    src = (ushort *)src + 1;
  }
}

inline void cpynoshadow(void* dest, const void* src, int len)
{
  memcpy(dest, src, len * 2);
}

void writeViewRec1(TView* view, short x1, short x2, ushort shadowCounter)
{
  int bufpos, srcpos, l, dx;
  TView* p = view;
  while (1)
  {
    p = p->next;
    if (p == staticVar2.target)
    {
      TGroup* grp = p->owner;
      if (grp->buffer != NULL)
      {
        bufpos = grp->size.x * staticVar2.y + x1;
        srcpos = x1 - staticVar2.offset;
        l = x2 - x1;
        if (shadowCounter == 0) cpynoshadow(grp->buffer + bufpos, (ushort *)staticVar1 + srcpos, l);
        else cpywithshadow((ushort *)grp->buffer + bufpos, (ushort *)staticVar1 + srcpos, l);
        if (grp->buffer == TScreen::screenBuffer)
        {
          //no need for these (on Windows platform, at least)
          TMouse::hide(); //asm source used THWMouse because TMouse's methods are inline
          THardwareInfo::screenWrite(x1, staticVar2.y, (ushort *)grp->buffer + bufpos, l);
          TMouse::show();
        }
      }
      if (grp->lockFlag == 0) writeViewRec2(grp, x1, x2, shadowCounter);
      break;
    }

    if ((p->state & sfVisible) && (staticVar2.y >= p->origin.y))
    {
      if (staticVar2.y < p->origin.y + p->size.y)
      {
        if (x1 < p->origin.x)
        {
          if (x2 <= p->origin.x) continue;
          writeViewRec1(p, x1, p->origin.x, shadowCounter);
          x1 = p->origin.x;
        }
        dx = p->origin.x + p->size.x;
        if (x2 <= dx) break;
        if (x1 < dx) x1 = dx;
        dx += shadowSize.x;
        if ((p->state & sfShadow) && (staticVar2.y >= p->origin.y + shadowSize.y))
          if (x1 > dx) continue;
          else
          {
            shadowCounter++;
            if (x2 <= dx) continue;
            else
            {
              writeViewRec1(p, x1, dx, shadowCounter);
              x1 = dx;
              shadowCounter--;
              continue;
            }
          }
        else continue;
      }

      if ((p->state & sfShadow) && (staticVar2.y < p->origin.y + p->size.y + shadowSize.y))
      {
        dx = p->origin.x + shadowSize.x;
        if (x1 < dx)
        {
          if (x2 <= dx) continue;
          writeViewRec1(p, x1, dx, shadowCounter);
          x1 = dx;
        }
        dx += p->size.x;
        if (x1 >= dx) continue;
        shadowCounter++;
        if (x2 <= dx) continue;
        else
        {
          writeViewRec1(p, x1, dx, shadowCounter);
          x1 = dx;
          shadowCounter--;
        }
      }
    }
  }
};


//@@10 - @@2?
void writeViewRec2(TView* view, short x1, short x2, ushort shadowCounter)
{
  TGroup* grp = view->owner;
  if ((view->state & sfVisible) && (grp != NULL))
  {
    TStaticVar2 savedStatics = staticVar2;
    staticVar2.y += view->origin.y;
    short dx = view->origin.x;
    x1 += dx;
    x2 += dx;
    staticVar2.offset += dx;
    staticVar2.target = view;
    if ((staticVar2.y >= grp->clip.a.y) && (staticVar2.y < grp->clip.b.y))
    {
      if (x1 < grp->clip.a.x) x1 = grp->clip.a.x;
      if (x2 > grp->clip.b.x) x2 = grp->clip.b.x;
      if (x1 < x2) writeViewRec1(grp->last, x1, x2, shadowCounter);
    }
    staticVar2 = savedStatics;
  }
}
