#!/bin/bash

echo "Compiling Project4_TextAnalyzer..."
g++ Project4_TextAnalyzer/main.cpp -o Project4_TextAnalyzer/text_analyzer

echo "Compiling Project5_SDL_Ball (requires SDL2)..."
g++ Project5_SDL_Ball/main.cpp -o Project5_SDL_Ball/sdl_ball `sdl2-config --cflags --libs` || echo "⚠️ SDL2 not installed. Skipping SDL Ball."

echo "Compiling Project6_TwoPlayerRacer (requires SDL2)..."
g++ Project6_TwoPlayerRacer/main.cpp -o Project6_TwoPlayerRacer/two_player_racer `sdl2-config --cflags --libs` || echo "⚠️ SDL2 not installed. Skipping Two Player Racer."

echo "Compiling Project7_StarWarsShooter (requires SDL2)..."
g++ Project7_StarWarsShooter/main.cpp -o Project7_StarWarsShooter/star_shooter `sdl2-config --cflags --libs` || echo "⚠️ SDL2 not installed. Skipping Star Wars Shooter."

echo "✅ Compilation completed!"
