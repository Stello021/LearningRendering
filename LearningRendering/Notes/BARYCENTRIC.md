# Barycentric Coordinate System

In geometry and computer graphics, a **barycentric coordinate system** is a coordinate system 
used to express the position of a point relative to the vertices of a tringle,
or more generally a **simplex**.

---

### Simplex

A generalization of a triangle to arbitrary dimensions. 
It represents, in any given dimension, the simplest possible **Polytope**, 
the generalization of a geometrical shape in a number of dimensions
(a polygon is a **2-polytope** because bidimensional, 
while polyhedron is a **3-polytope** because tridimensional).

| Dimension of polytope | Description | Simplex      |
| --------------------- | ----------- | ------------ |
| -1                    | Nullitope   | Null         |
| 0                     | Monon       | Point        |
| 1                     | Dion        | Line Segment |
| 2                     | Polygon     | Triangle     |
| 3                     | Polyhedron  | Tetrahedron  |

---

In a 2D setting, for example, a tringle with vertices ***A***, ***B*** and ***C***,
any point ***P*** inside or outside the triangle can be represented as 
a weighted average of these vertices. The wights ate the **barycentric coordinates**
of ***P***.

### Definition

Given a triangle with vertices ***A***, ***B*** and ***C***,
the barycentryc coordinates of a point ***P*** are 3 values  $\lambda_1$, $\lambda_2$ and $\lambda_3$,
also called as **Barycentric Weights**, such that: 

$$
P = \lambda_1A + \lambda_2B + \lambda_3C
$$

where:

$$
\lambda_1 + \lambda_2 + \lambda_3 = 1
$$

so:

$$
\lambda_1 = 1 - \lambda_2 -\lambda_3 \rightarrow P = (1 - \lambda_2 - \lambda_3)A + \lambda_2B + \lambda_3C
$$

### 

### Vector-oriented perspective

Considering triangle's vertices as forming a coordinate system, the point $P$ is expressed as combination of vectors based at $A$, using the sides $\vec{AB}$ and $\vec{AC}$ as basis vectors that form an oblique basis because they are not perpendicular.

 $P$ has coordinates $(x, y)$ in the oblique basis $(A, \vec{AB}, \vec{AC})$ formed by the triangle vertices:

$$
P = A + \lambda_2\vec{AB} + \lambda_3\vec{AC} \rightarrow \vec{PA} + \lambda_2\vec{AB} + \lambda_3\vec{AC} = 0
$$

that became a linear system when vectors are broken down ino two coordinates:

$$
\begin{cases} \vec{PA}_x + \lambda_2\vec{AB}_x + \lambda_3\vec{AC}_x = 0 \\\ \vec{PA}_y + \lambda_2\vec{AB}_y + \lambda_3\vec{AC}_y = 0 \end{cases}
$$

In matrices:

$$
\begin{bmatrix} 1 & 1 & 1 \\\ \vec{PA}_x & \vec{AB}_x & \vec{AC}_x \\\ \vec{PA}_y & \vec{AB}_y & \vec{AC}_y \end{bmatrix} * \begin{bmatrix} 1 \\\ \lambda_2 \\\ \lambda_3 \end{bmatrix} = 0 \rightarrow Cross Product
$$

#### Weights as areas

Each coordinate $\lambda_A$ is proportional to the area of the triangle formed by $P$ and the two vertices opposite to vertex $A$. For example $\lambda_1$ is proportional to the area of the triangle formed by points $P$, $B$ and $C$.

#### Point Location

Barycentric coordinates are positive when point is inside the triangle, 0 when is on the edge and negative when is outside
