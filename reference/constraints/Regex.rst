Regex
=====

Valida que un valor coincide con una expresión regular.

.. code-block:: yaml

    properties:
        titulo:
            - Regex: /\w+/

Opciones
--------

* ``pattern`` (**predefinido**, requerido): El patrón de la expresión regular
* ``match``: Si el patrón debe coincidir o no.
  Predeterminado: ``true``
* ``message``: El mensaje de error si la validación falla
