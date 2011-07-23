Url
===

Valida que un valor es una cadena URL válida.

.. code-block:: yaml

    properties:
        website:
            - Url: ~

Opciones
--------

* ``protocols``: Una lista de protocolos permitidos. Predefinido: "http", "https", "ftp" y "ftps".
* ``message``: El mensaje de error si la validación falla
