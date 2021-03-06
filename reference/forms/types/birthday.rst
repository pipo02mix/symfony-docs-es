.. index::
   single: Formularios; Campos; birthday

Tipo de campo ``birthday``
==========================

Un campo :doc:`date</reference/forms/types/date>` que se especializa en el manejo de fechas de cumpleaños.

Se puede reproducir como un cuadro de texto, tres cuadros de texto (mes, día y año), o tres cuadros de selección.

Este tipo esencialmente es el mismo que el tipo :doc:`date </reference/forms/types/date>`, pero con un predeterminado más apropiado para la opción `years`_. La opción predeterminada de `years`_ es 120 años atrás del año en curso.

+----------------------+----------------------------------------------------------------------------------------------------------------+
| Tipo de dato         | puede ser ``DateTime``, ``string``, ``timestamp``, o ``array``                                                 |
| subyacente           | (ve la :ref:`input option <form-reference-date-input>`)                                                        |
+----------------------+----------------------------------------------------------------------------------------------------------------+
| Reproducido como     | pueden ser tres cajas de selección o 1 o 3 cajas de texto, basándose en la opción `widget`_                    |
+----------------------+----------------------------------------------------------------------------------------------------------------+
| Opciones             | - `years`_                                                                                                     |
+----------------------+----------------------------------------------------------------------------------------------------------------+
| Opciones heredadas   | - `widget`_                                                                                                    |
|                      | - `input`_                                                                                                     |
|                      | - `months`_                                                                                                    |
|                      | - `days`_                                                                                                      |
|                      | - `format`_                                                                                                    |
|                      | - `pattern`_                                                                                                   |
|                      | - `data_timezone`_                                                                                             |
|                      | - `user_timezone`_                                                                                             |
+----------------------+----------------------------------------------------------------------------------------------------------------+
| Tipo del padre       | :doc:`date</reference/forms/types/date>`                                                                       |
+----------------------+----------------------------------------------------------------------------------------------------------------+
| Clase                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\BirthdayType`                                         |
+----------------------+----------------------------------------------------------------------------------------------------------------+

Opciones del campo
~~~~~~~~~~~~~~~~~~

``years``
~~~~~~~~~

**tipo**: ``array`` **predeterminado**: 120 años atrás de la fecha actual

Lista de años disponibles para el tipo de campo ``year``.  Esta opción sólo es relevante cuando la opción `widget`_ está establecida en ``choice``.

Opciones heredadas
------------------

Estas opciones las hereda del tipo :doc:`date </reference/forms/types/date>`:

.. include:: /reference/forms/types/options/date_widget.rst.inc
    
.. include:: /reference/forms/types/options/date_input.rst.inc

.. include:: /reference/forms/types/options/months.rst.inc

.. include:: /reference/forms/types/options/days.rst.inc

.. include:: /reference/forms/types/options/date_format.rst.inc
    
.. include:: /reference/forms/types/options/date_pattern.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
