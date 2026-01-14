.. _sec-dev-common-issues:

Common issues
=============

Invalid read (**C++ only**)
---------------------------

Referenced in Issue `307 <https://github.com/codac-team/codac/issues/307>`_

Issue
~~~~~

When doing operations on matrices and vectors, Eigen doesn't evaluate the value of the result unless it is required.

This behavior can lead to an 'invalid read' issue when the result of an operation is stored in a variable with an **auto** type.

For example the following code raises an invalid read :

.. tabs::

  .. code-tab:: c++

    Matrix m ({{0.5,0.5},{0.,1.}});
    IntervalVector b ({1.,2.});
    
    auto B1 = m.inverse()*b;
    auto B2 = inverse_enclosure(m)*b;

    auto boo = B1.is_subset(B2);  // invalid read here


Explanation
~~~~~~~~~~~

See https://libeigen.gitlab.io/eigen/docs-nightly/TopicPitfalls.html#TopicPitfalls_auto_keyword

Fix
~~~

Explicitly declare the type that you are expecting. For example the previous code becomes :

.. tabs::

  .. code-tab:: c++

    Matrix m ({{0.5,0.5},{0.,1.}});
    IntervalVector b ({1.,2.});
    
    IntervalVector B1 = m.inverse()*b;
    IntervalVector B2 = inverse_enclosure(m)*b;

    auto boo = B1.is_subset(B2);  // no more invalid read
