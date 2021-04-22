#ifndef NULLSPACE_LVZCONTROLLER_H_
#define NULLSPACE_LVZCONTROLLER_H_

#include "Types.h"
#include "render/Animation.h"

namespace null {

struct Camera;
struct FileRequester;
struct MemoryArena;
struct PacketDispatcher;
struct SpriteRenderer;

struct LvzObject {
  u16 map_object : 1;
  u16 object_id : 15;

  union {
    s16 x_map;
    struct {
      u16 x_type : 4;
      s16 x_screen : 12;
    };
  };

  union {
    s16 y_map;
    struct {
      u16 y_type : 4;
      s16 y_screen : 12;
    };
  };

  size_t animation_index;
  Layer layer;

  u16 display_time : 12;
  u16 display_mode : 4;
};

struct LvzController {
  MemoryArena& perm_arena;
  MemoryArena& temp_arena;
  FileRequester& requester;
  SpriteRenderer& renderer;

  size_t animation_count = 0;
  Animation animations[1024];
  AnimatedSprite sprites[1024];

  size_t object_count = 0;
  LvzObject objects[2048];

  size_t active_screen_object_count = 0;
  LvzObject* active_screen_objects[2048];
  size_t active_map_object_count = 0;
  LvzObject* active_map_objects[2048];

  LvzController(MemoryArena& perm_arena, MemoryArena& temp_arena, FileRequester& requester, SpriteRenderer& renderer,
                PacketDispatcher& dispatcher);

  void OnMapInformation(u8* pkt, size_t size);
  void OnLvzToggle(u8* pkt, size_t size);
  void OnFileDownload(struct FileRequest* request, u8* data);

  void Update(float dt);
  void Render(Camera& ui_camera, Camera& game_camera);

 private:
  void ProcessGraphicFile(const char* filename, u8* data, size_t size);
  void ProcessObjects(u8* data, size_t size);
};

}  // namespace null

#endif
