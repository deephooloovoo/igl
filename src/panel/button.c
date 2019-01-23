/*****************************************************************
    Panel Library/Electropaint Copyright (c) 1986 David A. Tristram.
    Electropaint (TM) is a Registered U.S. Trademark of Tristram Visual.
    Tristram Visual can be contacted at www.tristram.com.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
*****************************************************************/
#include <gl.h>
#include <panel.h>

void _newvalbutton(Actuator *a,Panel *p,Coord x,Coord y)
{
  if (pnl_mousedown) a->val=a->maxval;
  else		     a->val=a->minval;
  pnl_setdirty(a);
}

void _newvaltogglebutton(Actuator *a,Panel *p,Coord x,Coord y)
{
  if (pnl_justdown) a->val=(a->val==a->minval)?a->maxval:a->minval;
  pnl_setdirty(a);
}

void _newvalradiobutton(Actuator *a,Panel *p,Coord x,Coord y)
{
  Actuator *b;

  if (pnl_justdown) a->val=a->maxval;
  for (b=a->group;(b!=a);b=b->group) {
    b->val=b->minval;
    pnl_fixact(b);
  }
  pnl_setdirty(a);
}

void _newvalarrowbutton(Actuator *a,Panel *p,Coord x,Coord y)
{
    _newvalbutton(a,p,x,y);
    pnl_setdirty(a);
}

void
_addradiobutton(Actuator *a, Panel* p)
{
    pnl_addtogroup(a, p);
}

void
_addwidebutton(Actuator *a, Panel *p)
{
  a->w=MAX(a->w, pnl_strwidth(a->p, a->label)+2*PNL_DIM_2);
}  

void
_fixwidebutton(Actuator* a)
{
  a->w=MAX(a->w, a->lw+2*PNL_DIM_2);
}  

void
_drawbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

    if (a->val!=a->minval) {
	color(pnl_highlight_color);
    } else {
	color(pnl_normal_color);
    }
    rectf(a->x,a->y,a->x+a->w,a->y+a->h);
    color(pnl_black_color);
    rect(a->x,a->y,a->x+a->w,a->y+a->h);
    if (a->beveled) pnl_drawbevel(a, p);
    if (a->label) pnl_drawlabel(a, p);
}

void
_drawtogglebutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

    if (a->val!=a->minval) {
	color(pnl_highlight_color);
    } else {
	color(pnl_normal_color);
    }
    rectf(a->x,a->y,a->x+a->w,a->y+a->h);
    color(pnl_black_color);
    rect(a->x,a->y,a->x+a->w,a->y+a->h);
    if (a->val!=a->minval) {
      color(pnl_white_color);
      move2(a->x,a->y);
      draw2(a->x+a->w,a->y+a->h);
      move2(a->x,a->y+a->h);
      draw2(a->x+a->w,a->y);
    }
    if (a->beveled) pnl_drawbevel(a, p);
    if (a->label) pnl_drawlabel(a, p);
}

void
_drawwidebuttonshape(Actuator *a, Panel *p, int style)
{
  if (style==PNL_FILLED) {
    move2(0.0,0.0);
    rpmv2(PNL_DIM_2, 0.0);
    rpdr2(a->w-2.0*PNL_DIM_2, 0.0);
    rpdr2(PNL_DIM_2, PNL_DIM_2);
    rpdr2(0.0, a->h-2.0*PNL_DIM_2);
    rpdr2(-PNL_DIM_2, PNL_DIM_2);
    rpdr2(-(a->w-2.0*PNL_DIM_2), 0.0);
    rpdr2(-PNL_DIM_2, -PNL_DIM_2);
    rpdr2(0.0, -(a->h-2.0*PNL_DIM_2));
    pclos();
  } else {
    move2(0.0,0.0);
    rmv2(PNL_DIM_2, 0.0);
    rdr2(a->w-2.0*PNL_DIM_2, 0.0);
    rdr2(PNL_DIM_2, PNL_DIM_2);
    rdr2(0.0, a->h-2.0*PNL_DIM_2);
    rdr2(-PNL_DIM_2, PNL_DIM_2);
    rdr2(-(a->w-2.0*PNL_DIM_2), 0.0);
    rdr2(-PNL_DIM_2, -PNL_DIM_2);
    rdr2(0.0, -(a->h-2.0*PNL_DIM_2));
    rdr2(PNL_DIM_2, -PNL_DIM_2);
  }
}

