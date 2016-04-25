#define Uses_TTerminal
#define Uses_MsgBox
#include <tvision\tv.h>

ushort TTerminal::prevLines( ushort pos, ushort lines )
{
  if (lines == 0)
  {
    bufInc(pos);
    bufInc(pos);
    return pos;
  };

  if (pos == queBack) return queBack; // Nothing to do

  bufDec(pos); // pos might be pointing to a '\n'

  if (pos < queBack)
  {
    while ( !( buffer[pos]=='\n' && !--lines ) && pos-- );
    if (lines) pos = bufSize - 1;
  };

  if (lines)
    /* SS: we should check if there is an available character before read it */
    while (pos > queBack && !( buffer[pos] == '\n' && !--lines ))
    {
      pos--;
    }

  if (lines)
    return queBack;
  else
    bufInc(pos);

  return pos;
}
