``pattern``
~~~~~~~~~~~

**tipo**: ``string``

Esta opción sólo es relevante cuando el `widget`_ se ajusta a ``choice``.
El patrón predeterminado está basado en la opción `format`_, y trata de coincidir con los caracteres ``M``, ``d``, e ``y`` en el patrón del formato. Si no hay coincidencia, el valor predeterminado es la cadena ``{{ year }}-{{ month }}-{{ day }}``.
Los elementos para esta opción son:

  * ``{{ year }}``: Reemplazado con el elemento gráfico ``year``
  * ``{{ month }}``: Reemplazado con el elemento gráfico ``month``
  * ``{{ day }}``: Reemplazado por el elemento gráfico ``day``