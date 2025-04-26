#!/bin/bash

echo "Compiling Project4_TextAnalyzer..."
g++ Project4_TextAnalyzer/main.cpp -o Project4_TextAnalyzer/text_analyzer

echo "Compiling Project5_SDL_Ball (requires SDL2)..."
g++ Project5_SDL_Ball/main.cpp -o Project5_SDL_Ball/sdl_ball `sdl2-config --cflags --libs` || echo "⚠️ SDL2 not installed. Skipping SDL Ball."

echo "✅ Compilation completed!"