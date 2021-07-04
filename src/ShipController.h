#ifndef NULLSPACE_SHIPCONTROLLER_H_
#define NULLSPACE_SHIPCONTROLLER_H_

#include "Types.h"
#include "render/Animation.h"

namespace null {

struct Camera;
struct InputState;
struct NotificationSystem;
struct PacketDispatcher;
struct Player;
struct PlayerManager;
struct SpriteRenderer;
struct Weapon;
struct WeaponManager;

struct Exhaust {
  Animation animation;
  Vector2f velocity;
};

enum class Prize {
  None,
  Recharge,
  Energy,
  Rotation,
  Stealth,
  Cloak,
  XRadar,
  Warp,
  Guns,
  Bombs,
  BouncingBullets,
  Thruster,
  TopSpeed,
  FullCharge,
  EngineShutdown,
  Multifire,
  Proximity,
  Super,
  Shields,
  Shrapnel,
  Antiwarp,
  Repel,
  Burst,
  Decoy,
  Thor,
  Multiprize,
  Brick,
  Rocket,
  Portal,

  Count
};

enum {
  ShipCapability_Stealth = (1 << 0),
  ShipCapability_Cloak = (1 << 1),
  ShipCapability_XRadar = (1 << 2),
  ShipCapability_Antiwarp = (1 << 3),
  ShipCapability_Multifire = (1 << 4),
  ShipCapability_Proximity = (1 << 5),
  ShipCapability_BouncingBullets = (1 << 6),
};
using ShipCapabilityFlags = int;

struct Ship {
  u32 energy;
  u32 recharge;
  u32 rotation;
  u32 guns;
  u32 bombs;
  u32 thrust;
  u32 speed;
  u32 shrapnel;

  u32 repels;
  u32 bursts;
  u32 decoys;
  u32 thors;
  u32 bricks;
  u32 rockets;
  u32 portals;

  bool multifire;
  float emped_time;

  ShipCapabilityFlags capability;
};

struct ShipController {
  PlayerManager& player_manager;
  WeaponManager& weapon_manager;
  NotificationSystem& notifications_;
  Ship ship;
  u32 next_bullet_tick = 0;
  u32 next_bomb_tick = 0;
  u32 next_repel_tick = 0;
  size_t exhaust_count = 0;
  Exhaust exhausts[64];

  ShipController(PlayerManager& player_manager, WeaponManager& weapon_manager, PacketDispatcher& dispatcher,
                 NotificationSystem& notifications);

  void Update(const InputState& input, float dt);
  void FireWeapons(Player& self, const InputState& input, float dt);

  void Render(Camera& ui_camera, Camera& camera, SpriteRenderer& renderer);
  void RenderIndicators(Camera& ui_camera, SpriteRenderer& renderer);
  void RenderItemIndicator(Camera& ui_camera, SpriteRenderer& renderer, int value, size_t index, float* y);
  size_t GetGunIconIndex();
  size_t GetBombIconIndex();

  void OnPlayerFreqAndShipChange(u8* pkt, size_t size);
  void OnCollectedPrize(u8* pkt, size_t size);

  void ResetShip();

  void ApplyPrize(Player* self, s32 prize_id, bool notify);
  s32 GeneratePrize(bool negative_allowed);

  void OnWeaponHit(Weapon& weapon);
};

}  // namespace null

#endif
