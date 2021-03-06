.. index::
   single: Formularios; Campos; locale

Tipo de campo locale
====================

El tipo ``locale`` es un subconjunto de ``ChoiceType`` que permite al usuario seleccionar entre una larga lista de regiones (idioma + país). Como bono adicional, los nombres regionales se muestran en el idioma del usuario.

El "valor" de cada localidad es o bien el del código de *idioma* de dos letras ISO639-1 (por ejemplo, ``es``), o el código de idioma seguido de un guión bajo (``_``), entonces el código de *país* ISO3166 (por ejemplo, ``es_ES`` para el Español/España).

.. note::

   La configuración regional del usuario se adivina usando `Locale::getDefault()`_

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
| Clase       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\LanguageType` |
+-------------+------------------------------------------------------------------------+

Opciones heredadas
------------------

Estas opciones las hereda del tipo :doc:`choice</reference/forms/types/choice>`:

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

Estas opciones las hereda del tipo :doc:`field </reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. _`Locale::getDefault()`: http://php.net/manual/en/locale.getdefault.php
