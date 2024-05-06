#!/bin/bash

# Check if output.ppm exists
if [ -f output.ppm ]; then
  # Delete existing output.ppm
  rm output.ppm
  echo "Deleted existing output.ppm"
fi

# Run Raytracer and pipe output to output.ppm
./Raytracer >> output.ppm

# Informative message
echo "Raytracer output saved to output.ppm"
