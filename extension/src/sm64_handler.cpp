#include <sm64_handler.hpp>

#include <string>
#include <cstdio>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/array_mesh.hpp>


static struct SM64TextureAtlasInfo mario_texture_atlas_info = {
    .offset             = 0x114750,
    .numUsedTextures    = 11,
    .atlasWidth         = 11*64,
    .atlasHeight        = 64,
    .texInfos = {
        { .offset = 144, .width = 64, .height = 32, .format = FORMAT_RGBA },
        { .offset = 4240, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 6288, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 8336, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 10384, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 12432, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 14480, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 16528, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 30864, .width = 32, .height = 32, .format = FORMAT_RGBA },
        { .offset = 32912, .width = 32, .height = 64, .format = FORMAT_RGBA },
        { .offset = 37008, .width = 32, .height = 64, .format = FORMAT_RGBA },
    }
};

static void SM64DebugPrintFunction(const char *msg)
{
    godot::UtilityFunctions::print(godot::String("[libsm64] ") + godot::String(msg) + godot::String("\n"));
}

static uint8_t *utils_read_file_alloc(const char *path, size_t *fileLength)
{
    FILE *f = fopen(path, "rb");

    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    size_t length = (size_t)ftell(f);
    rewind(f);
    uint8_t *buffer = (uint8_t *)malloc(length + 1);
    fread(buffer, 1, length, f);
    buffer[length] = 0;
    fclose(f);

    if (fileLength)
        *fileLength = length;

    return buffer;
}

static bool check_in_bounds(real_t num)
{
    constexpr real_t bounds = 0x7FFF;
    return (num > -bounds && num < bounds);
}

static bool check_in_bounds(const godot::Vector3 &vec)
{
    return check_in_bounds(vec.x) && check_in_bounds(vec.y) && check_in_bounds(vec.z);
}

static void invert_vertex_order_2d(float *arr, size_t triangle_count)
{
    for (size_t i = 0; i < triangle_count * 6; i += 6)
    {
        float temp_x = arr[i+0];
        float temp_y = arr[i+1];
        arr[i+0] = arr[i+2];
        arr[i+1] = arr[i+3];
        arr[i+2] = temp_x;
        arr[i+3] = temp_y;
    }
}


static void invert_vertex_order_3d(float *arr, size_t triangle_count)
{
    for (size_t i = 0; i < triangle_count * 9; i += 9)
    {
        float temp_x = arr[i+0];
        float temp_y = arr[i+1];
        float temp_z = arr[i+2];
        arr[i+0] = arr[i+3];
        arr[i+1] = arr[i+4];
        arr[i+2] = arr[i+5];
        arr[i+3] = temp_x;
        arr[i+4] = temp_y;
        arr[i+5] = temp_z;
    }
}

static void invert_vertex_order(godot::PackedVector3Array &arr)
{
    for (size_t i = 0; i < arr.size() / 3; i++)
    {
        godot::Vector3 temp = arr[3*i+0];
        arr.set(3*i+0, arr[3*i+1]);
        arr.set(3*i+1, temp);
    }
}

SM64Handler::SM64Handler()
{
    mario_geometry.position = (float*) malloc(sizeof(float) * 9 * SM64_GEO_MAX_TRIANGLES);
    mario_geometry.color    = (float*) malloc(sizeof(float) * 9 * SM64_GEO_MAX_TRIANGLES);
    mario_geometry.normal   = (float*) malloc(sizeof(float) * 9 * SM64_GEO_MAX_TRIANGLES);
    mario_geometry.uv       = (float*) malloc(sizeof(float) * 6 * SM64_GEO_MAX_TRIANGLES);
}

SM64Handler::~SM64Handler()
{
    sm64_global_terminate();

    if (mario_geometry.position)
        ::free(mario_geometry.position);
    if (mario_geometry.color)
        ::free(mario_geometry.color);
    if (mario_geometry.normal)
        ::free(mario_geometry.normal);
    if (mario_geometry.uv)
        ::free(mario_geometry.uv);
}