void
_drawwidebutton(Actuator *a,Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x,a->y,0.0);
  
  if (a->val==a->minval)
    color(pnl_normal_color);
  else
    color(pnl_highlight_color);

  _drawwidebuttonshape(a, p, PNL_FILLED);
  color(pnl_black_color);
  _drawwidebuttonshape(a, p, PNL_OPEN);

  if (a->label&&(p->ppu>pnl_char_threshold)) {
    if (a->val==a->minval)
	color(pnl_label_color);
    else
	color(pnl_white_color);
    cmov2(a->lx,a->ly+a->ld);
    charstr(a->label);
  }

  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
}

void
_drawarrowbutton(Actuator *a, Panel* p, int style) /* defaults to point right, origin at center */
{
  float sx=a->w;
  float sy=a->h;

  if (style==PNL_OPEN) { /* unselected */
    color(pnl_normal_color);
    rectf(-sx/2, -sy/2, sx/2, sy/2);
    
    color(pnl_other_color);
    pmv2(-0.3*sx, -0.3*sy);
    pdr2(0.3*sx, 0.0*sy);
    pdr2(-0.3*sx, 0.3*sy);
    pclos();
    color(pnl_black_color);
    move2(-0.3*sx, -0.3*sy);
    draw2(0.3*sx, 0.0*sy);
    draw2(-0.3*sx, 0.3*sy);
    draw2(-0.3*sx, -0.3*sy);
  } else {
    color(pnl_highlight_color);
    rectf(-sx/2, -sy/2, sx/2, sy/2);
    
    color(pnl_normal_color);
    pmv2(-0.3*sx, -0.3*sy);
    pdr2(0.3*sx, 0.0*sy);
    pdr2(-0.3*sx, 0.3*sy);
    pclos();
    color(pnl_white_color);
    move2(-0.3*sx, -0.3*sy);
    draw2(0.3*sx, 0.0*sy);
    draw2(-0.3*sx, 0.3*sy);
    draw2(-0.3*sx, -0.3*sy);
  }
  color(pnl_black_color);
  rect(-sx/2, -sy/2, sx/2, sy/2);
}

void
_drawdoublearrowbutton(Actuator *a, Panel *p, int style)
{
  float sx=a->w;
  float sy=a->h;

  if (style==PNL_OPEN) { /* unselected */
    color(pnl_normal_color);
    rectf(-sx/2, -sy/2, sx/2, sy/2);
    
    color(pnl_other_color);
    pmv2(-0.3*sx, -0.3*sy);
    pdr2(-0.03*sx, 0.0*sy);
    pdr2(-0.3*sx, 0.3*sy);
    pclos();
    pmv2(0.03*sx, -0.3*sy);
    pdr2(0.3*sx, 0.0*sy);
    pdr2(0.03*sx, 0.3*sy);
    pclos();
    color(pnl_black_color);
    move2(-0.3*sx, -0.3*sy);
    draw2(-0.03*sx, 0.0*sy);
    draw2(-0.3*sx, 0.3*sy);
    draw2(-0.3*sx, -0.3*sy);
    move2(0.03*sx, -0.3*sy);
    draw2(0.3*sx, 0.0*sy);
    draw2(0.03*sx, 0.3*sy);
    draw2(0.03*sx, -0.3*sy);
  } else {
    color(pnl_highlight_color);
    rectf(-sx/2, -sy/2, sx/2, sy/2);
    
    color(pnl_normal_color);
    pmv2(-0.3*sx, -0.3*sy);
    pdr2(-0.03*sx, 0.0*sy);
    pdr2(-0.3*sx, 0.3*sy);
    pclos();
    pmv2(0.03*sx, -0.3*sy);
    pdr2(0.3*sx, 0.0*sy);
    pdr2(0.03*sx, 0.3*sy);
    pclos();
    color(pnl_white_color);
    move2(-0.3*sx, -0.3*sy);
    draw2(-0.03*sx, 0.0*sy);
    draw2(-0.3*sx, 0.3*sy);
    draw2(-0.3*sx, -0.3*sy);
    move2(0.03*sx, -0.3*sy);
    draw2(0.3*sx, 0.0*sy);
    draw2(0.03*sx, 0.3*sy);
    draw2(0.03*sx, -0.3*sy);
  }
  color(pnl_black_color);
  rect(-sx/2, -sy/2, sx/2, sy/2);
}

