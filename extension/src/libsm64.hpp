#ifndef LIBSM64GD_LIBSM64_H
#define LIBSM64GD_LIBSM64_H

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/image.hpp>

#include <godot_cpp/variant/callable.hpp>

#include <libsm64.h>

class LibSM64MarioInputs;
class LibSM64SurfaceArray;

class LibSM64 : public godot::Object
{
    GDCLASS(LibSM64, godot::Object);

    inline static LibSM64 *singleton = nullptr;

public:
    LibSM64();
    ~LibSM64();

    static LibSM64 *get_singleton();
    static void register_project_settings();

    static constexpr double tick_delta_time = 1.0 / 30.0;

    // SurfaceType and TerrainType are from "libsm64/src/decomp/include/surface_terrains.h"

    enum SurfaceType
    {
        SURFACE_DEFAULT                  = 0x0000, // Environment default
        SURFACE_BURNING                  = 0x0001, // Lava / Frostbite (in SL), but is used mostly for Lava
        SURFACE_0004                     = 0x0004, // Unused, has no function and has parameters
        SURFACE_HANGABLE                 = 0x0005, // Ceiling that Mario can climb on
        SURFACE_SLOW                     = 0x0009, // Slow down Mario, unused
        SURFACE_DEATH_PLANE              = 0x000A, // Death floor
        SURFACE_CLOSE_CAMERA             = 0x000B, // Close camera
        SURFACE_WATER                    = 0x000D, // Water, has no action, used on some waterboxes below
        SURFACE_FLOWING_WATER            = 0x000E, // Water (flowing), has parameters
        SURFACE_INTANGIBLE               = 0x0012, // Intangible (Separates BBH mansion from merry-go-round, for room usage)
        SURFACE_VERY_SLIPPERY            = 0x0013, // Very slippery, mostly used for slides
        SURFACE_SLIPPERY                 = 0x0014, // Slippery
        SURFACE_NOT_SLIPPERY             = 0x0015, // Non-slippery, climbable
        SURFACE_TTM_VINES                = 0x0016, // TTM vines, has no action defined
        SURFACE_MGR_MUSIC                = 0x001A, // Plays the Merry go round music, see handle_merry_go_round_music in bbh_merry_go_round.inc.c for more details
        SURFACE_INSTANT_WARP_1B          = 0x001B, // Instant warp to another area, used to warp between areas in WDW and the endless stairs to warp back
        SURFACE_INSTANT_WARP_1C          = 0x001C, // Instant warp to another area, used to warp between areas in WDW
        SURFACE_INSTANT_WARP_1D          = 0x001D, // Instant warp to another area, used to warp between areas in DDD, SSL and TTM
        SURFACE_INSTANT_WARP_1E          = 0x001E, // Instant warp to another area, used to warp between areas in DDD, SSL and TTM
        SURFACE_SHALLOW_QUICKSAND        = 0x0021, // Shallow Quicksand (depth of 10 units)
        SURFACE_DEEP_QUICKSAND           = 0x0022, // Quicksand (lethal, slow, depth of 160 units)
        SURFACE_INSTANT_QUICKSAND        = 0x0023, // Quicksand (lethal, instant)
        SURFACE_DEEP_MOVING_QUICKSAND    = 0x0024, // Moving quicksand (flowing, depth of 160 units)
        SURFACE_SHALLOW_MOVING_QUICKSAND = 0x0025, // Moving quicksand (flowing, depth of 25 units)
        SURFACE_QUICKSAND                = 0x0026, // Moving quicksand (60 units)
        SURFACE_MOVING_QUICKSAND         = 0x0027, // Moving quicksand (flowing, depth of 60 units)
        SURFACE_WALL_MISC                = 0x0028, // Used for some walls, Cannon to adjust the camera, and some objects like Warp Pipe
        SURFACE_NOISE_DEFAULT            = 0x0029, // Default floor with noise
        SURFACE_NOISE_SLIPPERY           = 0x002A, // Slippery floor with noise
        SURFACE_HORIZONTAL_WIND          = 0x002C, // Horizontal wind, has parameters
        SURFACE_INSTANT_MOVING_QUICKSAND = 0x002D, // Quicksand (lethal, flowing)
        SURFACE_ICE                      = 0x002E, // Slippery Ice, in snow levels and THI's water floor
        SURFACE_LOOK_UP_WARP             = 0x002F, // Look up and warp (Wing cap entrance)
        SURFACE_HARD                     = 0x0030, // Hard floor (Always has fall damage)
        SURFACE_WARP                     = 0x0032, // Surface warp
        SURFACE_TIMER_START              = 0x0033, // Timer start (Peach's secret slide)
        SURFACE_TIMER_END                = 0x0034, // Timer stop (Peach's secret slide)
        SURFACE_HARD_SLIPPERY            = 0x0035, // Hard and slippery (Always has fall damage)
        SURFACE_HARD_VERY_SLIPPERY       = 0x0036, // Hard and very slippery (Always has fall damage)
        SURFACE_HARD_NOT_SLIPPERY        = 0x0037, // Hard and Non-slippery (Always has fall damage)
        SURFACE_VERTICAL_WIND            = 0x0038, // Death at bottom with vertical wind
        SURFACE_BOSS_FIGHT_CAMERA        = 0x0065, // Wide camera for BOB and WF bosses
        SURFACE_CAMERA_FREE_ROAM         = 0x0066, // Free roam camera for THI and TTC
        SURFACE_THI3_WALLKICK            = 0x0068, // Surface where there's a wall kick section in THI 3rd area, has no action defined
        SURFACE_CAMERA_8_DIR             = 0x0069, // Surface that enables far camera for platforms, used in THI
        SURFACE_CAMERA_MIDDLE            = 0x006E, // Surface camera that returns to the middle, used on the 4 pillars of SSL
        SURFACE_CAMERA_ROTATE_RIGHT      = 0x006F, // Surface camera that rotates to the right (Bowser 1 & THI)
        SURFACE_CAMERA_ROTATE_LEFT       = 0x0070, // Surface camera that rotates to the left (BOB & TTM)
        SURFACE_CAMERA_BOUNDARY          = 0x0072, // Intangible Area, only used to restrict camera movement
        SURFACE_NOISE_VERY_SLIPPERY_73   = 0x0073, // Very slippery floor with noise, unused
        SURFACE_NOISE_VERY_SLIPPERY_74   = 0x0074, // Very slippery floor with noise, unused
        SURFACE_NOISE_VERY_SLIPPERY      = 0x0075, // Very slippery floor with noise, used in CCM
        SURFACE_NO_CAM_COLLISION         = 0x0076, // Surface with no cam collision flag
        SURFACE_NO_CAM_COLLISION_77      = 0x0077, // Surface with no cam collision flag, unused
        SURFACE_NO_CAM_COL_VERY_SLIPPERY = 0x0078, // Surface with no cam collision flag, very slippery with noise (THI)
        SURFACE_NO_CAM_COL_SLIPPERY      = 0x0079, // Surface with no cam collision flag, slippery with noise (CCM, PSS and TTM slides)
        SURFACE_SWITCH                   = 0x007A, // Surface with no cam collision flag, non-slippery with noise, used by switches and Dorrie
        SURFACE_VANISH_CAP_WALLS         = 0x007B, // Vanish cap walls, pass through them with Vanish Cap
        SURFACE_PAINTING_WOBBLE_A6       = 0x00A6, // Painting wobble (BOB Left)
        SURFACE_PAINTING_WOBBLE_A7       = 0x00A7, // Painting wobble (BOB Middle)
        SURFACE_PAINTING_WOBBLE_A8       = 0x00A8, // Painting wobble (BOB Right)
        SURFACE_PAINTING_WOBBLE_A9       = 0x00A9, // Painting wobble (CCM Left)
        SURFACE_PAINTING_WOBBLE_AA       = 0x00AA, // Painting wobble (CCM Middle)
        SURFACE_PAINTING_WOBBLE_AB       = 0x00AB, // Painting wobble (CCM Right)
        SURFACE_PAINTING_WOBBLE_AC       = 0x00AC, // Painting wobble (WF Left)
        SURFACE_PAINTING_WOBBLE_AD       = 0x00AD, // Painting wobble (WF Middle)
        SURFACE_PAINTING_WOBBLE_AE       = 0x00AE, // Painting wobble (WF Right)
        SURFACE_PAINTING_WOBBLE_AF       = 0x00AF, // Painting wobble (JRB Left)
        SURFACE_PAINTING_WOBBLE_B0       = 0x00B0, // Painting wobble (JRB Middle)
        SURFACE_PAINTING_WOBBLE_B1       = 0x00B1, // Painting wobble (JRB Right)
        SURFACE_PAINTING_WOBBLE_B2       = 0x00B2, // Painting wobble (LLL Left)
        SURFACE_PAINTING_WOBBLE_B3       = 0x00B3, // Painting wobble (LLL Middle)
        SURFACE_PAINTING_WOBBLE_B4       = 0x00B4, // Painting wobble (LLL Right)
        SURFACE_PAINTING_WOBBLE_B5       = 0x00B5, // Painting wobble (SSL Left)
        SURFACE_PAINTING_WOBBLE_B6       = 0x00B6, // Painting wobble (SSL Middle)
        SURFACE_PAINTING_WOBBLE_B7       = 0x00B7, // Painting wobble (SSL Right)
        SURFACE_PAINTING_WOBBLE_B8       = 0x00B8, // Painting wobble (Unused - Left)
        SURFACE_PAINTING_WOBBLE_B9       = 0x00B9, // Painting wobble (Unused - Middle)
        SURFACE_PAINTING_WOBBLE_BA       = 0x00BA, // Painting wobble (Unused - Right)
        SURFACE_PAINTING_WOBBLE_BB       = 0x00BB, // Painting wobble (DDD - Left), makes the painting wobble if touched
        SURFACE_PAINTING_WOBBLE_BC       = 0x00BC, // Painting wobble (Unused, DDD - Middle)
        SURFACE_PAINTING_WOBBLE_BD       = 0x00BD, // Painting wobble (Unused, DDD - Right)
        SURFACE_PAINTING_WOBBLE_BE       = 0x00BE, // Painting wobble (WDW Left)
        SURFACE_PAINTING_WOBBLE_BF       = 0x00BF, // Painting wobble (WDW Middle)
        SURFACE_PAINTING_WOBBLE_C0       = 0x00C0, // Painting wobble (WDW Right)
        SURFACE_PAINTING_WOBBLE_C1       = 0x00C1, // Painting wobble (THI Tiny - Left)
        SURFACE_PAINTING_WOBBLE_C2       = 0x00C2, // Painting wobble (THI Tiny - Middle)
        SURFACE_PAINTING_WOBBLE_C3       = 0x00C3, // Painting wobble (THI Tiny - Right)
        SURFACE_PAINTING_WOBBLE_C4       = 0x00C4, // Painting wobble (TTM Left)
        SURFACE_PAINTING_WOBBLE_C5       = 0x00C5, // Painting wobble (TTM Middle)
        SURFACE_PAINTING_WOBBLE_C6       = 0x00C6, // Painting wobble (TTM Right)
        SURFACE_PAINTING_WOBBLE_C7       = 0x00C7, // Painting wobble (Unused, TTC - Left)
        SURFACE_PAINTING_WOBBLE_C8       = 0x00C8, // Painting wobble (Unused, TTC - Middle)
        SURFACE_PAINTING_WOBBLE_C9       = 0x00C9, // Painting wobble (Unused, TTC - Right)
        SURFACE_PAINTING_WOBBLE_CA       = 0x00CA, // Painting wobble (Unused, SL - Left)
        SURFACE_PAINTING_WOBBLE_CB       = 0x00CB, // Painting wobble (Unused, SL - Middle)
        SURFACE_PAINTING_WOBBLE_CC       = 0x00CC, // Painting wobble (Unused, SL - Right)
        SURFACE_PAINTING_WOBBLE_CD       = 0x00CD, // Painting wobble (THI Huge - Left)
        SURFACE_PAINTING_WOBBLE_CE       = 0x00CE, // Painting wobble (THI Huge - Middle)
        SURFACE_PAINTING_WOBBLE_CF       = 0x00CF, // Painting wobble (THI Huge - Right)
        SURFACE_PAINTING_WOBBLE_D0       = 0x00D0, // Painting wobble (HMC & COTMC - Left), makes the painting wobble if touched
        SURFACE_PAINTING_WOBBLE_D1       = 0x00D1, // Painting wobble (Unused, HMC & COTMC - Middle)
        SURFACE_PAINTING_WOBBLE_D2       = 0x00D2, // Painting wobble (Unused, HMC & COTMC - Right)
        SURFACE_PAINTING_WARP_D3         = 0x00D3, // Painting warp (BOB Left)
        SURFACE_PAINTING_WARP_D4         = 0x00D4, // Painting warp (BOB Middle)
        SURFACE_PAINTING_WARP_D5         = 0x00D5, // Painting warp (BOB Right)
        SURFACE_PAINTING_WARP_D6         = 0x00D6, // Painting warp (CCM Left)
        SURFACE_PAINTING_WARP_D7         = 0x00D7, // Painting warp (CCM Middle)
        SURFACE_PAINTING_WARP_D8         = 0x00D8, // Painting warp (CCM Right)
        SURFACE_PAINTING_WARP_D9         = 0x00D9, // Painting warp (WF Left)
        SURFACE_PAINTING_WARP_DA         = 0x00DA, // Painting warp (WF Middle)
        SURFACE_PAINTING_WARP_DB         = 0x00DB, // Painting warp (WF Right)
        SURFACE_PAINTING_WARP_DC         = 0x00DC, // Painting warp (JRB Left)
        SURFACE_PAINTING_WARP_DD         = 0x00DD, // Painting warp (JRB Middle)
        SURFACE_PAINTING_WARP_DE         = 0x00DE, // Painting warp (JRB Right)
        SURFACE_PAINTING_WARP_DF         = 0x00DF, // Painting warp (LLL Left)
        SURFACE_PAINTING_WARP_E0         = 0x00E0, // Painting warp (LLL Middle)
        SURFACE_PAINTING_WARP_E1         = 0x00E1, // Painting warp (LLL Right)
        SURFACE_PAINTING_WARP_E2         = 0x00E2, // Painting warp (SSL Left)
        SURFACE_PAINTING_WARP_E3         = 0x00E3, // Painting warp (SSL Medium)
        SURFACE_PAINTING_WARP_E4         = 0x00E4, // Painting warp (SSL Right)
        SURFACE_PAINTING_WARP_E5         = 0x00E5, // Painting warp (Unused - Left)
        SURFACE_PAINTING_WARP_E6         = 0x00E6, // Painting warp (Unused - Medium)
        SURFACE_PAINTING_WARP_E7         = 0x00E7, // Painting warp (Unused - Right)
        SURFACE_PAINTING_WARP_E8         = 0x00E8, // Painting warp (DDD - Left)
        SURFACE_PAINTING_WARP_E9         = 0x00E9, // Painting warp (DDD - Middle)
        SURFACE_PAINTING_WARP_EA         = 0x00EA, // Painting warp (DDD - Right)
        SURFACE_PAINTING_WARP_EB         = 0x00EB, // Painting warp (WDW Left)
        SURFACE_PAINTING_WARP_EC         = 0x00EC, // Painting warp (WDW Middle)
        SURFACE_PAINTING_WARP_ED         = 0x00ED, // Painting warp (WDW Right)
        SURFACE_PAINTING_WARP_EE         = 0x00EE, // Painting warp (THI Tiny - Left)
        SURFACE_PAINTING_WARP_EF         = 0x00EF, // Painting warp (THI Tiny - Middle)
        SURFACE_PAINTING_WARP_F0         = 0x00F0, // Painting warp (THI Tiny - Right)
        SURFACE_PAINTING_WARP_F1         = 0x00F1, // Painting warp (TTM Left)
        SURFACE_PAINTING_WARP_F2         = 0x00F2, // Painting warp (TTM Middle)
        SURFACE_PAINTING_WARP_F3         = 0x00F3, // Painting warp (TTM Right)
        SURFACE_TTC_PAINTING_1           = 0x00F4, // Painting warp (TTC Left)
        SURFACE_TTC_PAINTING_2           = 0x00F5, // Painting warp (TTC Medium)
        SURFACE_TTC_PAINTING_3           = 0x00F6, // Painting warp (TTC Right)
        SURFACE_PAINTING_WARP_F7         = 0x00F7, // Painting warp (SL Left)
        SURFACE_PAINTING_WARP_F8         = 0x00F8, // Painting warp (SL Middle)
        SURFACE_PAINTING_WARP_F9         = 0x00F9, // Painting warp (SL Right)
        SURFACE_PAINTING_WARP_FA         = 0x00FA, // Painting warp (THI Tiny - Left)
        SURFACE_PAINTING_WARP_FB         = 0x00FB, // Painting warp (THI Tiny - Middle)
        SURFACE_PAINTING_WARP_FC         = 0x00FC, // Painting warp (THI Tiny - Right)
        SURFACE_WOBBLING_WARP            = 0x00FD, // Pool warp (HMC & DDD)
        SURFACE_TRAPDOOR                 = 0x00FF, // Bowser Left trapdoor, has no action defined
    };