void SM64Handler::global_init()
{
    size_t rom_size;
    uint8_t *rom = utils_read_file_alloc(rom_filename.ascii().get_data(), &rom_size);

    if (rom == NULL)
    {
        godot::UtilityFunctions::print(godot::String("[SM64Handler] Failed to read ROM file: ") + rom_filename);
        return;
    }

    uint8_t *mario_texture_raw = (uint8_t *) malloc(4 * SM64_TEXTURE_WIDTH * SM64_TEXTURE_HEIGHT);

    sm64_global_init(rom, SM64DebugPrintFunction);
    sm64_texture_load(rom, &mario_texture_atlas_info, mario_texture_raw);

    godot::PackedByteArray mario_texture_packed;
    mario_texture_packed.resize(4 * SM64_TEXTURE_WIDTH * SM64_TEXTURE_HEIGHT);
    for (size_t i = 0; i < 4 * SM64_TEXTURE_WIDTH * SM64_TEXTURE_HEIGHT; i++)
        mario_texture_packed.set(i, mario_texture_raw[i]);

    mario_image.instantiate();
    mario_image->create_from_data(SM64_TEXTURE_WIDTH, SM64_TEXTURE_HEIGHT, false, godot::Image::FORMAT_RGBA8, mario_texture_packed);

    init = true;

    ::free(rom);
    ::free(mario_texture_raw);
}

bool SM64Handler::is_init() const
{
    return init;
}

godot::Ref<godot::Image> SM64Handler::get_mario_image() const
{
    return mario_image;
}

void SM64Handler::set_rom_filename(const godot::String &value)
{
    rom_filename = value;
}

godot::String SM64Handler::get_rom_filename() const
{
    return rom_filename;
}

void SM64Handler::set_scale_factor(real_t value)
{
    scale_factor = value;
}

real_t SM64Handler::get_scale_factor() const
{
    return scale_factor;
}

void SM64Handler::static_surfaces_load(godot::PackedVector3Array vertexes, godot::TypedArray<SM64SurfaceProperties> surface_properties_array)
{
    struct SM64Surface *surface_array = (SM64Surface *) malloc(sizeof(SM64Surface) * vertexes.size() / 3);
    godot::Ref<SM64SurfaceProperties> default_surface_properties;

    default_surface_properties.instantiate();

    if (surface_properties_array.size() != vertexes.size() / 3)
        surface_properties_array.resize(vertexes.size() / 3);

    invert_vertex_order(vertexes);

    uint32_t j = 0;
    for (size_t i = 0; i < vertexes.size(); i += 3)
    {
        if (!check_in_bounds(vertexes[i] * scale_factor)
                || !check_in_bounds(vertexes[i+1] * scale_factor)
                || !check_in_bounds(vertexes[i+2] * scale_factor))
            continue;

        godot::Ref<SM64SurfaceProperties> surface_properties = surface_properties_array[i/3];
        if (surface_properties.is_null())
            surface_properties = default_surface_properties;

        surface_array[j].type = (int16_t) surface_properties->get_surface_type();
        surface_array[j].force = (int16_t) surface_properties->get_force();
        surface_array[j].terrain = (uint16_t) surface_properties->get_terrain_type();
        surface_array[j].vertices[0][0] = (int32_t) ( vertexes[i+0].z * scale_factor);
        surface_array[j].vertices[0][1] = (int32_t) ( vertexes[i+0].y * scale_factor);
        surface_array[j].vertices[0][2] = (int32_t) (-vertexes[i+0].x * scale_factor);
        surface_array[j].vertices[1][0] = (int32_t) ( vertexes[i+1].z * scale_factor);
        surface_array[j].vertices[1][1] = (int32_t) ( vertexes[i+1].y * scale_factor);
        surface_array[j].vertices[1][2] = (int32_t) (-vertexes[i+1].x * scale_factor);
        surface_array[j].vertices[2][0] = (int32_t) ( vertexes[i+2].z * scale_factor);
        surface_array[j].vertices[2][1] = (int32_t) ( vertexes[i+2].y * scale_factor);
        surface_array[j].vertices[2][2] = (int32_t) (-vertexes[i+2].x * scale_factor);

        j++;
    }

    sm64_static_surfaces_load(surface_array, j);

    ::free(surface_array);
}

