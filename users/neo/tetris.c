// Minimaler Tetris-Prototyp: ein Teil (T-Tetromino), Bewegung/Rotation/
// Softdrop/Harddrop, Zeilen-Clear. Rendert nur Zell-Deltas als ANSI-Escapes
// über HID-Keystrokes an ein Terminal (siehe keyboards/keychron/v1_max/
// TETRIS.md für die PC-seitige `stty raw -echo && cat`-Vorbereitung).
#include "tetris.h"
#include "quantum.h"
#include <string.h>

#define TET_W 10
#define TET_H 20
#define TET_DROP_MS 600
#define TET_ORIGIN_ROW 2
#define TET_ORIGIN_COL 2

// 0 = leer, 1 = eingerastet, 2 = aktives Teil
static uint8_t board[TET_H][TET_W];
static uint8_t prev_board[TET_H][TET_W]; // letzter gerenderter Stand

static const int8_t PROGMEM t_piece[4][4][2] = {
    {{0, 0}, {1, 0}, {2, 0}, {1, 1}},
    {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {1, 0}},
    {{1, 0}, {0, 1}, {1, 1}, {1, 2}},
};

static int8_t  piece_x, piece_y;
static uint8_t piece_rot;
static bool    game_over;
static bool    active;
static uint32_t last_tick;

static void piece_cell(int8_t bx, int8_t by, uint8_t rot, uint8_t idx, int8_t *cx, int8_t *cy) {
  *cx = bx + (int8_t)pgm_read_byte(&t_piece[rot][idx][0]);
  *cy = by + (int8_t)pgm_read_byte(&t_piece[rot][idx][1]);
}

static bool piece_fits(int8_t bx, int8_t by, uint8_t rot) {
  for (uint8_t i = 0; i < 4; i++) {
    int8_t cx, cy;
    piece_cell(bx, by, rot, i, &cx, &cy);
    if (cx < 0 || cx >= TET_W || cy >= TET_H) return false;
    if (cy >= 0 && board[cy][cx] == 1) return false;
  }
  return true;
}

static void stamp_piece(uint8_t value) {
  for (uint8_t i = 0; i < 4; i++) {
    int8_t cx, cy;
    piece_cell(piece_x, piece_y, piece_rot, i, &cx, &cy);
    if (cy >= 0) board[cy][cx] = value;
  }
}

static void clear_lines(void) {
  for (int8_t y = TET_H - 1; y >= 0; y--) {
    bool full = true;
    for (uint8_t x = 0; x < TET_W; x++) {
      if (board[y][x] != 1) {
        full = false;
        break;
      }
    }
    if (full) {
      for (int8_t yy = y; yy > 0; yy--) {
        memcpy(board[yy], board[yy - 1], TET_W);
      }
      memset(board[0], 0, TET_W);
      y++; // gleiche Zeile nach dem Nachrutschen erneut prüfen
    }
  }
}

static void spawn_piece(void) {
  piece_x  = 3;
  piece_y  = 0;
  piece_rot = 0;
  if (!piece_fits(piece_x, piece_y, piece_rot)) {
    game_over = true;
    return;
  }
  stamp_piece(2);
}

static void lock_piece(void) {
  stamp_piece(1);
  clear_lines();
  spawn_piece();
}

static void move_or_lock(int8_t dx, int8_t dy, bool lock_on_fail) {
  stamp_piece(0);
  if (piece_fits(piece_x + dx, piece_y + dy, piece_rot)) {
    piece_x += dx;
    piece_y += dy;
    stamp_piece(2);
  } else {
    stamp_piece(2);
    if (lock_on_fail && dy > 0) {
      lock_piece();
    }
  }
}

// ── Rendering: nur geänderte Zellen, per Cursor-Positionierung ─────────────

static void append_uint(char *buf, uint8_t *pos, uint8_t v) {
  if (v >= 10) append_uint(buf, pos, v / 10);
  buf[(*pos)++] = '0' + (v % 10);
}

static void send_cursor_to(uint8_t bx, uint8_t by) {
  char    buf[12];
  uint8_t pos = 0;
  buf[pos++]  = 0x1B;
  buf[pos++]  = '[';
  append_uint(buf, &pos, TET_ORIGIN_ROW + by);
  buf[pos++] = ';';
  append_uint(buf, &pos, TET_ORIGIN_COL + (uint8_t)(bx * 2));
  buf[pos++] = 'H';
  buf[pos]   = '\0';
  send_string(buf);
}

static char glyph_for(uint8_t v) {
  switch (v) {
    case 0:
      return '.';
    case 2:
      return '@';
    default:
      return '#';
  }
}

static void render_diff(void) {
  for (uint8_t y = 0; y < TET_H; y++) {
    for (uint8_t x = 0; x < TET_W; x++) {
      uint8_t v = board[y][x];
      if (prev_board[y][x] != v) {
        send_cursor_to(x, y);
        char c[2] = {glyph_for(v), 0};
        send_string(c);
        prev_board[y][x] = v;
      }
    }
  }
}

// ── Public API ───────────────────────────────────────────────────────────

void tetris_start(void) {
  memset(board, 0, sizeof(board));
  memset(prev_board, 0xFF, sizeof(prev_board)); // erzwingt volles Redraw
  game_over = false;
  active    = true;
  last_tick = timer_read32();
  send_string("\x1b[2J\x1b[H"); // Bildschirm löschen, Cursor Home
  spawn_piece();
  render_diff();
}

void tetris_stop(void) {
  active = false;
}

bool tetris_is_active(void) {
  return active;
}

void tetris_task(void) {
  if (!active || game_over) return;
  if (timer_elapsed32(last_tick) >= TET_DROP_MS) {
    last_tick = timer_read32();
    move_or_lock(0, 1, true);
    render_diff();
  }
}

void tetris_input_left(void) {
  if (!active || game_over) return;
  move_or_lock(-1, 0, false);
  render_diff();
}

void tetris_input_right(void) {
  if (!active || game_over) return;
  move_or_lock(1, 0, false);
  render_diff();
}

void tetris_input_soft_drop(void) {
  if (!active || game_over) return;
  move_or_lock(0, 1, true);
  last_tick = timer_read32();
  render_diff();
}

void tetris_input_rotate(void) {
  if (!active || game_over) return;
  uint8_t new_rot = (piece_rot + 1) % 4;
  stamp_piece(0);
  if (piece_fits(piece_x, piece_y, new_rot)) {
    piece_rot = new_rot;
  }
  stamp_piece(2);
  render_diff();
}

void tetris_input_hard_drop(void) {
  if (!active || game_over) return;
  stamp_piece(0);
  while (piece_fits(piece_x, piece_y + 1, piece_rot)) {
    piece_y++;
  }
  lock_piece();
  last_tick = timer_read32();
  render_diff();
}
