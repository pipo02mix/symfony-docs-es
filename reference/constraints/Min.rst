Min
===

Valida que un valor no es menor que el límite dado.

.. code-block:: yaml

    properties:
        age:
            - Min: 1

Opciones
--------

* ``limit`` (**default**, requerido): El límite
* ``message``: El mensaje de error si falla la validación