int SM64Handler::mario_create(godot::Vector3 position, godot::Vector3 rotation)
{
    if (!check_in_bounds(position))
        return -2;

    float x = (float) ( position.z * scale_factor);
    float y = (float) ( position.y * scale_factor);
    float z = (float) (-position.x * scale_factor);
    int16_t rx = (int16_t)  godot::Math::rad2deg(rotation.z);
    int16_t ry = (int16_t)  godot::Math::rad2deg(rotation.y);
    int16_t rz = (int16_t) -godot::Math::rad2deg(rotation.x);

    return sm64_mario_create(x, y, z, rx, ry, rz, 0);
}

godot::Dictionary SM64Handler::mario_tick(int mario_id, godot::Ref<SM64Input> input)
{
    godot::Dictionary ret;
    godot::Array mesh_array;
    struct SM64MarioState out_state;

    godot::Vector2 cam_look = input->get_cam_look();
    godot::Vector2 stick = input->get_stick();

    struct SM64MarioInputs mario_inputs = {
        cam_look.y, // camLookX
        -cam_look.x, // camLookZ
        stick.x,
        stick.y,
        (uint8_t) input->get_a(),
        (uint8_t) input->get_b(),
        (uint8_t) input->get_z()
    };

    sm64_mario_tick(mario_id, &mario_inputs, &out_state, &mario_geometry);

    ret["position"]   = godot::Vector3(-out_state.position[2] / scale_factor, out_state.position[1] / scale_factor, out_state.position[0] / scale_factor);
    ret["velocity"]   = godot::Vector3(-out_state.velocity[2] / scale_factor, out_state.velocity[1] / scale_factor, out_state.velocity[0] / scale_factor);
    ret["face_angle"] = (real_t) out_state.faceAngle;
    ret["health"]     = (int) out_state.health;

    {
        size_t array_size = mario_geometry.numTrianglesUsed * 3;

        if (mesh_array.size() != godot::ArrayMesh::ARRAY_MAX)
            mesh_array.resize(godot::ArrayMesh::ARRAY_MAX);
        if (mario_position.size() != array_size)
            mario_position.resize(array_size);
        if (mario_normal.size() != array_size)
            mario_normal.resize(array_size);
        if (mario_color.size() != array_size)
            mario_color.resize(array_size);
        if (mario_uv.size() != array_size)
            mario_uv.resize(array_size);
    }


    invert_vertex_order_3d(mario_geometry.position, mario_geometry.numTrianglesUsed);
    invert_vertex_order_3d(mario_geometry.normal, mario_geometry.numTrianglesUsed);
    invert_vertex_order_3d(mario_geometry.color, mario_geometry.numTrianglesUsed);
    invert_vertex_order_2d(mario_geometry.uv, mario_geometry.numTrianglesUsed);

    for (int i = 0; i < mario_geometry.numTrianglesUsed; i++)
    {
        godot::Vector3 positions[3];
        positions[0].z =  mario_geometry.position[9*i+0] / scale_factor;
        positions[0].y =  mario_geometry.position[9*i+1] / scale_factor;
        positions[0].x = -mario_geometry.position[9*i+2] / scale_factor;
        positions[1].z =  mario_geometry.position[9*i+3] / scale_factor;
        positions[1].y =  mario_geometry.position[9*i+4] / scale_factor;
        positions[1].x = -mario_geometry.position[9*i+5] / scale_factor;
        positions[2].z =  mario_geometry.position[9*i+6] / scale_factor;
        positions[2].y =  mario_geometry.position[9*i+7] / scale_factor;
        positions[2].x = -mario_geometry.position[9*i+8] / scale_factor;
        mario_position.set(3*i+0, positions[0]);
        mario_position.set(3*i+1, positions[1]);
        mario_position.set(3*i+2, positions[2]);

        godot::Vector3 normals[3];
        normals[0].z =  mario_geometry.normal[9*i+0];
        normals[0].y =  mario_geometry.normal[9*i+1];
        normals[0].x = -mario_geometry.normal[9*i+2];
        normals[1].z =  mario_geometry.normal[9*i+3];
        normals[1].y =  mario_geometry.normal[9*i+4];
        normals[1].x = -mario_geometry.normal[9*i+5];
        normals[2].z =  mario_geometry.normal[9*i+6];
        normals[2].y =  mario_geometry.normal[9*i+7];
        normals[2].x = -mario_geometry.normal[9*i+8];
        mario_normal.set(3*i+0, normals[0]);
        mario_normal.set(3*i+1, normals[1]);
        mario_normal.set(3*i+2, normals[2]);

        mario_color.set(3*i+0, godot::Color(mario_geometry.color[9*i+0], mario_geometry.color[9*i+1], mario_geometry.color[9*i+2]));
        mario_color.set(3*i+1, godot::Color(mario_geometry.color[9*i+3], mario_geometry.color[9*i+4], mario_geometry.color[9*i+5]));
        mario_color.set(3*i+2, godot::Color(mario_geometry.color[9*i+6], mario_geometry.color[9*i+7], mario_geometry.color[9*i+8]));

        mario_uv.set(3*i+0, godot::Vector2(mario_geometry.uv[6*i+0], mario_geometry.uv[6*i+1]));
        mario_uv.set(3*i+1, godot::Vector2(mario_geometry.uv[6*i+2], mario_geometry.uv[6*i+3]));
        mario_uv.set(3*i+2, godot::Vector2(mario_geometry.uv[6*i+4], mario_geometry.uv[6*i+5]));
    }

    mesh_array[godot::ArrayMesh::ARRAY_VERTEX] = mario_position;
    mesh_array[godot::ArrayMesh::ARRAY_NORMAL] = mario_normal;
    mesh_array[godot::ArrayMesh::ARRAY_COLOR] = mario_color;
    mesh_array[godot::ArrayMesh::ARRAY_TEX_UV] = mario_uv;

    ret["mesh_array"] = mesh_array;

    return ret;
}

