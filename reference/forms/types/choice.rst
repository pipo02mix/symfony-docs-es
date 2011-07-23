.. index::
   single: Formularios; Campos; choice

Tipo de campo choice
====================

Un campo multipropósito para que el usuario pueda "elegir" una o más opciones.
Este se puede representar como una etiqueta ``select``, botones de radio o casillas de verificación.

Para utilizar este campo, debes especificar *algún* elemento de la ``choice_list`` o ``choices``.

+-------------+-----------------------------------------------------------------------------+
| Reproducido | pueden varias etiquetas (ve abajo)                                          |
| como        |                                                                             |
+-------------+-----------------------------------------------------------------------------+
| Opciones    | - `choices`_                                                                |
|             | - `choice_list`_                                                            |
|             | - `multiple`_                                                               |
|             | - `expanded`_                                                               |
|             | - `preferred_choices`_                                                      |
+-------------+-----------------------------------------------------------------------------+
| Opciones    | - `required`_                                                               |
| heredadas   | - `label`_                                                                  |
|             | - `read_only`_                                                              |
|             | - `error_bubbling`_                                                         |
+-------------+-----------------------------------------------------------------------------+
| Tipo del    | :doc:`form</reference/forms/types/form>` (si expanded),                     |
| padre       | ``field`` en caso contrario                                                 |
+-------------+-----------------------------------------------------------------------------+
| Clase       | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\ChoiceType`        |
+-------------+-----------------------------------------------------------------------------+

Ejemplo de Uso
--------------

La forma fácil de utilizar este campo es especificando las opciones directamente a través de la opción ``choices``. La clave de la matriz se convierte en el valor que en realidad estableces en el objeto subyacente (por ejemplo, ``m``), mientras que el valor es lo que el usuario ve en el formulario (por ejemplo, ``Hombre``).

.. code-block:: php

    $builder->add('genero', 'choice', array(
        'choices'   => array('m' => 'Masculino', 'f' => 'Femenino'),
        'required'  => false,
    ));

Al establecer ``multiple`` a ``true``, puedes permitir al usuario elegir varios valores. El elemento gráfico se reproduce como una etiqueta ``select`` múltiple o una serie de casillas de verificación dependiendo de la opción ``expanded``:

.. code-block:: php

    $builder->add('availability', 'choice', array(
        'choices'   => array(
            'morning'   => 'Morning',
            'afternoon' => 'Afternoon',
            'evening'   => 'Evening',
        ),
        'multiple'  => true,
    ));

También puedes utilizar la opción ``choice_list``, la cual toma un objeto que puede especificar las opciones para el elemento gráfico.

.. _forms-reference-choice-tags:

.. include:: /reference/forms/types/options/select_how_rendered.rst.inc

.. include:: /reference/forms/types/options/empty_value.rst.inc

Opciones del campo
~~~~~~~~~~~~~~~~~~

``choices``
~~~~~~~~~~~

**tipo**: ``array`` **predeterminado**: ``array()``

Esta es la forma más sencilla de especificar las opciones que debe utilizar por este campo. La opción ``choices`` es una matriz, donde la clave del arreglo es el valor del elemento y el valor del arreglo es la etiqueta del elemento::

    $builder->add('genero', 'choice', array(
        'choices' => array('m' => 'Masculino', 'f' => 'Femenino')
    ));

``choice_list``
~~~~~~~~~~~~~~~

**tipo**: ``Symfony\Component\Form\ChoiceList\ChoiceListInterface``

Esta es una manera de especificar las opciones que se utilizan para este campo.
La opción ``choice_list`` debe ser una instancia de ``ChoiceListInterface``.
Para casos más avanzados, puedes crear una clase personalizada que implemente la interfaz para suplir las opciones.

.. include:: /reference/forms/types/options/multiple.rst.inc

.. include:: /reference/forms/types/options/expanded.rst.inc

.. include:: /reference/forms/types/options/preferred_choices.rst.inc

Opciones heredadas
------------------

Estas opciones las hereda del tipo :doc:`field </reference/forms/types/field>`:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc
