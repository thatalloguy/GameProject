# DuckWatchers Remake
![concept](https://github.com/user-attachments/assets/1d66660f-a70d-4f0d-8fe7-cf480b367e0e)
###### ( Concept art made in blender )

### Dependencies
- Fastgltf
- glfw
- glm
- imgui
- Jolt
- MiniAudio
- SteamAudio
- spdlog
- stb (image)
- vk-bootstrap
- VulkanMemoryAllocator
- Tweeny


### Planning:

----

- Window
- - [x] Window creation
- - [x] darkmode titlebar (windows only)
- - [x] Resizing (NOTE: resizing isnt finished on the render renderer side).
- - [x] Fullscreen + borderless
----
- Input
- - [x] Keyboard
- - [x] Controller
- - [x] Mouse
----
- Physics
- - [x] Setup
- - [x] Rigid body creation.
- - [x] static Bodies?
- - [x] Collision shapes (including triangles?)
- - [x] Character controller
- - [x] Triggers
----

- Audio:
- - [x] Loading sound
- - [x] Binaural Effect.
- - [x] Spatial effect.

----
- Map Editor
- - [x] Loading assets from json
- - [x] Loading blueprints (entity templates) from json
- - [x] Creating instances of blueprints.
- - [x] Saving instances to a file.
- - [x] Loading instances from a file.
- - [x] Hot reloading Assets and blueprints.
- - [x] Moving entities (via debug window)
- - [x] Free fly camera.
- - [x] Runtime map loading.
- - **Animator**:
- - - [x] Keyframes.
- - - [x] Interpolating between keyframes.
- - - [x] Serialization. 
---
- Gameplay
- - [x] Player controller?
- - [x] Fishing
- - [ ] Shop system
- - [ ] Day and night cycle
- - [ ] Event player.
- - [ ] Fishing rod upgrades

---
- Rendering part 2:
- - [ ] Shadows
- - [ ] Physics debug renderer (optional)
---
- Polish phase
