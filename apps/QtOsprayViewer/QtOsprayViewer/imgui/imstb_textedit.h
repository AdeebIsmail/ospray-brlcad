// Minimal stb_textedit.h stub - most functionality not needed for ImGui
// This is just a placeholder header to allow imgui to compile

#ifndef INCLUDE_IMSTB_TEXTEDIT_H
#define INCLUDE_IMSTB_TEXTEDIT_H

#ifndef IMSTB_TEXTEDIT_UNDOSTATECOUNT
#define IMSTB_TEXTEDIT_UNDOSTATECOUNT 99
#endif
#ifndef IMSTB_TEXTEDIT_UNDOCHARCOUNT
#define IMSTB_TEXTEDIT_UNDOCHARCOUNT 999
#endif
#ifndef IMSTB_TEXTEDIT_CHARTYPE
#define IMSTB_TEXTEDIT_CHARTYPE int
#endif
#ifndef IMSTB_TEXTEDIT_POSITIONTYPE
#define IMSTB_TEXTEDIT_POSITIONTYPE int
#endif

typedef struct
{
  IMSTB_TEXTEDIT_POSITIONTYPE where;
  IMSTB_TEXTEDIT_POSITIONTYPE insert_length;
  IMSTB_TEXTEDIT_POSITIONTYPE delete_length;
  int char_storage;
} StbUndoRecord;

typedef struct
{
  StbUndoRecord undo_rec[IMSTB_TEXTEDIT_UNDOSTATECOUNT];
  IMSTB_TEXTEDIT_CHARTYPE undo_char[IMSTB_TEXTEDIT_UNDOCHARCOUNT];
  short undo_point, redo_point;
  int undo_char_point, redo_char_point;
} StbUndoState;

typedef struct STB_TexteditState
{
  int cursor;
  int select_start;
  int select_end;
  unsigned char insert_mode;
  int row_count_per_page;
  unsigned char cursor_at_end_of_line;
  unsigned char initialized;
  unsigned char has_preferred_x;
  unsigned char single_line;
  unsigned char padding1, padding2, padding3;
  float preferred_x;
  StbUndoState undostate;
} STB_TexteditState;

typedef struct
{
  float x0, x1;
  float baseline_y_delta;
  float ymin, ymax;
  int num_chars;
} StbTexteditRow;

// STB_TEXTEDIT key constants
#define STB_TEXTEDIT_K_LEFT (1 << 16)
#define STB_TEXTEDIT_K_RIGHT (1 << 17)
#define STB_TEXTEDIT_K_UP (1 << 18)
#define STB_TEXTEDIT_K_DOWN (1 << 19)
#define STB_TEXTEDIT_K_LINESTART (1 << 20)
#define STB_TEXTEDIT_K_LINEEND (1 << 21)
#define STB_TEXTEDIT_K_TEXTSTART (1 << 22)
#define STB_TEXTEDIT_K_TEXTEND (1 << 23)
#define STB_TEXTEDIT_K_DELETE (1 << 24)
#define STB_TEXTEDIT_K_BACKSPACE (1 << 25)
#define STB_TEXTEDIT_K_UNDO (1 << 26)
#define STB_TEXTEDIT_K_REDO (1 << 27)
#define STB_TEXTEDIT_K_SHIFT (1 << 15)
#define STB_TEXTEDIT_K_WORDLEFT (1 << 28)
#define STB_TEXTEDIT_K_WORDRIGHT (1 << 29)

// Stub implementations - provide minimal working versions for ImGui
static inline void stb_textedit_initialize_state(
    STB_TexteditState *state, int single_line)
{
  state->cursor = 0;
  state->select_start = 0;
  state->select_end = 0;
  state->insert_mode = 1;
  state->single_line = single_line ? 1 : 0;
  state->initialized = 1;
}

static inline void stb_textedit_click(
    void *str, STB_TexteditState *state, float x, float y)
{
  // Placeholder implementation
}

static inline void stb_textedit_drag(
    void *str, STB_TexteditState *state, float x, float y)
{
  // Placeholder implementation
}

static inline void stb_textedit_clamp(void *str, STB_TexteditState *state)
{
  // Placeholder implementation
}

static inline void stb_textedit_cut(void *str, STB_TexteditState *state)
{
  // Placeholder implementation
}

static inline void stb_textedit_paste(
    void *str, STB_TexteditState *state, const char *ctext, int len)
{
  // Placeholder implementation
}

static inline void stb_textedit_key(
    void *str, STB_TexteditState *state, int key)
{
  // Placeholder implementation - keys are handled by ImGui
}

static inline void stb_textedit_text(
    void *str, STB_TexteditState *state, const char *text, int len)
{
  // Text insertion is handled by ImGui through STB_TEXTEDIT_INSERTCHARS
}

static inline void stb_text_makeundo_replace(
    void *str, STB_TexteditState *state, int where, int old_len, int new_len)
{
  // Placeholder implementation
}

static inline IMSTB_TEXTEDIT_CHARTYPE *stb_text_createundo(
    StbUndoState *state, int pos, int ins_len, int del_len)
{
  // Placeholder implementation - return null pointer
  return nullptr;
}

#define STB_TEXT_HAS_SELECTION(s) ((s)->select_start != (s)->select_end)

static inline void stb_textedit_prep_selection_at_cursor(
    STB_TexteditState *state)
{
  if (state->select_start != state->select_end)
    state->select_start = state->select_end = state->cursor;
}

#endif
