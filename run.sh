#!/bin/bash

echo "Running Project4_TextAnalyzer:"
./Project4_TextAnalyzer/text_analyzer Project4_TextAnalyzer/sample.txt

echo "Running Project5_SDL_Ball (if built):"
./Project5_SDL_Ball/sdl_ball || echo "⚠️ SDL Ball not built or SDL2 not installed."

echo "✅ All projects attempted to run!"