    enum TerrainType
    {
        TERRAIN_GRASS  = 0x0000,
        TERRAIN_STONE  = 0x0001,
        TERRAIN_SNOW   = 0x0002,
        TERRAIN_SAND   = 0x0003,
        TERRAIN_SPOOKY = 0x0004,
        TERRAIN_WATER  = 0x0005,
        TERRAIN_SLIDE  = 0x0006,
        TERRAIN_MASK   = 0x0007,
    };

    // ParticleFlags, MarioFlags, ActionFlags are from "libsm64/src/decomp/include/sm64.h"

    enum ParticleFlags
    {
        PARTICLE_DUST                 /* 0x00000001 */ = (1 <<  0),
        PARTICLE_VERTICAL_STAR        /* 0x00000002 */ = (1 <<  1),
        PARTICLE_2                    /* 0x00000004 */ = (1 <<  2),
        PARTICLE_SPARKLES             /* 0x00000008 */ = (1 <<  3),
        PARTICLE_HORIZONTAL_STAR      /* 0x00000010 */ = (1 <<  4),
        PARTICLE_BUBBLE               /* 0x00000020 */ = (1 <<  5),
        PARTICLE_WATER_SPLASH         /* 0x00000040 */ = (1 <<  6),
        PARTICLE_IDLE_WATER_WAVE      /* 0x00000080 */ = (1 <<  7),
        PARTICLE_SHALLOW_WATER_WAVE   /* 0x00000100 */ = (1 <<  8),
        PARTICLE_PLUNGE_BUBBLE        /* 0x00000200 */ = (1 <<  9),
        PARTICLE_WAVE_TRAIL           /* 0x00000400 */ = (1 << 10),
        PARTICLE_FIRE                 /* 0x00000800 */ = (1 << 11),
        PARTICLE_SHALLOW_WATER_SPLASH /* 0x00001000 */ = (1 << 12),
        PARTICLE_LEAF                 /* 0x00002000 */ = (1 << 13),
        PARTICLE_SNOW                 /* 0x00004000 */ = (1 << 14),
        PARTICLE_DIRT                 /* 0x00008000 */ = (1 << 15),
        PARTICLE_MIST_CIRCLE          /* 0x00010000 */ = (1 << 16),
        PARTICLE_BREATH               /* 0x00020000 */ = (1 << 17),
        PARTICLE_TRIANGLE             /* 0x00040000 */ = (1 << 18),
        PARTICLE_19                   /* 0x00080000 */ = (1 << 19),
    };

    enum MarioFlags
    {
        MARIO_NORMAL_CAP          = 0x00000001,
        MARIO_VANISH_CAP          = 0x00000002,
        MARIO_METAL_CAP           = 0x00000004,
        MARIO_WING_CAP            = 0x00000008,
        MARIO_CAP_ON_HEAD         = 0x00000010,
        MARIO_CAP_IN_HAND         = 0x00000020,
        MARIO_METAL_SHOCK         = 0x00000040,
        MARIO_TELEPORTING         = 0x00000080,
        MARIO_UNKNOWN_08          = 0x00000100,
        MARIO_UNKNOWN_13          = 0x00002000,
        MARIO_ACTION_SOUND_PLAYED = 0x00010000,
        MARIO_MARIO_SOUND_PLAYED  = 0x00020000,
        MARIO_UNKNOWN_18          = 0x00040000,
        MARIO_PUNCHING            = 0x00100000,
        MARIO_KICKING             = 0x00200000,
        MARIO_TRIPPING            = 0x00400000,
        MARIO_UNKNOWN_25          = 0x02000000,
        MARIO_UNKNOWN_30          = 0x40000000,
        MARIO_UNKNOWN_31          = 0x80000000,

        MARIO_SPECIAL_CAPS        = (MARIO_VANISH_CAP | MARIO_METAL_CAP | MARIO_WING_CAP),
        MARIO_CAPS                = (MARIO_NORMAL_CAP | MARIO_SPECIAL_CAPS),
    };

    enum ActionFlags
    {
        ACT_ID_MASK = 0x000001FF,

        ACT_GROUP_MASK                        = 0x000001C0,
        ACT_GROUP_STATIONARY /* 0x00000000 */ = (0 << 6),
        ACT_GROUP_MOVING     /* 0x00000040 */ = (1 << 6),
        ACT_GROUP_AIRBORNE   /* 0x00000080 */ = (2 << 6),
        ACT_GROUP_SUBMERGED  /* 0x000000C0 */ = (3 << 6),
        ACT_GROUP_CUTSCENE   /* 0x00000100 */ = (4 << 6),
        ACT_GROUP_AUTOMATIC  /* 0x00000140 */ = (5 << 6),
        ACT_GROUP_OBJECT     /* 0x00000180 */ = (6 << 6),

        ACT_FLAG_STATIONARY                  /* 0x00000200 */ = (1 <<  9),
        ACT_FLAG_MOVING                      /* 0x00000400 */ = (1 << 10),
        ACT_FLAG_AIR                         /* 0x00000800 */ = (1 << 11),
        ACT_FLAG_INTANGIBLE                  /* 0x00001000 */ = (1 << 12),
        ACT_FLAG_SWIMMING                    /* 0x00002000 */ = (1 << 13),
        ACT_FLAG_METAL_WATER                 /* 0x00004000 */ = (1 << 14),
        ACT_FLAG_SHORT_HITBOX                /* 0x00008000 */ = (1 << 15),
        ACT_FLAG_RIDING_SHELL                /* 0x00010000 */ = (1 << 16),
        ACT_FLAG_INVULNERABLE                /* 0x00020000 */ = (1 << 17),
        ACT_FLAG_BUTT_OR_STOMACH_SLIDE       /* 0x00040000 */ = (1 << 18),
        ACT_FLAG_DIVING                      /* 0x00080000 */ = (1 << 19),
        ACT_FLAG_ON_POLE                     /* 0x00100000 */ = (1 << 20),
        ACT_FLAG_HANGING                     /* 0x00200000 */ = (1 << 21),
        ACT_FLAG_IDLE                        /* 0x00400000 */ = (1 << 22),
        ACT_FLAG_ATTACKING                   /* 0x00800000 */ = (1 << 23),
        ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION  /* 0x01000000 */ = (1 << 24),
        ACT_FLAG_CONTROL_JUMP_HEIGHT         /* 0x02000000 */ = (1 << 25),
        ACT_FLAG_ALLOW_FIRST_PERSON          /* 0x04000000 */ = (1 << 26),
        ACT_FLAG_PAUSE_EXIT                  /* 0x08000000 */ = (1 << 27),
        ACT_FLAG_SWIMMING_OR_FLYING          /* 0x10000000 */ = (1 << 28),
        ACT_FLAG_WATER_OR_TEXT               /* 0x20000000 */ = (1 << 29),
        ACT_FLAG_THROWING                    /* 0x80000000 */ = (1 << 31),

        ACT_UNINITIALIZED              = 0x00000000, // (0x000)

    // group 0x000: stationary actions
        ACT_IDLE                       = 0x0C400201, // (0x001 | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_START_SLEEPING             = 0x0C400202, // (0x002 | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_SLEEPING                   = 0x0C000203, // (0x003 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_WAKING_UP                  = 0x0C000204, // (0x004 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_PANTING                    = 0x0C400205, // (0x005 | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_PANTING_UNUSED        = 0x08000206, // (0x006 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_IDLE                  = 0x08000207, // (0x007 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_HEAVY_IDLE            = 0x08000208, // (0x008 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_STANDING_AGAINST_WALL      = 0x0C400209, // (0x009 | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_COUGHING                   = 0x0C40020A, // (0x00A | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_SHIVERING                  = 0x0C40020B, // (0x00B | ACT_FLAG_STATIONARY | ACT_FLAG_IDLE | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_IN_QUICKSAND               = 0x0002020D, // (0x00D | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_UNKNOWN_0002020E           = 0x0002020E, // (0x00E | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_CROUCHING                  = 0x0C008220, // (0x020 | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_START_CROUCHING            = 0x0C008221, // (0x021 | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_STOP_CROUCHING             = 0x0C008222, // (0x022 | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_START_CRAWLING             = 0x0C008223, // (0x023 | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_STOP_CRAWLING              = 0x0C008224, // (0x024 | ACT_FLAG_STATIONARY | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_SLIDE_KICK_SLIDE_STOP      = 0x08000225, // (0x025 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_SHOCKWAVE_BOUNCE           = 0x00020226, // (0x026 | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_FIRST_PERSON               = 0x0C000227, // (0x027 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_BACKFLIP_LAND_STOP         = 0x0800022F, // (0x02F | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_JUMP_LAND_STOP             = 0x0C000230, // (0x030 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_DOUBLE_JUMP_LAND_STOP      = 0x0C000231, // (0x031 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_FREEFALL_LAND_STOP         = 0x0C000232, // (0x032 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_SIDE_FLIP_LAND_STOP        = 0x0C000233, // (0x033 | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_JUMP_LAND_STOP        = 0x08000234, // (0x034 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_FREEFALL_LAND_STOP    = 0x08000235, // (0x035 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_AIR_THROW_LAND             = 0x80000A36, // (0x036 | ACT_FLAG_STATIONARY | ACT_FLAG_AIR | ACT_FLAG_THROWING)
        ACT_TWIRL_LAND                 = 0x18800238, // (0x038 | ACT_FLAG_STATIONARY | ACT_FLAG_ATTACKING | ACT_FLAG_PAUSE_EXIT | ACT_FLAG_SWIMMING_OR_FLYING)
        ACT_LAVA_BOOST_LAND            = 0x08000239, // (0x039 | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_TRIPLE_JUMP_LAND_STOP      = 0x0800023A, // (0x03A | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_LONG_JUMP_LAND_STOP        = 0x0800023B, // (0x03B | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_GROUND_POUND_LAND          = 0x0080023C, // (0x03C | ACT_FLAG_STATIONARY | ACT_FLAG_ATTACKING)
        ACT_BRAKING_STOP               = 0x0C00023D, // (0x03D | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_BUTT_SLIDE_STOP            = 0x0C00023E, // (0x03E | ACT_FLAG_STATIONARY | ACT_FLAG_ALLOW_FIRST_PERSON | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_BUTT_SLIDE_STOP       = 0x0800043F, // (0x03F | ACT_FLAG_MOVING | ACT_FLAG_PAUSE_EXIT)

