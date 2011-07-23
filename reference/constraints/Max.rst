Max
===

Valida que un valor no es mayor que el límite dado.

.. code-block:: yaml

    properties:
        age:
            - Max: 99

Opciones
--------

* ``limit`` (**default**, requerido): El límite
* ``message``: El mensaje de error si falla la validación
