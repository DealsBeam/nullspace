#include "AnimatedTileRenderer.h"

#include "../Game.h"
#include "../Map.h"
#include "../Math.h"
#include "Camera.h"
#include "Graphics.h"
#include "SpriteRenderer.h"
#include "TileRenderer.h"

namespace null {

inline void Animate(Animation& anim, float dt) {
  anim.t += dt;
  if (anim.sprite && anim.t >= anim.sprite->duration) {
    anim.t -= anim.sprite->duration;
  }
}

void AnimatedTileRenderer::Update(float dt) {
  Animate(anim_prize, dt);
  Animate(anim_flag, dt);
  Animate(anim_flag_team, dt);
  Animate(anim_goal, dt);
  Animate(anim_goal_team, dt);
  Animate(anim_asteroid_small1, dt);
  Animate(anim_asteroid_small2, dt);
  Animate(anim_asteroid_large, dt);
  Animate(anim_space_station, dt);
  Animate(anim_wormhole, dt);
  Animate(anim_doors[0], dt);
  Animate(anim_doors[1], dt);
}

void AnimatedTileRenderer::Render(SpriteRenderer& renderer, Map& map, Camera& camera, const Vector2f& screen_dim,
                                  struct GameFlag* flags, size_t flag_count, struct PrizeGreen* greens,
                                  size_t green_count, u32 freq) {
  Vector2f half_dim = screen_dim * (0.5f / 16.0f);
  Vector2f min = camera.position - half_dim;
  Vector2f max = camera.position + half_dim;

  for (size_t i = 0; i < green_count; ++i) {
    SpriteRenderable* renderable = &anim_prize.GetFrame();
    PrizeGreen* green = greens + i;

    renderer.Draw(camera, *renderable, green->position, Layer::AfterTiles);
  }

  for (size_t i = 0; i < flag_count; ++i) {
    SpriteRenderable* renderable = &anim_flag.GetFrame();
    GameFlag* flag = flags + i;

    if (flag->id == 0xFFFF || !flag->dropped) continue;

    if (flag->owner == freq) {
      renderable = &anim_flag_team.GetFrame();
    }

    renderer.Draw(camera, *renderable, flag->position, Layer::AfterTiles);
  }

  for (size_t i = 0; i < map.door_count; ++i) {
    Tile* door = map.doors + i;
    int index = (door->id - 162) / 4;

    if (map.GetTileId(door->x, door->y) == 170) continue;

    SpriteRenderable& renderable = anim_doors[index].GetFrame();
    renderer.Draw(camera, renderable, Vector2f((float)door->x, (float)door->y), Layer::Tiles);
  }

  for (s32 y = (s32)min.y - 5; y < (s32)max.y + 5; ++y) {
    if (y < 0 || y > 1023) continue;

    for (s32 x = (s32)min.x - 5; x < (s32)max.x + 5; ++x) {
      if (x < 0 || x > 1023) continue;

      u32 id = map.GetTileId((u16)x, (u16)y);

      if (id == 172) {
        // TODO: Determine if goal is team
        SpriteRenderable& renderable = anim_goal.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      } else if (id == 216) {
        SpriteRenderable& renderable = anim_asteroid_small1.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      } else if (id == 217) {
        SpriteRenderable& renderable = anim_asteroid_large.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      } else if (id == 218) {
        SpriteRenderable& renderable = anim_asteroid_small2.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      } else if (id == 219) {
        SpriteRenderable& renderable = anim_space_station.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      } else if (id == 220) {
        SpriteRenderable& renderable = anim_wormhole.GetFrame();
        renderer.Draw(camera, renderable, Vector2f((float)x, (float)y), Layer::Tiles);
      }
    }
  }
}

void AnimatedTileRenderer::Initialize() {
  anim_prize.sprite = &Graphics::anim_prize;
  anim_prize.t = 0.0f;

  anim_flag.sprite = &Graphics::anim_flag;
  anim_flag.t = 0.0f;

  anim_flag_team.sprite = &Graphics::anim_flag_team;
  anim_flag_team.t = 0.0f;

  anim_goal.sprite = &Graphics::anim_goal;
  anim_goal.t = 0.0f;

  anim_goal_team.sprite = &Graphics::anim_goal_team;
  anim_goal_team.t = 0.0f;

  anim_asteroid_small1.sprite = &Graphics::anim_asteroid_small1;
  anim_asteroid_small1.t = 0.0f;

  anim_asteroid_small2.sprite = &Graphics::anim_asteroid_small2;
  anim_asteroid_small2.t = 0.0f;

  anim_asteroid_large.sprite = &Graphics::anim_asteroid_large;
  anim_asteroid_large.t = 0.0f;

  anim_space_station.sprite = &Graphics::anim_space_station;
  anim_space_station.t = 0.0f;

  anim_wormhole.sprite = &Graphics::anim_wormhole;
  anim_wormhole.t = 0.0f;

  anim_doors[0].sprite = nullptr;
  anim_doors[1].sprite = nullptr;
}

void AnimatedTileRenderer::InitializeDoors(TileRenderer& tile_renderer) {
  for (size_t i = 0; i < 8; ++i) {
    float uv_x_start = i / 8.0f;
    float uv_x_end = (i + 1) / 8.0f;

    door_renderables[i].texture = tile_renderer.door_texture;
    door_renderables[i].dimensions = Vector2f(16.0f, 16.0f);
    door_renderables[i].uvs[0] = Vector2f(uv_x_start, 0.0f);
    door_renderables[i].uvs[1] = Vector2f(uv_x_end, 0.0f);
    door_renderables[i].uvs[2] = Vector2f(uv_x_start, 1.0f);
    door_renderables[i].uvs[3] = Vector2f(uv_x_end, 1.0f);
  }

  for (size_t i = 0; i < 2; ++i) {
    door_sprites[i].duration = 0.4f;
    door_sprites[i].frames = door_renderables + i * 4;
    door_sprites[i].frame_count = 4;
  }

  anim_doors[0].t = 0.0f;
  anim_doors[0].sprite = door_sprites + 0;
  anim_doors[1].t = 0.0f;
  anim_doors[1].sprite = door_sprites + 1;
}

}  // namespace null
