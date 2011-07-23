.. index::
   single: Formularios; Campos; date

Tipo de campo date
==================

Un campo que permite al usuario modificar información de fecha a través de una variedad de diferentes elementos HTML.

Los datos subyacentes utilizados para este tipo de campo pueden ser un objeto ``DateTime``, una cadena, una marca de tiempo o una matriz. Siempre y cuando la opción `input`_ se configure correctamente, el campo se hará cargo de todos los detalles.

El campo se puede reproducir como un cuadro de texto, tres cuadros de texto (mes, día y año) o tres cuadros de selección (ve la opción `widget`_).

+----------------------+-----------------------------------------------------------------------------+
| Tipo de dato         | puede ser ``DateTime``, string, timestamp, o array (consulta la opción      |
| subyacente           | ``input``)                                                                  |
+----------------------+-----------------------------------------------------------------------------+
| Reproducido como     | cuadro de texto o tres campos de selección                                  |
+----------------------+-----------------------------------------------------------------------------+
| Opciones             | - `widget`_                                                                 |
|                      | - `input`_                                                                  |
|                      | - `years`_                                                                  |
|                      | - `months`_                                                                 |
|                      | - `days`_                                                                   |
|                      | - `format`_                                                                 |
|                      | - `pattern`_                                                                |
|                      | - `data_timezone`_                                                          |
|                      | - `user_timezone`_                                                          |
+----------------------+-----------------------------------------------------------------------------+
| Tipo del padre       | ``field`` (si es texto), de lo contrario ``form``                           |
+----------------------+-----------------------------------------------------------------------------+
| Clase                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\DateType`          |
+----------------------+-----------------------------------------------------------------------------+

Uso básico
----------

Este tipo de campo es altamente configurable, pero fácil de usar. Las opciones más importantes son ``input`` y ``widget``.

Supongamos que tienes un campo ``publishedAt`` cuya fecha subyacente es un objeto ``DateTime``. El siguiente código configura el tipo ``date`` para ese campo como tres campos de opciones diferentes:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

La opción ``input`` se *debe* cambiar para que coincida con el tipo de dato de la fecha subyacente. Por ejemplo, si los datos del campo ``publishedAt`` eran una marca de tiempo Unix, habría la necesidad de establecer ``input`` a ``timestamp``:

.. code-block:: php

    $builder->add('publishedAt', 'date', array(
        'input'  => 'timestamp',
        'widget' => 'choice',
    ));

El campo también es compatible con ``array`` y ``string`` como valores  válidos de la opción ``input``.

Opciones del campo
~~~~~~~~~~~~~~~~~~

.. include:: /reference/forms/types/options/date_widget.rst.inc

.. _form-reference-date-input:

.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/years.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc

.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
