#!/bin/bash

# read in current volume
VOLUME=$(wpctl get-volume @DEFAULT_AUDIO_SINK@ | sed 's/0[^0-9]//g')

EXPRESSION="<span font_desc='Roboto 100'>${VOLUME} 🎶</span>"

# generate JSON string with emoji
JSON_STRING="{\"type\": \"text\", \"expression\": \"${EXPRESSION}\", \"settings\": {\"window_timeout\": 500, \"anchor\": \"bottom-left\", \"margin_bottom\": 150}}"

echo $JSON_STRING
