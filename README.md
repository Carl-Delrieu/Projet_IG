# M1 Projet Informatique Graphique

## TODO:
- [ ] Bezier curves
    - [ ] Discritization
        - [x] From a given number of line segments
        - [ ] From a given length of line segment
    - [x] Visualization
- [ ] Bezier surfaces
    - [ ] Discritization
        - [x] Position
        - [x] Normals
        - [ ] (u, v) coordinates
    - [ ] Visualization
        - [x] Lambertian reflectance
        - [x] Surface normals visualization with lines
        - [ ] Surface normals visualization with colors
        - [ ] Parameter space
- [ ] Real-time rendering
    - [ ] BRDF Implementation
    - [ ] Texture support
    - [x] Three-point lighting
- [ ] BONUS
    - [ ] Interactive Bezier curve manipulation
    - [ ] Skybox


## Dépendances

-   [GLFW](https://www.glfw.org/)
-   [GLEW](http://glew.sourceforge.net/)
-   [GLM](https://glm.g-truc.net/0.9.9/index.html)

```shell
    $ sudo apt install libglfw3-dev libglew-dev libglm-dev libassimp-dev
```


## Description

### building project:
```shell
    $ cmake -S . -B build/
    $ cmake --build build/
```

### running project:

#### Bezier curves
```shell
    $ build/curve
```
#### Bezier curves with control polygon
```shell
    $ build/curve_poly
```
#### Bezier surface
```shell
    $ build/surface
```
#### Bezier surface with control polygon
```shell
    $ build/surface_poly
```
#### Bezier surface with control polygon and normals
```shell
    $ build/surface_normals
```

### how to use:
-   `Z` to move forward
-   `S` to move backward
-   `Q` to move left
-   `D` to move right
-   `Space` to move up
-   `Left Control` to move down
-   `Arrow key up` to look up
-   `Arrow key down` to look down
-   `Arrow key left` to look left
-   `Arrow key right` to look right
-   `Scroll up` to zoom in
-   `Scroll down` to zoom out
-   `Escape` to quit

## Sources

-   [Learn OpenGL](https://learnopengl.com/)


## Auteur

-   [**@Carl-Delrieu**](https://github.com/Carl-Delrieu)
