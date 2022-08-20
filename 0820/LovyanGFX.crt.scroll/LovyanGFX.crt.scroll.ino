#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define CRT_SPC    (16)
#define CRT_WIDTH  (320 + CRT_SPC)
#define CRT_HEIGHT (256 + CRT_SPC)
#define CRT_PORT (26)

uint16_t _w, _h, _hw, _hh;
class LGFX : public lgfx::LGFX_Device
{
  public:

    lgfx::Panel_CVBS _panel_instance;

    LGFX(void) {
      {
        auto cfg = _panel_instance.config();
        cfg.memory_width  = CRT_WIDTH;
        cfg.memory_height = CRT_HEIGHT;
        cfg.panel_width  = CRT_WIDTH - CRT_SPC;
        cfg.panel_height = CRT_HEIGHT - CRT_SPC;
        cfg.offset_x = CRT_SPC / 2;
        cfg.offset_y = CRT_SPC / 2;
        _panel_instance.config(cfg);
      }

      {
        auto cfg = _panel_instance.config_detail();
        // cfg.signal_type = cfg.signal_type_t::NTSC;
        cfg.signal_type = cfg.signal_type_t::NTSC_J;
        // cfg.signal_type = cfg.signal_type_t::PAL;
        // cfg.signal_type = cfg.signal_type_t::PAL_M;
        // cfg.signal_type = cfg.signal_type_t::PAL_N;
        cfg.pin_dac = CRT_PORT;
        cfg.use_psram = 0;      // 0..2
        cfg.output_level = 128; // 0..255
        cfg.chroma_level = 128; // 0..255
        _panel_instance.config_detail(cfg);
      }

      setPanel(&_panel_instance);
    }
};
static LGFX crt;

int32_t getfps(void) {
  static uint32_t psec = 0;
  static uint32_t cnt = 0;
  static uint32_t fps = 0;
  uint32_t sec = 0;

  sec = millis() / 1000;
  ++cnt;
  if (psec != sec) {
    psec = sec;
    fps = cnt;
    cnt = 0;
  }
  return fps;
}

void drawfps(void) {
  crt.setTextColor(TFT_WHITE, TFT_BLACK);
  crt.setCursor(0, _h - 8);
  crt.printf("%3dx%3d %6d Free %3dFps", _w, _h, esp_get_minimum_free_heap_size(), getfps());
}

void setup(void) {
  crt.init(); _w = crt.width(); _h = crt.height(); _hw = _w >> 1; _hh = _h >> 1;
  crt.clear(TFT_BLACK);
  crt.drawRect(0, 0, _w, _h, TFT_BLUE);
  crt.drawCircle(_hw, _hh, _hh, TFT_BLUE);

  crt.setTextScroll(true);
  crt.setScrollRect(0, 0, _w, _h);
}

void loop(void) {
  static int cnt = 0;
  crt.printf("%04x:", cnt);
  for (int i = 0; i < 16; i++) crt.printf("%02x", i);
  crt.println();
  cnt++;
//  crt.scroll(0, 8);
  delay(100);
}
