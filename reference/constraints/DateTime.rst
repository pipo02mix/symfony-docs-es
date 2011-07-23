DateTime
========

Valida que un valor es una cadena de fecha y hora válida con el formato "AAAA-MM-DD HH: MM: SS".

.. code-block:: yaml

    properties:
        createdAt:
            - DateTime: ~

Opciones
--------

* ``message``: El mensaje de error si falla la validación