void
_drawleftarrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(1800, 'z');
  if (a->val!=a->minval) _drawarrowbutton(a, p, PNL_FILLED);
  else			 _drawarrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawrightarrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  if (a->val!=a->minval) _drawarrowbutton(a, p, PNL_FILLED);
  else			 _drawarrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawuparrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(900, 'z');
  if (a->val!=a->minval) _drawarrowbutton(a, p, PNL_FILLED);
  else			 _drawarrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawdownarrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(-900, 'z');
  if (a->val!=a->minval) _drawarrowbutton(a, p, PNL_FILLED);
  else			 _drawarrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawleftdoublearrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(1800, 'z');
  if (a->val!=a->minval) _drawdoublearrowbutton(a, p, PNL_FILLED);
  else			 _drawdoublearrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawrightdoublearrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  if (a->val!=a->minval) _drawdoublearrowbutton(a, p, PNL_FILLED);
  else			 _drawdoublearrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawupdoublearrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(900, 'z');
  if (a->val!=a->minval) _drawdoublearrowbutton(a, p, PNL_FILLED);
  else			 _drawdoublearrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
_drawdowndoublearrowbutton(Actuator *a, Panel *p)
{
  if (!a->dirtycnt) return;

  pushmatrix();
  translate(a->x, a->y, 0.0);
  translate(a->w/2, a->h/2, 0.0);
  rotate(-900, 'z');
  if (a->val!=a->minval) _drawdoublearrowbutton(a, p, PNL_FILLED);
  else			 _drawdoublearrowbutton(a, p, PNL_OPEN);
  popmatrix();
  if (a->beveled) pnl_drawbevel(a, p);
  if (a->label) pnl_drawlabel(a, p);
}

void
pnl_button(Actuator *a)
{
    a->type=PNL_BUTTON;

    a->w=PNL_BUTTON_EDGE;
    a->h=PNL_BUTTON_EDGE;
    a->labeltype=PNL_LABEL_RIGHT;
    a->newvalfunc=_newvalbutton;
    a->drawfunc=_drawbutton;
}

void
pnl_radio_button(Actuator *a)
{
    a->type=PNL_RADIO_BUTTON;

    a->w=PNL_BUTTON_EDGE;
    a->h=PNL_BUTTON_EDGE;
    a->labeltype=PNL_LABEL_RIGHT;
    a->addfunc=_addradiobutton;
    a->newvalfunc=_newvalradiobutton;
    a->drawfunc=_drawtogglebutton;
}

void
pnl_toggle_button(Actuator *a)
{
    a->type=PNL_TOGGLE_BUTTON;

    a->w=PNL_BUTTON_EDGE;
    a->h=PNL_BUTTON_EDGE;
    a->labeltype=PNL_LABEL_RIGHT;
    a->newvalfunc=_newvaltogglebutton;
    a->drawfunc=_drawtogglebutton;
}

void
pnl_wide_button(Actuator *a)
{
    a->type=PNL_WIDE_BUTTON;

    a->w=PNL_WIDE_BUTTON_WIDTH;
    a->h=PNL_WIDE_BUTTON_HEIGHT;
    a->labeltype=PNL_LABEL_CENTER;
    a->addfunc=_addwidebutton;
    a->fixfunc=_fixwidebutton;
    a->newvalfunc=_newvalbutton;
    a->drawfunc=_drawwidebutton;
}

void
pnl_left_arrow_button(Actuator *a)
{
  a->type=PNL_LEFT_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawleftarrowbutton;
}

void
pnl_right_arrow_button(Actuator *a)
{
  a->type=PNL_RIGHT_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawrightarrowbutton;
}

void
pnl_up_arrow_button(Actuator *a)
{
  a->type=PNL_UP_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawuparrowbutton;
}

void
pnl_down_arrow_button(Actuator *a)
{
  a->type=PNL_DOWN_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalbutton;
  a->drawfunc=_drawdownarrowbutton;
}

void
pnl_left_double_arrow_button(Actuator *a)
{
  a->type=PNL_LEFT_DOUBLE_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawleftdoublearrowbutton;
}

void
pnl_right_double_arrow_button(Actuator *a)
{
  a->type=PNL_RIGHT_DOUBLE_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawrightdoublearrowbutton;
}

void
pnl_up_double_arrow_button(Actuator *a)
{
  a->type=PNL_UP_DOUBLE_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawupdoublearrowbutton;
}

void
pnl_down_double_arrow_button(Actuator *a)
{
  a->type=PNL_DOWN_DOUBLE_ARROW_BUTTON;

  a->w=a->h=PNL_BUTTON_EDGE;
  a->labeltype=PNL_LABEL_RIGHT;
  a->newvalfunc=_newvalarrowbutton;
  a->drawfunc=_drawdowndoublearrowbutton;
}

