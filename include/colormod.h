#ifndef COLORMOD_H
#define COLORMOD_H

void irgb_to_rgb( int ir,int  ig, int ib,float *r, float *g, float *b);

void cmy_to_rgb ( float c, float m, float y, float *r, float *g, float *b)
;

void hls_to_rgb (float h, float l, float s, float *r, float *g, float *b)
;

void hsv_to_rgb( float h, float s, float v, float *r, float *g, float *b);

void yiq_to_rgb (float y, float i, float q, float *r, float *g, float *b);

void rgb_to_rgb( float fr, float fg, float fb, float *tr, float *tg, float *tb);

void rgb_to_cmy( float r, float g, float b, float *c, float *m, float *y);

void rgb_to_irgb( float r, float g, float b, int *ir, int *ig, int *ib);

void rgb_to_hls( float r, float g, float b, float *h, float *l, float *s);

void rgb_to_hsv (float r, float g, float b, float *lh, float *ls, float *lv);

void rgb_to_yiq (float r, float g, float b, float *y, float *i, float *q);









#endif /*COLORMOD_H*/
