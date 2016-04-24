#define Uses_TEditor
#define Uses_MsgBox
#include <tvision\tv.h>

extern "C" int countLines(void* buf, uint count)
{
  ulong lines = 0;
  for (ulong i = 0; i < count; i++)
  {
    //TODO: implement here different line endings styles
    if (((char*)buf)[i] == 0x0A) lines++;
  }
  return lines;
}

extern "C" ulong scan(const void *block, ulong size, const char *str)
{
  ulong len = strlen(str);
  ulong j;
  for (ulong i = 0; i < size; i++)
  {
    j = 0;
    while ((((char* )block)[i + j] == str[j]) && (j < len) && (i + j < size))
    {
      j++;
    }
    if (j == len) return i;
  }
  return sfSearchFailed;
}

extern "C" ulong iScan(const void *block, ulong size, const char *str)
{
  ulong len = strlen(str);
  ulong j;
  for (ulong i = 0; i < size; i++)
  {
    j = 0;
    while ((tolower(((char* )block)[i + j]) == tolower(str[j])) && (j < len) && (i + j < size))
    {
      j++;
    }
    if (j == len) return i;
  }
  return sfSearchFailed;
}

char TEditor::bufChar(ulong p)
{
  if (p > curPtr) p += gapLen;
  return buffer[p];
}

ulong TEditor::bufPtr(ulong p)
{
  if (p >= curPtr) return p + gapLen;
  else return p;
}

void fillSpace(void*& outPtr, ulong i, ushort attr)
{
  ulong w = attr | 0x20;
  while (i > 0)
  {
    *(ushort *)outPtr = w;
    outPtr = (ushort *)outPtr + 1;
    i--;
  }
}

Boolean TEditor::formatUntil(void*& outPtr, ulong& outCnt, ulong& idxpos, ulong endpos, ulong width, ushort attr)
{
  uchar* p;
  //TODO: move this to global scope
  const ushort tabSize = 8;
  p = (uchar *)buffer + idxpos;
  while (endpos > idxpos && outCnt < width)
  {
    switch (*p)
    {
    case '\x09':
      fillSpace(outPtr, tabSize - (outCnt % tabSize), attr);
      outCnt += tabSize - (outCnt % tabSize);
      idxpos++;
      continue;
    case '\x0A':
    case '\x0D':
    {
      fillSpace(outPtr, width - outCnt, attr);
      outCnt += width - outCnt;
      return True;
    }
    }
    *(ushort *)outPtr = attr | *p;
    outPtr = (ushort *)outPtr + 1;
    outCnt++;
    p++;
    idxpos++;
  }
  return False;
}

void TEditor::formatLine(void *DrawBuf, ulong LinePtr, int Width, ushort Color)
{
  ulong outCnt = 0;
  void* outPtr = DrawBuf;
  ulong idxPos = LinePtr;
  ushort attr = (Color & 0xFF) << 8;
  if (formatUntil(outPtr, outCnt, idxPos, selStart, Width, attr)) return;
  attr = Color & 0xFF00;
  if (formatUntil(outPtr, outCnt, idxPos, curPtr, Width, attr)) return;
  idxPos += gapLen;
  if (formatUntil(outPtr, outCnt, idxPos, selEnd + gapLen, Width, attr)) return;
  attr = (Color & 0xFF) << 8;
  if (formatUntil(outPtr, outCnt, idxPos, bufSize, Width, attr)) return;
  fillSpace(outPtr, Width - outCnt, attr);
}

ulong TEditor::lineEnd(ulong p)
{
  ulong start, i;
  char *pc;

  if (p < curPtr)
  {
    i = curPtr - p;
    pc = buffer + p;
    while (i > 0)
    {
      if ((*pc == 10) || (*pc == 13))
      {
        return pc - buffer;
      }
      pc++;
      i--;
    }
    start = curPtr;
  }
  else
    start = p;
  i = bufLen - start;
  pc = buffer + gapLen + start;
  while (i > 0)
  {
    if ((*pc == 10) || (*pc == 13))
    {
      return pc - (buffer + gapLen);
    }
    pc++;
    i--;
  }
  return pc - (buffer + gapLen);
}

ulong TEditor::lineStart(ulong p)
{
  ulong i;
  char *start, *pc;

  if (p > curPtr)
  {
    start = buffer + gapLen;
    pc = start;
    i = p - curPtr;
    pc--;
    while (i > 0)
    {
      if ((*pc == 10) || (*pc == 13))
        break;
      pc--;
      i--;
    }
  }
  else i = 0;
  if (i == 0)
  {
    start = buffer;
    i = p;
    pc = start + p;
    pc--;
    while (i > 0)
    {
      if ((*pc == 10) || (*pc == 13))
        break;
      pc--;
      i--;
    }
    if (i == 0)
    {
      return 0;
    }
  }
//oc = *pc;
  return pc - start + 1;
}

ulong TEditor::nextChar(ulong p)
{
  char *pc;

  if (p != bufLen)
  {
    p++;
    if (p != bufLen)
    {
      pc = buffer;
      if (p >= curPtr)
        pc += gapLen;
      pc += p - 1;
      if (*pc + *(pc + 1) == 23)
        p++;
    }
  }
  return p;
}

ulong TEditor::prevChar(ulong p)
{
  char *pc;

  if (p != 0)
  {
    p--;
    if (p != 0)
    {
      pc = buffer;
      if (p >= curPtr)
        pc += gapLen;
      pc += p - 1;
      if (*pc + *(pc + 1) == 23)
        p--;
    }
  }
  return p;
}
