# Basic Ray Tracer

## Overview
This project is a basic ray tracer implemented in C that renders spheres in a 3D environment using ray tracing techniques. The final output is an image in PPM format, with support for lighting, shading, and anti-aliasing.

## Features
- **Ray Tracing**: Calculates pixel colors by tracing rays from a virtual camera.
- **Lighting & Shadows**: Implements simple lighting with shading effects.
- **Anti-Aliasing**: Uses grid sampling to smooth jagged edges.
- **Dynamic Scene Management**: Reads scene information from an input file.
- **PPM Image Output**: Saves the rendered image in PPM format.

## File Structure
```
src/
|-- assg.c            # Entry point
|-- assg.h            # Function declarations
|-- vector.c          # Vector operations
|-- vector.h          # Vector function declarations
|-- spheres.c         # Sphere management
|-- spheres.h         # Sphere structure and function declarations
|-- color.c           # Color operations and HEX to RGB conversion
|-- color.h           # Color function declarations

basic-ray-tracer/
|-- Makefile          # Compilation rules
```

## Compilation
To compile the project, use the provided Makefile:
```
make
```
This will generate the executable `FS_assg`.

## Usage
Run the program with:
```
./FS_assg <input_file> <output_file>
```
where:
- `<input_file>` contains scene configuration (image size, viewport, spheres, colors, etc.).
- `<output_file>` is the output PPM image.

## Input File Format
The input file should follow this structure:
```
<image width (int)> <image height (int)>
<viewport height (float)>
<focal length (float)>
<light position (3float)> <light brightness (float)>
<number of colors `m` (int)>
<color 1 (HEX)> <color 2 (HEX)> ... <color m (HEX)>
<background color index (int)>
<number of spheres `n` (int)>
<sphere 1 position (3float)> <sphere 1 radius (float)> <sphere 1 color index (int)>
<sphere 2 position (3float)> <sphere 2 radius (float)> <sphere 2 color index (int)>
...
<sphere n position (3float)> <sphere n radius (float)> <sphere n color index (int)>
```

## Example Input
```
640 480
2.0
5.0
0.0 20.0 0.0 500.0
5
0x97C4EB 0x88CC66 0xEB6E0A 0xEBBE0A 0xEB3EDA
0
4
0.0 -41.0 -18.0 40.0 1
0.4 1.0 -10.0 0.5 3
0.6 -0.3 -10.2 0.3 4
-0.4 -0.1 -10.5 0.7 2
```

## Example Output
The program generates a PPM image representing the scene.

## Dependencies
- Standard C libraries (`stdio.h`, `stdlib.h`, `math.h`)

## Future Improvements
- Support for reflections and refractions.
- More complex shapes.
- Optimized ray-sphere intersection.

## Author
Anant Gupta

