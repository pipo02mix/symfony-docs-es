NotBlank
========

Valida que un valor no está vacío (según lo determinado por la construcción `empty
<http://mx.php.net/empty>`_).

.. code-block:: yaml

    properties:
        nombreDePila:
            - NotBlank: ~

Opciones
--------

* ``message``: El mensaje de error si la validación falla