void SM64Handler::mario_delete(int mario_id)
{
    sm64_mario_delete(mario_id);
}

void SM64Handler::set_mario_position(int mario_id, godot::Vector3 position)
{
    sm64_set_mario_position(mario_id,
                            position.z * scale_factor,
                            position.y * scale_factor,
                            -position.x * scale_factor);
}

void SM64Handler::set_mario_angle(int mario_id, real_t angle)
{
    sm64_set_mario_angle(mario_id, angle);
}


int SM64Handler::surface_object_create(godot::PackedVector3Array vertexes, godot::Vector3 position, godot::Vector3 rotation, godot::TypedArray<SM64SurfaceProperties> surface_properties_array)
{
    struct SM64SurfaceObject surface_object;
    int id;
    struct SM64Surface *surface_array = (SM64Surface *) malloc(sizeof(SM64Surface) * vertexes.size() / 3);
    godot::Ref<SM64SurfaceProperties> default_surface_properties;

    default_surface_properties.instantiate();

    if (surface_properties_array.size() != vertexes.size() / 3)
        surface_properties_array.resize(vertexes.size() / 3);

    invert_vertex_order(vertexes);

    uint32_t j = 0;
    for (size_t i = 0; i < vertexes.size(); i += 3)
    {
        if (!check_in_bounds(vertexes[i] * scale_factor)
                || !check_in_bounds(vertexes[i+1] * scale_factor)
                || !check_in_bounds(vertexes[i+2] * scale_factor))
            continue;

        godot::Ref<SM64SurfaceProperties> surface_properties = surface_properties_array[i/3];
        if (surface_properties.is_null())
            surface_properties = default_surface_properties;

        surface_array[j].type = (int16_t) surface_properties->get_surface_type();
        surface_array[j].force = (int16_t) surface_properties->get_force();
        surface_array[j].terrain = (uint16_t) surface_properties->get_terrain_type();
        surface_array[j].vertices[0][0] = (int32_t) ( vertexes[i+0].z * scale_factor);
        surface_array[j].vertices[0][1] = (int32_t) ( vertexes[i+0].y * scale_factor);
        surface_array[j].vertices[0][2] = (int32_t) (-vertexes[i+0].x * scale_factor);
        surface_array[j].vertices[1][0] = (int32_t) ( vertexes[i+1].z * scale_factor);
        surface_array[j].vertices[1][1] = (int32_t) ( vertexes[i+1].y * scale_factor);
        surface_array[j].vertices[1][2] = (int32_t) (-vertexes[i+1].x * scale_factor);
        surface_array[j].vertices[2][0] = (int32_t) ( vertexes[i+2].z * scale_factor);
        surface_array[j].vertices[2][1] = (int32_t) ( vertexes[i+2].y * scale_factor);
        surface_array[j].vertices[2][2] = (int32_t) (-vertexes[i+2].x * scale_factor);

        j++;
    }

    surface_object.surfaces = surface_array;
    surface_object.surfaceCount = j;

    surface_object.transform.position[0] =  position.z * scale_factor;
    surface_object.transform.position[1] =  position.y * scale_factor;
    surface_object.transform.position[2] = -position.x * scale_factor;

    surface_object.transform.eulerRotation[0] = -godot::Math::rad2deg(rotation.z);
    surface_object.transform.eulerRotation[1] = -godot::Math::rad2deg(rotation.y);
    surface_object.transform.eulerRotation[2] =  godot::Math::rad2deg(rotation.x);

    id = sm64_surface_object_create(&surface_object);

    ::free(surface_array);

    return id;
}

