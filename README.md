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
- The Map
- - [x] The lake
- - [x] The beach
---
- Gameplay
- - [x] Player controller?
- - [x] Fishing
- - [x] Day and night cycle
- - [x] Sleeping
- - [x] Dialog System
- - [x] Shop system
- - [x] Inventory
- - [x] Fishing rod upgrades
- - [x] Daily Events
- - - [x] Monday
- - - [x] Tuesday
- - - [x] Wednesday
- - - [x] Thursday
- - - [x] Friday
- - - [x] Saturday
- - - [x] Sunday
- - [x] Tutorial
- - [x] Debt Collector.
- - [x] Ending
- - - [x] Ending 1
- - - [x] Ending 2
- - - [x] Ending 3
---
- Sound
- - [x] Play sounds
- - [ ] random Ambient sound effects.

---
- Misc.
- - [ ] Saving and loading
- - [ ] Settings.
---
- Fix Bugs
- - [x] Fish not counting after getting caught.
- - [x] Selling not working.
- - [x] Stamina bars not shrinking properly.
- - [x] DebtCollector not showing up
- - [x] Resizing.
