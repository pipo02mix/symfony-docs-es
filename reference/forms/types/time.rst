.. index::
   single: Formularios; Campos; time

Tipo de campo ``time``
======================

Un campo para capturar entradas de tiempo.

Este se puede reproducir como un campo de texto, una serie de campos de texto (por ejemplo, horas, minutos, segundos) o una serie de campos de selección. Los datos subyacentes se pueden almacenar como un objeto ``DateTime``, una cadena, una marca de tiempo (``timestamp``) o una matriz.

+----------------------+-----------------------------------------------------------------------------+
| Tipo de dato         | puede ser ``DateTime``, string, timestamp, o array (consulta la opción      |
| subyacente           | ``input``)                                                                  |
+----------------------+-----------------------------------------------------------------------------+
| Reproducido como     | pueden ser varias etiquetas (ve abajo)                                      |
+----------------------+-----------------------------------------------------------------------------+
| Opciones             | - `widget`_                                                                 |
|                      | - `input`_                                                                  |
|                      | - `with_seconds`_                                                           |
|                      | - `hours`_                                                                  |
|                      | - `minutes`_                                                                |
|                      | - `seconds`_                                                                |
|                      | - `data_timezone`_                                                          |
|                      | - `user_timezone`_                                                          |
+----------------------+-----------------------------------------------------------------------------+
| Tipo del padre       | ``form``                                                                    |
+----------------------+-----------------------------------------------------------------------------+
| Clase                | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimeType`          |
+----------------------+-----------------------------------------------------------------------------+

Uso básico
----------

Este tipo de campo es altamente configurable, pero fácil de usar. Las opciones más importantes son ``input`` y ``widget``.

Supongamos que tienes un campo ``startTime`` cuyo dato de hora subyacente es un objeto ``DateTime``. Lo siguiente configura el tipo ``time`` para ese campo como tres campos de opciones diferentes:

.. code-block:: php

    $builder->add('startTime', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

La opción ``input`` se *debe* cambiar para que coincida con el tipo de dato de la fecha subyacente. Por ejemplo, si los datos del campo ``startTime`` fueran una marca de tiempo Unix, habría necesidad de establecer la ``entrada`` a ``timestamp``:

.. code-block:: php

    $builder->add('startTime', 'date', array(
        'input'  => 'datetime',
        'widget' => 'choice',
    ));

El campo también es compatible con ``array`` y ``string`` como valores  válidos de la opción ``input``.

Opciones del campo
~~~~~~~~~~~~~~~~~~

``widget``
~~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``choice``

La forma básica en que se debe reproducir este campo. Puede ser una de las siguientes:

* ``choice``: reproduce dos (o tres di `with_seconds`_ es ``true``) cuadros de selección.

* ``text``: reproduce dos o tres cuadros de texto (hora, minuto, segundo).

* ``single_text``: reproduce un sólo cuadro de tipo texto. La entrada del usuario se validará contra la forma ``hh:mm`` (o ``hh:mm:ss`` si se utilizan segundos).

``input``
~~~~~~~~~

**tipo**: ``string`` **predeterminado**: ``datetime``

El formato del dato *input* - es decir, el formato de la fecha en que se almacena en el objeto subyacente. Los valores válidos son los siguientes:

* ``string`` (p. ej. ``12:17:26``)
* ``datetime`` (un objeto ``DateTime``)
* ``array`` (p. ej. ``array(12, 17, 26)``)
* ``timestamp`` (p. ej. ``1307232000``)

El valor devuelto por el formulario también se normaliza de nuevo a este formato.

.. include:: /reference/forms/types/options/with_seconds.rst.inc

.. include:: /reference/forms/types/options/hours.rst.inc

.. include:: /reference/forms/types/options/minutes.rst.inc

.. include:: /reference/forms/types/options/seconds.rst.inc

.. include:: /reference/forms/types/options/data_timezone.rst.inc

.. include:: /reference/forms/types/options/user_timezone.rst.inc
