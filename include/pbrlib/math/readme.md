# pbrlib math library

The pbrlib library provides a comprehensive set of mathematical types and operations for 3D graphics programming. Below are examples demonstrating the usage of core mathematical components.

## Table of Contents

- [Vectors](#vectors)
- [Matrices](#matrices)
- [Quaternions](#quaternions)
- [Transforms](#transforms)
- [Interpolation](#interpolation)
- [AABB](#aabb-axis-aligned-bounding-box)

## Vectors

### Vec2, Vec3, Vec4

Vectors support various data types: `float`, `double`, `int32_t`, `uint32_t`, and others.

```cpp
#include <pbrlib/math/vec3.hpp>

using namespace pbrlib::math;

// creating vectors
vec3 position(1.0f, 2.0f, 3.0f);
vec3 velocity(0.5f, 0.0f, -1.0f);

vec3 direction = normalize(position); // normalize vector

// basic operations
vec3 result     = position + velocity;  // addition
vec3 diff       = position - velocity;  // subtraction
vec3 scaled     = position * 2.0f;      // scalar multiplication
vec3 scaled2    = 2.0f * position;      // scalar multiplication (commutative)

// dot product
auto dot_product = dot(position, velocity);

// cross product
vec3 normal = cross(position, velocity);

// vector length
const auto len          = position.length();
const auto len_squared  = position.lengthSquared(); // Without square root (faster)

// normalization
position.normalize();                   // modifies the vector itself
auto normalized = normalize(position);  // returns a new normalized vector

// value clamping
const auto clamped  = clamp(position, 0.0f, 10.0f);
const auto clamped2 = clamp(position, vec3(0.0f), vec3(10.0f)); // per-component

// absolute value
vec3 absolute = abs(position);

// component access
const auto x = position.x;
const auto y = position.y;
const auto z = position.z;

// or via indexing
const auto first = position[0];
```

### Working with Colors

Vectors can be used to represent colors:

```cpp
vec3 color(1.0f, 0.5f, 0.0f); // rgb: orange

// or via color components
color.r = 0.8f;
color.g = 0.2f;
color.b = 0.6f;

constexpr vec4 rgba(1.0f, 0.5f, 0.0f, 1.0f); // rgba with alpha channel
```

### Vec2 and Vec4

Similarly to Vec3, Vec2 and Vec4 are available:

```cpp
#include <pbrlib/math/vec2.hpp>
#include <pbrlib/math/vec4.hpp>

using namespace pbrlib::math;

// vec2 for 2d operations
constexpr vec2 position2d(10.0f, 20.0f);
constexpr vec2 velocity2d(1.0f, -1.0f);

constexpr vec2 result2d = position2d + velocity2d;

// vec4 for homogeneous coordinates or rgba
constexpr vec4 homogeneous    (1.0f, 2.0f, 3.0f, 1.0f);
constexpr vec4 color_rgba     (1.0f, 0.5f, 0.0f, 0.8f); // with transparency
```

## Matrices

### Matrix4x4

4x4 matrices are used for transformations in 3d space.

```cpp
#include <pbrlib/math/matrix4x4.hpp>

using namespace pbrlib::math;

// creating matrices
constexpr mat4 identity;        // identity matrix
constexpr mat4 matrix(1.0f);    // matrix with identical diagonal values

// creating a matrix with explicit values
mat4 transform (
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
);

// matrix operations
constexpr mat4 m1;
constexpr mat4 m2;

constexpr mat4 sum        = m1 + m2;    // addition
constexpr mat4 diff       = m1 - m2;    // subtraction
constexpr mat4 product    = m1 * m2;    // matrix multiplication
constexpr mat4 scaled     = m1 * 2.0f;  // scalar multiplication

// matrix-vector multiplication
constexpr vec4 v(1.0f, 2.0f, 3.0f, 1.0f);
constexpr vec4 transformed = transform * v;

// transposition
transform.transpose();                         // modifies the matrix itself
const mat4 transposed = transpose(transform);  // returns a new matrix

// matrix inversion
transform.inverse();                       // modifies the matrix itself
const mat4 inverted = inverse(transform);  // returns a new matrix

// matrix determinant
const auto determinant = transform.det();

// element access
const auto element    = transform[0][1];    // row 0, column 1
const auto elem       = transform.at(0, 1); // alternative method
```

### Matrix2x2 and Matrix3x3

Matrices of other dimensions are also available:

```cpp
#include <pbrlib/math/matrix2x2.hpp>
#include <pbrlib/math/matrix3x3.hpp>

using namespace pbrlib::math;

constexpr mat2 matrix2x2;  // 2x2 matrix
constexpr mat3 matrix3x3;  // 3x3 matrix
```

## Quaternions

Quaternions are used to represent rotations in 3D space.

```cpp
#include <pbrlib/math/quat.hpp>

using namespace pbrlib::math;

// creating quaternions
quat rotation(0.0f, 0.0f, 0.0f, 1.0f); // (x, y, z, w)

constexpr quat identity = quat::identity(); // identity quaternion (0, 0, 0, 1)

// special quaternions
const quat i = quat::i(); // (1, 0, 0, 0)
const quat j = quat::j(); // (0, 1, 0, 0)
const quat k = quat::k(); // (0, 0, 1, 0)

// operations
constexpr quat q1;
constexpr quat q2;

constexpr quat sum        = q1 + q2;    // addition
constexpr quat diff       = q1 - q2;    // subtraction
constexpr quat product    = q1 * q2;    // quaternion multiplication (rotation composition)
constexpr quat scaled     = q1 * 2.0f;  // scalar multiplication
constexpr quat divided    = q1 / 2.0f;  // scalar division

// dot product
float dot_product = dot(q1, q2);

// normalization
rotation.normalize();                           // modifies the quaternion itself
const quat normalized = normalize(rotation);    // returns a new quaternion

// inversion
rotation.inverse();                         // modifies the quaternion itself
const quat inverted = inverse(rotation);    // returns a new quaternion

// conjugate
const quat conjugated = conjugate(rotation);

// length
const float len         = rotation.length();
const float len_squared = rotation.lengthSquared();
```

## Transforms

The library provides functions for creating transformation matrices.

```cpp
#include <pbrlib/transforms.hpp>

using namespace pbrlib::transforms;
using namespace pbrlib::math;

// translation matrix
constexpr vec3 translation(10.0f, 5.0f, 0.0f);
constexpr mat4 translate_matrix = translate(translation);

// scale matrix
constexpr vec3 scale(2.0f, 2.0f, 2.0f);
const mat4 scale_matrix = scale(scale);

// rotation matrices around axes
const mat4 rot_x = rotateX(3.14159f / 4.0f);    // 45 degrees around X
const mat4 rot_y = rotateY(3.14159f / 2.0f);    // 90 degrees around Y
const mat4 rot_z = rotateZ(3.14159f);           // 180 degrees around Z

// rotation around arbitrary axis
constexpr vec3 axis(0.0f, 1.0f, 0.0f);  // y axis
constexpr auto angle = 3.14159f / 4.0f;

const mat4 rotation_matrix = rotate(axis, angle);

// view matrix (look-at)
constexpr vec3 eye      (0.0f, 0.0f, 5.0f); // camera position
constexpr vec3 target   (0.0f, 0.0f, 0.0f); // target point
constexpr vec3 up       (0.0f, 1.0f, 0.0f); // up vector

const mat4 view_matrix = lookAt(eye, target, up);

// perspective projection matrix
constexpr auto fovy   = 3.14159f / 4.0f;    // field of view (45 degrees)
constexpr auto width  = 1920u;
constexpr auto height = 1080u;
constexpr auto z_near = 0.1f;
constexpr auto z_far  = 100.0f;

const mat4 proj_matrix = perspective(fovy, width, height, z_near, z_far);

// quaternion from angle and axis
const quat rotation_quat = angleAxis(3.14159f / 4.0f, vec3(0.0f, 1.0f, 0.0f));

// combining transformations
const mat4 model_matrix = translate(translation) * rotate(axis, angle) * scale(scale);
```

## Interpolation

The library supports linear and spherical interpolation.

```cpp
#include <pbrlib/math/lerp.hpp>

using namespace pbrlib::math;

// linear interpolation (lerp)
constexpr vec3 start  (0.0f, 0.0f, 0.0f);
constexpr vec3 end    (10.0f, 10.0f, 10.0f);

constexpr vec3 interpolated = lerp(start, end, 0.5f); // Result: (5.0, 5.0, 5.0)

// spherical interpolation (slerp) for quaternions
constexpr quat q_start = quat::identity();
constexpr quat q_end(0.0f, 1.0f, 0.0f, 0.0f);

constexpr quat q_interpolated = slerp(q_start, q_end, 0.5f);

// interpolation for other types
constexpr float f_start     = 0.0f;
constexpr float f_end       = 100.0f;
constexpr float f_result    = lerp(f_start, f_end, 0.3f); // 30.0
```

## AABB (Axis-Aligned Bounding Box)

AABB is used to represent axis-aligned bounding boxes.

```cpp
#include <pbrlib/math/aabb.hpp>

using namespace pbrlib::math;

// creating AABB
AABB box;                             // empty aabb
AABB box2(vec3(0.0f, 0.0f, 0.0f));    // aabb from a single point
AABB box3(
    vec3(-1.0f, -1.0f, -1.0f),        // minimum point
    vec3(1.0f, 1.0f, 1.0f)            // maximum point
);

// adding points to aabb
box.add(vec3(5.0f, 3.0f, 2.0f));
box.add(vec3(-2.0f, 1.0f, 0.0f));

// getting corners
vec3 corner0 = box3.corner(0); // minimum corner
vec3 corner7 = box3.corner(7); // maximum corner

// Geometric properties
vec3 diagonal = box3.diagonal();        // diagonal
float surface = box3.surfaceArea();     // surface area
float vol = box3.volume();              // volume

// empty check
bool is_empty = box.empty();

// boundary access
vec3 min_point = box3.p_min;
vec3 max_point = box3.p_max;
```

## Additional Information

For more detailed information about the implementation, see the source code in the `include/pbrlib/math/` directory. Tests can be found in `pbrlib-tests/math/`.
