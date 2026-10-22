C++: efficient matrix operations using Eigen
============================================

Eigen is a C++ template library for linear algebra that provides powerful tools for matrix and vector operations. One of Eigen's key features is its use of **expression templates**, which allow efficient and optimized computations. This page explains how to leverage Eigen for high-performance matrix operations.

What Are Expression Templates?
------------------------------
Expression templates are a metaprogramming technique used in Eigen to delay the evaluation of mathematical expressions. Instead of creating intermediate objects for every operation, Eigen builds an abstract representation of the computation and evaluates it only when necessary. This approach reduces memory usage and improves performance.

For example:

.. code-block:: cpp

   #include <Eigen/Dense>
   #include <iostream>

   int main() {
       Eigen::MatrixXd A = Eigen::MatrixXd::Random(3, 3);
       Eigen::MatrixXd B = Eigen::MatrixXd::Random(3, 3);
       Eigen::MatrixXd C = Eigen::MatrixXd::Random(3, 3);

       // The expression (A + B) * C is not evaluated immediately.
       Eigen::MatrixXd D = (A + B) * C;

       std::cout << "Result: \n" << D << std::endl;
       return 0;
   }

In this example, the computation `(A + B)` is not immediately performed. Instead, Eigen builds a representation of the entire expression `(A + B) * C` and optimizes it before execution.

Key Advantages
--------------
- **No Temporary Objects:** Expression templates avoid creating temporary matrices for intermediate results.
- **Optimized Operations:** Eigen combines multiple operations into a single, efficient computation.
- **Readable Code:** High-level syntax simplifies complex mathematical expressions.

Efficient Use of Eigen
----------------------
To maximize efficiency with Eigen, follow these best practices:

1. **Avoid Explicit Loops:** Use Eigen's built-in functions and operators to manipulate matrices and vectors.

   .. code-block:: cpp

      // Inefficient: Explicit loop
      for (int i = 0; i < A.rows(); ++i) {
          for (int j = 0; j < A.cols(); ++j) {
              A(i, j) += B(i, j);
          }
      }

      // Efficient: Vectorized addition
      A += B;

2. **Use Fixed-Size Matrices When Possible:** Eigen is optimized for small, fixed-size matrices.

   .. code-block:: cpp

      Eigen::Matrix3d A;  // Fixed size 3x3 matrix
      Eigen::Vector3d v;  // Fixed size 3x1 vector

3. **Leverage Lazy Evaluation:** Combine operations to benefit from expression templates.

   .. code-block:: cpp

      Eigen::MatrixXd A, B, C, D;
      D = A + B + C;  // Combined into a single operation

4. **Enable Vectorization:** Eigen can use SIMD instructions for certain operations. To ensure vectorization is enabled, define the following macro:

   .. code-block:: cpp

      #define EIGEN_USE_BLAS

Common Pitfalls
---------------
1. **Unaligned Memory:** Avoid operations on matrices with unaligned memory, as they may lead to performance degradation.
2. **Unnecessary Copies:** Avoid explicit copies of matrices unless required.

   .. code-block:: cpp

      Eigen::MatrixXd A = B + C;  // Efficient
      Eigen::MatrixXd D = A;      // Inefficient if unnecessary

Further Reading
---------------
- `Eigen Documentation <https://eigen.tuxfamily.org>`_
- `Expression Templates Explained <https://en.wikipedia.org/wiki/Expression_templates>`_

By understanding and utilizing Eigen's expression templates, you can write efficient, high-performance matrix computations that are both elegant and powerful.


Limitations in Python
----------------------

Although expression templates provide significant performance benefits in C++, they cannot be directly applied in Python due to the differences in how the two languages handle operations and memory.

In Python, operations on matrices are typically not evaluated lazily, and Python's dynamic nature does not support the same kind of compile-time optimizations that C++ offers. As a result, when performing matrix operations in Python, intermediate objects are often created, leading to potential performance bottlenecks, especially for large-scale computations.

Libraries like NumPy in Python do use optimized C extensions behind the scenes, but they rely on a different approach (such as broadcasting and memory views) to achieve performance. While NumPy can handle many operations efficiently, it does not implement expression templates in the same way as Eigen in C++.

Therefore, while the concept of expression templates is powerful in C++, it is not directly transferable to Python without significant adjustments to the language's execution model.





Using Eigen in Codac for Interval Matrices
-------------------------------------------

In the Codac library, Eigen's powerful template-based architecture is leveraged to handle matrices of intervals. By utilizing the `Interval` class from Codac, we can extend the capabilities of Eigen to represent matrices that store intervals, rather than just fixed values. This allows Codac to efficiently perform computations on uncertain or bounded data, which is common in constraint programming and optimization problems.

An `Interval` represents a range of values, typically defined by a lower and upper bound. When creating matrices of `Interval` objects, we can perform interval arithmetic directly, benefiting from Eigen’s expression templates for efficient evaluation.

For example, you can create a matrix of intervals as follows:

.. code-block:: cpp

   #include <Codac/Interval.h>
   #include <Eigen/Dense>
   #include <iostream>

   int main() {
       // Create two 3x3 matrices of intervals
       Codac::Interval<int> a(1, 3), b(2, 5), c(4, 6);
       Eigen::Matrix<Codac::Interval<int>, 3, 3> M1;
       Eigen::Matrix<Codac::Interval<int>, 3, 3> M2;

       M1 << a, b, c,
             b, c, a,
             c, a, b;

       M2 << c, a, b,
             a, b, c,
             b, c, a;

       // Perform matrix addition with interval values
       Eigen::Matrix<Codac::Interval<int>, 3, 3> M3 = M1 + M2;

       std::cout << "Matrix of Intervals M3: \n" << M3 << std::endl;
       return 0;
   }