void SM64Handler::surface_object_move(int object_id, godot::Vector3 position, godot::Vector3 rotation)
{
    struct SM64ObjectTransform transform;

    transform.position[0] =  position.z * scale_factor;
    transform.position[1] =  position.y * scale_factor;
    transform.position[2] = -position.x * scale_factor;

    transform.eulerRotation[0] = -godot::Math::rad2deg(rotation.z);
    transform.eulerRotation[1] = -godot::Math::rad2deg(rotation.y);
    transform.eulerRotation[2] =  godot::Math::rad2deg(rotation.x);

    sm64_surface_object_move(object_id, &transform);
}

void SM64Handler::surface_object_delete(int object_id)
{
    sm64_surface_object_delete(object_id);
}

void SM64Handler::_bind_methods()
{
    godot::ClassDB::bind_method(godot::D_METHOD("global_init"), &SM64Handler::global_init);
    godot::ClassDB::bind_method(godot::D_METHOD("is_init"), &SM64Handler::is_init);
    godot::ClassDB::bind_method(godot::D_METHOD("get_mario_image"), &SM64Handler::get_mario_image);
    godot::ClassDB::bind_method(godot::D_METHOD("set_rom_filename", "value"), &SM64Handler::set_rom_filename);
    godot::ClassDB::bind_method(godot::D_METHOD("get_rom_filename"), &SM64Handler::get_rom_filename);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::STRING, "rom_filename", godot::PROPERTY_HINT_GLOBAL_FILE, "*.n64,*.z64,"), "set_rom_filename", "get_rom_filename");
    godot::ClassDB::bind_method(godot::D_METHOD("set_scale_factor", "value"), &SM64Handler::set_scale_factor);
    godot::ClassDB::bind_method(godot::D_METHOD("get_scale_factor"), &SM64Handler::get_scale_factor);
    ADD_PROPERTY(godot::PropertyInfo(godot::Variant::FLOAT, "scale_factor"), "set_scale_factor", "get_scale_factor");
    godot::ClassDB::bind_method(godot::D_METHOD("static_surfaces_load", "vertexes", "surface_properties_array"), &SM64Handler::static_surfaces_load);
    godot::ClassDB::bind_method(godot::D_METHOD("mario_create", "position", "rotation"), &SM64Handler::mario_create);
    godot::ClassDB::bind_method(godot::D_METHOD("mario_tick", "mario_id", "input"), &SM64Handler::mario_tick);
    godot::ClassDB::bind_method(godot::D_METHOD("mario_delete", "mario_id"), &SM64Handler::mario_delete);
    godot::ClassDB::bind_method(godot::D_METHOD("set_mario_position", "mario_id", "position"), &SM64Handler::set_mario_position);
    godot::ClassDB::bind_method(godot::D_METHOD("set_mario_angle", "mario_id", "angle"), &SM64Handler::set_mario_angle);
    godot::ClassDB::bind_method(godot::D_METHOD("surface_object_create", "vertexes", "position", "rotation", "surface_properties_array"), &SM64Handler::surface_object_create);
    godot::ClassDB::bind_method(godot::D_METHOD("surface_object_move", "object_id", "position", "rotation"), &SM64Handler::surface_object_move);
    godot::ClassDB::bind_method(godot::D_METHOD("surface_object_delete", "object_id"), &SM64Handler::surface_object_delete);
}