Type
====

Valida que un valor tiene un tipo de dato específico

.. configuration-block::

    .. code-block:: yaml

        properties:
            age:
                - Type: integer

    .. code-block:: php-annotations

        /**
         * @Assert\Type(type="integer")
         */
       protected $age;


Opciones
--------

* ``type`` (**predeterminado**, requerido): Un nombre de clase completamente cualificado o uno de los tipos de datos PHP según lo determinado por las funciones ``is_`` de PHP.

  * `array <http://mx.php.net/is_array>`_
  * `bool <http://mx.php.net/is_bool>`_
  * `callable <http://mx.php.net/is_callable>`_
  * `float <http://mx.php.net/is_float>`_ 
  * `double <http://mx.php.net/is_double>`_
  * `int <http://mx.php.net/is_int>`_ 
  * `integer <http://mx.php.net/is_integer>`_
  * `long <http://mx.php.net/is_long>`_
  * `null <http://mx.php.net/is_null>`_
  * `numeric <http://mx.php.net/is_numeric>`_
  * `object <http://mx.php.net/is_object>`_
  * `real <http://mx.php.net/is_real>`_
  * `resource <http://mx.php.net/is_resource>`_
  * `scalar <http://mx.php.net/is_scalar>`_
  * `string <http://mx.php.net/is_string>`_
* ``message``: El mensaje de error en caso de que falle la validación
