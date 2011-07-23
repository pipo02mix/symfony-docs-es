Colección
=========

Valida la matriz ingresada con diferentes limitaciones.

.. code-block:: yaml

    - Collection:
        fields:
            key1:
                - NotNull: ~
            key2:
                - MinLength: 10

Opciones
--------

* ``fields`` (required): Una matriz asociativa de claves de matriz y una o más restricciones
* ``allowMissingFields``: Si algunas claves no pueden estar presentes en la matriz. Predeterminado: ``false``
* ``allowExtraFields``: Cuando la matriz puede contener claves que no están presentes en la opción ``fields``. Predeterminado: ``false``
* ``missingFieldsMessage``: El mensaje de error si falla la validación de ``allowMissingFields``
* ``allowExtraFields``: El mensaje de error si falla la validación ``allowExtraFields``

Ejemplo:
--------

Vamos a validar una matriz con dos índices ``nombreDePila`` y ``apellido``. El valor de ``nombreDePila`` no debe estar en blanco, mientras que el valor de ``apellido`` no debe estar en blanco con una longitud mínima de cuatro caracteres. Además, las dos claves no existen en la matriz.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HolaBundle/Resources/config/validation.yml
        Acme\HolaBundle\Autor:
            properties:
                options:
                    - Collection:
                        fields:
                            nombreDePila:
                                - NotBlank: ~
                            apellido:
                                - NotBlank: ~
                                - MinLength: 4
                        allowMissingFields: true

    .. code-block:: xml

        <!-- src/Acme/HolaBundle/Resources/config/validation.xml -->
        <class name="Acme\HolaBundle\Autor">
            <property name="options">
                <constraint name="Collection">
                    <option name="fields">
                        <value key="nombreDePila">
                            <constraint name="NotNull" />
                        </value>
                        <value key="apellido">
                            <constraint name="NotNull" />
                            <constraint name="MinLength">4</constraint>
                        </value>
                    </option>
                    <option name="allowMissingFields">true</option>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Autor
        {
            /**
             * @Assert\Collection(
             *   fields = {
             *     "nombreDePila" = @Assert\NotNull(),
             *     "apellido" = { @Assert\NotBlank(), @Assert\MinLength(4) }
             *   },
             *   allowMissingFields = true
             * )
             */
            private $opciones = array();
        }

    .. code-block:: php

        // src/Acme/HolaBundle/Autor.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Collection;
        use Symfony\Component\Validator\Constraints\NotNull;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Autor
        {
            private $opciones = array();
            
            public static function loadValidatorMetadata(ClassMetadata $metadatos)
            {
                $metadatos->addPropertyConstraint('options', new Collection(array(
                    'fields' => array(
                        'nombreDePila' => new NotNull(),
                        'apellido' => array(new NotBlank(), new MinLength(4)),
                    ),
                    'allowMissingFields' => true,
                )));
            }
        }

Los siguientes objetos no superan la validación.

.. code-block:: php

    $autor = new Autor();
    $autor->options['nombreDePila'] = null;
    $autor->options['apellido'] = 'foo';

    print $validador->validate($autor);

Deberías ver el siguiente mensaje de error:

.. code-block:: text

    Acme\HolaBundle\Autor.options[nombreDePila]:
        This value should not be null
    Acme\HolaBundle\Autor.options[apellido]:
        This value is too short. Este debe tener 4 caracteres o más
