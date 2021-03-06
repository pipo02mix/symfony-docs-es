.. index::
   single: Formularios; Campos; radio

Tipo de campo radio
===================

Crea un solo botón de radio. Este se debe utilizar siempre para un campo que tiene un valor booleano: si el botón de radio es seleccionado, el campo se establece en ``true``, si el botón no está seleccionado, el valor se establece en ``false``.

El tipo ``radio`` no suele usarse directamente. Comúnmente se utiliza internamente por otros tipos, tales como :doc:`choice </reference/forms/types/choice>`.
Si quieres tener un campo booleano, utiliza una :doc:`casilla de verificación </reference/forms/types/checkbox>`.

+-------------+---------------------------------------------------------------------+
| Reproducido | campo ``input`` ``text``                                            |
| como        |                                                                     |
+-------------+---------------------------------------------------------------------+
| Opciones    | - `value`_                                                          |
+-------------+---------------------------------------------------------------------+
| Opciones    | - `required`_                                                       |
| heredadas   | - `label`_                                                          |
|             | - `read_only`_                                                      |
|             | - `error_bubbling`_                                                 |
+-------------+---------------------------------------------------------------------+
| Tipo del    | :doc:`field</reference/forms/types/field>`                          |
| padre       |                                                                     |
+-------------+---------------------------------------------------------------------+
| Clase       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\RadioType` |
+-------------+---------------------------------------------------------------------+

Opciones del campo
~~~~~~~~~~~~~~~~~~

``value``
~~~~~~~~~

**tipo**: ``mixed`` **predeterminado**: ``1``

El valor utilizado realmente como valor para el botón de radio. Esto no afecta al valor establecido en tu objeto.

Opciones heredadas
------------------

Estas opciones las hereda del tipo :doc:`field </reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
