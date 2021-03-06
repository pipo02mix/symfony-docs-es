.. index::
   single: Formularios; Campos; datetime

Tipo de campo ``datetime``
==========================

Este tipo de campo permite al usuario modificar los datos que representan una fecha y hora específica (por ejemplo, ``05/06/2011 12:15:30``).

Se pueden reproducir como una entrada de texto o etiquetas de selección. El formato subyacente de los datos puede ser un objeto ``DateTime``, una cadena, una marca de tiempo o una matriz.

+----------------------+-----------------------------------------------------------------------------+
| Tipo de dato         | puede ser ``DateTime``, string, timestamp, o array (ve la opción            |
| subyacente           | ``input``)                                                                  |
+----------------------+-----------------------------------------------------------------------------+
| Reproducido como     | un solo campo de texto o tres cuadros de selección                          |
+----------------------+-----------------------------------------------------------------------------+
| Opciones             | - `date_widget`_                                                            |
|                      | - `time_widget`_                                                            |
|                      | - `input`_                                                                  |
|                      | - `date_format`_                                                            |
|                      | - `years`_                                                                  |
|                      | - `months`_                                                                 |
|                      | - `days`_                                                                   |
+----------------------+-----------------------------------------------------------------------------+
| Tipo del padre       | :doc:`form</reference/forms/types/form>`                                    |
+----------------------+-----------------------------------------------------------------------------+
| Clase                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateTimeType`      |
+----------------------+-----------------------------------------------------------------------------+

Opciones del campo
~~~~~~~~~~~~~~~~~~

``date_widget``
~~~~~~~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``choice``

Define la opción ``widget`` para el tipo :doc:`date </reference/forms/types/date>`

``time_widget``
~~~~~~~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``choice``

Define la opción ``widget`` para el tipo :doc:`time </reference/forms/types/time>`

``input``
~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``datetime``

El formato del dato *input* - es decir, el formato de la fecha en que se almacena en el objeto subyacente. Los valores válidos son los siguientes:

* ``string`` (p. ej. ``2011-06-05 12:15:00``)
* ``datetime`` (un objeto ``DateTime``)
* ``array`` (p. ej. ``array(2011, 06, 05, 12, 15, 0)``)
* ``timestamp`` (p. ej. ``1307276100``)

El valor devuelto por el formulario también se normaliza de nuevo a este formato.

``date_format``
~~~~~~~~~~~~~~~

**tipo**: ``integer`` o ``string`` **predeterminado**: ``IntlDateFormatter::MEDIUM``

Define la opción ``format`` que se transmite al campo de fecha.

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
