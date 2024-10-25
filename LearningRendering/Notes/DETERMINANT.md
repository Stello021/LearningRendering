## Determinant

A **determinant** is a scalar value that is computed from elements of a square matrix. It is a fundamental concept in linear algebra with several important properties and applications, especially related to solving systems of linear equations, matrix inversions, and transformations in geometry.

### Determinant of a 2x2 Matrix (2D Space)

$$
det(\vec{v_1}, \vec{v_2}) = \begin{pmatrix} {v_1}_x & {v_2}_x \\\ {v_1}_y & {v_2}_y \end{pmatrix} \rightarrow det(\vec{v_1}, \vec{v_2}) = ({v_1}_x * {v_2}_y) - ({v_2}_x * {v_1}_y)
$$

The determinant value gives the **signed area** of the parallelogram formed by $\vec{v_1}$ and $\vec{v_2}$, while the **sign** indicates the orientation:

- **positive** if vectors are oriented counterclockwise otherwhise are oriented clockwise

- **zero** if vectors are collinear (area is zero because parallelogram is collapsed to a line)

In other words, determinant in 2D gives both the **area** of the parallelogram and the **relative direction** of the vectors

### Determinant of a 3x3 Matrix (3D Space)

$$
det(\vec{v_1}, \vec{v_2}, \vec{v_3}) = \begin{pmatrix} {v_1}_x & {v_2}_x & {v_3}_x \\\ {v_1}_y & {v_2}_y  & {v_3}_y \\\ {v_1}_z & {v_2}_z  & {v_3}_z \end{pmatrix} \rightarrow det(\vec{v_1}, \vec{v_2}, \vec{v_3}) = {v_1}_x({v_2}_y{v_3}_z - {v_2}_z{v_3}_y) - {v_1}_y({v_2}_x{v_3}_z - {v_2}_z{v_3}_x) + {v_1}_z({v_2}_x{v_3}_y - {v_2}_y{v_3}_x)
$$

The **determinant absolute value** gives the **volume** of the parallelepiped formed by $\vec{v_1}$, $\vec{v_2}$ and $\vec{v_3}$, while the **sign** indicates the orientation:

* **positive** if vectors form a  right-handed coordinate system otherwhise left-handed

* **zero** if vectors are **coplanar** (they lie in the same plane), so parallelepiped collapses to zero volume

In other words, determinant in 2D gives both the **area** of the parallelogram and the **relative direction** of the vectors

### Properties of Determinats

#### Multiplicative Property

$$
det(matrix_1matrix_2) = det(matrix_1)det(matrix_2)
$$

#### Row Operations

- Swapping 2 rows of a matrix multiplies the determinat by $-1$

- Scaling a row by a constant scales the determinant by the same constant

- Adding a multiple of one row to another row does not change the determinant
