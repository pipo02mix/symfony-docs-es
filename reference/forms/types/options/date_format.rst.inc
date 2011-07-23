``format``
~~~~~~~~~~

**tipo**: ``integer`` o ``string`` **predeterminado**: ``IntlDateFormatter::MEDIUM``

Opción pasada a la clase ``IntlDateFormatter``, utilizada para transformar la entrada del usuario al formato adecuado. Esto es crítico cuando la opción `widget`_ está establecida en ``single_text``, y definirá la forma de transformar la entrada. De manera predeterminada, el formato es determinado basándose en la configuración regional del usuario actual; la puedes redefinir pasando el formato como una cadena.