In this example, `M1` and `M2` are matrices of `Interval` objects, and we perform matrix addition as usual. The result, `M3`, is also a matrix of intervals, where each entry is the sum of the corresponding interval values from `M1` and `M2`.

This approach allows Codac to work with uncertain or constrained data efficiently, benefiting from Eigen’s optimizations and providing robust handling of interval-based computations.




Eigen in Codac: Interval Matrices
----------------------------------

In the **Codac** library, Eigen is leveraged to efficiently handle **interval matrices** using its template-based approach. The `Interval` object in Codac represents a mathematical interval, and combining this with Eigen's matrix operations allows for the creation and manipulation of matrices that store intervals instead of exact values. This is particularly useful in fields such as constraint programming, where uncertainty or variability in data needs to be represented.

The use of Eigen’s expression templates ensures that operations on these interval matrices remain efficient, avoiding unnecessary intermediate computations.

### Example 1: Creating an Interval Matrix

.. code-block:: cpp

   #include <Codac/Eigen/Dense>  // Codac-specific headers
   #include <Eigen/Dense>
   #include <iostream>

   int main() {
       // Define an interval matrix
       Eigen::Matrix<Interval, 2, 2> M;

       // Initialize with some intervals
       M(0, 0) = Interval(1.0, 2.0);  // Interval from 1.0 to 2.0
       M(0, 1) = Interval(2.0, 3.0);  // Interval from 2.0 to 3.0
       M(1, 0) = Interval(3.0, 4.0);  // Interval from 3.0 to 4.0
       M(1, 1) = Interval(4.0, 5.0);  // Interval from 4.0 to 5.0

       std::cout << "Interval Matrix M: \n" << M << std::endl;

       return 0;
   }

In this example, `M1` and `M2` are matrices of `Interval` objects, and we perform matrix addition as usual. The result, `M3`, is also a matrix of intervals, where each entry is the sum of the corresponding interval values from `M1` and `M2`.

This approach allows Codac to work with uncertain or constrained data efficiently, benefiting from Eigen’s optimizations and providing robust handling of interval-based computations.



Operations Between a Regular Matrix and an Interval Matrix
----------------------------------------------------------

In **Codac**, it is possible to perform operations between a regular matrix (e.g., `Eigen::MatrixXd`) and an interval matrix (e.g., `Eigen::Matrix<Interval, -1, -1>`) using Eigen’s expression templates. This is particularly useful when you want to combine exact values from a standard matrix with uncertain or bounded values represented by intervals.

Eigen allows element-wise operations between matrices of different types (like `MatrixXd` and `IntervalMatrix`), and it will automatically handle the differences in data types. The result of such operations will be an interval matrix, where each element of the resulting matrix will be the corresponding interval from the interval matrix plus (or minus, or multiplied by, etc.) the corresponding value from the regular matrix.

### Example: Adding a Regular Matrix to an Interval Matrix

.. code-block:: cpp

   #include <Eigen/Dense>
   #include <Codac/Interval.h>  // Codac-specific headers
   #include <iostream>

   int main() {
       // Define a regular matrix
       Eigen::MatrixXd A(2, 2);
       A(0, 0) = 1.0;
       A(0, 1) = 2.0;
       A(1, 0) = 3.0;
       A(1, 1) = 4.0;

       // Define an interval matrix
       Eigen::Matrix<Interval, 2, 2> B;
       B(0, 0) = Interval(1.0, 2.0);  // Interval from 1.0 to 2.0
       B(0, 1) = Interval(2.0, 3.0);  // Interval from 2.0 to 3.0
       B(1, 0) = Interval(3.0, 4.0);  // Interval from 3.0 to 4.0
       B(1, 1) = Interval(4.0, 5.0);  // Interval from 4.0 to 5.0

       // Add a regular matrix to an interval matrix
       Eigen::Matrix<Interval, 2, 2> C = B + A.cast<Interval>();

       std::cout << "Result of B + A: \n" << C << std::endl;
       return 0;
   }


In this example, we perform an element-wise addition between the regular matrix A (of type Eigen::MatrixXd) and the interval matrix B (of type Eigen::Matrix<Interval, 2, 2>). Since A contains exact values, and B contains intervals, we first cast the regular matrix A to an interval matrix using A.cast<Interval>(). This operation converts each element of A into a corresponding interval, where each element is represented by an interval with a lower and upper bound equal to the value of A.

The result is stored in matrix C, which will be an interval matrix where each element represents the sum of the corresponding interval from B and the value from A (now cast to intervals).



Inversion of Interval Matrices
------------------------------

Eigen provides several algorithms for matrix inversion, such as the **LU decomposition** and **QR decomposition**, which can be applied to interval matrices. However, when working with interval matrices, the results of matrix inversion may not always be guaranteed due to the nature of interval arithmetic.

Since interval matrices represent ranges of values rather than exact numbers, the inversion of an interval matrix is inherently more complex. Inverting an interval matrix involves computing bounds for each element in the inverse matrix, which may not always be precise or reliable. Additionally, the matrix may not be invertible if it contains intervals that lead to a singular or nearly singular matrix, which can cause issues in numerical stability.

### Example: Inversion of an Interval Matrix

.. code-block:: cpp

   Eigen::Matrix<Interval, 2, 2> A;
   A(0, 0) = Interval(1.0, 2.0);
   A(0, 1) = Interval(2.0, 3.0);
   A(1, 0) = Interval(3.0, 4.0);
   A(1, 1) = Interval(4.0, 5.0);

   // Attempting to invert an interval matrix
   Eigen::Matrix<Interval, 2, 2> A_inv = A.inverse();
   std::cout << "Inverse of A: \n" << A_inv << std::endl;
