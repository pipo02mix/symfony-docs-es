Time
====

Valida que un valor es una cadena de tiempo válida con el formato "HH:MM:SS".

.. code-block:: yaml

    properties:
        createdAt:
            - DateTime: ~

Opciones
--------

* ``message``: El mensaje de error si falla la validación
