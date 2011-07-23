``input``
~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``datetime``

El formato del dato *input* - es decir, el formato de la fecha en que se almacena en el objeto subyacente. Los valores válidos son los siguientes:

* ``string`` (p. ej. ``2011-06-05 12:15:00``)
* ``datetime`` (un objeto ``DateTime``)
* ``array`` (p. ej. ``array(2011, 06, 05)``)
* ``timestamp`` (p. ej. ``1307232000``)

El valor devuelto por el formulario también se normaliza de nuevo a este formato.