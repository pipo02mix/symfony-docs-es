.. index::
   single: Formularios; Campos; language

Tipo de campo language
======================

El tipo ``lenguaje`` es un subconjunto de ``ChoiceType`` que permite al usuario seleccionar entre una larga lista de idiomas. Como bono adicional, los nombres de idioma se muestran en el idioma del usuario.

El "valor" de cada localidad es o bien el código de dos letras ISO639-1 *idioma* (por ejemplo, ``es``).

.. note::

   La configuración regional del usuario se adivina usando `Locale::getDefault()`_

A diferencia del tipo ``choice``, no es necesario especificar una opción ``choice`` o ``choice_list`` ya que el tipo de campo automáticamente utiliza una larga lista de idiomas. *Puedes* especificar cualquiera de estas opciones manualmente, pero entonces sólo debes utilizar el tipo ``choice`` directamente.

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