    // group 0x040: moving (ground) actions
        ACT_WALKING                    = 0x04000440, // (0x040 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_HOLD_WALKING               = 0x00000442, // (0x042 | ACT_FLAG_MOVING)
        ACT_TURNING_AROUND             = 0x00000443, // (0x043 | ACT_FLAG_MOVING)
        ACT_FINISH_TURNING_AROUND      = 0x00000444, // (0x044 | ACT_FLAG_MOVING)
        ACT_BRAKING                    = 0x04000445, // (0x045 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_RIDING_SHELL_GROUND        = 0x20810446, // (0x046 | ACT_FLAG_MOVING | ACT_FLAG_RIDING_SHELL | ACT_FLAG_ATTACKING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_HEAVY_WALKING         = 0x00000447, // (0x047 | ACT_FLAG_MOVING)
        ACT_CRAWLING                   = 0x04008448, // (0x048 | ACT_FLAG_MOVING | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_BURNING_GROUND             = 0x00020449, // (0x049 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_DECELERATING               = 0x0400044A, // (0x04A | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_HOLD_DECELERATING          = 0x0000044B, // (0x04B | ACT_FLAG_MOVING)
        ACT_BEGIN_SLIDING              = 0x00000050, // (0x050)
        ACT_HOLD_BEGIN_SLIDING         = 0x00000051, // (0x051)
        ACT_BUTT_SLIDE                 = 0x00840452, // (0x052 | ACT_FLAG_MOVING | ACT_FLAG_BUTT_OR_STOMACH_SLIDE | ACT_FLAG_ATTACKING)
        ACT_STOMACH_SLIDE              = 0x008C0453, // (0x053 | ACT_FLAG_MOVING | ACT_FLAG_BUTT_OR_STOMACH_SLIDE | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING)
        ACT_HOLD_BUTT_SLIDE            = 0x00840454, // (0x054 | ACT_FLAG_MOVING | ACT_FLAG_BUTT_OR_STOMACH_SLIDE | ACT_FLAG_ATTACKING)
        ACT_HOLD_STOMACH_SLIDE         = 0x008C0455, // (0x055 | ACT_FLAG_MOVING | ACT_FLAG_BUTT_OR_STOMACH_SLIDE | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING)
        ACT_DIVE_SLIDE                 = 0x00880456, // (0x056 | ACT_FLAG_MOVING | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING)
        ACT_MOVE_PUNCHING              = 0x00800457, // (0x057 | ACT_FLAG_MOVING | ACT_FLAG_ATTACKING)
        ACT_CROUCH_SLIDE               = 0x04808459, // (0x059 | ACT_FLAG_MOVING | ACT_FLAG_SHORT_HITBOX | ACT_FLAG_ATTACKING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_SLIDE_KICK_SLIDE           = 0x0080045A, // (0x05A | ACT_FLAG_MOVING | ACT_FLAG_ATTACKING)
        ACT_HARD_BACKWARD_GROUND_KB    = 0x00020460, // (0x060 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_HARD_FORWARD_GROUND_KB     = 0x00020461, // (0x061 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_BACKWARD_GROUND_KB         = 0x00020462, // (0x062 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_FORWARD_GROUND_KB          = 0x00020463, // (0x063 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_SOFT_BACKWARD_GROUND_KB    = 0x00020464, // (0x064 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_SOFT_FORWARD_GROUND_KB     = 0x00020465, // (0x065 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_GROUND_BONK                = 0x00020466, // (0x066 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_DEATH_EXIT_LAND            = 0x00020467, // (0x067 | ACT_FLAG_MOVING | ACT_FLAG_INVULNERABLE)
        ACT_JUMP_LAND                  = 0x04000470, // (0x070 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_FREEFALL_LAND              = 0x04000471, // (0x071 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_DOUBLE_JUMP_LAND           = 0x04000472, // (0x072 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_SIDE_FLIP_LAND             = 0x04000473, // (0x073 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_HOLD_JUMP_LAND             = 0x00000474, // (0x074 | ACT_FLAG_MOVING)
        ACT_HOLD_FREEFALL_LAND         = 0x00000475, // (0x075 | ACT_FLAG_MOVING)
        ACT_QUICKSAND_JUMP_LAND        = 0x00000476, // (0x076 | ACT_FLAG_MOVING)
        ACT_HOLD_QUICKSAND_JUMP_LAND   = 0x00000477, // (0x077 | ACT_FLAG_MOVING)
        ACT_TRIPLE_JUMP_LAND           = 0x04000478, // (0x078 | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_LONG_JUMP_LAND             = 0x00000479, // (0x079 | ACT_FLAG_MOVING)
        ACT_BACKFLIP_LAND              = 0x0400047A, // (0x07A | ACT_FLAG_MOVING | ACT_FLAG_ALLOW_FIRST_PERSON)

    // group 0x080: airborne actions
        ACT_JUMP                       = 0x03000880, // (0x080 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_DOUBLE_JUMP                = 0x03000881, // (0x081 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_TRIPLE_JUMP                = 0x01000882, // (0x082 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_BACKFLIP                   = 0x01000883, // (0x083 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_STEEP_JUMP                 = 0x03000885, // (0x085 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_WALL_KICK_AIR              = 0x03000886, // (0x086 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_SIDE_FLIP                  = 0x01000887, // (0x087 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_LONG_JUMP                  = 0x03000888, // (0x088 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_WATER_JUMP                 = 0x01000889, // (0x089 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_DIVE                       = 0x0188088A, // (0x08A | ACT_FLAG_AIR | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_FREEFALL                   = 0x0100088C, // (0x08C | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_TOP_OF_POLE_JUMP           = 0x0300088D, // (0x08D | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_BUTT_SLIDE_AIR             = 0x0300088E, // (0x08E | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_FLYING_TRIPLE_JUMP         = 0x03000894, // (0x094 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_SHOT_FROM_CANNON           = 0x00880898, // (0x098 | ACT_FLAG_AIR | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING)
        ACT_FLYING                     = 0x10880899, // (0x099 | ACT_FLAG_AIR | ACT_FLAG_DIVING | ACT_FLAG_ATTACKING | ACT_FLAG_SWIMMING_OR_FLYING)
        ACT_RIDING_SHELL_JUMP          = 0x0281089A, // (0x09A | ACT_FLAG_AIR | ACT_FLAG_RIDING_SHELL | ACT_FLAG_ATTACKING | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_RIDING_SHELL_FALL          = 0x0081089B, // (0x09B | ACT_FLAG_AIR | ACT_FLAG_RIDING_SHELL | ACT_FLAG_ATTACKING)
        ACT_VERTICAL_WIND              = 0x1008089C, // (0x09C | ACT_FLAG_AIR | ACT_FLAG_DIVING | ACT_FLAG_SWIMMING_OR_FLYING)
        ACT_HOLD_JUMP                  = 0x030008A0, // (0x0A0 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_HOLD_FREEFALL              = 0x010008A1, // (0x0A1 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_HOLD_BUTT_SLIDE_AIR        = 0x010008A2, // (0x0A2 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_HOLD_WATER_JUMP            = 0x010008A3, // (0x0A3 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_TWIRLING                   = 0x108008A4, // (0x0A4 | ACT_FLAG_AIR | ACT_FLAG_ATTACKING | ACT_FLAG_SWIMMING_OR_FLYING)
        ACT_FORWARD_ROLLOUT            = 0x010008A6, // (0x0A6 | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_AIR_HIT_WALL               = 0x000008A7, // (0x0A7 | ACT_FLAG_AIR)
        ACT_RIDING_HOOT                = 0x000004A8, // (0x0A8 | ACT_FLAG_MOVING)
        ACT_GROUND_POUND               = 0x008008A9, // (0x0A9 | ACT_FLAG_AIR | ACT_FLAG_ATTACKING)
        ACT_SLIDE_KICK                 = 0x018008AA, // (0x0AA | ACT_FLAG_AIR | ACT_FLAG_ATTACKING | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_AIR_THROW                  = 0x830008AB, // (0x0AB | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT | ACT_FLAG_THROWING)
        ACT_JUMP_KICK                  = 0x018008AC, // (0x0AC | ACT_FLAG_AIR | ACT_FLAG_ATTACKING | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_BACKWARD_ROLLOUT           = 0x010008AD, // (0x0AD | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_CRAZY_BOX_BOUNCE           = 0x000008AE, // (0x0AE | ACT_FLAG_AIR)
        ACT_SPECIAL_TRIPLE_JUMP        = 0x030008AF, // (0x0AF | ACT_FLAG_AIR | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION | ACT_FLAG_CONTROL_JUMP_HEIGHT)
        ACT_BACKWARD_AIR_KB            = 0x010208B0, // (0x0B0 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_FORWARD_AIR_KB             = 0x010208B1, // (0x0B1 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_HARD_FORWARD_AIR_KB        = 0x010208B2, // (0x0B2 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_HARD_BACKWARD_AIR_KB       = 0x010208B3, // (0x0B3 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_BURNING_JUMP               = 0x010208B4, // (0x0B4 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_BURNING_FALL               = 0x010208B5, // (0x0B5 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_SOFT_BONK                  = 0x010208B6, // (0x0B6 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_LAVA_BOOST                 = 0x010208B7, // (0x0B7 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_GETTING_BLOWN              = 0x010208B8, // (0x0B8 | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_THROWN_FORWARD             = 0x010208BD, // (0x0BD | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_ACTION)
        ACT_THROWN_BACKWARD            = 0x010208BE, // (0x0BE | ACT_FLAG_AIR | ACT_FLAG_INVULNERABLE | ACT_FLAG_ALLOW_VERTICAL_WIND_

    // group 0x0C0: submerged actions
        ACT_WATER_IDLE                 = 0x380022C0, // (0x0C0 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_PAUSE_EXIT | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_WATER_IDLE            = 0x380022C1, // (0x0C1 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_PAUSE_EXIT | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_ACTION_END           = 0x300022C2, // (0x0C2 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_WATER_ACTION_END      = 0x300022C3, // (0x0C3 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_DROWNING                   = 0x300032C4, // (0x0C4 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_BACKWARD_WATER_KB          = 0x300222C5, // (0x0C5 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_FORWARD_WATER_KB           = 0x300222C6, // (0x0C6 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_DEATH                = 0x300032C7, // (0x0C7 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_SHOCKED              = 0x300222C8, // (0x0C8 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_BREASTSTROKE               = 0x300024D0, // (0x0D0 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_SWIMMING_END               = 0x300024D1, // (0x0D1 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_FLUTTER_KICK               = 0x300024D2, // (0x0D2 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_BREASTSTROKE          = 0x300024D3, // (0x0D3 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_SWIMMING_END          = 0x300024D4, // (0x0D4 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_HOLD_FLUTTER_KICK          = 0x300024D5, // (0x0D5 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_SHELL_SWIMMING       = 0x300024D6, // (0x0D6 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_THROW                = 0x300024E0, // (0x0E0 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_PUNCH                = 0x300024E1, // (0x0E1 | ACT_FLAG_MOVING | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_WATER_PLUNGE               = 0x300022E2, // (0x0E2 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_CAUGHT_IN_WHIRLPOOL        = 0x300222E3, // (0x0E3 | ACT_FLAG_STATIONARY | ACT_FLAG_SWIMMING | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING_OR_FLYING | ACT_FLAG_WATER_OR_TEXT)
        ACT_METAL_WATER_STANDING       = 0x080042F0, // (0x0F0 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER | ACT_FLAG_PAUSE_EXIT)
        ACT_HOLD_METAL_WATER_STANDING  = 0x080042F1, // (0x0F1 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER | ACT_FLAG_PAUSE_EXIT)
        ACT_METAL_WATER_WALKING        = 0x000044F2, // (0x0F2 | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)
        ACT_HOLD_METAL_WATER_WALKING   = 0x000044F3, // (0x0F3 | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)
        ACT_METAL_WATER_FALLING        = 0x000042F4, // (0x0F4 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER)
        ACT_HOLD_METAL_WATER_FALLING   = 0x000042F5, // (0x0F5 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER)
        ACT_METAL_WATER_FALL_LAND      = 0x000042F6, // (0x0F6 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER)
        ACT_HOLD_METAL_WATER_FALL_LAND = 0x000042F7, // (0x0F7 | ACT_FLAG_STATIONARY | ACT_FLAG_METAL_WATER)
        ACT_METAL_WATER_JUMP           = 0x000044F8, // (0x0F8 | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)
        ACT_HOLD_METAL_WATER_JUMP      = 0x000044F9, // (0x0F9 | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)
        ACT_METAL_WATER_JUMP_LAND      = 0x000044FA, // (0x0FA | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)
        ACT_HOLD_METAL_WATER_JUMP_LAND = 0x000044FB, // (0x0FB | ACT_FLAG_MOVING | ACT_FLAG_METAL_WATER)

