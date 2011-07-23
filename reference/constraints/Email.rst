Correo
======

Valida que un valor es una dirección de correo electrónico válida.

.. code-block:: yaml

    properties:
        email:
            - Email: ~

Opciones
--------

* ``checkMX``: Si se deben revisar los registros MX para el dominio. predeterminado: ``false``
* ``message``: El mensaje de error si la validación falla
