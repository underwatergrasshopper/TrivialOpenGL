# v0.2.3 (05-02-2024)
* Updated date in LICENSE.
* Fixed bug where temporal OpenGL Rendering Context was not destroyed.
# v0.2.2 (16-08-2023)
* Added `glPushAttrib` and `glPopAttrib` to examples.
* Changed callback call order to make `do_on_create` first callback called which is more useful.
  Creating window do not call other callbacks than `do_on_create`, now.
* Removed redundant declarations of method.
* Fixed `TOGL_TextDrawer::Reset.
* Renamed `TOGL_Extra.keybaord_side` to `TOGL_Extra.keybaord_side_id`.
* Updated README.md.
* Added missing descriptions.
* Fixed many minor bugs.
* Fixed wrong log level in `Window` class.
* Corrected `WM_ToStr`.
* Moved creating time to better place in code.
* Refactored `Window::Center` method.
* Fixed wrong flags for `togl_wglCreateContextAttribsARB`.
* Fixed bug with undefined `WM_` constants.
* Added alternative way to get opengl version for created context.
* Fixed error handling in `TOGL_FontGenerator`.
* Fixed missing checks in `TOGL_TextAdjuster::SplitSentenceToParts` method.
* Added `draw_array` example.
* Fixed missing call of `Initialize` method in `TOGL_FontDataGenerator` constructor.
* Optimized `TOGL_FontDataGenerator`.
* Fixed `Window::RequestClose` method to work as intended.
* Removed redundant member variables.
# v0.2.1 (04-06-2023)
- Added missing descriptions.
# v0.2.0 (04-06-2023)
- Changed TOGL_FineText, TOGL_FineTextElementContainer and its dependencies.
# v0.1.0r2 (23-05-2023)
- Changed minimal language version to C++17.