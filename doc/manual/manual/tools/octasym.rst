.. _sec-tools-octasym:

Octahedral symmetries
=====================

In Codac, the class :class:`OctaSym` is used to represent the symmetries of the hypercube, which are elements of the hyperoctahedral group. This class provides methods to create, manipulate, and apply these symmetries to objects.

Hyperoctahedral group
---------------------

The hyperoctahedral :math:`B_n` group is the group of symmetries of the hypercube :math:`[-1,1]^n`. It contains :math:`2^n \cdot n!` elements.

It is a signed symmetric group of permutations. It means that each symmetry of the hypercube can be represented by a 
permutation of the coordinates, and a sign for each coordinate (i.e., a reflection or not).

Two main representations of the hyperoctahedral group exist:

- The **Cauchy's representation**: It is a two-row representation where the first row contains the indices of the coordinates, and the second row contains the new indices after the permutation, with a sign indicating whether there is a reflection or not.

- The **Permutation representation**: It is a :math:`n\times n` matrix representation where each row and column has exactly one non-zero entry, which is either +1 or -1, indicating the permutation and reflection.

For example, in dimension three the same symmetry :math:`\sigma` can be written in Cauchy's representation as:

.. math:: 

   \sigma = \begin{pmatrix}
   1 & 2 & 3 \\
   3 & -1 & 2
   \end{pmatrix}

And in permutation representation as:

.. math:: 

   P_{\sigma} = \begin{pmatrix}
   0 & 0 & 1 \\
   -1 & 0 & 0 \\
   0 & 1 & 0
   \end{pmatrix}

If we apply the symmetry :math:`\sigma` to a point :math:`\mathbf{x} = \begin{pmatrix} x_1 \\ x_2 \\ x_3 \end{pmatrix}`, we get:

.. math:: 

   \sigma(\mathbf{x}) = P_{\sigma}\cdot \mathbf{x} = \begin{pmatrix} x_3 \\ -x_1 \\ x_2 \end{pmatrix}


Codac implementation
--------------------

The main representation used in Codac is the Cauchy's notation. **We suppose that the first row of the cauchy's representation is in increasing order, i.e. (1,2,...n)**.

**Note that the indexing starts at 1 as 0 and -0 can not be distinguished**.

The constructors of the :class:`OctaSym` class then require a single vector reprensenting the second row of the Cauchy's representation.

For example, the symmetry :math:`\sigma` mentionned earlier can be created in Codac as follows:

.. tabs::
  
  .. code-tab:: py

    sigma = OctaSym([3,-1,2])

  .. code-tab:: c++

    OctaSym sigma ({3,-1,2});

  .. code-tab:: matlab

    sigma = OctaSym({3,-1,2});

The permutation matrix can be recovered using the method `permutation_matrix()`. It returns the matrix :math:`P_{\sigma}` as a :class:`Matrix` object 

.. tabs::
  
  .. code-tab:: py

    P_sigma = sigma.permutation_matrix()

  .. code-tab:: c++

    Matrix P_sigma = sigma.permutation_matrix();

  .. code-tab:: matlab

    P_sigma = sigma.permutation_matrix();

The inverse of a symmetry can be computed using the method `invert()`. These symmetries can also be composed using the multiplication operator `*`.

.. tabs::
  
  .. code-tab:: py

    sigma_inv = sigma.invert()
    sigma_comp = sigma * sigma_inv  # identity symmetry

  .. code-tab:: c++

    OctaSym sigma_inv = sigma.invert();
    OctaSym sigma_comp = sigma * sigma_inv; // identity symmetry

  .. code-tab:: matlab

    sigma_inv = sigma.invert();
    sigma_comp = sigma * sigma_inv; % identity symmetry

To apply the symmetry to an object, the classical operator `()` is used. Objects which support the application of a symmetry are listed below.
**Objects in bold are not supported yet but will be in the future**.

- Vectors

  - :class:`Vector`. :ref:`see more <sec-linear-vecmat>`
  - :class:`IntervalVector`. :ref:`see more <sec-intervals-intervalvector-class>`
  - :class:`VectorVar` and :class:`VectorExpr` (for functions). :ref:`see more <sec-functions-analytic-class>`

- Contractors and separators. :ref:`see more <sec-ctc>`

- **Analytic** and sampled trajectories. :class:`SampledTraj`.

- **Sliced tubes**.


For example, with the symmetry :math:`\sigma` defined earlier:

.. tabs::
  
  .. code-tab:: py

    X = Vector([4,5,6])
    X_s = sigma(X)  # X_s is the (poncual) interval vector (6,-4,5)

    x = VectorVar(3)    
    f = AnalyticFunction([x], sigma(2*x))
    Y = f.eval(X) # Y is the (poncual) interval vector (12,-8,10)

  .. code-tab:: c++
      
    Vector X ({4,5,6});
    Vector X_s = sigma(X); // X_s is (poncual) interval the vector (6,-4,5)

    VectorVar x(3);    
    AnalyticFunction f({x}, sigma(2*x));
    auto Y = f.eval(X); // Y is the (poncual) interval vector (12,-8,10)

  .. code-tab:: matlab

    X = Vector({4,5,6});
    X_s = sigma(X); % X_s is the (poncual) interval vector (6,-4,5)

    x = VectorVar(3);
    f = AnalyticFunction({x}, sigma(2*x));
    Y = f.eval(X); % Y is the (poncual) interval vector (12,-8,10)