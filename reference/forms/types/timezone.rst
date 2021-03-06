.. index::
   single: Formularios; Campos; timezone

Tipo de campo timezone
======================

El tipo ``timezone`` es un subconjunto de ``ChoiceType`` que permite al usuario seleccionar entre todas las posibles zonas horarias.

El "value" para cada zona horaria es el nombre completo de la zona horaria, por ejemplo ``América/Chicago`` o  ``Europa/Estambul``.

A diferencia del tipo ``choice``, no es necesario especificar una opción ``choices`` o ``choice_list``, ya que el tipo de campo utiliza automáticamente una larga lista de regiones. *Puedes* especificar cualquiera de estas opciones manualmente, pero entonces sólo debes utilizar el tipo ``choice`` directamente.

+-------------+------------------------------------------------------------------------+
| Reproducido | pueden ser varias etiquetas (ve :ref:`forms-reference-choice-tags`)    |
| como        |                                                                        |
+-------------+------------------------------------------------------------------------+
| Opciones    | - `multiple`_                                                          |
| heredadas   | - `expanded`_                                                          |
|             | - `preferred_choices`_                                                 |
|             | - `error_bubbling`_                                                    |
|             | - `required`_                                                          |
|             | - `label`_                                                             |
|             | - `read_only`_                                                         |
+-------------+------------------------------------------------------------------------+
| Tipo del    | :doc:`choice</reference/forms/types/choice>`                           |
| padre       |                                                                        |
+-------------+------------------------------------------------------------------------+
| Clase       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\TimezoneType` |
+-------------+------------------------------------------------------------------------+

Opciones heredadas
------------------

Estas opciones las hereda del tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

Estas opciones las hereda del tipo :doc:`field </reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