    // group 0x100: cutscene actions
        ACT_DISAPPEARED                = 0x00001300, // (0x100 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_INTRO_CUTSCENE             = 0x04001301, // (0x101 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_ALLOW_FIRST_PERSON)
        ACT_STAR_DANCE_EXIT            = 0x00001302, // (0x102 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_STAR_DANCE_WATER           = 0x00001303, // (0x103 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_FALL_AFTER_STAR_GRAB       = 0x00001904, // (0x104 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_READING_AUTOMATIC_DIALOG   = 0x20001305, // (0x105 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_WATER_OR_TEXT)
        ACT_READING_NPC_DIALOG         = 0x20001306, // (0x106 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_WATER_OR_TEXT)
        ACT_STAR_DANCE_NO_EXIT         = 0x00001307, // (0x107 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_READING_SIGN               = 0x00001308, // (0x108 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_JUMBO_STAR_CUTSCENE        = 0x00001909, // (0x109 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_WAITING_FOR_DIALOG         = 0x0000130A, // (0x10A | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_DEBUG_FREE_MOVE            = 0x0000130F, // (0x10F | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_STANDING_DEATH             = 0x00021311, // (0x111 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_QUICKSAND_DEATH            = 0x00021312, // (0x112 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_ELECTROCUTION              = 0x00021313, // (0x113 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_SUFFOCATION                = 0x00021314, // (0x114 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_DEATH_ON_STOMACH           = 0x00021315, // (0x115 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_DEATH_ON_BACK              = 0x00021316, // (0x116 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_EATEN_BY_BUBBA             = 0x00021317, // (0x117 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE | ACT_FLAG_INVULNERABLE)
        ACT_END_PEACH_CUTSCENE         = 0x00001918, // (0x118 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_CREDITS_CUTSCENE           = 0x00001319, // (0x119 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_END_WAVING_CUTSCENE        = 0x0000131A, // (0x11A | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_PULLING_DOOR               = 0x00001320, // (0x120 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_PUSHING_DOOR               = 0x00001321, // (0x121 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_WARP_DOOR_SPAWN            = 0x00001322, // (0x122 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_EMERGE_FROM_PIPE           = 0x00001923, // (0x123 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_SPAWN_SPIN_AIRBORNE        = 0x00001924, // (0x124 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_SPAWN_SPIN_LANDING         = 0x00001325, // (0x125 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_EXIT_AIRBORNE              = 0x00001926, // (0x126 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_EXIT_LAND_SAVE_DIALOG      = 0x00001327, // (0x127 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_DEATH_EXIT                 = 0x00001928, // (0x128 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_UNUSED_DEATH_EXIT          = 0x00001929, // (0x129 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_FALLING_DEATH_EXIT         = 0x0000192A, // (0x12A | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_SPECIAL_EXIT_AIRBORNE      = 0x0000192B, // (0x12B | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_SPECIAL_DEATH_EXIT         = 0x0000192C, // (0x12C | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_FALLING_EXIT_AIRBORNE      = 0x0000192D, // (0x12D | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_UNLOCKING_KEY_DOOR         = 0x0000132E, // (0x12E | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_UNLOCKING_STAR_DOOR        = 0x0000132F, // (0x12F | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_ENTERING_STAR_DOOR         = 0x00001331, // (0x131 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_SPAWN_NO_SPIN_AIRBORNE     = 0x00001932, // (0x132 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_SPAWN_NO_SPIN_LANDING      = 0x00001333, // (0x133 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_BBH_ENTER_JUMP             = 0x00001934, // (0x134 | ACT_FLAG_AIR | ACT_FLAG_INTANGIBLE)
        ACT_BBH_ENTER_SPIN             = 0x00001535, // (0x135 | ACT_FLAG_MOVING | ACT_FLAG_INTANGIBLE)
        ACT_TELEPORT_FADE_OUT          = 0x00001336, // (0x136 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_TELEPORT_FADE_IN           = 0x00001337, // (0x137 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_SHOCKED                    = 0x00020338, // (0x138 | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_SQUISHED                   = 0x00020339, // (0x139 | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_HEAD_STUCK_IN_GROUND       = 0x0002033A, // (0x13A | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_BUTT_STUCK_IN_GROUND       = 0x0002033B, // (0x13B | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_FEET_STUCK_IN_GROUND       = 0x0002033C, // (0x13C | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_PUTTING_ON_CAP             = 0x0000133D, // (0x13D | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)

    // group 0x140: "automatic" actions
        ACT_HOLDING_POLE               = 0x08100340, // (0x140 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE | ACT_FLAG_PAUSE_EXIT)
        ACT_GRAB_POLE_SLOW             = 0x00100341, // (0x141 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE)
        ACT_GRAB_POLE_FAST             = 0x00100342, // (0x142 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE)
        ACT_CLIMBING_POLE              = 0x00100343, // (0x143 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE)
        ACT_TOP_OF_POLE_TRANSITION     = 0x00100344, // (0x144 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE)
        ACT_TOP_OF_POLE                = 0x00100345, // (0x145 | ACT_FLAG_STATIONARY | ACT_FLAG_ON_POLE)
        ACT_START_HANGING              = 0x08200348, // (0x148 | ACT_FLAG_STATIONARY | ACT_FLAG_HANGING | ACT_FLAG_PAUSE_EXIT)
        ACT_HANGING                    = 0x00200349, // (0x149 | ACT_FLAG_STATIONARY | ACT_FLAG_HANGING)
        ACT_HANG_MOVING                = 0x0020054A, // (0x14A | ACT_FLAG_MOVING | ACT_FLAG_HANGING)
        ACT_LEDGE_GRAB                 = 0x0800034B, // (0x14B | ACT_FLAG_STATIONARY | ACT_FLAG_PAUSE_EXIT)
        ACT_LEDGE_CLIMB_SLOW_1         = 0x0000054C, // (0x14C | ACT_FLAG_MOVING)
        ACT_LEDGE_CLIMB_SLOW_2         = 0x0000054D, // (0x14D | ACT_FLAG_MOVING)
        ACT_LEDGE_CLIMB_DOWN           = 0x0000054E, // (0x14E | ACT_FLAG_MOVING)
        ACT_LEDGE_CLIMB_FAST           = 0x0000054F, // (0x14F | ACT_FLAG_MOVING)
        ACT_GRABBED                    = 0x00020370, // (0x170 | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE)
        ACT_IN_CANNON                  = 0x00001371, // (0x171 | ACT_FLAG_STATIONARY | ACT_FLAG_INTANGIBLE)
        ACT_TORNADO_TWIRLING           = 0x10020372, // (0x172 | ACT_FLAG_STATIONARY | ACT_FLAG_INVULNERABLE | ACT_FLAG_SWIMMING_OR_FLYING)

    // group 0x180: object actions
        ACT_PUNCHING                   = 0x00800380, // (0x180 | ACT_FLAG_STATIONARY | ACT_FLAG_ATTACKING)
        ACT_PICKING_UP                 = 0x00000383, // (0x183 | ACT_FLAG_STATIONARY)
        ACT_DIVE_PICKING_UP            = 0x00000385, // (0x185 | ACT_FLAG_STATIONARY)
        ACT_STOMACH_SLIDE_STOP         = 0x00000386, // (0x186 | ACT_FLAG_STATIONARY)
        ACT_PLACING_DOWN               = 0x00000387, // (0x187 | ACT_FLAG_STATIONARY)
        ACT_THROWING                   = 0x80000588, // (0x188 | ACT_FLAG_MOVING | ACT_FLAG_THROWING)
        ACT_HEAVY_THROW                = 0x80000589, // (0x189 | ACT_FLAG_MOVING | ACT_FLAG_THROWING)
        ACT_PICKING_UP_BOWSER          = 0x00000390, // (0x190 | ACT_FLAG_STATIONARY)
        ACT_HOLDING_BOWSER             = 0x00000391, // (0x191 | ACT_FLAG_STATIONARY)
        ACT_RELEASING_BOWSER           = 0x00000392, // (0x192 | ACT_FLAG_STATIONARY)
    };

    enum SeqPlayer
    {
        SEQ_PLAYER_LEVEL            = 0,  // Level background music
        SEQ_PLAYER_ENV              = 1,  // Misc music like the puzzle jingle
        SEQ_PLAYER_SFX              = 2,  // Sound effects
    };

    // SeqId is from "libsm64/src/decomp/include/seq_ids.h"

    #define SEQ_VARIATION 0x80

    enum SeqId
    {
        SEQ_SOUND_PLAYER,                 // 0x00
        SEQ_EVENT_CUTSCENE_COLLECT_STAR,  // 0x01
        SEQ_MENU_TITLE_SCREEN,            // 0x02
        SEQ_LEVEL_GRASS,                  // 0x03
        SEQ_LEVEL_INSIDE_CASTLE,          // 0x04
        SEQ_LEVEL_WATER,                  // 0x05
        SEQ_LEVEL_HOT,                    // 0x06
        SEQ_LEVEL_BOSS_KOOPA,             // 0x07
        SEQ_LEVEL_SNOW,                   // 0x08
        SEQ_LEVEL_SLIDE,                  // 0x09
        SEQ_LEVEL_SPOOKY,                 // 0x0A
        SEQ_EVENT_PIRANHA_PLANT,          // 0x0B
        SEQ_LEVEL_UNDERGROUND,            // 0x0C
        SEQ_MENU_STAR_SELECT,             // 0x0D
        SEQ_EVENT_POWERUP,                // 0x0E
        SEQ_EVENT_METAL_CAP,              // 0x0F
        SEQ_EVENT_KOOPA_MESSAGE,          // 0x10
        SEQ_LEVEL_KOOPA_ROAD,             // 0x11
        SEQ_EVENT_HIGH_SCORE,             // 0x12
        SEQ_EVENT_MERRY_GO_ROUND,         // 0x13
        SEQ_EVENT_RACE,                   // 0x14
        SEQ_EVENT_CUTSCENE_STAR_SPAWN,    // 0x15
        SEQ_EVENT_BOSS,                   // 0x16
        SEQ_EVENT_CUTSCENE_COLLECT_KEY,   // 0x17
        SEQ_EVENT_ENDLESS_STAIRS,         // 0x18
        SEQ_LEVEL_BOSS_KOOPA_FINAL,       // 0x19
        SEQ_EVENT_CUTSCENE_CREDITS,       // 0x1A
        SEQ_EVENT_SOLVE_PUZZLE,           // 0x1B
        SEQ_EVENT_TOAD_MESSAGE,           // 0x1C
        SEQ_EVENT_PEACH_MESSAGE,          // 0x1D
        SEQ_EVENT_CUTSCENE_INTRO,         // 0x1E
        SEQ_EVENT_CUTSCENE_VICTORY,       // 0x1F
        SEQ_EVENT_CUTSCENE_ENDING,        // 0x20
        SEQ_MENU_FILE_SELECT,             // 0x21
        SEQ_EVENT_CUTSCENE_LAKITU,        // 0x22 (not in JP)
        SEQ_COUNT
    };

    // From "include/sounds.h" of the sm64 decomp project

