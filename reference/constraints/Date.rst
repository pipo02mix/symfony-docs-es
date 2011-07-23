Date
====

Valida que un valor es una cadena de fecha válida con el formato "AAAA-MM-DD".

.. code-block:: yaml

    properties:
        birthday:
            - Date: ~

Opciones
--------

* ``message``: El mensaje de error si falla la validación
