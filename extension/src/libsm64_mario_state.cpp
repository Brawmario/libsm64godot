#include <libsm64_mario_state.hpp>

#include <godot_cpp/core/class_db.hpp>

#include <libsm64.hpp>

LibSM64MarioState::LibSM64MarioState()
    : position(0.0, 0.0, 0.0),
      velocity(0.0, 0.0, 0.0),
      health(0),
      action(0),
      flags(0),
      particle_flags(0),
      invincibility_time(0.0) {}

LibSM64MarioState::LibSM64MarioState(const SM64MarioState &state)
    : position(-state.position[2], state.position[1], state.position[0]),
      velocity(-state.velocity[2], state.velocity[1], state.velocity[0]),
      health(state.health),
      action(state.action),
      flags(state.flags),
      particle_flags(state.particleFlags),
      invincibility_time(state.invincTimer * g_sm64_timestep_interval) {}

void LibSM64MarioState::set_position(godot::Vector3 p_value)
{
    position = p_value;
}

godot::Vector3 LibSM64MarioState::get_position() const
{
    return position;
}

void LibSM64MarioState::set_velocity(godot::Vector3 p_value)
{
    velocity = p_value;
}

godot::Vector3 LibSM64MarioState::get_velocity() const
{
    return velocity;
}

void LibSM64MarioState::set_health(int p_value)
{
    health = p_value;
}

int LibSM64MarioState::get_health() const
{
    return health;
}

void LibSM64MarioState::set_action(int p_value)
{
    action = p_value;
}

int LibSM64MarioState::get_action() const
{
    return action;
}

void LibSM64MarioState::set_flags(int p_value)
{
    flags = p_value;
}

int LibSM64MarioState::get_flags() const
{
    return flags;
}

void LibSM64MarioState::set_particle_flags(int p_value)
{
    particle_flags = p_value;
}

int LibSM64MarioState::get_particle_flags() const
{
    return particle_flags;
}

void LibSM64MarioState::set_invincibility_time(double p_value)
{
    invincibility_time = p_value;
}

double LibSM64MarioState::get_invincibility_time() const
{
    return invincibility_time;
}

void LibSM64MarioState::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("set_position", "value"), &LibSM64MarioState::set_position);
    godot::ClassDB::bind_method(godot::D_METHOD("get_position"), &LibSM64MarioState::get_position);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::VECTOR3, "position"), "set_position", "get_position");
    godot::ClassDB::bind_method(godot::D_METHOD("set_velocity", "value"), &LibSM64MarioState::set_velocity);
    godot::ClassDB::bind_method(godot::D_METHOD("get_velocity"), &LibSM64MarioState::get_velocity);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::VECTOR3, "velocity"), "set_velocity", "get_velocity");
    godot::ClassDB::bind_method(godot::D_METHOD("set_health", "value"), &LibSM64MarioState::set_health);
    godot::ClassDB::bind_method(godot::D_METHOD("get_health"), &LibSM64MarioState::get_health);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "health"), "set_health", "get_health");
    godot::ClassDB::bind_method(godot::D_METHOD("set_action", "value"), &LibSM64MarioState::set_action);
    godot::ClassDB::bind_method(godot::D_METHOD("get_action"), &LibSM64MarioState::get_action);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "action"), "set_action", "get_action");
    godot::ClassDB::bind_method(godot::D_METHOD("set_flags", "value"), &LibSM64MarioState::set_flags);
    godot::ClassDB::bind_method(godot::D_METHOD("get_flags"), &LibSM64MarioState::get_flags);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "flags"), "set_flags", "get_flags");
    godot::ClassDB::bind_method(godot::D_METHOD("set_particle_flags", "value"), &LibSM64MarioState::set_particle_flags);
    godot::ClassDB::bind_method(godot::D_METHOD("get_particle_flags"), &LibSM64MarioState::get_particle_flags);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::INT, "particle_flags"), "set_particle_flags", "get_particle_flags");
    godot::ClassDB::bind_method(godot::D_METHOD("set_invincibility_time", "value"), &LibSM64MarioState::set_invincibility_time);
    godot::ClassDB::bind_method(godot::D_METHOD("get_invincibility_time"), &LibSM64MarioState::get_invincibility_time);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "invincibility_time"), "set_invincibility_time", "get_invincibility_time");
}