    // Sound Magic Definition:
    // First Byte (Upper Nibble): Sound Bank (not the same as audio bank!)
    // First Byte (Lower Nibble): Bitflags for audio playback?
    // Second Byte: Sound ID
    // Third Byte: Priority
    // Fourth Byte (Upper Nibble): More bitflags
    // Fourth Byte (Lower Nibble): Sound Status (this is set to SOUND_STATUS_PLAYING when passed to the audio driver.)
    #define SOUND_ARG_LOAD(bank, soundID, priority, flags) (\
        ((uint32_t) (bank) << 28) | \
        ((uint32_t) (soundID) << 16) | \
        ((uint32_t) (priority) << 8) | \
        (flags) | \
        SOUND_STATUS_WAITING)

    #define SOUNDARGS_MASK_BANK         0xF0000000
    #define SOUNDARGS_MASK_SOUNDID      0x00FF0000
    #define SOUNDARGS_MASK_PRIORITY     0x0000FF00
    #define SOUNDARGS_MASK_STATUS       0x0000000F

    #define SOUNDARGS_SHIFT_BANK        28
    #define SOUNDARGS_SHIFT_SOUNDID     16
    #define SOUNDARGS_SHIFT_PRIORITY    8

    /* Sound banks */
    #define SOUND_BANK_ACTION     0
    #define SOUND_BANK_MOVING     1
    #define SOUND_BANK_VOICE      2
    #define SOUND_BANK_GENERAL    3
    #define SOUND_BANK_ENV        4
    #define SOUND_BANK_OBJ        5
    #define SOUND_BANK_AIR        6
    #define SOUND_BANK_MENU       7
    #define SOUND_BANK_GENERAL2   8
    #define SOUND_BANK_OBJ2       9
    #define SOUND_BANK_COUNT     10

    #define SOUND_BANKS_ALL_BITS 0xffff
    #define SOUND_BANKS_ALL ((1 << SOUND_BANK_COUNT) - 1)
    #define SOUND_BANKS_FOREGROUND (\
        (1 << SOUND_BANK_ACTION) |\
        (1 << SOUND_BANK_VOICE) |\
        (1 << SOUND_BANK_MENU))
    #define SOUND_BANKS_BACKGROUND (SOUND_BANKS_ALL & ~SOUND_BANKS_FOREGROUND)
    #define SOUND_BANKS_DISABLED_DURING_INTRO_CUTSCENE (\
        (1 << SOUND_BANK_ENV) |\
        (1 << SOUND_BANK_OBJ) |\
        (1 << SOUND_BANK_GENERAL2) |\
        (1 << SOUND_BANK_OBJ2))
    #define SOUND_BANKS_DISABLED_AFTER_CREDITS (\
        (1 << SOUND_BANK_ACTION) |\
        (1 << SOUND_BANK_MOVING) |\
        (1 << SOUND_BANK_VOICE) |\
        (1 << SOUND_BANK_GENERAL))

    /* Audio playback bitflags. */
    #define SOUND_NO_VOLUME_LOSS        0x1000000 // No volume loss with distance
    #define SOUND_VIBRATO               0x2000000 // Randomly alter frequency each audio frame
    #define SOUND_NO_PRIORITY_LOSS      0x4000000 // Do not prioritize closer sounds
    #define SOUND_CONSTANT_FREQUENCY    0x8000000 // Frequency is not affected by distance or speed. If
                                                // not set, frequency will increase with distance.
                                                // For sounds in SOUND_BANK_MOVING, frequency will
                                                // further increase with speed, and volume will
                                                // decrease at slow speeds.

    /* Audio lower bitflags. */
    #define SOUND_LOWER_BACKGROUND_MUSIC  0x10 // Lower volume of background music while playing
    #define SOUND_NO_ECHO                 0x20 // Disable level reverb. Not in JP
    #define SOUND_DISCRETE                0x80 // Every call to play_sound restarts the sound. If not
                                            // set, the sound is continuous and play_sound should be
                                            // called at least every other frame to keep it playing

    /* Audio Status */
    #define SOUND_STATUS_STOPPED        0
    #define SOUND_STATUS_WAITING        1
    #define SOUND_STATUS_PLAYING        2

    /**
     * Terrain types. mario_get_terrain_sound_addend computes a
     * sound terrain type between 0 and 7, depending on the terrain type of the
     * level and the floor type that Mario is standing on. That value is then added
     * to the sound ID for the TERRAIN_* sounds.
     */
    enum SoundTerrain
    {
        SOUND_TERRAIN_DEFAULT   = 0, // e.g. air
        SOUND_TERRAIN_GRASS     = 1,
        SOUND_TERRAIN_WATER     = 2,
        SOUND_TERRAIN_STONE     = 3,
        SOUND_TERRAIN_SPOOKY    = 4, // squeaky floor
        SOUND_TERRAIN_SNOW      = 5,
        SOUND_TERRAIN_ICE       = 6,
        SOUND_TERRAIN_SAND      = 7,
    };

    // silence
    #define NO_SOUND                    0

    /**
     * The table below defines all sounds that exist in the game, and which flags
     * they are used with. If a sound is used with multiple sets of flags (e.g.
     * different priorities), they are given distinguishing suffixes.
     * A _2 suffix means the sound is the same despite a different sound ID.
     */

    enum SoundBits
    {
    /* Terrain sounds */
    // Terrain-dependent moving sounds; a value 0-7 is added to the sound ID before
    // playing. See higher up for the different terrain types.
        SOUND_ACTION_TERRAIN_JUMP                /* 0x04008080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x00, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_LANDING             /* 0x04088080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x08, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_STEP                /* 0x06108080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x10, 0x80, SOUND_VIBRATO | SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_BODY_HIT_GROUND     /* 0x04188080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x18, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_STEP_TIPTOE         /* 0x06208080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x20, 0x80, SOUND_VIBRATO | SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_STUCK_IN_GROUND     /* 0x04488080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x48, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TERRAIN_HEAVY_LANDING       /* 0x04608080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x60, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

        SOUND_ACTION_METAL_JUMP                  /* 0x04289080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x28, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_LANDING               /* 0x04299080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x29, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_STEP                  /* 0x042A9080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2A, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_HEAVY_LANDING         /* 0x042B9080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2B, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_CLAP_HANDS_COLD             /* 0x062C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2C, 0x00, SOUND_VIBRATO | SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_HANGING_STEP                /* 0x042DA080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2D, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_QUICKSAND_STEP              /* 0x042E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2E, 0x00, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_STEP_TIPTOE           /* 0x042F9080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x2F, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNKNOWN430                  /* 0x0430C080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x30, 0xC0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_UNKNOWN431                  /* 0x04316080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x31, 0x60, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_UNKNOWN432                  /* 0x04328080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x32, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_SWIM                        /* 0x04338080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x33, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNKNOWN434                  /* 0x04348080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x34, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_THROW                       /* 0x04358080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x35, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_KEY_SWISH                   /* 0x04368080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x36, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_SPIN                        /* 0x04378080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x37, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_TWIRL                       /* 0x04388080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x38, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // same sound as spin
        SOUND_ACTION_CLIMB_UP_TREE               /* 0x043A8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3A, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_CLIMB_DOWN_TREE             /* 0x003B0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3B, 0x00, 0), // unverified, unused
        SOUND_ACTION_UNK3C                       /* 0x003C0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3C, 0x00, 0), // unverified, unused
        SOUND_ACTION_UNKNOWN43D                  /* 0x043D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3D, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_UNKNOWN43E                  /* 0x043E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3E, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_PAT_BACK                    /* 0x043F8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x3F, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_BRUSH_HAIR                  /* 0x04408080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x40, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_CLIMB_UP_POLE               /* 0x04418080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x41, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_METAL_BONK                  /* 0x04428080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x42, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNSTUCK_FROM_GROUND         /* 0x04438080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x43, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_HIT                         /* 0x0444C080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x44, 0xC0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_HIT_2                       /* 0x0444B080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x44, 0xB0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_HIT_3                       /* 0x0444A080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x44, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_BONK                        /* 0x0445A080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x45, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_SHRINK_INTO_BBH             /* 0x0446A080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x46, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_SWIM_FAST                   /* 0x0447A080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x47, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_JUMP_WATER            /* 0x04509080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x50, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_LAND_WATER            /* 0x04519080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x51, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_METAL_STEP_WATER            /* 0x04529080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x52, 0x90, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNK53                       /* 0x00530000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x53, 0x00, 0), // unverified, unused
        SOUND_ACTION_UNK54                       /* 0x00540000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x54, 0x00, 0), // unverified, unused
        SOUND_ACTION_UNK55                       /* 0x00550000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x55, 0x00, 0), // unverified, unused
        SOUND_ACTION_FLYING_FAST                 /* 0x04568080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x56, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // "swoop"?, unverified
        SOUND_ACTION_TELEPORT                    /* 0x0457C080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x57, 0xC0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNKNOWN458                  /* 0x0458A080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x58, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_BOUNCE_OFF_OBJECT           /* 0x0459B080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x59, 0xB0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_SIDE_FLIP_UNK               /* 0x045A8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5A, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_READ_SIGN                   /* 0x045BFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5B, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_ACTION_UNKNOWN45C                  /* 0x045C8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5C, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_UNK5D                       /* 0x005D0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5D, 0x00, 0), // unverified, unused
        SOUND_ACTION_INTRO_UNK45E                /* 0x045E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5E, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_ACTION_INTRO_UNK45F                /* 0x045F8080 */ = SOUND_ARG_LOAD(SOUND_BANK_ACTION,   0x5F, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified

    /* Moving Sound Effects */

    // Terrain-dependent moving sounds; a value 0-7 is added to the sound ID before
    // playing. See higher up for the different terrain types.
        SOUND_MOVING_TERRAIN_SLIDE               /* 0x14000000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x00, 0x00, SOUND_NO_PRIORITY_LOSS),
        SOUND_MOVING_TERRAIN_RIDING_SHELL        /* 0x14200000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x20, 0x00, SOUND_NO_PRIORITY_LOSS),

        SOUND_MOVING_LAVA_BURN                   /* 0x14100000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x10, 0x00, SOUND_NO_PRIORITY_LOSS), // ?
        SOUND_MOVING_SLIDE_DOWN_POLE             /* 0x14110000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x11, 0x00, SOUND_NO_PRIORITY_LOSS), // ?
        SOUND_MOVING_SLIDE_DOWN_TREE             /* 0x14128000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x12, 0x80, SOUND_NO_PRIORITY_LOSS),
        SOUND_MOVING_QUICKSAND_DEATH             /* 0x14140000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x14, 0x00, SOUND_NO_PRIORITY_LOSS),
        SOUND_MOVING_SHOCKED                     /* 0x14160000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x16, 0x00, SOUND_NO_PRIORITY_LOSS),
        SOUND_MOVING_FLYING                      /* 0x14170000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x17, 0x00, SOUND_NO_PRIORITY_LOSS),
        SOUND_MOVING_ALMOST_DROWNING             /* 0x1C180000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x18, 0x00, SOUND_NO_PRIORITY_LOSS | SOUND_CONSTANT_FREQUENCY),
        SOUND_MOVING_AIM_CANNON                  /* 0x1D192000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x19, 0x20, SOUND_NO_VOLUME_LOSS | SOUND_NO_PRIORITY_LOSS | SOUND_CONSTANT_FREQUENCY),
        SOUND_MOVING_UNK1A                       /* 0x101A0000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x1A, 0x00, 0), // ?, unused
        SOUND_MOVING_RIDING_SHELL_LAVA           /* 0x14280000 */ = SOUND_ARG_LOAD(SOUND_BANK_MOVING,   0x28, 0x00, SOUND_NO_PRIORITY_LOSS),

    /* Mario Sound Effects */
    // A random number 0-2 is added to the sound ID before playing, producing Yah/Wah/Hoo
        SOUND_MARIO_YAH_WAH_HOO                  /* 0x24008080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x00, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_HOOHOO                       /* 0x24038080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x03, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_YAHOO                        /* 0x24048080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x04, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_UH                           /* 0x24058080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x05, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_HRMM                         /* 0x24068080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x06, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_WAH2                         /* 0x24078080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x07, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_WHOA                         /* 0x2408C080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x08, 0xC0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_EEUH                         /* 0x24098080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x09, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_ATTACKED                     /* 0x240AFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0A, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_OOOF                         /* 0x240B8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0B, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_OOOF2                        /* 0x240BD080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0B, 0xD0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_HERE_WE_GO                   /* 0x240C8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0C, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_YAWNING                      /* 0x240D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0D, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_SNORING1                     /* 0x240E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0E, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_SNORING2                     /* 0x240F8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x0F, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_WAAAOOOW                     /* 0x2410C080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x10, 0xC0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_HAHA                         /* 0x24118080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x11, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_HAHA_2                       /* 0x2411F080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x11, 0xF0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_UH2                          /* 0x2413D080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x13, 0xD0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_UH2_2                        /* 0x24138080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x13, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_ON_FIRE                      /* 0x2414A080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x14, 0xA0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_DYING                        /* 0x2415FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x15, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE), // unverified
        SOUND_MARIO_PANTING_COLD                 /* 0x24168080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x16, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

    // A random number 0-2 is added to the sound ID before playing
        SOUND_MARIO_PANTING                      /* 0x24188080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x18, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

        SOUND_MARIO_COUGHING1                    /* 0x241B8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x1B, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_COUGHING2                    /* 0x241C8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x1C, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_COUGHING3                    /* 0x241D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x1D, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_PUNCH_YAH                    /* 0x241E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x1E, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_PUNCH_HOO                    /* 0x241F8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x1F, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_MAMA_MIA                     /* 0x24208080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x20, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_OKEY_DOKEY                   /* 0x20210000 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x21, 0x00, 0), // unused
        SOUND_MARIO_GROUND_POUND_WAH             /* 0x24228080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x22, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_DROWNING                     /* 0x2423F080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x23, 0xF0, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_PUNCH_WAH                    /* 0x24248080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x24, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

    /* Mario Sound Effects (US/EU only), */
        SOUND_PEACH_DEAR_MARIO                   /* 0x2428FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x28, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

    // A random number 0-4 is added to the sound ID before playing, producing one of
    // Yahoo! (60% chance),, Waha! (20%),, or Yippee! (20%),.
        SOUND_MARIO_YAHOO_WAHA_YIPPEE            /* 0x242B8080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x2B, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

        SOUND_MARIO_DOH                          /* 0x24308080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x30, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_GAME_OVER                    /* 0x2431FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x31, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_HELLO                        /* 0x2432FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x32, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_PRESS_START_TO_PLAY          /* 0x2433FFA0 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x33, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_NO_ECHO | SOUND_DISCRETE),
        SOUND_MARIO_TWIRL_BOUNCE                 /* 0x24348080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x34, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_SNORING3                     /* 0x2435FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x35, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_SO_LONGA_BOWSER              /* 0x24368080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x36, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_MARIO_IMA_TIRED                    /* 0x24378080 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x37, 0x80, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

    /* Princess Peach Sound Effects (US/EU only), */
        SOUND_PEACH_MARIO                        /* 0x2438FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x38, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_POWER_OF_THE_STARS           /* 0x2439FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x39, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_THANKS_TO_YOU                /* 0x243AFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3A, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_THANK_YOU_MARIO              /* 0x243BFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3B, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_SOMETHING_SPECIAL            /* 0x243CFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3C, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_BAKE_A_CAKE                  /* 0x243DFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3D, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_FOR_MARIO                    /* 0x243EFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3E, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),
        SOUND_PEACH_MARIO2                       /* 0x243FFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_VOICE,    0x3F, 0xFF, SOUND_NO_PRIORITY_LOSS | SOUND_DISCRETE),

    /* General Sound Effects */
        SOUND_GENERAL_ACTIVATE_CAP_SWITCH        /* 0x30008080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x00, 0x80, SOUND_DISCRETE),
        SOUND_GENERAL_FLAME_OUT                  /* 0x30038080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x03, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_OPEN_WOOD_DOOR             /* 0x3004C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x04, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CLOSE_WOOD_DOOR            /* 0x3005C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x05, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_OPEN_IRON_DOOR             /* 0x3006C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x06, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CLOSE_IRON_DOOR            /* 0x3007C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x07, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BUBBLES                    /* 0x30080000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x08, 0x00, 0), // unverified, unused
        SOUND_GENERAL_MOVING_WATER               /* 0x30090080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x09, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SWISH_WATER                /* 0x300A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_QUIET_BUBBLE               /* 0x300B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0B, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_VOLCANO_EXPLOSION          /* 0x300C8080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0C, 0x80, SOUND_DISCRETE),
        SOUND_GENERAL_QUIET_BUBBLE2              /* 0x300D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0D, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CASTLE_TRAP_OPEN           /* 0x300E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0E, 0x80, SOUND_DISCRETE),
        SOUND_GENERAL_WALL_EXPLOSION             /* 0x300F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x0F, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_COIN                       /* 0x38118080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x11, 0x80, SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_COIN_WATER                 /* 0x38128080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x12, 0x80, SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SHORT_STAR                 /* 0x30160090 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x16, 0x00, SOUND_LOWER_BACKGROUND_MUSIC | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BIG_CLOCK                  /* 0x30170080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x17, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_LOUD_POUND                 /* 0x30180000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x18, 0x00, 0), // _TERRAIN?, unverified, unused
        SOUND_GENERAL_LOUD_POUND2                /* 0x30190000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x19, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND1               /* 0x301A0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1A, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND2               /* 0x301B0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1B, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND3               /* 0x301C0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1C, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND4               /* 0x301D0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1D, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND5               /* 0x301E0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1E, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHORT_POUND6               /* 0x301F0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x1F, 0x00, 0), // unverified, unused
        SOUND_GENERAL_OPEN_CHEST                 /* 0x31208080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x20, 0x80, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_GENERAL_CLAM_SHELL1                /* 0x31228080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x22, 0x80, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOX_LANDING                /* 0x30240080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x24, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOX_LANDING_2              /* 0x32240080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x24, 0x00, SOUND_VIBRATO | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNKNOWN1                   /* 0x30250080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x25, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNKNOWN1_2                 /* 0x32250080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x25, 0x00, SOUND_VIBRATO | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CLAM_SHELL2                /* 0x30264080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x26, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CLAM_SHELL3                /* 0x30274080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x27, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_PAINTING_EJECT             /* 0x39280080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x28, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE),
        SOUND_GENERAL_LEVEL_SELECT_CHANGE        /* 0x302B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x2B, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_PLATFORM                   /* 0x302D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x2D, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_DONUT_PLATFORM_EXPLOSION   /* 0x302E2080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x2E, 0x20, SOUND_DISCRETE),
        SOUND_GENERAL_BOWSER_BOMB_EXPLOSION      /* 0x312F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x2F, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_GENERAL_COIN_SPURT                 /* 0x38300080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x30, 0x00, SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_EXPLOSION6                 /* 0x30310000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x31, 0x00, 0), // unverified, unused
        SOUND_GENERAL_UNK32                      /* 0x30320000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x32, 0x00, 0), // unverified, unused
        SOUND_GENERAL_BOAT_TILT1                 /* 0x30344080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x34, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOAT_TILT2                 /* 0x30354080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x35, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_COIN_DROP                  /* 0x30364080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x36, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNKNOWN3                   /* 0x38378080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x37, 0x80, SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_PENDULUM_SWING             /* 0x30380080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x38, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_CHAIN_CHOMP1               /* 0x30390080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x39, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CHAIN_CHOMP2               /* 0x303A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_DOOR_TURN_KEY              /* 0x303B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3B, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_MOVING_IN_SAND             /* 0x303C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3C, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNKNOWN4_LOWPRIO           /* 0x303D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3D, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNKNOWN4                   /* 0x303D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3D, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_MOVING_PLATFORM_SWITCH     /* 0x303E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3E, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_CAGE_OPEN                  /* 0x303FA080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x3F, 0xA0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_QUIET_POUND1_LOWPRIO       /* 0x30400080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x40, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_QUIET_POUND1               /* 0x30404080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x40, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BREAK_BOX                  /* 0x3041C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x41, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_DOOR_INSERT_KEY            /* 0x30420080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x42, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL_QUIET_POUND2               /* 0x30430080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x43, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BIG_POUND                  /* 0x30440080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x44, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNK45                      /* 0x30450080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x45, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_UNK46                      /* 0x30468080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x46, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_CANNON_UP                  /* 0x30478080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x47, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_GRINDEL_ROLL               /* 0x30480080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x48, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_EXPLOSION7                 /* 0x30490000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x49, 0x00, 0), // unverified, unused
        SOUND_GENERAL_SHAKE_COFFIN               /* 0x304A0000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x4A, 0x00, 0), // unverified, unused
        SOUND_GENERAL_RACE_GUN_SHOT              /* 0x314D4080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x4D, 0x40, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_STAR_DOOR_OPEN             /* 0x304EC080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x4E, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_STAR_DOOR_CLOSE            /* 0x304FC080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x4F, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_POUND_ROCK                 /* 0x30560080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x56, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_STAR_APPEARS               /* 0x3057FF90 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x57, 0xFF, SOUND_LOWER_BACKGROUND_MUSIC | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_COLLECT_1UP                /* 0x3058FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x58, 0xFF, SOUND_DISCRETE),
        SOUND_GENERAL_BUTTON_PRESS_LOWPRIO       /* 0x305A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BUTTON_PRESS               /* 0x305A4080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5A, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BUTTON_PRESS_2_LOWPRIO     /* 0x315A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5A, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BUTTON_PRESS_2             /* 0x315A4080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5A, 0x40, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_ELEVATOR_MOVE              /* 0x305B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5B, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_ELEVATOR_MOVE_2            /* 0x315B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5B, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SWISH_AIR                  /* 0x305C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5C, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SWISH_AIR_2                /* 0x315C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5C, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_HAUNTED_CHAIR              /* 0x305D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5D, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SOFT_LANDING               /* 0x305E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5E, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_HAUNTED_CHAIR_MOVE         /* 0x305F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x5F, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOWSER_PLATFORM            /* 0x30628080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x62, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOWSER_PLATFORM_2          /* 0x31628080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x62, 0x80, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_GENERAL_HEART_SPIN                 /* 0x3064C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x64, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_POUND_WOOD_POST            /* 0x3065C080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x65, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_WATER_LEVEL_TRIG           /* 0x30668080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x66, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SWITCH_DOOR_OPEN           /* 0x3067A080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x67, 0xA0, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_RED_COIN                   /* 0x30689080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x68, 0x90, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BIRDS_FLY_AWAY             /* 0x30690080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x69, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_METAL_POUND                /* 0x306B8080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6B, 0x80, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOING1                     /* 0x306C4080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6C, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOING2_LOWPRIO             /* 0x306D2080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6D, 0x20, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOING2                     /* 0x306D4080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6D, 0x40, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_YOSHI_WALK                 /* 0x306E2080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6E, 0x20, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_ENEMY_ALERT1               /* 0x306F3080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x6F, 0x30, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_YOSHI_TALK                 /* 0x30703080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x70, 0x30, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_SPLATTERING                /* 0x30713080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x71, 0x30, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOING3                     /* 0x30720000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x72, 0x00, 0), // unverified, unused
        SOUND_GENERAL_GRAND_STAR                 /* 0x30730080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x73, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_GRAND_STAR_JUMP            /* 0x30740080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x74, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_BOAT_ROCK                  /* 0x30750080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x75, 0x00, SOUND_DISCRETE), // unverified
        SOUND_GENERAL_VANISH_SFX                 /* 0x30762080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL,  0x76, 0x20, SOUND_DISCRETE), // unverified

    /* Environment Sound Effects */
        SOUND_ENV_WATERFALL1                     /* 0x40000000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x00, 0x00, 0), // unverified
        SOUND_ENV_WATERFALL2                     /* 0x40010000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x01, 0x00, 0), // unverified
        SOUND_ENV_ELEVATOR1                      /* 0x40020000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x02, 0x00, 0), // unverified
        SOUND_ENV_DRONING1                       /* 0x41030000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x03, 0x00, SOUND_NO_VOLUME_LOSS), // unverified
        SOUND_ENV_DRONING2                       /* 0x40040000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x04, 0x00, 0), // unverified
        SOUND_ENV_WIND1                          /* 0x40050000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x05, 0x00, 0), // unverified
        SOUND_ENV_MOVING_SAND_SNOW               /* 0x40060000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x06, 0x00, 0), // unverified, unused
        SOUND_ENV_UNK07                          /* 0x40070000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x07, 0x00, 0), // unverified, unused
        SOUND_ENV_ELEVATOR2                      /* 0x40080000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x08, 0x00, 0), // unverified
        SOUND_ENV_WATER                          /* 0x40090000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x09, 0x00, 0), // unverified
        SOUND_ENV_UNKNOWN2                       /* 0x400A0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0A, 0x00, 0), // unverified
        SOUND_ENV_BOAT_ROCKING1                  /* 0x400B0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0B, 0x00, 0), // unverified
        SOUND_ENV_ELEVATOR3                      /* 0x400C0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0C, 0x00, 0), // unverified
        SOUND_ENV_ELEVATOR4                      /* 0x400D0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0D, 0x00, 0), // unverified
        SOUND_ENV_ELEVATOR4_2                    /* 0x410D0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0D, 0x00, SOUND_NO_VOLUME_LOSS), // unverified
        SOUND_ENV_MOVINGSAND                     /* 0x400E0000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0E, 0x00, 0), // unverified
        SOUND_ENV_MERRY_GO_ROUND_CREAKING        /* 0x400F4000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x0F, 0x40, 0), // unverified
        SOUND_ENV_WIND2                          /* 0x40108000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x10, 0x80, 0), // unverified
        SOUND_ENV_UNK12                          /* 0x40120000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x12, 0x00, 0), // unverified, unused
        SOUND_ENV_SLIDING                        /* 0x40130000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x13, 0x00, 0), // unverified
        SOUND_ENV_STAR                           /* 0x40140010 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x14, 0x00, SOUND_LOWER_BACKGROUND_MUSIC), // unverified
        SOUND_ENV_MOVING_BIG_PLATFORM            /* 0x41150000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x15, 0x00, SOUND_NO_VOLUME_LOSS), // unverified
        SOUND_ENV_WATER_DRAIN                    /* 0x41160000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x16, 0x00, SOUND_NO_VOLUME_LOSS), // unverified
        SOUND_ENV_METAL_BOX_PUSH                 /* 0x40178000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x17, 0x80, 0), // unverified
        SOUND_ENV_SINK_QUICKSAND                 /* 0x40188000 */ = SOUND_ARG_LOAD(SOUND_BANK_ENV,      0x18, 0x80, 0), // unverified

    /* Object Sound Effects */
        SOUND_OBJ_SUSHI_SHARK_WATER_SOUND        /* 0x50008080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x00, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_MRI_SHOOT                      /* 0x50010080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x01, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BABY_PENGUIN_WALK              /* 0x50020080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x02, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_WALK                    /* 0x50030080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x03, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_TAIL_PICKUP             /* 0x50050080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x05, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_DEFEATED                /* 0x50060080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x06, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_SPINNING                /* 0x50070080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x07, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_INHALING                /* 0x50080080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x08, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BIG_PENGUIN_WALK               /* 0x50098080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x09, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_BOO_BOUNCE_TOP                 /* 0x500A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0A, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BOO_LAUGH_SHORT                /* 0x500B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0B, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_THWOMP                         /* 0x500CA080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0C, 0xA0, SOUND_DISCRETE),
        SOUND_OBJ_CANNON1                        /* 0x500DF080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0D, 0xF0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_CANNON2                        /* 0x500EF080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0E, 0xF0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_CANNON3                        /* 0x500FF080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x0F, 0xF0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_JUMP_WALK_WATER                /* 0x50120000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x12, 0x00, 0), // unverified, unused
        SOUND_OBJ_UNKNOWN2                       /* 0x50130080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x13, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_MRI_DEATH                      /* 0x50140080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x14, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_POUNDING1                      /* 0x50155080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x15, 0x50, SOUND_DISCRETE), // unverified
        SOUND_OBJ_POUNDING1_HIGHPRIO             /* 0x50158080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x15, 0x80, SOUND_DISCRETE), // unverified
        SOUND_OBJ_WHOMP                          /* 0x50166080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x16, 0x60, SOUND_DISCRETE),
        SOUND_OBJ_KING_BOBOMB                    /* 0x50168080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x16, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_BULLY_METAL                    /* 0x50178080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x17, 0x80, SOUND_DISCRETE), // unverified
        SOUND_OBJ_BULLY_EXPLODE                  /* 0x5018A080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x18, 0xA0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_BULLY_EXPLODE_2                /* 0x5118A080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x18, 0xA0, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE), // unverified
        SOUND_OBJ_POUNDING_CANNON                /* 0x501A5080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x1A, 0x50, SOUND_DISCRETE), // unverified
        SOUND_OBJ_BULLY_WALK                     /* 0x501B3080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x1B, 0x30, SOUND_DISCRETE), // unverified
        SOUND_OBJ_UNKNOWN3                       /* 0x501D8080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x1D, 0x80, SOUND_DISCRETE), // unverified
        SOUND_OBJ_UNKNOWN4                       /* 0x501EA080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x1E, 0xA0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_BABY_PENGUIN_DIVE              /* 0x501F4080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x1F, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_GOOMBA_WALK                    /* 0x50200080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x20, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_UKIKI_CHATTER_LONG             /* 0x50210080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x21, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_MONTY_MOLE_ATTACK              /* 0x50220080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x22, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_EVIL_LAKITU_THROW              /* 0x50222080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x22, 0x20, SOUND_DISCRETE),
        SOUND_OBJ_UNK23                          /* 0x50230000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x23, 0x00, 0), // unverified, unused
        SOUND_OBJ_DYING_ENEMY1                   /* 0x50244080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x24, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_CANNON4                        /* 0x50254080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x25, 0x40, SOUND_DISCRETE), // unverified
        SOUND_OBJ_DYING_ENEMY2                   /* 0x50260000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x26, 0x00, 0), // unverified, unused
        SOUND_OBJ_BOBOMB_WALK                    /* 0x50270080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x27, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_SOMETHING_LANDING              /* 0x50288080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x28, 0x80, SOUND_DISCRETE), // unverified
        SOUND_OBJ_DIVING_IN_WATER                /* 0x5029A080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x29, 0xA0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_SNOW_SAND1                     /* 0x502A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_SNOW_SAND2                     /* 0x502B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2B, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_DEFAULT_DEATH                  /* 0x502C8080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2C, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_BIG_PENGUIN_YELL               /* 0x502D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2D, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_WATER_BOMB_BOUNCING            /* 0x502E8080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2E, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_GOOMBA_ALERT                   /* 0x502F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2F, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_WIGGLER_JUMP                   /* 0x502F6080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x2F, 0x60, SOUND_DISCRETE),
        SOUND_OBJ_STOMPED                        /* 0x50308080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x30, 0x80, SOUND_DISCRETE), // unverified
        SOUND_OBJ_UNKNOWN6                       /* 0x50310080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x31, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_DIVING_INTO_WATER              /* 0x50324080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x32, 0x40, SOUND_DISCRETE), // unverified
        SOUND_OBJ_PIRANHA_PLANT_SHRINK           /* 0x50334080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x33, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_KOOPA_THE_QUICK_WALK           /* 0x50342080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x34, 0x20, SOUND_DISCRETE),
        SOUND_OBJ_KOOPA_WALK                     /* 0x50350080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x35, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_BULLY_WALKING                  /* 0x50366080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x36, 0x60, SOUND_DISCRETE),
        SOUND_OBJ_DORRIE                         /* 0x50376080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x37, 0x60, SOUND_DISCRETE),
        SOUND_OBJ_BOWSER_LAUGH                   /* 0x50388080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x38, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_UKIKI_CHATTER_SHORT            /* 0x50390080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x39, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_UKIKI_CHATTER_IDLE             /* 0x503A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3A, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_UKIKI_STEP_DEFAULT             /* 0x503B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3B, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_UKIKI_STEP_LEAVES              /* 0x503C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3C, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_KOOPA_TALK                     /* 0x503DA080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3D, 0xA0, SOUND_DISCRETE),
        SOUND_OBJ_KOOPA_DAMAGE                   /* 0x503EA080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3E, 0xA0, SOUND_DISCRETE),
        SOUND_OBJ_KLEPTO1                        /* 0x503F4080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x3F, 0x40, SOUND_DISCRETE), // unverified
        SOUND_OBJ_KLEPTO2                        /* 0x50406080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x40, 0x60, SOUND_DISCRETE), // unverified
        SOUND_OBJ_KING_BOBOMB_TALK               /* 0x50410080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x41, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_KING_BOBOMB_JUMP               /* 0x50468080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x46, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_KING_WHOMP_DEATH               /* 0x5147C080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x47, 0xC0, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_OBJ_BOO_LAUGH_LONG                 /* 0x50480080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x48, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_EEL                            /* 0x504A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_EEL_2                          /* 0x524A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4A, 0x00, SOUND_VIBRATO | SOUND_DISCRETE), // unverified
        SOUND_OBJ_EYEROK_SHOW_EYE                /* 0x524B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4B, 0x00, SOUND_VIBRATO | SOUND_DISCRETE),
        SOUND_OBJ_MR_BLIZZARD_ALERT              /* 0x504C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4C, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_SNUFIT_SHOOT                   /* 0x504D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4D, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_SKEETER_WALK                   /* 0x504E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4E, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_WALKING_WATER                  /* 0x504F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x4F, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_BIRD_CHIRP3                    /* 0x50514000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x51, 0x40, 0),
        SOUND_OBJ_PIRANHA_PLANT_APPEAR           /* 0x50542080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x54, 0x20, SOUND_DISCRETE),
        SOUND_OBJ_FLAME_BLOWN                    /* 0x50558080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x55, 0x80, SOUND_DISCRETE),
        SOUND_OBJ_MAD_PIANO_CHOMPING             /* 0x52564080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x56, 0x40, SOUND_VIBRATO | SOUND_DISCRETE),
        SOUND_OBJ_BOBOMB_BUDDY_TALK              /* 0x50584080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x58, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_SPINY_UNK59                    /* 0x50591080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x59, 0x10, SOUND_DISCRETE), // unverified
        SOUND_OBJ_WIGGLER_HIGH_PITCH             /* 0x505C4080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x5C, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_HEAVEHO_TOSSED                 /* 0x505D4080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x5D, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_WIGGLER_DEATH                  /* 0x505E0000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x5E, 0x00, 0), // unverified, unused
        SOUND_OBJ_BOWSER_INTRO_LAUGH             /* 0x505F8090 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x5F, 0x80, SOUND_LOWER_BACKGROUND_MUSIC | SOUND_DISCRETE),
        SOUND_OBJ_ENEMY_DEATH_HIGH               /* 0x5060B080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x60, 0xB0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_ENEMY_DEATH_LOW                /* 0x5061B080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x61, 0xB0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_SWOOP_DEATH                    /* 0x5062B080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x62, 0xB0, SOUND_DISCRETE),
        SOUND_OBJ_KOOPA_FLYGUY_DEATH             /* 0x5063B080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x63, 0xB0, SOUND_DISCRETE),
        SOUND_OBJ_POKEY_DEATH                    /* 0x5063C080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x63, 0xC0, SOUND_DISCRETE),
        SOUND_OBJ_SNOWMAN_BOUNCE                 /* 0x5064C080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x64, 0xC0, SOUND_DISCRETE), // unverified
        SOUND_OBJ_SNOWMAN_EXPLODE                /* 0x5065D080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x65, 0xD0, SOUND_DISCRETE),
        SOUND_OBJ_POUNDING_LOUD                  /* 0x50684080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x68, 0x40, SOUND_DISCRETE), // unverified
        SOUND_OBJ_MIPS_RABBIT                    /* 0x506A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x6A, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_MIPS_RABBIT_WATER              /* 0x506C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x6C, 0x00, SOUND_DISCRETE), // unverified
        SOUND_OBJ_EYEROK_EXPLODE                 /* 0x506D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x6D, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_CHUCKYA_DEATH                  /* 0x516E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x6E, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_OBJ_WIGGLER_TALK                   /* 0x506F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x6F, 0x00, SOUND_DISCRETE),
        SOUND_OBJ_WIGGLER_ATTACKED               /* 0x50706080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x70, 0x60, SOUND_DISCRETE),
        SOUND_OBJ_WIGGLER_LOW_PITCH              /* 0x50712080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x71, 0x20, SOUND_DISCRETE),
        SOUND_OBJ_SNUFIT_SKEETER_DEATH           /* 0x5072C080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x72, 0xC0, SOUND_DISCRETE),
        SOUND_OBJ_BUBBA_CHOMP                    /* 0x50734080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x73, 0x40, SOUND_DISCRETE),
        SOUND_OBJ_ENEMY_DEFEAT_SHRINK            /* 0x50744080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ,      0x74, 0x40, SOUND_DISCRETE),

        SOUND_AIR_BOWSER_SPIT_FIRE               /* 0x60000000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x00, 0x00, 0),
        SOUND_AIR_UNK01                          /* 0x60010000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x01, 0x00, 0), // ?, unused
        SOUND_AIR_LAKITU_FLY                     /* 0x60028000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x02, 0x80, 0),
        SOUND_AIR_LAKITU_FLY_HIGHPRIO            /* 0x6002FF00 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x02, 0xFF, 0),
        SOUND_AIR_AMP_BUZZ                       /* 0x60034000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x03, 0x40, 0),
        SOUND_AIR_BLOW_FIRE                      /* 0x60048000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x04, 0x80, 0),
        SOUND_AIR_BLOW_WIND                      /* 0x60044000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x04, 0x40, 0),
        SOUND_AIR_ROUGH_SLIDE                    /* 0x60050000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x05, 0x00, 0),
        SOUND_AIR_HEAVEHO_MOVE                   /* 0x60064000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x06, 0x40, 0),
        SOUND_AIR_UNK07                          /* 0x60070000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x07, 0x00, 0), // ?, unused
        SOUND_AIR_BOBOMB_LIT_FUSE                /* 0x60086000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x08, 0x60, 0),
        SOUND_AIR_HOWLING_WIND                   /* 0x60098000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x09, 0x80, 0),
        SOUND_AIR_CHUCKYA_MOVE                   /* 0x600A4000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x0A, 0x40, 0),
        SOUND_AIR_PEACH_TWINKLE                  /* 0x600B4000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x0B, 0x40, 0),
        SOUND_AIR_CASTLE_OUTDOORS_AMBIENT        /* 0x60104000 */ = SOUND_ARG_LOAD(SOUND_BANK_AIR,      0x10, 0x40, 0),

    /* Menu Sound Effects */
        SOUND_MENU_CHANGE_SELECT                 /* 0x7000F880 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x00, 0xF8, SOUND_DISCRETE),
        SOUND_MENU_REVERSE_PAUSE                 /* 0x70010000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x01, 0x00, 0), // unverified, unused
        SOUND_MENU_PAUSE                         /* 0x7002FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x02, 0xFF, SOUND_DISCRETE),
        SOUND_MENU_PAUSE_2                       /* 0x7003FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x03, 0xFF, SOUND_DISCRETE),
        SOUND_MENU_MESSAGE_APPEAR                /* 0x70040080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x04, 0x00, SOUND_DISCRETE),
        SOUND_MENU_MESSAGE_DISAPPEAR             /* 0x70050080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x05, 0x00, SOUND_DISCRETE),
        SOUND_MENU_CAMERA_ZOOM_IN                /* 0x70060080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x06, 0x00, SOUND_DISCRETE),
        SOUND_MENU_CAMERA_ZOOM_OUT               /* 0x70070080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x07, 0x00, SOUND_DISCRETE),
        SOUND_MENU_PINCH_MARIO_FACE              /* 0x70080080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x08, 0x00, SOUND_DISCRETE),
        SOUND_MENU_LET_GO_MARIO_FACE             /* 0x70090080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x09, 0x00, SOUND_DISCRETE),
        SOUND_MENU_HAND_APPEAR                   /* 0x700A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0A, 0x00, SOUND_DISCRETE),
        SOUND_MENU_HAND_DISAPPEAR                /* 0x700B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0B, 0x00, SOUND_DISCRETE),
        SOUND_MENU_UNK0C                         /* 0x700C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0C, 0x00, SOUND_DISCRETE), // unverified
        SOUND_MENU_POWER_METER                   /* 0x700D0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0D, 0x00, SOUND_DISCRETE), // unverified
        SOUND_MENU_CAMERA_BUZZ                   /* 0x700E0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0E, 0x00, SOUND_DISCRETE),
        SOUND_MENU_CAMERA_TURN                   /* 0x700F0080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x0F, 0x00, SOUND_DISCRETE),
        SOUND_MENU_UNK10                         /* 0x70100000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x10, 0x00, 0), // unverified, unused
        SOUND_MENU_CLICK_FILE_SELECT             /* 0x70110080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x11, 0x00, SOUND_DISCRETE),
        SOUND_MENU_MESSAGE_NEXT_PAGE             /* 0x70130080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x13, 0x00, SOUND_DISCRETE), // unverified
        SOUND_MENU_COIN_ITS_A_ME_MARIO           /* 0x70140080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x14, 0x00, SOUND_DISCRETE),
        SOUND_MENU_YOSHI_GAIN_LIVES              /* 0x70150080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x15, 0x00, SOUND_DISCRETE),
        SOUND_MENU_ENTER_PIPE                    /* 0x7016A080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x16, 0xA0, SOUND_DISCRETE),
        SOUND_MENU_EXIT_PIPE                     /* 0x7017A080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x17, 0xA0, SOUND_DISCRETE),
        SOUND_MENU_BOWSER_LAUGH                  /* 0x70188080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x18, 0x80, SOUND_DISCRETE),
        SOUND_MENU_ENTER_HOLE                    /* 0x71198080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x19, 0x80, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_MENU_CLICK_CHANGE_VIEW             /* 0x701A8080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1A, 0x80, SOUND_DISCRETE), // unverified
        SOUND_MENU_CAMERA_UNUSED1                /* 0x701B0000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1B, 0x00, 0), // unverified, unused
        SOUND_MENU_CAMERA_UNUSED2                /* 0x701C0000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1C, 0x00, 0), // unverified, unused
        SOUND_MENU_MARIO_CASTLE_WARP             /* 0x701DB080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1D, 0xB0, SOUND_DISCRETE), // unverified
        SOUND_MENU_STAR_SOUND                    /* 0x701EFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1E, 0xFF, SOUND_DISCRETE),
        SOUND_MENU_THANK_YOU_PLAYING_MY_GAME     /* 0x701FFF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x1F, 0xFF, SOUND_DISCRETE),
        SOUND_MENU_READ_A_SIGN                   /* 0x70200000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x20, 0x00, 0), // unverified, unused
        SOUND_MENU_EXIT_A_SIGN                   /* 0x70210000 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x21, 0x00, 0), // unverified, unused
        SOUND_MENU_MARIO_CASTLE_WARP2            /* 0x70222080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x22, 0x20, SOUND_DISCRETE), // unverified
        SOUND_MENU_STAR_SOUND_OKEY_DOKEY         /* 0x7023FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x23, 0xFF, SOUND_DISCRETE),
        SOUND_MENU_STAR_SOUND_LETS_A_GO          /* 0x7024FF80 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x24, 0xFF, SOUND_DISCRETE),

    // US/EU only; an index between 0-7 or 0-4 is added to the sound ID before
    // playing, producing the same sound with different pitch.
        SOUND_MENU_COLLECT_RED_COIN              /* 0x78289080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x28, 0x90, SOUND_CONSTANT_FREQUENCY | SOUND_DISCRETE),
        SOUND_MENU_COLLECT_SECRET                /* 0x70302080 */ = SOUND_ARG_LOAD(SOUND_BANK_MENU,     0x30, 0x20, SOUND_DISCRETE),

    // Channel 8 loads sounds from the same place as channel 3, making it possible
    // to play two channel 3 sounds at once (since just one sound from each channel
    // can play at a given time),.
        SOUND_GENERAL2_BOBOMB_EXPLOSION          /* 0x802E2080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x2E, 0x20, SOUND_DISCRETE),
        SOUND_GENERAL2_PURPLE_SWITCH             /* 0x803EC080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x3E, 0xC0, SOUND_DISCRETE),
        SOUND_GENERAL2_ROTATING_BLOCK_CLICK      /* 0x80400080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x40, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL2_SPINDEL_ROLL              /* 0x80482080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x48, 0x20, SOUND_DISCRETE),
        SOUND_GENERAL2_PYRAMID_TOP_SPIN          /* 0x814BE080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x4B, 0xE0, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_GENERAL2_PYRAMID_TOP_EXPLOSION     /* 0x814CF080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x4C, 0xF0, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_GENERAL2_BIRD_CHIRP2               /* 0x80504000 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x50, 0x40, 0),
        SOUND_GENERAL2_SWITCH_TICK_FAST          /* 0x8054F010 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x54, 0xF0, SOUND_LOWER_BACKGROUND_MUSIC),
        SOUND_GENERAL2_SWITCH_TICK_SLOW          /* 0x8055F010 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x55, 0xF0, SOUND_LOWER_BACKGROUND_MUSIC),
        SOUND_GENERAL2_STAR_APPEARS              /* 0x8057FF90 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x57, 0xFF, SOUND_LOWER_BACKGROUND_MUSIC | SOUND_DISCRETE),
        SOUND_GENERAL2_ROTATING_BLOCK_ALERT      /* 0x80590080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x59, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL2_BOWSER_EXPLODE            /* 0x80600080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x60, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL2_BOWSER_KEY                /* 0x80610080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x61, 0x00, SOUND_DISCRETE),
        SOUND_GENERAL2_1UP_APPEAR                /* 0x8063D080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x63, 0xD0, SOUND_DISCRETE),
        SOUND_GENERAL2_RIGHT_ANSWER              /* 0x806AA080 */ = SOUND_ARG_LOAD(SOUND_BANK_GENERAL2, 0x6A, 0xA0, SOUND_DISCRETE),

    // Channel 9 loads sounds from the same place as channel 5.
        SOUND_OBJ2_BOWSER_ROAR                   /* 0x90040080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x04, 0x00, SOUND_DISCRETE),
        SOUND_OBJ2_PIRANHA_PLANT_BITE            /* 0x90105080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x10, 0x50, SOUND_DISCRETE),
        SOUND_OBJ2_PIRANHA_PLANT_DYING           /* 0x90116080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x11, 0x60, SOUND_DISCRETE),
        SOUND_OBJ2_BOWSER_PUZZLE_PIECE_MOVE      /* 0x90192080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x19, 0x20, SOUND_DISCRETE),
        SOUND_OBJ2_BULLY_ATTACKED                /* 0x901C0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x1C, 0x00, SOUND_DISCRETE),
        SOUND_OBJ2_KING_BOBOMB_DAMAGE            /* 0x91424080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x42, 0x40, SOUND_NO_VOLUME_LOSS | SOUND_DISCRETE),
        SOUND_OBJ2_SCUTTLEBUG_WALK               /* 0x90434080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x43, 0x40, SOUND_DISCRETE),
        SOUND_OBJ2_SCUTTLEBUG_ALERT              /* 0x90444080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x44, 0x40, SOUND_DISCRETE),
        SOUND_OBJ2_BABY_PENGUIN_YELL             /* 0x90450080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x45, 0x00, SOUND_DISCRETE),
        SOUND_OBJ2_SWOOP                         /* 0x90490080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x49, 0x00, SOUND_DISCRETE),
        SOUND_OBJ2_BIRD_CHIRP1                   /* 0x90524000 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x52, 0x40, 0),
        SOUND_OBJ2_LARGE_BULLY_ATTACKED          /* 0x90570080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x57, 0x00, SOUND_DISCRETE),
        SOUND_OBJ2_EYEROK_SOUND_SHORT            /* 0x935A0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x5A, 0x00, SOUND_NO_VOLUME_LOSS | SOUND_VIBRATO | SOUND_DISCRETE),
        SOUND_OBJ2_WHOMP_SOUND_SHORT             /* 0x935AC080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x5A, 0xC0, SOUND_NO_VOLUME_LOSS | SOUND_VIBRATO | SOUND_DISCRETE),
        SOUND_OBJ2_EYEROK_SOUND_LONG             /* 0x925B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x5B, 0x00, SOUND_VIBRATO | SOUND_DISCRETE),
        SOUND_OBJ2_BOWSER_TELEPORT               /* 0x90668080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x66, 0x80, SOUND_DISCRETE),
        SOUND_OBJ2_MONTY_MOLE_APPEAR             /* 0x90678080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x67, 0x80, SOUND_DISCRETE),
        SOUND_OBJ2_BOSS_DIALOG_GRUNT             /* 0x90694080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x69, 0x40, SOUND_DISCRETE),
        SOUND_OBJ2_MRI_SPINNING                  /* 0x906B0080 */ = SOUND_ARG_LOAD(SOUND_BANK_OBJ2,     0x6B, 0x00, SOUND_DISCRETE),
    };

    // From "libsm64/src/decomp/game/interaction.h"
    #define INT_SUBTYPE_DELAY_INVINCIBILITY 0x00000002
    #define INT_SUBTYPE_BIG_KNOCKBACK 0x00000008 /* Used by Bowser, sets Mario's forward velocity to 40 on hit */

    void set_scale_factor(real_t p_value);
    real_t get_scale_factor() const;

    double get_tick_delta_time() const;

    // typedef void (*SM64DebugPrintFunctionPtr)( const char * );
    // extern SM64_LIB_FN void sm64_register_debug_print_function( SM64DebugPrintFunctionPtr debugPrintFunction );
    void register_debug_print_function(const godot::Callable &p_callback);
    const godot::Callable &get_debug_print_function() const;

    // typedef void (*SM64PlaySoundFunctionPtr)( uint32_t soundBits, float *pos );
    // extern SM64_LIB_FN void sm64_register_play_sound_function( SM64PlaySoundFunctionPtr playSoundFunction );
    void register_play_sound_function(const godot::Callable &p_callback);
    const godot::Callable &get_play_sound_function() const;

    // extern SM64_LIB_FN void sm64_global_init( const uint8_t *rom, uint8_t *outTexture );
    godot::Ref<godot::Image> global_init(const godot::PackedByteArray &p_rom);
    // extern SM64_LIB_FN void sm64_global_terminate( void );
    void global_terminate();

    // extern SM64_LIB_FN void sm64_audio_init( const uint8_t *rom );
    void audio_init(const godot::PackedByteArray &p_rom);
    // extern SM64_LIB_FN uint32_t sm64_audio_tick( uint32_t numQueuedSamples, uint32_t numDesiredSamples, int16_t *audio_buffer );
    godot::PackedVector2Array audio_tick(int p_queued_frames, int p_desired_frames);

    // extern SM64_LIB_FN void sm64_static_surfaces_load( const struct SM64Surface *surfaceArray, uint32_t numSurfaces );
    void static_surfaces_load(const godot::Ref<LibSM64SurfaceArray> &p_surfaces);

    // extern SM64_LIB_FN int32_t sm64_mario_create( float x, float y, float z );
    int32_t mario_create(const godot::Vector3 &p_position);
    // extern SM64_LIB_FN void sm64_mario_tick( int32_t marioId, const struct SM64MarioInputs *inputs, struct SM64MarioState *outState, struct SM64MarioGeometryBuffers *outBuffers );
    godot::Array mario_tick(int32_t p_mario_id, const godot::Ref<LibSM64MarioInputs> &p_mario_inputs);
    // extern SM64_LIB_FN void sm64_mario_delete( int32_t marioId );
    void mario_delete(int32_t p_mario_id);

    // extern SM64_LIB_FN void sm64_set_mario_action(int32_t marioId, uint32_t action);
    void set_mario_action(int32_t p_mario_id, godot::BitField<ActionFlags> p_action);
    // extern SM64_LIB_FN void sm64_set_mario_action_arg(int32_t marioId, uint32_t action, uint32_t actionArg);
    void set_mario_action_arg(int32_t p_mario_id, godot::BitField<ActionFlags> p_action, uint32_t p_action_arg);
    // extern SM64_LIB_FN void sm64_set_mario_animation(int32_t marioId, int32_t animID);
    void set_mario_animation(int32_t p_mario_id, int32_t p_anim_id);
    // extern SM64_LIB_FN void sm64_set_mario_anim_frame(int32_t marioId, int16_t animFrame);
    void set_mario_anim_frame(int32_t p_mario_id, int16_t p_anim_frame);
    // extern SM64_LIB_FN void sm64_set_mario_state(int32_t marioId, uint32_t flags);
    void set_mario_state(int32_t p_mario_id, godot::BitField<MarioFlags> p_flags);
    // extern SM64_LIB_FN void sm64_set_mario_position(int32_t marioId, float x, float y, float z);
    void set_mario_position(int32_t p_mario_id, const godot::Vector3 &p_position);
    // extern SM64_LIB_FN void sm64_set_mario_angle(int32_t marioId, float x, float y, float z);
    void set_mario_angle(int32_t p_mario_id, const godot::Quaternion &p_angle);
    // extern SM64_LIB_FN void sm64_set_mario_faceangle(int32_t marioId, float y);
    void set_mario_face_angle(int32_t p_mario_id, float p_y);
    // extern SM64_LIB_FN void sm64_set_mario_velocity(int32_t marioId, float x, float y, float z);
    void set_mario_velocity(int32_t p_mario_id, const godot::Vector3 &p_velocity);
    // extern SM64_LIB_FN void sm64_set_mario_forward_velocity(int32_t marioId, float vel);
    void set_mario_forward_velocity(int32_t p_mario_id, float p_velocity);
    // extern SM64_LIB_FN void sm64_set_mario_invincibility(int32_t marioId, int16_t timer);
    void set_mario_invincibility(int32_t p_mario_id, double p_time);
    // extern SM64_LIB_FN void sm64_set_mario_water_level(int32_t marioId, signed int level);
    void set_mario_water_level(int32_t p_mario_id, real_t p_level);
    // extern SM64_LIB_FN void sm64_set_mario_gas_level(int32_t marioId, signed int level);
    void set_mario_gas_level(int32_t p_mario_id, real_t p_level);
    // extern SM64_LIB_FN void sm64_set_mario_health(int32_t marioId, uint16_t health);
    void set_mario_health(int32_t p_mario_id, uint16_t p_health);
    // extern SM64_LIB_FN void sm64_mario_take_damage(int32_t marioId, uint32_t damage, uint32_t subtype, float x, float y, float z);
    void mario_take_damage(int32_t p_mario_id, uint32_t p_damage, uint32_t p_subtype, const godot::Vector3 &p_position);
    // extern SM64_LIB_FN void sm64_mario_heal(int32_t marioId, uint8_t healCounter);
    void mario_heal(int32_t p_mario_id, uint8_t p_heal_counter);
    // extern SM64_LIB_FN void sm64_mario_kill(int32_t marioId);
    void mario_kill(int32_t p_mario_id);
    // extern SM64_LIB_FN void sm64_mario_interact_cap(int32_t marioId, uint32_t capFlag, uint16_t capTime, uint8_t playMusic);
    void mario_interact_cap(int32_t p_mario_id, godot::BitField<MarioFlags> p_cap_flag, double p_cap_time = 0.0, bool p_play_music = true);
    // extern SM64_LIB_FN void sm64_mario_extend_cap(int32_t marioId, uint16_t capTime);
    void mario_extend_cap(int32_t p_mario_id, double p_cap_time);
    // extern SM64_LIB_FN bool sm64_mario_attack(int32_t marioId, float x, float y, float z, float hitboxHeight);
    void mario_attack(int32_t p_mario_id, const godot::Vector3 &p_position, real_t p_hitbox_height);

    // extern SM64_LIB_FN uint32_t sm64_surface_object_create( const struct SM64SurfaceObject *surfaceObject );
    int surface_object_create(const godot::Vector3 &p_position, const godot::Quaternion &p_rotation, const godot::Ref<LibSM64SurfaceArray> &p_surfaces);
    // extern SM64_LIB_FN void sm64_surface_object_move( uint32_t objectId, const struct SM64ObjectTransform *transform );
    void surface_object_move(uint32_t p_object_id, const godot::Vector3 &p_position, const godot::Quaternion &p_rotation);
    // extern SM64_LIB_FN void sm64_surface_object_delete( uint32_t objectId );
    void surface_object_delete(uint32_t p_object_id);

    // extern SM64_LIB_FN int32_t sm64_surface_find_wall_collision( float *xPtr, float *yPtr, float *zPtr, float offsetY, float radius );
    // extern SM64_LIB_FN int32_t sm64_surface_find_wall_collisions( struct SM64WallCollisionData *colData );
    // extern SM64_LIB_FN float sm64_surface_find_ceil( float posX, float posY, float posZ, struct SM64SurfaceCollisionData **pceil );
    // extern SM64_LIB_FN float sm64_surface_find_floor_height_and_data( float xPos, float yPos, float zPos, struct SM64FloorCollisionData **floorGeo );
    // extern SM64_LIB_FN float sm64_surface_find_floor_height( float x, float y, float z );
    // extern SM64_LIB_FN float sm64_surface_find_floor( float xPos, float yPos, float zPos, struct SM64SurfaceCollisionData **pfloor );
    // extern SM64_LIB_FN float sm64_surface_find_water_level( float x, float z );
    // extern SM64_LIB_FN float sm64_surface_find_poison_gas_level( float x, float z );

    // extern SM64_LIB_FN void sm64_seq_player_play_sequence(uint8_t player, uint8_t seqId, uint16_t arg2);
    void seq_player_play_sequence(SeqPlayer p_player, SeqId p_seq_id, double p_fade_in_time = 0.0);
    // extern SM64_LIB_FN void sm64_play_music(uint8_t player, uint16_t seqArgs, uint16_t fadeTimer);
    void play_music(SeqPlayer p_player, uint16_t p_seq_args, double p_fade_in_time = 0.0);
    // extern SM64_LIB_FN void sm64_stop_background_music(uint16_t seqId);
    void stop_background_music(SeqId p_seq_id);
    // extern SM64_LIB_FN void sm64_fadeout_background_music(uint16_t arg0, uint16_t fadeOut);
    void fadeout_background_music(SeqId p_seq_id, double p_fade_out_time);
    // extern SM64_LIB_FN uint16_t sm64_get_current_background_music();
    uint16_t get_current_background_music();
    // extern SM64_LIB_FN void sm64_play_sound(int32_t soundBits, float *pos);
    void play_sound(godot::BitField<SoundBits> p_sound_bits, const godot::Vector3 &p_position);
    // extern SM64_LIB_FN void sm64_play_sound_global(int32_t soundBits);
    void play_sound_global(godot::BitField<SoundBits> p_sound_bits);
    // extern SM64_LIB_FN void sm64_set_sound_volume(float vol);
    void set_sound_volume(float p_volume);

protected:
    static void _bind_methods();

private:
    real_t scale_factor;

    godot::Callable debug_print_function;
    godot::Callable play_sound_function;
};

VARIANT_ENUM_CAST(LibSM64::SurfaceType);
VARIANT_ENUM_CAST(LibSM64::TerrainType);

VARIANT_BITFIELD_CAST(LibSM64::ParticleFlags);
VARIANT_BITFIELD_CAST(LibSM64::MarioFlags);
VARIANT_BITFIELD_CAST(LibSM64::ActionFlags);

VARIANT_ENUM_CAST(LibSM64::SeqPlayer);
VARIANT_ENUM_CAST(LibSM64::SeqId);

VARIANT_ENUM_CAST(LibSM64::SoundTerrain)
VARIANT_BITFIELD_CAST(LibSM64::SoundBits);

#endif // LIBSM64GD_LIBSM64_H
