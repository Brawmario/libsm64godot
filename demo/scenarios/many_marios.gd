extends Node3D

func _ready() -> void:
	if not LibSM64Global.init():
		push_error("Failed to initialize LibSM64Global")
		return

	$LibSM64StaticSurfacesHandler.load_static_surfaces()
	$LibSM64SurfaceObjectsHandler.load_all_surface_objects()

	for node in get_tree().get_nodes_in_group("libsm64_mario"):
		var mario := node as LibSM64Mario
		if not mario:
			continue
		mario.create()
		mario.water_level = 0.0

	$LibSM64AudioStreamPlayer.play()


func _on_tree_exiting() -> void:
	LibSM64.stop_background_music(LibSM64.get_current_background_music())
	for node in get_tree().get_nodes_in_group("libsm64_mario"):
		var mario := node as LibSM64Mario
		if not mario:
			continue
		mario.delete()
	LibSM64Global.terminate()
