#include "types.h"
#include "x86.h"
#include "defs.h"
#include "kbd.h"
#include "window.h"
#include "gui.h"

static int flag_caps = 0;
static int flag_shift = 0;
static int flag_event = 0;

void
kbdintr(void)
{
  uint ch;
  ch = inb(0x64);
  if((ch & 0x01) == 0)
  {
    return;
  }

  ch = inb(0x60);

  //press down, interrupt
  if((ch & 0x80) == 0)
  {
    //flag
    int flag_temp;
    flag_temp = shiftcode[ch];
    if(flag_temp != NO)
    {
      flag_shift = flag_temp;
      return;
    }

    //caps lock
    if(togglecode[ch] == CAPSLOCK)
    {
      flag_caps = 1;
      return;
    }

    //get result
    char result;
    if(flag_caps == 0)
    {
      switch(flag_shift)
      {
        case SHIFT:
          result = shiftmap[ch];
          break;
        case CTL:
          result = ctlmap[ch];
          break;
        default:
          result = normalmap[ch];
          break;
      }
    }
    else
    {
      switch(flag_shift)
      {
        case SHIFT:
          result = normalmap[ch];
          break;
        case CTL:
          result = ctlmap[ch];
          break;
        default:
          result = shiftmap[ch];
          break;
      }
    }

    //cprintf("ch: %d\n", ch);

    switch (ch) {
      case 1: 
        flag_event = KBD_ESC;
        break;
      case 75:
        flag_event = KBD_LEFT;
        break;
      case 77:
        flag_event = KBD_RIGHT;
        break;
      case 72:
        flag_event = KBD_UP;
        break;
      case 80:
        flag_event = KBD_DOWN;
        break;
      case 79:
        flag_event = KBD_END;
        break;
      default:
        flag_event = 0;
        break;
    }

    int cur_icon = WindowLine->next->Cur_icon;
    if(cur_icon == ICON_TEXT)
    {
      kbd_text(result, flag_event);
    }
    else if(cur_icon == ICON_PHOTO)
      photo(result, flag_event);
  }
  //release
  else {
    ch = ch & 0x7F;

    //caps lock
    if(togglecode[ch] == CAPSLOCK) {
      flag_caps = 0;
    }

    //shift
    if(shiftcode[ch] == flag_shift) {
      flag_shift = NO;
    }

    return;
  }
}

/*
int
kbdgetc(void)
{
  static uint shift;
  static uchar *charcode[4] = {
    normalmap, shiftmap, ctlmap, ctlmap
  };
  uint st, data, c;

  st = inb(KBSTATP);
  if((st & KBS_DIB) == 0)
    return -1;
  data = inb(KBDATAP);

  if(data == 0xE0){
    shift |= E0ESC;
    return 0;
  } else if(data & 0x80){
    // Key released
    data = (shift & E0ESC ? data : data & 0x7F);
    shift &= ~(shiftcode[data] | E0ESC);
    return 0;
  } else if(shift & E0ESC){
    // Last character was an E0 escape; or with 0x80
    data |= 0x80;
    shift &= ~E0ESC;
  }

  shift |= shiftcode[data];
  shift ^= togglecode[data];
  c = charcode[shift & (CTL | SHIFT)][data];
  if(shift & CAPSLOCK){
    if('a' <= c && c <= 'z')
      c += 'A' - 'a';
    else if('A' <= c && c <= 'Z')
      c += 'a' - 'A';
  }
  return c;
}
*/