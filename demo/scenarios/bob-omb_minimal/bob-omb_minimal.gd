extends Node3D


@export var start_cap := SM64Mario.Caps.NORMAL

@onready var battlefield: MeshInstance3D = $Battlefield
@onready var sm_64_mario: Node3D = $SM64Mario
@onready var hud: HUD = $HUD
@onready var sm_64_audio_stream_player: SM64AudioStreamPlayer = $SM64AudioStreamPlayer

func _ready() -> void:
	SM64Global.scale_factor = 75
	SM64Global.init()

	battlefield.mesh = BombOmbMinimalSurfaces.generate_godot_mesh()
	battlefield.mesh.surface_set_material(0, preload("res://demo/scenarios/bob-omb_minimal/bob-omb_minimal_material.tres"))
	BombOmbMinimalSurfaces.load_static_surfaces()

	sm_64_mario.create()
	sm_64_mario.interact_cap(start_cap)

	hud.mario = sm_64_mario

	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	
	sm_64_audio_stream_player.play_music(SM64AudioStreamPlayer.SEQ_ID_LEVEL_GRASS)
	sm_64_audio_stream_player.play()


func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("toggle_mouse_lock"):
		_toggle_mouse_lock()


func _toggle_mouse_lock() -> void:
	if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _on_tree_exiting():
	sm_64_audio_stream_player.stop_background_music(SM64AudioStreamPlayer.SEQ_ID_LEVEL_GRASS)
	sm_64_mario.delete()
	SM64Global.terminate()
