// stb_rect_pack.h - v1.00 - public domain - rectangle packing
// Minimal stb_rect_pack header for ImGui

#ifndef STB_RECT_PACK_H
#define STB_RECT_PACK_H

typedef struct
{
  int w, h; // width and height
  int x, y;
  int id;
  unsigned int was_packed;
} stbrp_rect;

typedef struct stbrp_node
{
  int x, y;
} stbrp_node;

typedef struct
{
  int width, height;
  stbrp_node *nodes;
  int num_nodes;
  int active_head;
} stbrp_context;

static inline void stbrp_init_target(
    stbrp_context *con, int pw, int ph, stbrp_node *nodes, int num_nodes)
{
  con->width = pw;
  con->height = ph;
  con->nodes = nodes;
  con->num_nodes = num_nodes;
  con->active_head = 0;
}

static inline void stbrp_pack_rects(
    stbrp_context *con, stbrp_rect *rects, int num_rects)
{
  int i;
  for (i = 0; i < num_rects; ++i) {
    rects[i].was_packed = 1;
    rects[i].x = 0;
    rects[i].y = 0;
  }
}

#endif
