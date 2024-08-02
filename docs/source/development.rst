Development
===========

Enable nanobind debug
---------------------

Set the value of environment variable ``SKBUILD_CMAKE_BUILD_TYPE`` to ``Debug``.

Debug memory issues
-------------------

e.g. ``Misaligned address error``

Use ``gdb``. For example:

.. code-block:: bash

   gdb --args python ./example/parse_ir_assmebly.py

Inside ``gdb``, do ``run`` and ``bt``.
