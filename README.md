
This repository contains two projects:
Software Renderer: A 3D rendering engine that supports vertex and fragment shaders, implements Gouraud and Phong lighting, and provides post-processing effects.
Ray Tracer: A physically-based renderer capable of simulating realistic materials such as Lambertian, metallic, and dielectric surfaces with anti-aliasing and multiple bounce reflections.

Features
Software Renderer
Vertex and Fragment Shader Support: Customize rendering pipelines with programmable shaders.
Post-Processing: Includes effects such as bloom, blur, or custom filters.
Primitive Rendering: Capable of rendering points, lines, and polygons.
Dependencies: Utilizes SDL for window management and glm for mathematics.
Ray Tracer
Material Types:
Lambertian: Diffuse surfaces.
Metallic: Reflective and glossy surfaces.
Dielectric: Transparent materials with refraction.
Lighting and Reflections: Supports global illumination with multiple bounce reflections.
Anti-Aliasing: Smooths jagged edges for enhanced realism.
Complex Models: Capable of rendering scenes with intricate geometries.

Technical Details
Software Renderer 
Shader Support: Implements basic vertex and fragment shader functionality. Supports custom transformations and lighting calculations.
Post-Processing:
Effects such as edge detection or Gaussian blur can be added.
<img width="619" alt="None_Culling" src="https://github.com/user-attachments/assets/5a6ebef5-e8b1-437b-a78d-5755783d025e">

Ray Tracer
Material Handling:
Lambertian surfaces for diffuse reflections.
Metallic materials for mirror-like reflections with optional fuzziness.
Dielectric materials for refraction and light bending.
Anti-Aliasing: Uses stochastic sampling to smooth edges.
Global Illumination: Simulates light interactions with multiple bounces for realistic effects.
<img width="601" alt="Scene" src="https://github.com/user-attachments/assets/dfdce097-2260-4191-8868-9d0fd47b959c">

Attribution
Libraries:
SDL2
glm

Conclusion:
Please have fun looking and exploring this github see what area you think you could improve and always try and be better. 

