Choice
======

La restricción ``Choice`` valida que un valor sea uno o más de una lista de determinadas opciones.

+----------------+-----------------------------------------------------------------------+
| Valida         | un valor escalar o una matriz de valores escalares (su ``multiple``   |
|                | es true)                                                              |
+----------------+-----------------------------------------------------------------------+
| Opciones       | - ``choices``                                                         |
|                | - ``callback``                                                        |
|                | - ``multiple``                                                        |
|                | - ``min``                                                             |
|                | - ``max``                                                             |
|                | - ``message``                                                         |
|                | - ``minMessage``                                                      |
|                | - ``maxMessage``                                                      |
|                | - ``strict``                                                          |
+----------------+-----------------------------------------------------------------------+
| Opción         | ``choices``                                                           |
| predeterminada |                                                                       |
+----------------+-----------------------------------------------------------------------+
| Clase          | :class:`Symfony\\Component\\Validator\\Constraints\\Choice`           |
+----------------+-----------------------------------------------------------------------+

Opciones disponibles
--------------------

*   ``choices`` (**predeterminado**) [tipo: array] Una opción requerida (a menos que se especifique ``callback``) - este es el arreglo de opciones que se deben considerar en el conjunto válido. El valor de entrada se compara con esta matriz.

*   ``callback``: [tipo: string|array] Este es un método estático de retrollamada que se puede utilizar en lugar de las opción ``choices`` para devolver la matriz de opciones.
    
    Si pasas una cadena con el nombre del método (por ejemplo, ``getGenders``), método estático que se debe llamar en la clase validada.
    
    Si pasas una matriz (por ejemplo, ``array('Util', 'getGenders')``), esta sigue la sintaxis invocable normal donde el primer argumento es el nombre de la clase y el segundo argumento es el nombre del método.

*   ``multiple``: [tipo: Boolean, predeterminado: false] Si esta opción es ``true``, el valor de entrada se espera que sea una matriz en lugar de un único valor escalar. La restricción debe verificar que cada valor de la matriz de entrada se puede encontrar en el arreglo de alternativas válidas. Si alguno de los valores de entrada no se puede encontrar, la validación fallará.

*   ``min``: [tipo: integer] Si la opción ``multiple`` es ``true``, entonces puedes utilizar la opción ``min`` para forzar al menos el número XX de los valores a seleccionar. Por ejemplo, si ``min`` es 3, pero la matriz de entrada sólo contiene dos elementos válidos, la validación fallará.

*   ``max``: [tipo: integer] Si la opción ``multiple`` es ``true``, entonces puedes utilizar la opción ``max`` para forzar no más que el número XX de los valores a seleccionar. Por ejemplo, si ``máx`` es 3, pero la matriz de entrada consta de 4 elementos válidos, la validación fallará.

*   ``message``: [tipo: string, predeterminado: `Este valor debe ser uno de entre las opciones dadas`] Este es el mensaje de error de validación que se muestra cuando el valor de entrada no es válido.

*   ``minMessage``: [tipo: string, predeterminado: `Debes seleccionar al menos {{ limit }} opciones`] Este es el mensaje de error de validación que se muestra cuando el usuario elige menos opciones para la opción ``min``.

*   ``maxMessage``: [tipo: string, predeterminado: `Debes seleccionar por lo menos {{ limit }} opciones`] Este es el mensaje de error de validación que se muestra cuando el usuario elige demasiadas opciones para la opción ``max``.

*   ``strict``: [tipo: bool, predeterminado: ``false``]
    Si es ``true``, el validador además debe verificar el tipo del valor ingresado.

Uso básico
----------

Si las opciones son simples, se pueden pasar a la definición de restricción como una matriz.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                genero:
                    - Choice:
                        choices:  [masculino, femenino]
                        message:  Elige un género válido.

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="genero">
                <constraint name="Choice">
                    <option name="choices">
                        <value>masculino</value>
                        <value>femenino</value>
                    </option>
                    <option name="message">Elige un género válido.</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice(choices = {"masculino", "femenino"}, message = "Elige un género válido.")
             */
            protected $genero;
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;
        
        class Autor
        {
            protected $genero;
            
            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('genero', new Choice(
                    'choices' => array('masculino', 'femenino'),
                    'message' => 'Elige un género válido.',
                ));
            }
        }

Suministrando las opciones con una función Callback
---------------------------------------------------

También puedes utilizar una función retrollamada para especificar tus opciones. Esto es útil si deseas mantener tus opciones en una ubicación central para que, por ejemplo, puedas acceder fácilmente a las opciones para validación o para construir un elemento de formulario seleccionado.

.. code-block:: php

    // src/Acme/HolaBundle/Autor.php
    class Autor
    {
        public static function getGenders()
        {
            return array('masculino', 'femenino');
        }
    }

Puedes pasar el nombre de este método a la opción ``callback`` de la restricción ``Choice``.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                genero:
                    - Choice: { callback: getGenders }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="genero">
                <constraint name="Choice">
                    <option name="callback">getGenders</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice(callback = "getGenders")
             */
            protected $genero;
        }

Si la retrollamada estática se almacena en una clase diferente, por ejemplo ``Util``, puedes pasar el nombre de clase y el método como una matriz.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                genero:
                    - Choice: { callback: [Util, getGenders] }

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="genero">
                <constraint name="Choice">
                    <option name="callback">
                        <value>Util</value>
                        <value>getGenders</value>
                    </option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Choice(callback = {"Util", "getGenders"})
             */
            protected $genero;
        }
