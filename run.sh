#!/bin/bash

echo "Running Project4_TextAnalyzer:"
echo Project4_TextAnalyzer/sample.txt | ./Project4_TextAnalyzer/text_analyzer

echo "Running Project5_SDL_Ball (if built):"
./Project5_SDL_Ball/sdl_ball || echo "⚠️ SDL Ball not built or SDL2 not installed."

echo "Running Project6_TwoPlayerRacer (if built):"
./Project6_TwoPlayerRacer/two_player_racer || echo "⚠️ Two Player Racer not built or SDL2 not installed."

echo "✅ All projects attempted to run!"